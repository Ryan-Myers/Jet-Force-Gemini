#!/usr/bin/env python3
"""
Extract overlay information from Jet Force Gemini ROM and generate splat YAML segments.

This script reads the overlayTable from the ROM and generates splat-compatible
YAML segment definitions for each overlay.

Usage:
    python3 tools/extract_overlays.py [--rom path/to/rom.z64] [--output overlays.yaml]
"""

import argparse
import struct
import sys
from pathlib import Path

# ROM addresses for Kiosk version
# OVERLAY_TABLE_ROM = 0x1A7F9E0
# OVERLAY_DATA_ROM = 0x1A80D60  # D_1A80D60 - start of overlay binary data

# ROM addresses for US version
OVERLAY_TABLE_ROM = 0x1ED2780
OVERLAY_DATA_ROM = 0x1ED3B20  # D_1ED3B20 - start of overlay binary data

# OverlayHeader structure (32 bytes):
# typedef struct OverlayHeader {
#     /* 0x00 */ s32 VramBase;           // (0 if not loaded, set after alloc)
#     /* 0x04 */ s32 RomAddress;         // Offset from OVERLAY_DATA_ROM
#     /* 0x08 */ s32 TextSize;
#     /* 0x0C */ s32 DataSize;           // Includes rodata (merged with data)
#     /* 0x10 */ s32 BssSize;            // Uninitialized data (zeroed at load)
#     /* 0x14 */ u16 RelocationTableSize;
#     /* 0x16 */ u16 SecondaryRelocationTableSize;
#     /* 0x18 */ s32 InitFunction;       // -1 if none, offset from VramBase
#     /* 0x1C */ s32 ResumeFunction;     // -1 if none, offset from VramBase
# } OverlayHeader;
#
# Memory layout at runtime: [text][data][bss][reloc_table]
# ROM layout:               [text][data][reloc_table]
#
# Notes:
# - BSS is zeroed at load time, not stored in ROM
# - Rodata is merged into the data section (no separate .rodata)
# - Overlays can OVERLAP in ROM (mutually exclusive overlays share space)
# - RelocationTableSize is number of 8-byte entries

OVERLAY_HEADER_SIZE = 0x20  # 32 bytes
OVERLAY_HEADER_FORMAT = ">iiiiihhi i"  # Big-endian: 5 ints, 2 shorts, 2 ints


class OverlayHeader:
    def __init__(self, data: bytes, index: int):
        self.index = index
        fields = struct.unpack(OVERLAY_HEADER_FORMAT, data)
        self.vram_base = fields[0]
        self.rom_address = fields[1]  # Offset from overlay data start
        self.text_size = fields[2]
        self.data_size = fields[3]
        self.bss_size = fields[4]     # BSS size (zeroed at runtime, not in ROM)
        self.reloc_table_size = fields[5]
        self.secondary_reloc_size = fields[6]
        self.init_function = fields[7]
        self.resume_function = fields[8]

    @property
    def absolute_rom(self) -> int:
        """Get absolute ROM address of the overlay"""
        return OVERLAY_DATA_ROM + self.rom_address

    @property
    def rom_size(self) -> int:
        """Size of the overlay in ROM (text + data + reloc, NO bss)"""
        return self.text_size + self.data_size + (self.reloc_table_size * 8)

    @property
    def vram_size(self) -> int:
        """Size of the overlay in VRAM (text + data + bss + reloc)"""
        return self.text_size + self.data_size + self.bss_size + (self.reloc_table_size * 8)

    @property
    def is_valid(self) -> bool:
        """Check if this overlay entry is valid"""
        return self.text_size > 0 or self.data_size > 0

    def __repr__(self):
        return (
            f"Overlay[{self.index}]("
            f"rom=0x{self.absolute_rom:X}, "
            f"vram=0x{self.vram_base:08X}, "
            f"text=0x{self.text_size:X}, "
            f"data=0x{self.data_size:X}, "
            f"bss=0x{self.bss_size:X}, "
            f"reloc={self.reloc_table_size})"
        )


def read_overlay_table(rom_path: Path) -> list[OverlayHeader]:
    """Read the overlay table from the ROM"""
    overlays = []
    
    with open(rom_path, "rb") as f:
        # Calculate table size
        table_size = OVERLAY_DATA_ROM - OVERLAY_TABLE_ROM
        # The table contains (table_size / 32) entries
        # Note: runtime uses (table_size >> 5) + 1 but that includes one past the last valid
        overlay_count = table_size // OVERLAY_HEADER_SIZE
        
        print(f"# Overlay table at ROM 0x{OVERLAY_TABLE_ROM:X}", file=sys.stderr)
        print(f"# Overlay data starts at ROM 0x{OVERLAY_DATA_ROM:X}", file=sys.stderr)
        print(f"# Table size: 0x{table_size:X} bytes ({table_size} bytes)", file=sys.stderr)
        print(f"# Overlay count: {overlay_count}", file=sys.stderr)
        print(file=sys.stderr)
        
        f.seek(OVERLAY_TABLE_ROM)
        
        for i in range(overlay_count):
            header_data = f.read(OVERLAY_HEADER_SIZE)
            if len(header_data) < OVERLAY_HEADER_SIZE:
                break
            
            overlay = OverlayHeader(header_data, i + 1)  # Indexing overlays from 1
            overlays.append(overlay)
    
    return overlays


def generate_splat_segment(overlay: OverlayHeader) -> str:
    """Generate a splat YAML segment for an overlay"""
    lines = []
    
    rom_start = overlay.absolute_rom
    vram = overlay.vram_base
    
    if overlay.vram_base == 0:
        #vram = 0xF0000000 # Default VRAM for overlays not loaded at fixed address
        vram = overlay.index << 20 # Use the overlay index to create a unique VRAM base
    
    # ROM layout: [text][data][reloc_table]
    # VRAM layout: [text][data][bss][reloc_table]
    # BSS is NOT in ROM - it's zeroed at load time
    
    text_start = rom_start
    text_end = text_start + overlay.text_size
    
    data_start = text_end
    data_end = data_start + overlay.data_size
    
    # Relocation table follows data in ROM (BSS is not stored in ROM)
    reloc_start = data_end
    reloc_size = overlay.reloc_table_size * 8  # Each reloc entry is 8 bytes
    reloc_end = reloc_start + reloc_size
    
    # Generate segment
    lines.append(f"  - name: overlay_{overlay.index}")
    lines.append(f"    type: code")
    lines.append(f"    start: 0x{rom_start:X}")
    lines.append(f"    vram: 0x{vram:X}")
    lines.append(f"    bss_size: 0x{overlay.bss_size:X}")
    lines.append(f"    align: 0x8")
    lines.append(f"    dir: overlays/o{overlay.index}")
    lines.append(f"    exclusive_ram_id: overlay_{overlay.index}")
    lines.append(f"    symbol_name_format: $SEG_$VRAM_$ROM")
    lines.append(f"    subsegments:")
    
    # Text section
    if overlay.text_size > 0:
        lines.append(f"      - [0x{text_start:X}, c, overlay_{overlay.index}]")
    
    # Data section
    if overlay.data_size > 0:
        lines.append(f"      - [0x{data_start:X}, data, overlay_{overlay.index}]")
    
    # BSS is declared in the segment header (bss_size), not as a subsegment
    # since it's not in ROM
    
    # Relocation table (as binary data)
    if overlay.reloc_table_size > 0:
        lines.append(f"      - [0x{reloc_start:X}, bin, overlay_{overlay.index}_reloc]")
    
    return "\n".join(lines)


def generate_overlay_info(overlay: OverlayHeader) -> str:
    """Generate detailed info comment for an overlay"""
    lines = []
    
    rom_start = overlay.absolute_rom
    
    lines.append(f"  # Overlay {overlay.index}:")
    lines.append(f"  #   ROM: 0x{rom_start:X} - 0x{rom_start + overlay.rom_size:X} (0x{overlay.rom_size:X} bytes)")
    lines.append(f"  #   VRAM size: 0x{overlay.vram_size:X} bytes")
    lines.append(f"  #   Text: 0x{overlay.text_size:X} bytes")
    lines.append(f"  #   Data: 0x{overlay.data_size:X} bytes")
    lines.append(f"  #   BSS: 0x{overlay.bss_size:X} bytes")
    lines.append(f"  #   Reloc entries: {overlay.reloc_table_size}")
    
    if overlay.init_function != -1:
        init_addr = overlay.vram_base + overlay.init_function if overlay.vram_base else overlay.init_function
        lines.append(f"  #   Init function: +0x{overlay.init_function:X}")
    
    if overlay.resume_function != -1:
        resume_addr = overlay.vram_base + overlay.resume_function if overlay.vram_base else overlay.resume_function
        lines.append(f"  #   Resume function: +0x{overlay.resume_function:X}")
    
    return "\n".join(lines)


def print_overlay_summary(overlays: list[OverlayHeader]):
    """Print a summary table of all overlays"""
    print("=" * 130)
    print(f"{'Idx':>4} {'ROM Start':>12} {'ROM End':>12} {'Text':>10} {'Data':>10} {'BSS':>10} {'Reloc':>6} {'ROM Size':>10} {'VRAM Size':>10} {'Init':>10} {'Resume':>10}")
    print("=" * 130)
    
    total_text = 0
    total_data = 0
    total_bss = 0
    total_rom = 0
    valid_count = 0
    
    for ov in overlays:
        if not ov.is_valid:
            continue
        
        valid_count += 1
        total_text += ov.text_size
        total_data += ov.data_size
        total_bss += ov.bss_size
        total_rom += ov.rom_size
        
        rom_start = ov.absolute_rom
        rom_end = rom_start + ov.rom_size
        
        init_str = f"+0x{ov.init_function:X}" if ov.init_function != -1 else "-"
        resume_str = f"+0x{ov.resume_function:X}" if ov.resume_function != -1 else "-"
        
        print(
            f"{ov.index:4} "
            f"0x{rom_start:08X}   "
            f"0x{rom_end:08X}   "
            f"0x{ov.text_size:06X}   "
            f"0x{ov.data_size:06X}   "
            f"0x{ov.bss_size:06X}   "
            f"{ov.reloc_table_size:6}   "
            f"0x{ov.rom_size:06X}   "
            f"0x{ov.vram_size:06X}   "
            f"{init_str:>10}   "
            f"{resume_str:>10}"
        )
    
    print("=" * 130)
    print(f"Valid overlays: {valid_count}")
    print(f"Total text: 0x{total_text:X} ({total_text:,} bytes)")
    print(f"Total data: 0x{total_data:X} ({total_data:,} bytes)")
    print(f"Total BSS: 0x{total_bss:X} ({total_bss:,} bytes)")
    print(f"Total ROM size: 0x{total_rom:X} ({total_rom:,} bytes)")


def main():
    parser = argparse.ArgumentParser(
        description="Extract overlay information from Jet Force Gemini ROM"
    )
    parser.add_argument(
        "--rom", "-r",
        type=Path,
        default=Path("baseroms/baserom.us.z64"),
        help="Path to the ROM file (default: baseroms/baserom.us.z64)"
    )
    parser.add_argument(
        "--output", "-o",
        type=Path,
        default=None,
        help="Output YAML file (default: print to stdout)"
    )
    parser.add_argument(
        "--summary", "-s",
        action="store_true",
        help="Print summary table only"
    )
    parser.add_argument(
        "--index", "-i",
        type=int,
        default=None,
        help="Generate segment for specific overlay index only"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Include detailed comments for each overlay"
    )
    
    args = parser.parse_args()
    
    if not args.rom.exists():
        print(f"Error: ROM file not found: {args.rom}", file=sys.stderr)
        sys.exit(1)
    
    overlays = read_overlay_table(args.rom)
    
    if args.summary:
        print_overlay_summary(overlays)
        return
    
    output_lines = []
    output_lines.append("# Jet Force Gemini Overlay Segments")
    output_lines.append(f"# Generated from ROM: {args.rom}")
    output_lines.append(f"# Overlay table: 0x{OVERLAY_TABLE_ROM:X}")
    output_lines.append(f"# Overlay data: 0x{OVERLAY_DATA_ROM:X}")
    output_lines.append("")
    output_lines.append("segments:")
    
    if args.index is not None:
        # Generate for specific overlay
        if args.index < 0 or args.index >= len(overlays):
            print(f"Error: Invalid overlay index {args.index}", file=sys.stderr)
            sys.exit(1)
        
        overlay = overlays[args.index]
        if args.verbose:
            output_lines.append(generate_overlay_info(overlay))
        output_lines.append(generate_splat_segment(overlay))
    else:
        # Generate for all valid overlays
        for overlay in overlays:
            if not overlay.is_valid:
                continue
            
            output_lines.append("")
            if args.verbose:
                output_lines.append(generate_overlay_info(overlay))
            output_lines.append(generate_splat_segment(overlay))
    
    output_text = "\n".join(output_lines)
    
    if args.output:
        with open(args.output, "w") as f:
            f.write(output_text)
        print(f"Written to {args.output}")
    else:
        print(output_text)


if __name__ == "__main__":
    main()
