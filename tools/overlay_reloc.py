#!/usr/bin/env python3
"""
Overlay Relocation Tool for Jet Force Gemini

This tool handles the conversion between:
1. "Source" form: Overlay code with real function calls (JAL to actual addresses)
2. "ROM" form: Overlay code with placeholder JALs (JAL 0) + relocation table

The game's runtime linker (runLink.c) patches the placeholders at load time.

Usage:
    # Analyze an existing reloc table
    python overlay_reloc.py analyze <rom> <overlay_num>
    
    # Generate reloc table from linked overlay (for building)
    python overlay_reloc.py generate <overlay.bin> <symbols.txt> -o <reloc.bin>
    
    # Patch an overlay to show real function names (for decompilation reference)
    python overlay_reloc.py patch-names <overlay.s> <rom> <overlay_num> -o <patched.s>
"""

import argparse
import struct
import sys
from pathlib import Path
from dataclasses import dataclass
from typing import List, Dict, Optional, Tuple

# US ROM offsets
ROM_OFFSETS = {
    'us': {
        'overlay_table': 0x1ED2780,
        'overlay_data_base': 0x1ED3B20,
        'overlay_rom_table': 0x1ED0270,
        'symbol_name_offsets': 0x1FEB040,
        'symbol_names_data': 0x1FED550,
        'num_symbols': 2371,
        'base_addr': 0x80000450,
        'num_overlays': 157,
    }
}

# Relocation types (flagsHi values - patch type)
RELOC_PATCH_WORD = 2   # R_MIPS_32: Full 32-bit word
RELOC_PATCH_JAL = 4    # R_MIPS_26: JAL target
RELOC_PATCH_HI16 = 5   # Upper 16 bits (LUI)
RELOC_PATCH_LO16 = 6   # Lower 16 bits (ADDIU/LW/SW immediate)

# Relocation types (relocType values - source type)  
RELOC_TYPE_EXTERNAL = 0  # Reference to external symbol (via overlayRomTable)
RELOC_TYPE_LOCAL = 1     # Local offset within overlay
RELOC_TYPE_JUMP = 2      # Jump target (relative)
RELOC_TYPE_DATA = 3      # Data section relocation


@dataclass
class RelocationEntry:
    """8-byte relocation entry"""
    symbol_index: int      # Index into overlayRomTable
    target_offset: int     # Offset in overlay where patch is applied
    reloc_type: int        # Source type (0=external, 1=local, 2=jump, 3=data)
    patch_type: int        # Patch type (2=word, 4=jal, 5=hi16, 6=lo16)
    
    def to_bytes(self) -> bytes:
        info = (self.target_offset << 8) | (self.patch_type << 4) | self.reloc_type
        return struct.pack(">II", self.symbol_index, info)
    
    @classmethod
    def from_bytes(cls, data: bytes) -> 'RelocationEntry':
        symbol_index, info = struct.unpack(">II", data)
        target_offset = (info >> 8) & 0xFFFFFF
        patch_type = (info >> 4) & 0xF
        reloc_type = info & 0xF
        return cls(symbol_index, target_offset, reloc_type, patch_type)


@dataclass
class OverlayHeader:
    """32-byte overlay header from overlay table"""
    vram_base: int
    rom_offset: int  # Relative to overlay_data_base
    text_size: int
    data_size: int
    bss_size: int
    reloc_table_size: int
    secondary_reloc_size: int
    init_function: int
    resume_function: int


@dataclass
class SymbolInfo:
    """Resolved symbol information"""
    name: str
    overlay_num: int
    offset: int
    vram: Optional[int]


def read_u32(data: bytes, offset: int) -> int:
    return struct.unpack(">I", data[offset:offset+4])[0]

def read_u16(data: bytes, offset: int) -> int:
    return struct.unpack(">H", data[offset:offset+2])[0]

def read_s32(data: bytes, offset: int) -> int:
    return struct.unpack(">i", data[offset:offset+4])[0]

def read_string(data: bytes, offset: int) -> str:
    end = data.find(b'\x00', offset)
    if end == -1:
        end = len(data)
    return data[offset:end].decode('ascii', errors='replace')


class OverlayRelocTool:
    def __init__(self, rom_path: str, version: str = 'us'):
        self.rom_path = Path(rom_path)
        self.version = version
        self.offsets = ROM_OFFSETS[version]
        
        with open(rom_path, 'rb') as f:
            self.rom = f.read()
    
    def get_symbol_name(self, index: int) -> str:
        """Get symbol name by index"""
        if index >= self.offsets['num_symbols']:
            return f"<invalid_{index}>"
        name_offset = read_u32(self.rom, self.offsets['symbol_name_offsets'] + index * 4)
        return read_string(self.rom, self.offsets['symbol_names_data'] + name_offset)
    
    def get_overlay_header(self, overlay_num: int) -> OverlayHeader:
        """Read overlay header from ROM"""
        if overlay_num < 1 or overlay_num > self.offsets['num_overlays']:
            raise ValueError(f"Invalid overlay number: {overlay_num}")
        
        # Overlay table starts at overlay 1, each entry is 0x20 bytes
        header_offset = self.offsets['overlay_table'] + (overlay_num - 1) * 0x20
        
        return OverlayHeader(
            vram_base=read_s32(self.rom, header_offset + 0x00),
            rom_offset=read_s32(self.rom, header_offset + 0x04),
            text_size=read_s32(self.rom, header_offset + 0x08),
            data_size=read_s32(self.rom, header_offset + 0x0C),
            bss_size=read_s32(self.rom, header_offset + 0x10),
            reloc_table_size=read_u16(self.rom, header_offset + 0x14),
            secondary_reloc_size=read_u16(self.rom, header_offset + 0x16),
            init_function=read_s32(self.rom, header_offset + 0x18),
            resume_function=read_s32(self.rom, header_offset + 0x1C),
        )
    
    def resolve_ort_entry(self, ort_index: int) -> Tuple[int, int]:
        """Resolve overlayRomTable entry to (overlay_num, func_offset)"""
        entry = read_u32(self.rom, self.offsets['overlay_rom_table'] + ort_index * 4)
        overlay_num = (entry >> 20) & 0xFFF
        func_offset = entry & 0xFFFFF
        return overlay_num, func_offset
    
    def find_symbol_by_address(self, overlay_num: int, offset: int) -> Optional[str]:
        """Find symbol name for a given overlay + offset"""
        for i in range(self.offsets['num_symbols']):
            ort_entry = read_u32(self.rom, self.offsets['overlay_rom_table'] + i * 4)
            entry_overlay = (ort_entry >> 20) & 0xFFF
            entry_offset = ort_entry & 0xFFFFF
            
            if entry_overlay == overlay_num and entry_offset == offset:
                return self.get_symbol_name(i)
        return None
    
    def get_relocation_entries(self, overlay_num: int, secondary: bool = False) -> List[RelocationEntry]:
        """Read relocation entries for an overlay"""
        header = self.get_overlay_header(overlay_num)
        rom_addr = self.offsets['overlay_data_base'] + header.rom_offset
        
        # Reloc table is after text + data (+ bss for secondary)
        if secondary:
            reloc_rom = rom_addr + header.text_size + header.data_size + header.reloc_table_size
            reloc_size = header.secondary_reloc_size
        else:
            reloc_rom = rom_addr + header.text_size + header.data_size
            reloc_size = header.reloc_table_size
        
        entries = []
        num_entries = reloc_size // 8
        
        for i in range(num_entries):
            entry_data = self.rom[reloc_rom + i * 8 : reloc_rom + i * 8 + 8]
            entries.append(RelocationEntry.from_bytes(entry_data))
        
        return entries
    
    def resolve_reloc_target(self, entry: RelocationEntry, header: OverlayHeader, overlay_num: int) -> Tuple[str, Optional[str]]:
        """
        Resolve a relocation entry to a human-readable target description.
        
        Returns (target_string, symbol_name_or_none)
        
        Relocation types:
        - Type 0 (EXT): symbol_index is an index into overlayRomTable
        - Type 1 (LOCAL): symbol_index is a direct offset within this overlay
        - Type 2 (JUMP): symbol_index is a direct offset (jump target)
        - Type 3 (DATA): symbol_index is an ORT index
        """
        if entry.reloc_type == RELOC_TYPE_LOCAL:
            # LOCAL: symbol_index is a direct offset within the overlay
            local_offset = entry.symbol_index
            
            # Determine which section this offset falls into
            if local_offset < header.text_size:
                section = "text"
                section_offset = local_offset
            elif local_offset < header.text_size + header.data_size:
                section = "data"
                section_offset = local_offset - header.text_size
            else:
                section = "bss"
                section_offset = local_offset - header.text_size - header.data_size
            
            target = f"local:{section}+0x{section_offset:04X} (overlay+0x{local_offset:04X})"
            return target, None
        
        elif entry.reloc_type == RELOC_TYPE_JUMP:
            # JUMP: symbol_index is an ORT index (can be to main, self, or other overlay)
            overlay, offset = self.resolve_ort_entry(entry.symbol_index)
            
            if overlay == 0:
                # Main code
                vram = self.offsets['base_addr'] + offset
                sym_name = self.find_symbol_by_address(0, offset)
                target = f"jump->main:0x{vram:08X}"
            elif overlay == overlay_num:
                # Same overlay - intra-overlay call
                if offset < header.text_size:
                    section = "text"
                    section_offset = offset
                else:
                    section = "data"
                    section_offset = offset - header.text_size
                target = f"jump->self:{section}+0x{section_offset:04X}"
                sym_name = self.find_symbol_by_address(overlay, offset)
            else:
                # Different overlay
                target = f"jump->overlay_{overlay}+0x{offset:05X}"
                sym_name = self.find_symbol_by_address(overlay, offset)
            
            return target, sym_name
        
        elif entry.reloc_type == RELOC_TYPE_EXTERNAL or entry.reloc_type == RELOC_TYPE_DATA:
            # EXTERNAL/DATA: symbol_index is an index into overlayRomTable
            overlay, offset = self.resolve_ort_entry(entry.symbol_index)
            
            if overlay == 0:
                # Main code
                vram = self.offsets['base_addr'] + offset
                sym_name = self.find_symbol_by_address(0, offset)
                target = f"main:0x{vram:08X}"
            elif overlay >= 0xFFD:
                # Special sections (data/BSS)
                target = f"section_{overlay:03X}+0x{offset:05X}"
                sym_name = None
            elif overlay == overlay_num:
                # Same overlay (intra-overlay reference)
                if offset < header.text_size:
                    section = "text"
                    section_offset = offset
                else:
                    section = "data"
                    section_offset = offset - header.text_size
                target = f"self:{section}+0x{section_offset:04X}"
                sym_name = self.find_symbol_by_address(overlay, offset)
            else:
                # Different overlay
                target = f"overlay_{overlay}+0x{offset:05X}"
                sym_name = self.find_symbol_by_address(overlay, offset)
            
            return target, sym_name
        
        else:
            return f"unknown_type_{entry.reloc_type}", None

    def analyze_overlay(self, overlay_num: int):
        """Analyze and print relocation info for an overlay"""
        header = self.get_overlay_header(overlay_num)
        rom_addr = self.offsets['overlay_data_base'] + header.rom_offset
        
        print(f"=== Overlay {overlay_num} ===")
        print(f"ROM Address: 0x{rom_addr:X}")
        print(f"Text Size: 0x{header.text_size:X}")
        print(f"Data Size: 0x{header.data_size:X}")
        print(f"BSS Size: 0x{header.bss_size:X}")
        print(f"Reloc Table: 0x{header.reloc_table_size:X} ({header.reloc_table_size // 8} entries)")
        print(f"Secondary Reloc: 0x{header.secondary_reloc_size:X} ({header.secondary_reloc_size // 8} entries)")
        print(f"Init Function: 0x{header.init_function:X}" if header.init_function != -1 else "Init Function: none")
        print(f"Resume Function: 0x{header.resume_function:X}" if header.resume_function != -1 else "Resume Function: none")
        
        patch_names = {2: "WORD", 4: "JAL", 5: "HI16", 6: "LO16"}
        type_names = {0: "EXT", 1: "LOCAL", 2: "JUMP", 3: "DATA"}
        
        print(f"\n=== Main Relocation Table ===")
        entries = self.get_relocation_entries(overlay_num, secondary=False)
        for i, entry in enumerate(entries):
            target, sym_name = self.resolve_reloc_target(entry, header, overlay_num)
            
            type_str = type_names.get(entry.reloc_type, f"T{entry.reloc_type}")
            patch_str = patch_names.get(entry.patch_type, f"P{entry.patch_type}")
            
            sym_str = f" ({sym_name})" if sym_name else ""
            print(f"  [{i:3d}] +0x{entry.target_offset:05X} {type_str:5} {patch_str:4} -> {target}{sym_str}")
        
        if header.secondary_reloc_size > 0:
            print(f"\n=== Secondary Relocation Table ===")
            entries = self.get_relocation_entries(overlay_num, secondary=True)
            for i, entry in enumerate(entries):
                target, sym_name = self.resolve_reloc_target(entry, header, overlay_num)
                
                type_str = type_names.get(entry.reloc_type, f"T{entry.reloc_type}")
                patch_str = patch_names.get(entry.patch_type, f"P{entry.patch_type}")
                
                sym_str = f" ({sym_name})" if sym_name else ""
                print(f"  [{i:3d}] +0x{entry.target_offset:05X} {type_str:5} {patch_str:4} -> {target}{sym_str}")
    
    def generate_patched_asm(self, asm_path: str, overlay_num: int) -> str:
        """
        Generate assembly with real function names in comments.
        Useful for understanding what the code actually does.
        """
        with open(asm_path, 'r') as f:
            lines = f.readlines()
        
        # Build a map of offset -> symbol name from reloc table
        reloc_map: Dict[int, str] = {}
        
        for entry in self.get_relocation_entries(overlay_num, secondary=False):
            if entry.patch_type == RELOC_PATCH_JAL:
                overlay, offset = self.resolve_ort_entry(entry.symbol_index)
                if overlay == 0:
                    sym_name = self.find_symbol_by_address(0, offset)
                    if sym_name:
                        reloc_map[entry.target_offset] = sym_name
                else:
                    sym_name = self.find_symbol_by_address(overlay, offset)
                    if sym_name:
                        reloc_map[entry.target_offset] = f"{sym_name} (overlay_{overlay})"
        
        for entry in self.get_relocation_entries(overlay_num, secondary=True):
            if entry.patch_type == RELOC_PATCH_JAL:
                overlay, offset = self.resolve_ort_entry(entry.symbol_index)
                if overlay == 0:
                    sym_name = self.find_symbol_by_address(0, offset)
                    if sym_name:
                        reloc_map[entry.target_offset] = sym_name
        
        # Process lines and add comments
        output_lines = []
        for line in lines:
            # Look for JAL 0 instructions
            if '0C000000' in line and 'jal' in line.lower():
                # Extract the offset from the line (F0000XXX format)
                import re
                match = re.search(r'F0([0-9A-Fa-f]{6})', line)
                if match:
                    offset = int(match.group(1), 16)
                    if offset in reloc_map:
                        # Add comment with real function name
                        line = line.rstrip() + f"  /* -> {reloc_map[offset]} */\n"
            
            output_lines.append(line)
        
        return ''.join(output_lines)


def main():
    parser = argparse.ArgumentParser(description='Overlay Relocation Tool for JFG')
    subparsers = parser.add_subparsers(dest='command', help='Command to run')
    
    # Analyze command
    analyze_parser = subparsers.add_parser('analyze', help='Analyze overlay relocation table')
    analyze_parser.add_argument('rom', help='Path to ROM file')
    analyze_parser.add_argument('overlay', type=int, help='Overlay number (1-157)')
    analyze_parser.add_argument('-v', '--version', default='us', help='ROM version')
    
    # Patch-names command  
    patch_parser = subparsers.add_parser('patch-names', help='Add function name comments to assembly')
    patch_parser.add_argument('asm', help='Path to assembly file')
    patch_parser.add_argument('rom', help='Path to ROM file')
    patch_parser.add_argument('overlay', type=int, help='Overlay number')
    patch_parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    patch_parser.add_argument('-v', '--version', default='us', help='ROM version')
    
    args = parser.parse_args()
    
    if args.command == 'analyze':
        tool = OverlayRelocTool(args.rom, args.version)
        tool.analyze_overlay(args.overlay)
    
    elif args.command == 'patch-names':
        tool = OverlayRelocTool(args.rom, args.version)
        result = tool.generate_patched_asm(args.asm, args.overlay)
        if args.output:
            with open(args.output, 'w') as f:
                f.write(result)
        else:
            print(result)
    
    else:
        parser.print_help()


if __name__ == '__main__':
    main()
