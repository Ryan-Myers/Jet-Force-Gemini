import struct
import argparse
from dataclasses import dataclass

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
    reloc_type_name: str = ""  # Optional: Name of the relocation type for easier debugging
    patch_type_name: str = ""  # Optional: Name of the relocation type for easier debugging
    
    def to_bytes(self) -> bytes:
        info = (self.target_offset << 8) | (self.patch_type << 4) | self.reloc_type
        return struct.pack(">II", self.symbol_index, info)
    
    @classmethod
    def from_bytes(cls, data: bytes) -> 'RelocationEntry':
        symbol_index, info = struct.unpack(">II", data)
        target_offset = (info >> 8) & 0xFFFFFF
        patch_type = (info >> 4) & 0xF
        reloc_type = info & 0xF
        reloc_type_name = {
            RELOC_TYPE_EXTERNAL: "EXTERNAL",
            RELOC_TYPE_LOCAL: "LOCAL",
            RELOC_TYPE_JUMP: "JUMP",
            RELOC_TYPE_DATA: "DATA"
        }.get(reloc_type, "UNKNOWN")
        patch_type_name = {
            RELOC_PATCH_WORD: "R_MIPS_32",
            RELOC_PATCH_JAL: "R_MIPS_26",
            RELOC_PATCH_HI16: "R_MIPS_HI16",
            RELOC_PATCH_LO16: "R_MIPS_LO16"
        }.get(patch_type, "UNKNOWN")
        return cls(symbol_index, target_offset, reloc_type, patch_type, reloc_type_name, patch_type_name)

def read_reloc_table_entry(reloc_data: bytes, index: int) -> RelocationEntry:
    """Read a single relocation table entry."""
    offset = index * 8
    entry_data = reloc_data[offset:offset + 8]
    return RelocationEntry.from_bytes(entry_data)


def main():
    parser = argparse.ArgumentParser(description='Reads all relocation entries from the given bin and prints them.')

    parser.add_argument('bin_path', type=str, help='Path to the overlay binary file')

    # Read the relocation table from the ROM file
    args = parser.parse_args()
    with open(args.bin_path, "rb") as f:
        reloc_data = f.read()

    num_entries = len(reloc_data) // 8
    print(f"Number of relocation entries: {num_entries}")
    print("RELOCATION RECORDS FOR [.text]:")
    print("OFFSET   TYPE              VALUE")

    for i in range(num_entries):
        entry = read_reloc_table_entry(reloc_data, i)
        print(f"{entry.target_offset:08X} {entry.patch_type_name:<17} {entry.reloc_type_name:<16}")
        # print(f"Relocation Table Entry {i:02}: " + 
        #       f"Index: {entry.symbol_index:05X}, Offset: {entry.target_offset:05X}, " + 
        #       f"Reloc Type: {entry.reloc_type_name}, Patch Type: {entry.patch_type_name}")

if __name__ == '__main__':
    main()
