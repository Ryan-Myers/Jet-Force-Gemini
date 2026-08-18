#!/usr/bin/env python3
"""
Scans asm/nonmatchings/**/*.s for `jal TrapDanglingJump` lines, resolves the
real overlay function being called (via overlay_which_func.resolve_symbol_name)
using the RAM address in the line's comment, and rewrites the line to call the
real symbol name instead of TrapDanglingJump.

Usage:
    python3 tools/patch_asm.py [--rom baseroms/baserom.us.z64] [--asm-dir asm/nonmatchings] [--dry-run]
"""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from overlay_which_func import resolve_symbol_name

# Matches a "/* ROM_OFFSET RAM_ADDR ENCODING */  jal  TrapDanglingJump" line.
JAL_TRAP_RE = re.compile(
    r'^(?P<prefix>\s*/\*\s*[0-9A-Fa-f]+\s+(?P<addr>[0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s*\*/\s*jal\s+)'
    r'TrapDanglingJump(?P<suffix>\s*)$'
)

def patch_asm_file(input_path, rom_data, symbol_cache, version):
    """Patch jal TrapDanglingJump lines in a single file. Returns True if the file was modified."""
    text = Path(input_path).read_text()
    lines = text.splitlines(keepends=True)
    modified = False

    for i, line in enumerate(lines):
        match = JAL_TRAP_RE.match(line.rstrip("\n"))
        if not match:
            continue

        addr = int(match.group("addr"), 16)
        if addr not in symbol_cache:
            symbol_cache[addr] = resolve_symbol_name(rom_data, addr, version)
        symbol_name = symbol_cache[addr]

        if not symbol_name:
            print(f"  WARNING: {input_path}:{i + 1} no symbol found for address 0x{addr:X}", file=sys.stderr)
            continue

        newline = "\n" if line.endswith("\n") else ""
        lines[i] = f"{match.group('prefix')}{symbol_name}{match.group('suffix')}{newline}"
        modified = True

    if modified:
        Path(input_path).write_text("".join(lines))

    return modified


def main():
    parser = argparse.ArgumentParser(description="Patch jal TrapDanglingJump lines with real symbol names")
    parser.add_argument("--rom", type=str, default="baseroms/baserom.us.z64", help="Path to the ROM file")
    parser.add_argument('-v', '--version', type=str, default='us', help='Version of the game (us or kiosk)')
    parser.add_argument("--asm-dir", type=str, default="asm/nonmatchings", help="Directory to scan for .s files")
    parser.add_argument("--dry-run", action="store_true", help="Report matches without writing changes")

    args = parser.parse_args()
    rom_data = Path(args.rom).read_bytes()

    symbol_cache = {}
    patched_files = 0
    for asm_path in sorted(Path(args.asm_dir).rglob("*.s")):
        text = asm_path.read_text()
        if "TrapDanglingJump" not in text:
            continue

        if args.dry_run:
            for i, line in enumerate(text.splitlines()):
                match = JAL_TRAP_RE.match(line)
                if not match:
                    continue
                addr = int(match.group("addr"), 16)
                if addr not in symbol_cache:
                    symbol_cache[addr] = resolve_symbol_name(rom_data, addr, args.version)
                print(f"{asm_path}:{i + 1} 0x{addr:X} -> {symbol_cache[addr]}")
            continue

        if patch_asm_file(asm_path, rom_data, symbol_cache, args.version):
            patched_files += 1
            print(f"Patched {asm_path}")

    if not args.dry_run:
        print(f"Done. Patched {patched_files} file(s).")


if __name__ == "__main__":
    main()