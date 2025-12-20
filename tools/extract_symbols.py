#!/usr/bin/env python3
"""
Extract symbol names and addresses from Jet Force Gemini ROM.

The ROM contains embedded debugging information:
- Address offset table: contains encoded section + offset values
  - High 12 bits (>> 20): section/module number
  - Low 20 bits (& 0xFFFFF): offset within section
- Symbol name offset table: contains offsets into the symbol names data
- Symbol names data: null-terminated ASCII strings

Section mapping:
- Section 0x000: Main code (functions loaded at 0x80000450)
- Section 0xFFD: Data section (tuneSeqPlayer base)
- Section 0xFFE: Data section (tuneSeqPlayer base)
- Section 0xFFF: BSS section (gMusicSequenceData base)
- Other sections: Dynamically loaded modules (addresses computed at runtime)

Usage:
    python extract_symbols.py <rom_file> [options]

Options:
    --output, -o      Output file (default: stdout)
    --format, -f      Output format: 'map', 'names', 'c_header', 'linker', 'sections'
    --version, -v     ROM version: 'kiosk' (default) or 'us'
    --section, -s     Filter by section number (e.g., 0 for main code)
    --main-only       Only output main code section (section 0)
    --sort            Sort output by 'address', 'name', or 'none' (default: address)
"""

import argparse
import struct
import sys
from pathlib import Path


# ROM offsets for different versions
ROM_OFFSETS = {
    'kiosk': {
        'addr_table': 0x1A7D500,      # Address offset table
        'name_offsets': 0x1B94430,    # Symbol name offset table  
        'symbol_names': 0x1B96910,    # Symbol names data
        'base_addr': 0x80000450,      # Base address for main code section
        'data_base': 0x800A0EF0,      # Base address for data section (tuneSeqPlayer)
        'bss_base': 0x800B16B0,       # Base address for BSS section (gMusicSequenceData)
        'num_symbols': 2359,          # Number of symbols (from name table size)
    },
    'us': {
        'addr_table': 0x1ED0270,      # Address offset table
        'name_offsets': 0x1FEB040,    # Symbol name offset table
        'symbol_names': 0x1FED550,    # Symbol names data
        'base_addr': 0x80000450,      # Base address for main code section
        'data_base': 0x800A0AA0,      # Base address for data section (tuneSeqPlayer)
        'bss_base': 0x800B1260,       # Base address for BSS section (gMusicSequenceData)
        'num_symbols': 2371,          # Number of symbols (from name table size)
    }
}

# Section type constants
SECTION_MAIN_CODE = 0x000
SECTION_DATA_1 = 0xFFD
SECTION_DATA_2 = 0xFFE  
SECTION_BSS = 0xFFF


def read_u32_be(data: bytes, offset: int) -> int:
    """Read a big-endian 32-bit unsigned integer."""
    return struct.unpack('>I', data[offset:offset+4])[0]


def read_null_terminated_string(data: bytes, offset: int) -> str:
    """Read a null-terminated ASCII string."""
    end = data.find(b'\x00', offset)
    if end == -1:
        end = len(data)
    return data[offset:end].decode('ascii', errors='replace')


def decode_address(raw_value: int, offsets: dict) -> tuple:
    """
    Decode a raw address table entry into section and computed address.
    
    Returns: (section, offset_within_section, computed_address_or_none)
    
    The address table encodes:
    - High 12 bits (>> 20): section/module number
    - Low 20 bits (& 0xFFFFF): offset within section
    
    Only section 0 (main code) and special sections (0xFFD, 0xFFE, 0xFFF)
    can be resolved to absolute addresses. Other sections are dynamically
    loaded modules whose addresses are computed at runtime.
    """
    section = raw_value >> 20
    offset = raw_value & 0xFFFFF
    
    if section == SECTION_MAIN_CODE:
        # Main code section - add to base address
        address = offsets['base_addr'] + offset
        return (section, offset, address)
    elif section == SECTION_DATA_1 or section == SECTION_DATA_2:
        # Data section
        address = offsets['data_base'] + offset
        return (section, offset, address)
    elif section == SECTION_BSS:
        # BSS section
        address = offsets['bss_base'] + offset
        return (section, offset, address)
    else:
        # Dynamic module - address not known at extraction time
        return (section, offset, None)


def extract_symbols(rom_data: bytes, version: str = 'kiosk', 
                    section_filter: int = None, main_only: bool = False) -> list:
    """
    Extract symbols from ROM data.
    
    Args:
        rom_data: The ROM file contents
        version: ROM version ('kiosk' or 'us')
        section_filter: If specified, only extract symbols from this section
        main_only: If True, only extract main code section (section 0)
    
    Returns a list of tuples: (address, symbol_name, section, offset)
    Address is None for dynamically loaded modules.
    """
    offsets = ROM_OFFSETS[version]
    
    addr_table = offsets['addr_table']
    name_offsets = offsets['name_offsets']
    symbol_names = offsets['symbol_names']
    num_symbols = offsets['num_symbols']
    
    symbols = []
    seen = set()  # Track (address, name) pairs to deduplicate
    
    for i in range(num_symbols):
        # Read raw address value from the address table
        raw_value = read_u32_be(rom_data, addr_table + i * 4)
        
        # Decode into section and address
        section, offset_in_section, address = decode_address(raw_value, offsets)
        
        # Apply filters
        if main_only and section != SECTION_MAIN_CODE:
            continue
        if section_filter is not None and section != section_filter:
            continue
        
        # Read the symbol name offset
        name_offset = read_u32_be(rom_data, name_offsets + i * 4)
        
        # Read the symbol name
        name = read_null_terminated_string(rom_data, symbol_names + name_offset)
        
        # Deduplicate - skip if we've seen this exact (address, name) pair
        key = (address, name)
        if key in seen:
            continue
        seen.add(key)
        
        symbols.append((address, name, section, offset_in_section))
    
    return symbols


def format_symbols_map(symbols: list) -> str:
    """Format symbols as address -> name mapping."""
    lines = []
    lines.append("")
    lines.append("---------------------- functions (main)")
    
    for address, name, section, offset in symbols:
        if address is not None:
            lines.append(f"{address:08X}  ->  {name}")
        else:
            # For dynamic modules, show section and offset
            lines.append(f"SEC{section:03X}+{offset:05X}  ->  {name}")
    
    return '\n'.join(lines)


def format_symbols_names(symbols: list) -> str:
    """Format symbols as names only (one per line)."""
    return '\n'.join(name for _, name, _, _ in symbols)


def format_symbols_c_header(symbols: list) -> str:
    """Format symbols as C header with extern declarations."""
    lines = []
    lines.append("/* Auto-generated symbol declarations */")
    lines.append("#ifndef SYMBOLS_H")
    lines.append("#define SYMBOLS_H")
    lines.append("")
    
    for address, name, section, offset in symbols:
        if address is not None:
            lines.append(f"extern void {name}(void); /* 0x{address:08X} */")
        else:
            lines.append(f"extern void {name}(void); /* section {section:03X} + 0x{offset:05X} */")
    
    lines.append("")
    lines.append("#endif /* SYMBOLS_H */")
    
    return '\n'.join(lines)


def format_symbols_linker(symbols: list) -> str:
    """Format symbols as linker script entries."""
    lines = []
    
    for address, name, section, offset in symbols:
        if address is not None:
            lines.append(f"{name} = 0x{address:08X};")
        # Skip dynamic module symbols for linker format
    
    return '\n'.join(lines)


def format_symbols_sections(symbols: list) -> str:
    """Format symbols grouped by section."""
    from collections import defaultdict
    
    sections = defaultdict(list)
    for address, name, section, offset in symbols:
        sections[section].append((address, name, offset))
    
    lines = []
    for section in sorted(sections.keys()):
        syms = sections[section]
        if section == SECTION_MAIN_CODE:
            section_name = "Main Code"
        elif section == SECTION_DATA_1:
            section_name = "Data (FFD)"
        elif section == SECTION_DATA_2:
            section_name = "Data (FFE)"
        elif section == SECTION_BSS:
            section_name = "BSS"
        else:
            section_name = f"Module {section:03X}"
        
        lines.append(f"\n=== Section 0x{section:03X}: {section_name} ({len(syms)} symbols) ===")
        
        for address, name, offset in syms:
            if address is not None:
                lines.append(f"  {address:08X}  {name}")
            else:
                lines.append(f"  +0x{offset:05X}  {name}")
    
    return '\n'.join(lines)


def auto_detect_version(rom_data: bytes) -> str:
    """Try to auto-detect the ROM version."""
    # Check ROM header for game code
    # JFG NTSC-U: "NJDE" at 0x3B-0x3E
    # JFG Kiosk: Similar but may differ
    
    if len(rom_data) < 0x40:
        return 'kiosk'
    
    game_code = rom_data[0x3B:0x3F].decode('ascii', errors='replace')
    
    # Try to detect based on ROM size or header
    # For now, default to kiosk since that's what we have info for
    return 'kiosk'


def main():
    parser = argparse.ArgumentParser(
        description='Extract symbol names and addresses from Jet Force Gemini ROM'
    )
    parser.add_argument('rom_file', help='Path to the ROM file')
    parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    parser.add_argument('-f', '--format', 
                        choices=['map', 'names', 'c_header', 'linker', 'sections'],
                        default='map',
                        help='Output format (default: map)')
    parser.add_argument('-v', '--version',
                        choices=['kiosk', 'us', 'auto'],
                        default='auto',
                        help='ROM version (default: auto)')
    parser.add_argument('-s', '--section', type=lambda x: int(x, 0),
                        help='Filter by section number (e.g., 0 or 0x0 for main code)')
    parser.add_argument('--main-only', action='store_true',
                        help='Only output main code section (section 0)')
    parser.add_argument('--sort', choices=['address', 'name', 'none'],
                        default='address',
                        help='Sort output by address or name (default: address)')
    
    args = parser.parse_args()
    
    # Read ROM file
    rom_path = Path(args.rom_file)
    if not rom_path.exists():
        print(f"Error: ROM file not found: {rom_path}", file=sys.stderr)
        sys.exit(1)
    
    with open(rom_path, 'rb') as f:
        rom_data = f.read()
    
    # Auto-detect version if needed
    version = args.version
    if version == 'auto':
        version = auto_detect_version(rom_data)
        print(f"Auto-detected ROM version: {version}", file=sys.stderr)
    
    # Extract symbols
    try:
        symbols = extract_symbols(rom_data, version, 
                                  section_filter=args.section,
                                  main_only=args.main_only)
    except Exception as e:
        print(f"Error extracting symbols: {e}", file=sys.stderr)
        sys.exit(1)
    
    print(f"Extracted {len(symbols)} symbols", file=sys.stderr)
    
    # Sort if requested
    if args.sort == 'address':
        # Sort by address, putting None addresses at the end
        symbols.sort(key=lambda x: (x[0] is None, x[0] or 0))
    elif args.sort == 'name':
        symbols.sort(key=lambda x: x[1].lower())
    
    # Format output
    formatters = {
        'map': format_symbols_map,
        'names': format_symbols_names,
        'c_header': format_symbols_c_header,
        'linker': format_symbols_linker,
        'sections': format_symbols_sections,
    }
    
    output = formatters[args.format](symbols)
    
    # Write output
    if args.output:
        with open(args.output, 'w') as f:
            f.write(output)
            f.write('\n')
        print(f"Output written to: {args.output}", file=sys.stderr)
    else:
        print(output)


if __name__ == '__main__':
    main()
