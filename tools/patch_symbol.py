#!/usr/bin/env python3

import sys
import struct
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection, Symbol


def patch_symbol(input_path, output_path, old_name,
                 new_name="TrapDanglingJump"):

    with open(input_path, "rb") as f:
        data = bytearray(f.read())

    with open(input_path, "rb") as f:
        elf = ELFFile(f)

        symtab : SymbolTableSection = elf.get_section_by_name(".symtab")
        strtab : SymbolTableSection = elf.get_section_by_name(".strtab")

        if symtab is None:
            raise RuntimeError("No .symtab found")

        if strtab is None:
            raise RuntimeError("No .strtab found")

        target_index : int = 0
        target_symbol : Symbol | None = None

        # For now assume it only found one symbol with that name. 
        # If there are multiple, we should probably patch all of them, but that would be a more complex change.
        
        for index, sym in enumerate(symtab.iter_symbols()):
            if sym.name == old_name:
                target_index = index
                target_symbol = sym
                break

        if target_symbol is None:
            raise RuntimeError(f"Symbol '{old_name}' not found")

        print(f"Found symbol {target_index}: {old_name}")

        strtab_offset = strtab["sh_offset"] # Section file offset
        strtab_size = strtab["sh_size"] # Section size in bytes

        old_strtab = bytes(
            data[
                strtab_offset: # Start
                strtab_offset + strtab_size # End
            ]
        )

        old_st_name : int = target_symbol["st_name"]  # Symbol name offset into .strtab

        # Verify the symbol really points to old_name
        actual_name = (
            old_strtab[old_st_name:]
            .split(b"\0", 1)[0]
            .decode("ascii")
        )

        if actual_name != old_name:
            raise RuntimeError(
                f"Symbol st_name={old_st_name} points to "
                f"'{actual_name}', not '{old_name}'"
            )

        # Don't replace the old string. Append the new name instead.

        new_name_bytes = new_name.encode("ascii") + b"\0"

        new_st_name = len(old_strtab)

        new_strtab = old_strtab + new_name_bytes

        e_shoff : int = elf.header["e_shoff"] # Section header table file offset
        e_shentsize : int = elf.header["e_shentsize"] # Section header table entry size

        symtab_index = elf.get_section_index(".symtab")
        strtab_index = elf.get_section_index(".strtab")

        if symtab_index is None:
            raise RuntimeError(
                "Could not determine .symtab index"
            )

        if strtab_index is None:
            raise RuntimeError(
                "Could not determine .strtab index"
            )

        # Append the new .strtab.
        # The original .strtab remains untouched.

        new_strtab_offset = len(data)

        data.extend(new_strtab)

        # struct {
        #     /* 0x00 */u32 sh_name;
        #     /* 0x04 */u32 sh_type;
        #     /* 0x08 */u32 sh_flags;
        #     /* 0x0C */u32 sh_addr;
        #     /* 0x10 */u32 sh_offset;
        #     /* 0x14 */u32 sh_size;
        # } SectionHeader;

        strtab_shdr_offset = (e_shoff + (strtab_index * e_shentsize))

        # Write the new .strtab offset
        struct.pack_into(
            ">I", # Big-endian unsigned int
            data,
            strtab_shdr_offset + 0x10, # sh_offset
            new_strtab_offset
        )

        # Write the new .strtab size
        struct.pack_into(
            ">I", # Big-endian unsigned int
            data,
            strtab_shdr_offset + 0x14, # sh_size
            len(new_strtab)
        )

        # struct {
        #     /* 0x00 */ u32 st_name;
        #     /* 0x04 */ u32 st_value;
        #     /* 0x08 */ u32 st_size;
        #     /* 0x0C */ u8  st_info;
        #     /* 0x0D */ u8  st_other;
        #     /* 0x0E */ u16 st_shndx;
        # } Symbol;

        symtab_offset = symtab["sh_offset"]
        symtab_entsize = symtab["sh_entsize"]

        symbol_offset = (
            symtab_offset +
            target_index * symtab_entsize
        )

        struct.pack_into(
            ">I", # Big-endian unsigned int
            data,
            symbol_offset,
            new_st_name
        )

    with open(output_path, "wb") as f:
        f.write(data)

    print(f"Patched {old_name} -> {new_name}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    

    patch_symbol(sys.argv[1], sys.argv[2], "mantisLightingGetFog")
    patch_symbol(sys.argv[1], sys.argv[2], "dayGetFog")