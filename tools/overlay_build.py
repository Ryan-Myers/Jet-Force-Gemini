#!/usr/bin/env python3
"""
Build script for JFG overlays.

Since JFG overlays OVERLAP in ROM (mutually exclusive overlays share ROM space),
they cannot be linked sequentially like normal code. Instead:

1. Overlays are extracted as individual binary blobs from the original ROM
2. Each overlay can be disassembled/decompiled separately
3. The final ROM is built by:
   a. Building the main code up to the overlay region
   b. Copying the "overlay_regions" bin file (which contains all overlays in their
      original overlapping layout) to the correct ROM offset

This script handles extracting and injecting overlays.
"""

import argparse
import struct
import sys
from pathlib import Path

# ROM addresses for US version
OVERLAY_TABLE_ROM = 0x1ED2780
OVERLAY_DATA_BASE = 0x1ED3B20

# The 3 overlay regions (groups of overlapping overlays)
OVERLAY_REGIONS = [
    {'start': 0x1ED3B20, 'end': 0x1F69B98, 'name': 'overlay_region_1', 'overlays': list(range(1, 50))},
    {'start': 0x1F69CA8, 'end': 0x1F7B780, 'name': 'overlay_region_2', 'overlays': [50, 51, 52, 53, 54, 55, 57, 58, 59, 60]},
    {'start': 0x1F7B850, 'end': 0x1FEB450, 'name': 'overlay_region_3', 'overlays': list(range(61, 158))},
]


def read_overlay_header(rom_data, index):
    """Read a single overlay header."""
    offset = OVERLAY_TABLE_ROM + ((index - 1) * 32)
    fields = struct.unpack(">iiiiiHHii", rom_data[offset:offset + 32])
    return {
        'vram_base': fields[0],
        'rom_offset': fields[1],  # Offset from OVERLAY_DATA_BASE
        'text_size': fields[2],
        'data_size': fields[3],
        'bss_size': fields[4],
        'reloc_count': fields[5],
        'secondary_reloc': fields[6],
        'init_func': fields[7],
        'resume_func': fields[8],
    }


def extract_overlay_regions(rom_path, output_dir):
    """Extract the 3 overlay region binary blobs."""
    rom_data = Path(rom_path).read_bytes()
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    for region in OVERLAY_REGIONS:
        data = rom_data[region['start']:region['end']]
        out_path = output_dir / f"{region['name']}.bin"
        out_path.write_bytes(data)
        print(f"Extracted {region['name']}: 0x{region['start']:X}-0x{region['end']:X} ({len(data)} bytes) -> {out_path}")


def extract_individual_overlays(rom_path, output_dir):
    """Extract each overlay as an individual binary file."""
    rom_data = Path(rom_path).read_bytes()
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    for i in range(1, 158):
        header = read_overlay_header(rom_data, i)
        
        if header['text_size'] == 0 and header['data_size'] == 0:
            continue
            
        rom_start = OVERLAY_DATA_BASE + header['rom_offset']
        rom_size = header['text_size'] + header['data_size'] + (header['reloc_count'] * 8)
        
        data = rom_data[rom_start:rom_start + rom_size]
        
        # Create subdirectory for overlay
        ovl_dir = output_dir / f"o{i}"
        ovl_dir.mkdir(parents=True, exist_ok=True)
        
        # Write full overlay
        (ovl_dir / f"overlay_{i}.bin").write_bytes(data)
        
        # Also split into sections
        text_end = header['text_size']
        data_end = text_end + header['data_size']
        
        if header['text_size'] > 0:
            (ovl_dir / f"overlay_{i}_text.bin").write_bytes(data[:text_end])
        if header['data_size'] > 0:
            (ovl_dir / f"overlay_{i}_data.bin").write_bytes(data[text_end:data_end])
        if header['reloc_count'] > 0:
            (ovl_dir / f"overlay_{i}_reloc.bin").write_bytes(data[data_end:])
        
        print(f"overlay_{i}: ROM 0x{rom_start:X}, text=0x{header['text_size']:X}, "
              f"data=0x{header['data_size']:X}, bss=0x{header['bss_size']:X}, relocs={header['reloc_count']}")


def inject_overlay_regions(built_rom_path, output_rom_path, overlay_regions_dir):
    """Inject overlay region binaries into the built ROM at correct offsets."""
    rom_data = bytearray(Path(built_rom_path).read_bytes())
    overlay_regions_dir = Path(overlay_regions_dir)
    
    for region in OVERLAY_REGIONS:
        bin_path = overlay_regions_dir / f"{region['name']}.bin"
        if not bin_path.exists():
            print(f"Warning: {bin_path} not found, skipping")
            continue
            
        overlay_data = bin_path.read_bytes()
        expected_size = region['end'] - region['start']
        
        if len(overlay_data) != expected_size:
            print(f"Warning: {bin_path} size mismatch: expected {expected_size}, got {len(overlay_data)}")
            
        rom_data[region['start']:region['start'] + len(overlay_data)] = overlay_data
        print(f"Injected {region['name']} at 0x{region['start']:X}")
    
    Path(output_rom_path).write_bytes(rom_data)
    print(f"Wrote {output_rom_path}")


def main():
    parser = argparse.ArgumentParser(description="JFG Overlay Build Tool")
    subparsers = parser.add_subparsers(dest='command', required=True)
    
    # Extract regions command
    extract_regions = subparsers.add_parser('extract-regions', 
        help='Extract the 3 overlay region binaries from ROM')
    extract_regions.add_argument('--rom', default='baseroms/baserom.us.z64',
        help='Path to source ROM')
    extract_regions.add_argument('--output', default='assets/overlay_regions',
        help='Output directory for region binaries')
    
    # Extract individual overlays command
    extract_individual = subparsers.add_parser('extract-overlays',
        help='Extract each overlay as individual binary files')
    extract_individual.add_argument('--rom', default='baseroms/baserom.us.z64',
        help='Path to source ROM')
    extract_individual.add_argument('--output', default='assets/overlays',
        help='Output directory for overlay binaries')
    
    # Inject command
    inject = subparsers.add_parser('inject',
        help='Inject overlay regions into built ROM')
    inject.add_argument('--built-rom', required=True,
        help='Path to built ROM (without overlays)')
    inject.add_argument('--output', required=True,
        help='Path for output ROM')
    inject.add_argument('--overlay-dir', default='assets/overlay_regions',
        help='Directory containing overlay region binaries')
    
    args = parser.parse_args()
    
    if args.command == 'extract-regions':
        extract_overlay_regions(args.rom, args.output)
    elif args.command == 'extract-overlays':
        extract_individual_overlays(args.rom, args.output)
    elif args.command == 'inject':
        inject_overlay_regions(args.built_rom, args.output, args.overlay_dir)


if __name__ == "__main__":
    main()
