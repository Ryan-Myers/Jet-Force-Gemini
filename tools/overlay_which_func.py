#!/usr/bin/env python3
"""
This script is used to determine which function is actually being called by a call to TrapDanglingJump in the main code.
Usage:
    python3 overlay_which_func.py <rom_path> <ram_address_that_calls_TrapDanglingJump>
Example:
    python3 overlay_which_func.py baseroms/baserom.us.z64 0x8004DD50

Can also be imported and used as a library:
    from overlay_which_func import resolve_symbol_name
    name = resolve_symbol_name(rom_data, 0x8004DD50)
"""

import argparse
import struct
import sys
from pathlib import Path

BASE_ADDRESS = 0x80000450

# ROM offsets
ROM_OFFSETS = {
    'us': {
        'main_reloc_table_count': 0x1ECF220,
        'main_reloc_table': 0x1ECF224,
        'overlay_rom_table': 0x1ED0270,
        'symbol_table_offsets': 0x1FEB040,
        'symbol_table_names': 0x1FED550,
    },
    'kiosk': {
        'main_reloc_table_count': 0x1A7C4D0,
        'main_reloc_table': 0x1A7C4D4,
        'overlay_rom_table': 0x1A7D500,
        'symbol_table_offsets': 0x1B94430,
        'symbol_table_names': 0x1B96910,
    }
}

# The table has a 4-byte count at the beginning, followed by 8-byte entries.
def read_reloc_table_count(rom_data, version):
    """Read the number of entries in the relocation table."""
    offset = ROM_OFFSETS[version]['main_reloc_table_count']
    count = struct.unpack(">I", rom_data[offset:offset + 4])[0]
    return count

def read_reloc_table_entry(rom_data, index, version):
    """Read a single relocation table entry."""
    offset = ROM_OFFSETS[version]['main_reloc_table'] + ((index) * 8)
    fields = struct.unpack(">ii", rom_data[offset:offset + 8])
    return {
        'targetOffset': (fields[1] >> 8) & 0xFFFFFF,  # The function address is stored in the upper 24 bits
        'patchOperation': (fields[1] & 0xFF) >> 4, # 2=R_MIPS_32, 4=JAL, 5=HI16, 6=LO16
        'relocType': (fields[1] & 0xF), # Relocation type (R_MIPS_32=0, LOCAL=1, R_MIPS_26=2, SPECIAL=3, HI16=5, LO16=6)
        'overlay_index': fields[0],
    }

def read_overlay_rom_table_entry(rom_data, index, version):
    """Read a single overlay ROM table entry."""
    offset = ROM_OFFSETS[version]['overlay_rom_table'] + ((index - 1) * 4)
    fields = struct.unpack(">i", rom_data[offset:offset + 4])
    return {
        'overlay_number': (fields[0] >> 20) & 0xFFF,
        'function_offset': fields[0] & 0xFFFFF,
    }

def get_symbol_name(rom_data, index, version):
    """Get the symbol name for a given address from the symbol table."""
    offset = ROM_OFFSETS[version]['symbol_table_offsets'] + ((index) * 4)
    symbol_offset = struct.unpack(">I", rom_data[offset:offset + 4])[0]
    if symbol_offset == 0:
        return None
    name_offset = ROM_OFFSETS[version]['symbol_table_names'] + symbol_offset
    name_bytes = bytearray()
    while True:
        byte = rom_data[name_offset]
        if byte == 0:
            break
        name_bytes.append(byte)
        name_offset += 1
    return name_bytes.decode('ascii')

def resolve_symbol_name(rom_data, ram_address, version):
    """Look up the overlay symbol name for a RAM address that calls TrapDanglingJump. Returns None if no match is found."""
    entry_value = ram_address - BASE_ADDRESS
    reloc_table_count = read_reloc_table_count(rom_data, version)

    for i in range(0, reloc_table_count - 1):
        entry = read_reloc_table_entry(rom_data, i, version)
        if entry['targetOffset'] == entry_value:
            return get_symbol_name(rom_data, entry['overlay_index'], version)
    return None

def main():
    parser = argparse.ArgumentParser(description="JFG Which Overlay Function")
    parser.add_argument('rom_path', type=str, help='Path to the ROM file')
    parser.add_argument('ram_address', type=lambda x: int(x, 0), help='RAM address that calls TrapDanglingJump')
    parser.add_argument('-v', '--version', type=str, default='us', help='Version of the game (us or kiosk)')

    args = parser.parse_args()
    rom_data = Path(args.rom_path).read_bytes()

    symbol_name = resolve_symbol_name(rom_data, args.ram_address, args.version)
    if symbol_name:
        print(f"Symbol Name: {symbol_name}")
    else:
        print("Symbol Name: Not found")

if __name__ == "__main__":
    main()