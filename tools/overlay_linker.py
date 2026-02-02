#!/usr/bin/env python3
"""
Generate linker scripts and build infrastructure for JFG overlays.

This creates:
1. Per-overlay linker scripts that place code at offset 0
2. A post-link step that generates the relocation table
3. Symbol export files for cross-overlay references

The workflow is:
1. Compile overlay C files normally
2. Link with the generated linker script (places at address 0)
3. Run the relocation generator to create the reloc table
4. Combine into final overlay binary
"""

import argparse
import struct
from pathlib import Path
from typing import Dict, List, Set, Tuple
from dataclasses import dataclass
import re


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


@dataclass 
class OverlayBuildInfo:
    """Information needed to build an overlay"""
    overlay_num: int
    vram_base: int
    text_size: int
    data_size: int
    bss_size: int
    rom_offset: int
    
    # Symbols this overlay exports
    exports: List[Tuple[str, int]]  # (name, offset)
    
    # Symbols this overlay imports
    imports: List[Tuple[str, int, int]]  # (name, target_overlay, offset)


def load_overlay_info(rom_data: bytes, overlay_num: int) -> OverlayBuildInfo:
    """Load overlay build info from ROM"""
    header_offset = ROM_OFFSETS['overlay_table'] + (overlay_num - 1) * 0x20
    
    vram_base = struct.unpack(">i", rom_data[header_offset:header_offset+4])[0]
    rom_offset = struct.unpack(">i", rom_data[header_offset+4:header_offset+8])[0]
    text_size = struct.unpack(">i", rom_data[header_offset+8:header_offset+12])[0]
    data_size = struct.unpack(">i", rom_data[header_offset+12:header_offset+16])[0]
    bss_size = struct.unpack(">i", rom_data[header_offset+16:header_offset+20])[0]
    
    # Find exports (symbols in this overlay)
    exports = []
    imports = []
    
    for i in range(ROM_OFFSETS['num_symbols']):
        ort_addr = ROM_OFFSETS['overlay_rom_table'] + i * 4
        entry = struct.unpack(">I", rom_data[ort_addr:ort_addr+4])[0]
        target_overlay = (entry >> 20) & 0xFFF
        offset = entry & 0xFFFFF
        
        if target_overlay == overlay_num:
            # Get symbol name
            name_offset_addr = ROM_OFFSETS['symbol_name_offsets'] + i * 4
            name_offset = struct.unpack(">I", rom_data[name_offset_addr:name_offset_addr+4])[0]
            name_addr = ROM_OFFSETS['symbol_names_data'] + name_offset
            end = rom_data.find(b'\x00', name_addr)
            name = rom_data[name_addr:end].decode('ascii', errors='replace')
            
            exports.append((name, offset))
    
    return OverlayBuildInfo(
        overlay_num=overlay_num,
        vram_base=vram_base,
        text_size=text_size,
        data_size=data_size,
        bss_size=bss_size,
        rom_offset=rom_offset,
        exports=exports,
        imports=imports
    )


def generate_overlay_ld_script(info: OverlayBuildInfo, output_path: Path):
    """Generate a linker script for an overlay"""
    
    script = f"""/* Auto-generated linker script for overlay {info.overlay_num} */
/* Original VRAM: 0x{info.vram_base:08X} */
/* For decompilation, we link at 0x80200000 and use relocations */

OUTPUT_FORMAT("elf32-bigmips")
OUTPUT_ARCH(mips)

SECTIONS
{{
    /* Text section - executable code */
    .text 0x80200000 : 
    {{
        overlay{info.overlay_num}_TEXT_START = .;
        *(.text)
        *(.text.*)
        overlay{info.overlay_num}_TEXT_END = .;
    }}
    
    /* Data section */
    .data :
    {{
        overlay{info.overlay_num}_DATA_START = .;
        *(.data)
        *(.data.*)
        *(.rodata)
        *(.rodata.*)
        overlay{info.overlay_num}_DATA_END = .;
    }}
    
    /* BSS section */
    .bss :
    {{
        overlay{info.overlay_num}_BSS_START = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        overlay{info.overlay_num}_BSS_END = .;
    }}
    
    /* Relocation table - filled in by post-link step */
    .reloc :
    {{
        overlay{info.overlay_num}_RELOC_START = .;
        *(.reloc)
        overlay{info.overlay_num}_RELOC_END = .;
    }}
    
    /DISCARD/ :
    {{
        *(.MIPS.abiflags)
        *(.MIPS.options)
        *(.note.gnu.build-id)
        *(.comment)
        *(.pdr)
        *(.mdebug*)
        *(.reginfo)
        *(.gnu.attributes)
    }}
}}

/* Export symbols for other overlays */
"""
    
    for name, offset in info.exports:
        script += f"/* EXPORT: {name} = 0x{offset:X} */\n"
    
    with open(output_path, 'w') as f:
        f.write(script)


def generate_extern_header(rom_data: bytes, output_path: Path):
    """Generate a header file with all external symbol declarations"""
    
    lines = []
    lines.append("/* Auto-generated external symbol declarations */")
    lines.append("#ifndef JFG_EXTERN_H")
    lines.append("#define JFG_EXTERN_H")
    lines.append("")
    lines.append("/* Main code symbols */")
    
    main_symbols = []
    overlay_symbols: Dict[int, List[Tuple[str, int]]] = {}
    
    for i in range(ROM_OFFSETS['num_symbols']):
        ort_addr = ROM_OFFSETS['overlay_rom_table'] + i * 4
        entry = struct.unpack(">I", rom_data[ort_addr:ort_addr+4])[0]
        target_overlay = (entry >> 20) & 0xFFF
        offset = entry & 0xFFFFF
        
        name_offset_addr = ROM_OFFSETS['symbol_name_offsets'] + i * 4
        name_offset = struct.unpack(">I", rom_data[name_offset_addr:name_offset_addr+4])[0]
        name_addr = ROM_OFFSETS['symbol_names_data'] + name_offset
        end = rom_data.find(b'\x00', name_addr)
        name = rom_data[name_addr:end].decode('ascii', errors='replace')
        
        if target_overlay == 0:
            main_symbols.append((name, offset))
        else:
            if target_overlay not in overlay_symbols:
                overlay_symbols[target_overlay] = []
            overlay_symbols[target_overlay].append((name, offset))
    
    for name, offset in sorted(main_symbols, key=lambda x: x[1]):
        addr = ROM_OFFSETS['base_addr'] + offset
        lines.append(f"extern void {name}(void);  /* 0x{addr:08X} */")
    
    lines.append("")
    lines.append("/* Overlay symbols - these need runtime linking */")
    
    for overlay_num in sorted(overlay_symbols.keys()):
        lines.append(f"")
        lines.append(f"/* Overlay {overlay_num} */")
        for name, offset in sorted(overlay_symbols[overlay_num], key=lambda x: x[1]):
            lines.append(f"extern void {name}(void);  /* overlay {overlay_num} + 0x{offset:X} */")
    
    lines.append("")
    lines.append("#endif /* JFG_EXTERN_H */")
    
    with open(output_path, 'w') as f:
        f.write("\n".join(lines))


def generate_symbol_addrs(rom_data: bytes, output_path: Path):
    """Generate symbol_addrs.txt for splat"""
    
    lines = []
    lines.append("// Auto-generated symbol addresses")
    lines.append("// Main code symbols")
    
    for i in range(ROM_OFFSETS['num_symbols']):
        ort_addr = ROM_OFFSETS['overlay_rom_table'] + i * 4
        entry = struct.unpack(">I", rom_data[ort_addr:ort_addr+4])[0]
        target_overlay = (entry >> 20) & 0xFFF
        offset = entry & 0xFFFFF
        
        name_offset_addr = ROM_OFFSETS['symbol_name_offsets'] + i * 4
        name_offset = struct.unpack(">I", rom_data[name_offset_addr:name_offset_addr+4])[0]
        name_addr = ROM_OFFSETS['symbol_names_data'] + name_offset
        end = rom_data.find(b'\x00', name_addr)
        name = rom_data[name_addr:end].decode('ascii', errors='replace')
        
        if target_overlay == 0:
            addr = ROM_OFFSETS['base_addr'] + offset
            lines.append(f"{name} = 0x{addr:08X};")
    
    with open(output_path, 'w') as f:
        f.write("\n".join(lines))


def main():
    parser = argparse.ArgumentParser(description='Generate overlay build infrastructure')
    subparsers = parser.add_subparsers(dest='command')
    
    # Generate linker script
    ld_parser = subparsers.add_parser('ld-script', help='Generate overlay linker script')
    ld_parser.add_argument('rom', help='ROM file')
    ld_parser.add_argument('overlay', type=int, help='Overlay number')
    ld_parser.add_argument('-o', '--output', required=True, help='Output .ld file')
    
    # Generate extern header
    extern_parser = subparsers.add_parser('extern-header', help='Generate external symbol header')
    extern_parser.add_argument('rom', help='ROM file')
    extern_parser.add_argument('-o', '--output', required=True, help='Output .h file')
    
    # Generate symbol_addrs.txt
    syms_parser = subparsers.add_parser('symbol-addrs', help='Generate symbol_addrs.txt for splat')
    syms_parser.add_argument('rom', help='ROM file')
    syms_parser.add_argument('-o', '--output', required=True, help='Output file')
    
    # Generate all build files
    all_parser = subparsers.add_parser('all', help='Generate all build infrastructure')
    all_parser.add_argument('rom', help='ROM file')
    all_parser.add_argument('-d', '--output-dir', required=True, help='Output directory')
    
    args = parser.parse_args()
    
    if args.command == 'ld-script':
        with open(args.rom, 'rb') as f:
            rom_data = f.read()
        info = load_overlay_info(rom_data, args.overlay)
        generate_overlay_ld_script(info, Path(args.output))
        print(f"Generated linker script for overlay {args.overlay}")
    
    elif args.command == 'extern-header':
        with open(args.rom, 'rb') as f:
            rom_data = f.read()
        generate_extern_header(rom_data, Path(args.output))
        print(f"Generated extern header")
    
    elif args.command == 'symbol-addrs':
        with open(args.rom, 'rb') as f:
            rom_data = f.read()
        generate_symbol_addrs(rom_data, Path(args.output))
        print(f"Generated symbol addresses")
    
    elif args.command == 'all':
        with open(args.rom, 'rb') as f:
            rom_data = f.read()
        
        output_dir = Path(args.output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
        
        # Generate extern header
        generate_extern_header(rom_data, output_dir / 'jfg_extern.h')
        
        # Generate symbol_addrs
        generate_symbol_addrs(rom_data, output_dir / 'symbol_addrs_main.txt')
        
        # Generate linker scripts for all overlays
        ld_dir = output_dir / 'ld'
        ld_dir.mkdir(exist_ok=True)
        
        for i in range(1, ROM_OFFSETS['num_overlays'] + 1):
            try:
                info = load_overlay_info(rom_data, i)
                generate_overlay_ld_script(info, ld_dir / f'overlay_{i}.ld')
            except Exception as e:
                print(f"Warning: Could not generate ld script for overlay {i}: {e}")
        
        print(f"Generated build infrastructure in {output_dir}")
    
    else:
        parser.print_help()


if __name__ == '__main__':
    main()