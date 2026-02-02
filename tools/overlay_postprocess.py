#!/usr/bin/env python3
"""
Post-processor for JFG overlay assembly files.

This script takes splat-generated assembly and:
1. Adds comments with real symbol names for relocated instructions
2. Generates a companion .inc file with symbol definitions
3. Can optionally rewrite the assembly to use macros for easier decompilation

Usage:
    python overlay_postprocess.py <asm_file> <rom> <overlay_num> [-o output.s]
    python overlay_postprocess.py --batch <asm_dir> <rom> [--in-place]
"""

import argparse
import re
import struct
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Set
from dataclasses import dataclass


# ROM offsets for US version
ROM_OFFSETS = {
    'overlay_table': 0x1ED2780,
    'overlay_data_base': 0x1ED3B20,
    'overlay_rom_table': 0x1ED0270,
    'symbol_name_offsets': 0x1FEB040,
    'symbol_names_data': 0x1FED550,
    'num_symbols': 2371,
    'base_addr': 0x80000450,
    'num_overlays': 157,
}

RELOC_PATCH_JAL = 4
RELOC_PATCH_HI16 = 5
RELOC_PATCH_LO16 = 6
RELOC_PATCH_WORD = 2

RELOC_TYPE_EXTERNAL = 0
RELOC_TYPE_LOCAL = 1
RELOC_TYPE_JUMP = 2
RELOC_TYPE_DATA = 3


@dataclass
class RelocationEntry:
    symbol_index: int
    target_offset: int
    reloc_type: int
    patch_type: int


class OverlayInfo:
    def __init__(self, rom_data: bytes, overlay_num: int):
        self.overlay_num = overlay_num
        self.rom_data = rom_data
        
        header_offset = ROM_OFFSETS['overlay_table'] + (overlay_num - 1) * 0x20
        self.vram_base = struct.unpack(">i", rom_data[header_offset:header_offset+4])[0]
        self.rom_offset = struct.unpack(">i", rom_data[header_offset+4:header_offset+8])[0]
        self.text_size = struct.unpack(">i", rom_data[header_offset+8:header_offset+12])[0]
        self.data_size = struct.unpack(">i", rom_data[header_offset+12:header_offset+16])[0]
        self.bss_size = struct.unpack(">i", rom_data[header_offset+16:header_offset+20])[0]
        self.reloc_count = struct.unpack(">H", rom_data[header_offset+20:header_offset+22])[0]
        
        self.rom_start = ROM_OFFSETS['overlay_data_base'] + self.rom_offset
        
        self.relocations = self._parse_relocations()
        # Map VRAM address to relocation
        self.reloc_by_vram: Dict[int, RelocationEntry] = {}
        for r in self.relocations:
            vram = self.vram_base + r.target_offset
            self.reloc_by_vram[vram] = r
    
    def _parse_relocations(self) -> List[RelocationEntry]:
        relocs = []
        reloc_start = self.rom_start + self.text_size + self.data_size
        
        for i in range(self.reloc_count):
            offset = reloc_start + i * 8
            val1 = struct.unpack(">I", self.rom_data[offset:offset+4])[0]
            val2 = struct.unpack(">I", self.rom_data[offset+4:offset+8])[0]
            
            relocs.append(RelocationEntry(
                symbol_index=val1 & 0xFFFF,
                target_offset=val2 & 0xFFFFFF,
                reloc_type=(val2 >> 24) & 0xF,
                patch_type=(val2 >> 28) & 0xF
            ))
        
        return relocs
    
    def get_symbol_name(self, index: int) -> str:
        if index >= ROM_OFFSETS['num_symbols']:
            return f"unk_sym_{index}"
        
        name_offset_addr = ROM_OFFSETS['symbol_name_offsets'] + index * 4
        name_offset = struct.unpack(">I", self.rom_data[name_offset_addr:name_offset_addr+4])[0]
        
        name_addr = ROM_OFFSETS['symbol_names_data'] + name_offset
        end = self.rom_data.find(b'\x00', name_addr)
        if end == -1:
            end = name_addr + 64
        
        return self.rom_data[name_addr:end].decode('ascii', errors='replace')
    
    def resolve_ort_entry(self, ort_index: int) -> Tuple[int, int]:
        entry_addr = ROM_OFFSETS['overlay_rom_table'] + ort_index * 4
        entry = struct.unpack(">I", self.rom_data[entry_addr:entry_addr+4])[0]
        return (entry >> 20) & 0xFFF, entry & 0xFFFFF
    
    def get_reloc_target_name(self, entry: RelocationEntry) -> str:
        if entry.reloc_type == RELOC_TYPE_EXTERNAL:
            target_overlay, func_offset = self.resolve_ort_entry(entry.symbol_index)
            
            # Try to find the symbol name
            for i in range(ROM_OFFSETS['num_symbols']):
                ort_addr = ROM_OFFSETS['overlay_rom_table'] + i * 4
                ort_entry = struct.unpack(">I", self.rom_data[ort_addr:ort_addr+4])[0]
                ort_overlay = (ort_entry >> 20) & 0xFFF
                ort_offset = ort_entry & 0xFFFFF
                
                if ort_overlay == target_overlay and ort_offset == func_offset:
                    return self.get_symbol_name(i)
            
            if target_overlay == 0:
                return f"func_{ROM_OFFSETS['base_addr'] + func_offset:08X}"
            return f"overlay{target_overlay}_func_{func_offset:05X}"
        
        elif entry.reloc_type == RELOC_TYPE_LOCAL:
            return f"L{self.overlay_num}_{entry.symbol_index:04X}"
        
        elif entry.reloc_type == RELOC_TYPE_DATA:
            return f"D{self.overlay_num}_{entry.symbol_index:04X}"
        
        return f"sym_{entry.symbol_index:04X}"


def process_asm_file(asm_path: Path, info: OverlayInfo, output_path: Optional[Path] = None) -> Tuple[str, Set[str]]:
    """
    Process an assembly file, adding relocation comments.
    
    Returns the processed assembly and a set of referenced symbols.
    """
    with open(asm_path, 'r') as f:
        lines = f.readlines()
    
    output_lines = []
    referenced_symbols: Set[str] = set()
    
    # Pattern to match instruction lines with addresses
    # Example: /* 1ED3B20 80200000 0C000000 */  jal       0
    addr_pattern = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s*\*/')
    
    for line in lines:
        match = addr_pattern.search(line)
        if match:
            vram = int(match.group(1), 16)
            
            if vram in info.reloc_by_vram:
                reloc = info.reloc_by_vram[vram]
                target_name = info.get_reloc_target_name(reloc)
                referenced_symbols.add(target_name)
                
                # Add comment with symbol name
                line = line.rstrip()
                if reloc.patch_type == RELOC_PATCH_JAL:
                    line += f"  // -> {target_name}"
                elif reloc.patch_type == RELOC_PATCH_HI16:
                    line += f"  // %hi({target_name})"
                elif reloc.patch_type == RELOC_PATCH_LO16:
                    line += f"  // %lo({target_name})"
                elif reloc.patch_type == RELOC_PATCH_WORD:
                    line += f"  // .word {target_name}"
                line += "\n"
        
        output_lines.append(line)
    
    result = "".join(output_lines)
    
    if output_path:
        with open(output_path, 'w') as f:
            f.write(result)
    
    return result, referenced_symbols


def generate_symbol_inc(symbols: Set[str], info: OverlayInfo, output_path: Path):
    """Generate an include file with symbol definitions"""
    lines = []
    lines.append(f"// Auto-generated symbol definitions for overlay {info.overlay_num}")
    lines.append(f"// Include this file to define external symbols")
    lines.append("")
    
    for sym in sorted(symbols):
        # Try to find the actual address
        for i in range(ROM_OFFSETS['num_symbols']):
            name = info.get_symbol_name(i)
            if name == sym:
                ort_addr = ROM_OFFSETS['overlay_rom_table'] + i * 4
                entry = struct.unpack(">I", info.rom_data[ort_addr:ort_addr+4])[0]
                overlay_num = (entry >> 20) & 0xFFF
                offset = entry & 0xFFFFF
                
                if overlay_num == 0:
                    addr = ROM_OFFSETS['base_addr'] + offset
                    lines.append(f".set {sym}, 0x{addr:08X}")
                else:
                    lines.append(f"// {sym} is in overlay {overlay_num} at offset 0x{offset:X}")
                break
        else:
            lines.append(f"// {sym} - address unknown")
    
    with open(output_path, 'w') as f:
        f.write("\n".join(lines))


def main():
    parser = argparse.ArgumentParser(description='Post-process JFG overlay assembly')
    parser.add_argument('asm', help='Assembly file or directory')
    parser.add_argument('rom', help='ROM file path')
    parser.add_argument('overlay', type=int, nargs='?', help='Overlay number (auto-detect if not specified)')
    parser.add_argument('-o', '--output', help='Output file')
    parser.add_argument('--batch', action='store_true', help='Process all .s files in directory')
    parser.add_argument('--in-place', action='store_true', help='Modify files in place')
    parser.add_argument('--gen-inc', action='store_true', help='Generate .inc file with symbols')
    
    args = parser.parse_args()
    
    with open(args.rom, 'rb') as f:
        rom_data = f.read()
    
    if args.batch:
        asm_dir = Path(args.asm)
        for asm_file in asm_dir.glob('**/*.s'):
            # Try to detect overlay number from path
            overlay_match = re.search(r'overlay[_/]?(\d+)', str(asm_file))
            if overlay_match:
                overlay_num = int(overlay_match.group(1))
                info = OverlayInfo(rom_data, overlay_num)
                
                output = asm_file if args.in_place else asm_file.with_suffix('.annotated.s')
                _, symbols = process_asm_file(asm_file, info, output)
                
                if args.gen_inc:
                    generate_symbol_inc(symbols, info, asm_file.with_suffix('.inc'))
                
                print(f"Processed {asm_file} (overlay {overlay_num})")
    else:
        if not args.overlay:
            # Try to detect from filename
            overlay_match = re.search(r'overlay[_/]?(\d+)', args.asm)
            if overlay_match:
                overlay_num = int(overlay_match.group(1))
            else:
                parser.error("Could not detect overlay number, please specify")
        else:
            overlay_num = args.overlay
        
        info = OverlayInfo(rom_data, overlay_num)
        asm_path = Path(args.asm)
        
        output_path = Path(args.output) if args.output else None
        result, symbols = process_asm_file(asm_path, info, output_path)
        
        if args.gen_inc:
            inc_path = (output_path or asm_path).with_suffix('.inc')
            generate_symbol_inc(symbols, info, inc_path)
        
        if not output_path:
            print(result)


if __name__ == '__main__':
    main()