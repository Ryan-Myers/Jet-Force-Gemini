#!/usr/bin/env python3
"""
This script is used to determine which function is actually being called by a call to TrapDanglingJump in the main code.
Usage:
    python3 overlay_which_func.py <rom_path> <ram_address_that_calls_TrapDanglingJump>
Example:
    python3 overlay_which_func.py baseroms/baserom.us.z64 0x8004DD50
"""

import argparse
import struct
import sys
from pathlib import Path

BASE_ADDRESS = 0x80000450

# ROM addresses for US version
MAIN_RELOC_TABLE = 0x1ECF220
OVERLAY_ROM_TABLE = 0x1ED0270
OVERLAY_TABLE  = 0x1ED2780
OVERLAY_DATA_BASE = 0x1ED3B20
SYMBOL_TABLE_OFFSETS = 0x1FEB040
SYMBOL_TABLE_NAMES = 0x1FED550

def read_reloc_table_entry(rom_data, index):
    """Read a single relocation table entry."""
    offset = MAIN_RELOC_TABLE + ((index) * 8)
    fields = struct.unpack(">ii", rom_data[offset:offset + 8])
    return {
        'function_address': (fields[0] >> 8) & 0xFFFFFF,  # The function address is stored in the upper 24 bits
        'unknown': fields[0] & 0xFF, # Almost always 0x40. Not sure what this is for.
        'overlay_index': fields[1],
    }

def read_overlay_rom_table_entry(rom_data, index):
    """Read a single overlay ROM table entry."""
    offset = OVERLAY_ROM_TABLE + ((index) * 4)
    fields = struct.unpack(">i", rom_data[offset:offset + 4])
    return {
        'overlay_number': (fields[0] >> 20) & 0xFFF,
        'function_offset': fields[0] & 0xFFFFF,
    }

def get_symbol_name(rom_data, index):
    """Get the symbol name for a given address from the symbol table."""
    offset = SYMBOL_TABLE_OFFSETS + ((index - 1) * 4)
    symbol_offset = struct.unpack(">I", rom_data[offset:offset + 4])[0]
    if symbol_offset == 0:
        return None
    name_offset = SYMBOL_TABLE_NAMES + symbol_offset
    name_bytes = bytearray()
    while True:
        byte = rom_data[name_offset]
        if byte == 0:
            break
        name_bytes.append(byte)
        name_offset += 1
    return name_bytes.decode('ascii')

def main():
    parser = argparse.ArgumentParser(description="JFG Which Overlay Function")
    parser.add_argument('rom_path', type=str, help='Path to the ROM file')
    parser.add_argument('ram_address', type=lambda x: int(x, 0), help='RAM address that calls TrapDanglingJump')

    args = parser.parse_args()
    entry_value = (args.ram_address - BASE_ADDRESS)

    rom_data = Path(args.rom_path).read_bytes()

    for i in range(0, 1000):  # Arbitrary limit to avoid infinite loop
        entry = read_reloc_table_entry(rom_data, i)
        if entry['function_address'] == entry_value:
            print(f"Found matching entry at index {i}:")
            print(f"Function Address: 0x{entry['function_address']:X}")
            print(f"Overlay Index: {entry['overlay_index']}")
            overlay_entry = read_overlay_rom_table_entry(rom_data, entry['overlay_index'])
            print(f"Overlay Number: {overlay_entry['overlay_number']}")
            print(f"Function Offset: 0x{overlay_entry['function_offset']:X}")
            symbol_name = get_symbol_name(rom_data, entry['overlay_index'])
            if symbol_name:
                print(f"Symbol Name: {symbol_name}")
            else:
                print("Symbol Name: Not found")
            return

if __name__ == "__main__":
    main()