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
- Section 0x001-0x09D: Overlay modules (1-157, dynamically loaded)
- Section 0xFFD: Data section (tuneSeqPlayer base)
- Section 0xFFE: Data section (tuneSeqPlayer base)
- Section 0xFFF: BSS section (gMusicSequenceData base)

Usage:
    python extract_symbols.py <rom_file> [options]

Options:
    --output, -o      Output file (default: stdout)
    --format, -f      Output format: 'map', 'names', 'linker', 'sections', 'splat'
    --version, -v     ROM version: 'kiosk' (default) or 'us'
    --section, -s     Filter by section number (e.g., 0 for main code)
    --main-only       Only output main code section (section 0)
    --overlay, -O     Filter by overlay number (e.g., 3 for overlay_3)
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
        'overlay_table': 0x1A7B2E0,   # Overlay table ROM address
        'overlay_data_base': 0x1A7C6A0,  # Overlay data base ROM address
        'num_overlays': 157,          # Number of overlay entries
    },
    'us': {
        'addr_table': 0x1ED0270,      # Address offset table
        'name_offsets': 0x1FEB040,    # Symbol name offset table
        'symbol_names': 0x1FED550,    # Symbol names data
        'base_addr': 0x80000450,      # Base address for main code section
        'data_base': 0x800A0660,      # Base address for data section (tuneSeqPlayer)
        'bss_base': 0x800B0B50,       # Base address for BSS section (gMusicSequenceData)
        'num_symbols': 2371,          # Number of symbols (from name table size)
        'overlay_table': 0x1ED2780,   # Overlay table ROM address
        'overlay_data_base': 0x1ED3B20,  # Overlay data base ROM address
        'num_overlays': 157,          # Number of overlay entries
    }
}

# Section type constants
SECTION_MAIN_CODE = 0x000
SECTION_DATA_1 = 0xFFD
SECTION_DATA_2 = 0xFFE  
SECTION_BSS = 0xFFF

# Overlay section range (sections 1-157 are overlays)
OVERLAY_SECTION_MIN = 0x001
OVERLAY_SECTION_MAX = 0x09D  # 157 in hex


def read_u32_be(data: bytes, offset: int) -> int:
    """Read a big-endian 32-bit unsigned integer."""
    return struct.unpack('>I', data[offset:offset+4])[0]


def read_null_terminated_string(data: bytes, offset: int) -> str:
    """Read a null-terminated ASCII string."""
    end = data.find(b'\x00', offset)
    if end == -1:
        end = len(data)
    return data[offset:end].decode('ascii', errors='replace')


def read_overlay_info(rom_data: bytes, offsets: dict, overlay_index: int) -> dict:
    """
    Read overlay header information for a specific overlay.
    
    Overlay header structure (32 bytes):
    - 0x00: VramBase (set at runtime, 0 if not loaded)
    - 0x04: RomAddress (offset from overlay_data_base)
    - 0x08: TextSize
    - 0x0C: DataSize
    - 0x10: BssSize
    - 0x14: RelocationTableSize (u16)
    - 0x16: SecondaryRelocationTableSize (u16)
    - 0x18: InitFunction (offset or -1)
    - 0x1C: ResumeFunction (offset or -1)
    """
    table_offset = offsets['overlay_table'] + (overlay_index - 1) * 32
    
    header = struct.unpack('>iiiiiHHii', rom_data[table_offset:table_offset + 32])
    
    return {
        'vram_base': header[0],
        'rom_offset': header[1],
        'text_size': header[2],
        'data_size': header[3],
        'bss_size': header[4],
        'reloc_count': header[5],
        'secondary_reloc': header[6],
        'init_func': header[7],
        'resume_func': header[8],
        'rom_start': offsets['overlay_data_base'] + header[1],
    }


def is_overlay_section(section: int) -> bool:
    """Check if a section number corresponds to an overlay."""
    return OVERLAY_SECTION_MIN <= section <= OVERLAY_SECTION_MAX


def get_section_name(section: int) -> str:
    """Get a human-readable name for a section."""
    if section == SECTION_MAIN_CODE:
        return "main"
    elif section == SECTION_DATA_1:
        return "data_ffd"
    elif section == SECTION_DATA_2:
        return "data_ffe"
    elif section == SECTION_BSS:
        return "bss"
    elif is_overlay_section(section):
        return f"overlay_{section}"
    else:
        return f"section_{section:03X}"


def decode_address(raw_value: int, offsets: dict) -> tuple:
    """
    Decode a raw address table entry into section and computed address.
    
    Returns: (section, offset_within_section, computed_address_or_none)
    
    The address table encodes:
    - High 12 bits (>> 20): section/module number
    - Low 20 bits (& 0xFFFFF): offset within section
    
    Only section 0 (main code) and special sections (0xFFD, 0xFFE, 0xFFF)
    can be resolved to absolute addresses. Overlay sections (1-157) have
    addresses computed at runtime when the overlay is loaded.
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
    elif is_overlay_section(section):
        # Overlay module - address is offset from overlay's VRAM base (set at runtime)
        # Return None for address since it's dynamic
        return (section, offset, None)
    else:
        # Unknown section - address not known
        return (section, offset, None)


def extract_symbols(rom_data: bytes, version: str = 'kiosk', 
                    section_filter: int = None, main_only: bool = False,
                    overlay_filter: int = None) -> list:
    """
    Extract symbols from ROM data.
    
    Args:
        rom_data: The ROM file contents
        version: ROM version ('kiosk' or 'us')
        section_filter: If specified, only extract symbols from this section
        main_only: If True, only extract main code section (section 0)
        overlay_filter: If specified, only extract symbols from this overlay number
    
    Returns a list of tuples: (address, symbol_name, section, offset)
    Address is None for dynamically loaded modules (overlays).
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
        if overlay_filter is not None:
            if not is_overlay_section(section) or section != overlay_filter:
                continue
        
        # Read the symbol name offset
        name_offset = read_u32_be(rom_data, name_offsets + i * 4)
        
        # Read the symbol name
        name = read_null_terminated_string(rom_data, symbol_names + name_offset)
        
        # Deduplicate - skip if we've seen this exact (address, name) pair
        key = (address, name, section)
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
        elif is_overlay_section(section):
            # Overlay symbol - show overlay and offset
            lines.append(f"overlay_{section}+0x{offset:05X}  ->  {name}")
        else:
            # Other dynamic section
            lines.append(f"SEC{section:03X}+{offset:05X}  ->  {name}")
    
    return '\n'.join(lines)


def format_symbols_names(symbols: list) -> str:
    """Format symbols as names only (one per line)."""
    return '\n'.join(name for _, name, _, _ in symbols)

def format_symbols_linker(symbols: list) -> str:
    """Format symbols as linker script entries."""
    lines = []
    
    for address, name, section, offset in symbols:
        if address is not None:
            lines.append(f"{name} = 0x{address:08X};")
        # Skip dynamic module symbols for linker format
    
    return '\n'.join(lines)


def format_symbols_splat(symbols: list) -> str:
    """
    Format symbols for splat symbol_addrs file.
    
    For main code: name = address; // type:func (or type:data)
    For overlays: name = offset; // segment:overlay_N type:func
    
    The segment: tag corresponds to the segment name in the splat YAML,
    which should have exclusive_ram_id set to prevent cross-overlay symbol resolution.
    """
    lines = []
    
    for address, name, section, offset in symbols:
        if section == SECTION_MAIN_CODE:
            # Main code - use absolute address
            lines.append(f"{name} = 0x{address:08X}; // type:func")
        elif is_overlay_section(section):
            # Overlay - use fake VRAM (0xF0000000 + offset) and segment tag
            # The segment: tag tells splat which segment this symbol belongs to,
            # matching the segment name in the YAML (e.g., overlay_3)
            # fake_addr = 0xF0000000 + offset
            fake_addr = (section << 20) + offset # Use the overlay index to create a unique VRAM base
            lines.append(f"{name} = 0x{fake_addr:08X}; // segment:overlay_{section} type:func")
        elif address is not None:
            # Data/BSS sections
            sym_type = "data" if section in (SECTION_DATA_1, SECTION_DATA_2) else "data"
            lines.append(f"{name} = 0x{address:08X}; // type:{sym_type}")
    
    return '\n'.join(lines)


def generate_overlay_symbol_files(rom_data: bytes, version: str, output_dir: Path) -> None:
    """
    Generate individual symbol files for each overlay.
    
    Creates files like:
        overlay_1.txt
        overlay_2.txt
        ...
    
    Each file contains symbols in splat format with:
    - Fake VRAM addresses (0xF0000000 + offset)
    - segment:overlay_N tag to associate symbol with the overlay segment
    - type:func tag for function type
    
    The segment: tag matches the segment name in the splat YAML,
    which should have exclusive_ram_id set to the same value.
    """
    offsets = ROM_OFFSETS[version]
    
    # Extract all symbols
    all_symbols = extract_symbols(rom_data, version)
    
    # Group by overlay
    from collections import defaultdict
    overlay_symbols = defaultdict(list)
    
    for address, name, section, offset in all_symbols:
        if is_overlay_section(section):
            overlay_symbols[section].append((name, offset))
    
    output_dir.mkdir(parents=True, exist_ok=True)
    lines = []
    
    for overlay_num in sorted(overlay_symbols.keys()):
        syms = sorted(overlay_symbols[overlay_num], key=lambda x: x[1])
        
        segment_name = f"overlay_{overlay_num}"
        
        for name, offset in syms:
            # Use fake VRAM at 0xF0000000 + offset for splat compatibility
            # Include segment: tag to associate with the overlay segment
            # fake_addr = 0xF0000000 + offset
            fake_addr = (overlay_num << 20) + offset # Use the overlay index to create a unique VRAM base
            lines.append(f"{name} = 0x{fake_addr:08X}; // segment:{segment_name} type:func")
        
    out_path = output_dir / f"overlays.{version}.txt"
    out_path.write_text('\n'.join(lines) + '\n')
    print(f"Wrote symbols to {out_path}", file=sys.stderr)


def format_symbols_sections(symbols: list) -> str:
    """Format symbols grouped by section, sorted by address order."""
    from collections import defaultdict
    
    sections = defaultdict(list)
    for address, name, section, offset in symbols:
        sections[section].append((address, name, offset))
    
    # Define section order: main code first, then overlays, then data, BSS
    def section_sort_key(sec):
        if sec == SECTION_MAIN_CODE:
            return (0, 0)  # First
        elif is_overlay_section(sec):
            return (1, sec)  # Second (overlays, sorted by number)
        elif sec == SECTION_DATA_1:
            return (2, 0)  # Third (data)
        elif sec == SECTION_DATA_2:
            return (2, 1)  # Fourth (data)
        elif sec == SECTION_BSS:
            return (3, 0)  # Fifth (BSS)
        else:
            return (4, sec)  # Unknown sections last
    
    lines = []
    for section in sorted(sections.keys(), key=section_sort_key):
        syms = sections[section]
        
        # Sort symbols within section by offset
        syms.sort(key=lambda x: x[2])  # Sort by offset (third element)
        
        section_name = get_section_name(section)
        
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
                        choices=['map', 'names', 'linker', 'sections', 'splat'],
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
    parser.add_argument('-O', '--overlay', type=int,
                        help='Filter by overlay number (e.g., 3 for overlay_3)')
    parser.add_argument('--generate-overlay-files', metavar='DIR',
                        help='Generate individual symbol files for each overlay in DIR')
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
    
    # Handle --generate-overlay-files option
    if args.generate_overlay_files:
        generate_overlay_symbol_files(rom_data, version, Path(args.generate_overlay_files))
        return
    
    # Extract symbols
    try:
        symbols = extract_symbols(rom_data, version, 
                                  section_filter=args.section,
                                  main_only=args.main_only,
                                  overlay_filter=args.overlay)
    except Exception as e:
        print(f"Error extracting symbols: {e}", file=sys.stderr)
        sys.exit(1)
    
    print(f"Extracted {len(symbols)} symbols", file=sys.stderr)
    
    # Sort if requested
    if args.sort == 'address':
        # Sort by address, putting None addresses at the end
        symbols.sort(key=lambda x: (x[0] is None, x[0] or 0, x[3]))
    elif args.sort == 'name':
        symbols.sort(key=lambda x: x[1].lower())
    
    # Format output
    formatters = {
        'map': format_symbols_map,
        'names': format_symbols_names,
        'linker': format_symbols_linker,
        'sections': format_symbols_sections,
        'splat': format_symbols_splat,
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
