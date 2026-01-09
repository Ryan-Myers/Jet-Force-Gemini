#!/usr/bin/env python3
"""
Analyze JFG overlay ROM layout and detect overlapping regions.
This helps understand which overlays share ROM space.
"""

import struct
import sys
from pathlib import Path

OVERLAY_TABLE_ROM = 0x1ED2780
OVERLAY_DATA_BASE = 0x1ED3B20
ROM_PATH = "baseroms/baserom.us.z64"

def read_overlay_table(rom_data):
    """Read all overlay headers from ROM."""
    overlays = []
    
    # Read entries until we find an empty one or hit reasonable limit
    max_overlays = 200
    
    for i in range(max_overlays):
        offset = OVERLAY_TABLE_ROM + (i * 32)
        # Format: vram(i), rom_addr(i), text(i), data(i), bss(i), reloc(H), sec_reloc(H), init(i), resume(i)
        header = struct.unpack(">iiiiiHHii", rom_data[offset:offset + 32])
        
        vram_base, rom_address, text_size, data_size, bss_size, reloc_count, secondary_reloc, init_func, resume_func = header
        
        # Check for end of table (all zeros or invalid entry)
        if text_size == 0 and data_size == 0 and reloc_count == 0:
            continue  # Skip empty entries but keep looking
            
        # Sanity check - if text_size is huge, we've gone past the table
        if text_size > 0x100000 or text_size < 0:
            break
            
        rom_start = OVERLAY_DATA_BASE + rom_address
        rom_size = text_size + data_size + (reloc_count * 8)
        rom_end = rom_start + rom_size
        
        overlays.append({
            'index': i + 1,
            'rom_start': rom_start,
            'rom_end': rom_end,
            'rom_size': rom_size,
            'text_size': text_size,
            'data_size': data_size,
            'bss_size': bss_size,
            'reloc_count': reloc_count,
        })
    
    return overlays

def find_overlap_groups(overlays):
    """Find groups of overlays that overlap in ROM."""
    # Sort by start address
    sorted_ovls = sorted(overlays, key=lambda x: x['rom_start'])
    
    groups = []
    current_group = []
    current_end = 0
    
    for ovl in sorted_ovls:
        if ovl['rom_start'] < current_end:
            # Overlaps with current group
            current_group.append(ovl)
            current_end = max(current_end, ovl['rom_end'])
        else:
            # New group
            if current_group:
                groups.append(current_group)
            current_group = [ovl]
            current_end = ovl['rom_end']
    
    if current_group:
        groups.append(current_group)
    
    return groups

def find_non_overlapping_ranges(groups):
    """Find the actual ROM ranges needed (union of overlapping overlays)."""
    ranges = []
    for group in groups:
        start = min(o['rom_start'] for o in group)
        end = max(o['rom_end'] for o in group)
        ranges.append({
            'start': start,
            'end': end,
            'size': end - start,
            'overlays': [o['index'] for o in group],
            'count': len(group)
        })
    return ranges

def main():
    rom_path = Path(ROM_PATH)
    if not rom_path.exists():
        print(f"Error: ROM not found at {ROM_PATH}")
        sys.exit(1)
    
    rom_data = rom_path.read_bytes()
    overlays = read_overlay_table(rom_data)
    
    print("=" * 100)
    print("JFG Overlay ROM Analysis")
    print("=" * 100)
    
    # Find overlap groups
    groups = find_overlap_groups(overlays)
    ranges = find_non_overlapping_ranges(groups)
    
    print(f"\nTotal overlays: {len(overlays)}")
    print(f"Overlap groups: {len(groups)}")
    
    # Count solo vs grouped
    solo = sum(1 for g in groups if len(g) == 1)
    grouped = len(groups) - solo
    print(f"  - Non-overlapping (solo): {solo}")
    print(f"  - Overlapping groups: {grouped}")
    
    print("\n" + "=" * 100)
    print("ROM Ranges (for linker/build system)")
    print("=" * 100)
    print(f"{'Start':>12} {'End':>12} {'Size':>10} {'Overlays'}")
    print("-" * 100)
    
    total_rom = 0
    for r in ranges:
        if r['count'] > 1:
            ovl_str = f"{r['count']} overlays: {r['overlays'][:5]}{'...' if r['count'] > 5 else ''}"
        else:
            ovl_str = f"overlay_{r['overlays'][0]}"
        print(f"0x{r['start']:08X} 0x{r['end']:08X} 0x{r['size']:08X} {ovl_str}")
        total_rom += r['size']
    
    print("-" * 100)
    print(f"Total ROM needed for overlays: 0x{total_rom:X} bytes")
    
    # Show overlapping groups in detail
    print("\n" + "=" * 100)
    print("Overlapping Groups Detail")
    print("=" * 100)
    
    for i, group in enumerate(groups):
        if len(group) > 1:
            print(f"\nGroup {i+1}: {len(group)} overlays overlapping")
            print(f"  ROM Range: 0x{min(o['rom_start'] for o in group):08X} - 0x{max(o['rom_end'] for o in group):08X}")
            for o in sorted(group, key=lambda x: x['rom_start']):
                print(f"    overlay_{o['index']:3d}: 0x{o['rom_start']:08X} - 0x{o['rom_end']:08X} (size: 0x{o['rom_size']:X})")
    
    # Generate YAML-friendly output
    print("\n" + "=" * 100)
    print("Recommended YAML segments (non-overlapping ROM ranges)")
    print("=" * 100)
    
    for r in ranges:
        if r['count'] == 1:
            idx = r['overlays'][0]
            print(f"""
  - name: overlay_{idx}
    type: bin
    start: 0x{r['start']:X}""")
        else:
            # For overlapping groups, we need ONE bin segment covering the whole range
            indices = "_".join(str(x) for x in r['overlays'][:3])
            if r['count'] > 3:
                indices += "_etc"
            print(f"""
  # Overlapping: overlays {r['overlays']}
  - name: overlay_group_{indices}
    type: bin
    start: 0x{r['start']:X}
    # Contains {r['count']} overlapping overlays""")

if __name__ == "__main__":
    main()
