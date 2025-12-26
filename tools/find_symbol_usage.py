#!/usr/bin/env python3
"""
Script to find which asm files use symbols defined in a .bss.s or .rodata.s file.

Usage: python find_symbol_usage.py <input_file> [--search-dir <dir>]

Example: python find_symbol_usage.py asm_us/data/main_bss_start.bss.s
"""

import argparse
import os
import re
import subprocess
from pathlib import Path


def extract_symbols(input_file: str) -> list[tuple[str, str]]:
    """
    Extract dlabel symbols and their addresses from an asm file.
    Returns list of (address, symbol_name) tuples.
    """
    symbols = []
    
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    current_symbol = None
    
    for line in lines:
        # Match dlabel lines like: dlabel D_800B0B50_B1750
        dlabel_match = re.match(r'^dlabel\s+(\w+)', line)
        if dlabel_match:
            current_symbol = dlabel_match.group(1)
            continue
        
        # If we have a current symbol, look for its address in comment
        # Format: /* 800B0B50 */ or /* AE4A0 800AD8A0 */
        if current_symbol:
            # Try format with ROM and VRAM: /* AE4A0 800AD8A0 */
            addr_match = re.search(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s*\*/', line)
            if not addr_match:
                # Try format with just VRAM: /* 800B0B50 */
                addr_match = re.search(r'/\*\s*([0-9A-Fa-f]{8})\s*\*/', line)
            
            if addr_match:
                address = addr_match.group(1)
                symbols.append((address, current_symbol))
                current_symbol = None
    
    return symbols


def find_symbol_usages(symbol: str, search_dir: str) -> list[str]:
    """
    Find all .s files in search_dir that reference the given symbol.
    Returns list of relative file paths.
    """
    try:
        # Use grep to find files containing the symbol
        # Exclude the data subdirectory since those are definitions, not usages
        result = subprocess.run(
            ['grep', '-l', '-r', '--include=*.s', symbol, search_dir],
            capture_output=True,
            text=True
        )
        
        if result.returncode == 0:
            files = result.stdout.strip().split('\n')
            # Filter out empty strings and data files (definitions)
            files = [f for f in files if f and '/data/' not in f]
            return files
        return []
    except Exception as e:
        print(f"Error searching for {symbol}: {e}")
        return []


def main():
    parser = argparse.ArgumentParser(
        description='Find which asm files use symbols defined in a .bss.s or .rodata.s file'
    )
    parser.add_argument('input_file', help='Input asm file (e.g., asm_us/data/main_bss_start.bss.s)')
    parser.add_argument('--search-dir', default='asm_us', 
                        help='Directory to search for symbol usages (default: asm_us)')
    parser.add_argument('--markdown', action='store_true',
                        help='Output in markdown table format')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.input_file):
        print(f"Error: Input file '{args.input_file}' not found")
        return 1
    
    if not os.path.exists(args.search_dir):
        print(f"Error: Search directory '{args.search_dir}' not found")
        return 1
    
    # Extract symbols from input file
    symbols = extract_symbols(args.input_file)
    
    if not symbols:
        print("No symbols found in input file")
        return 1
    
    print(f"Found {len(symbols)} symbols in {args.input_file}")
    print()
    
    # Find usages for each symbol
    results = []
    for address, symbol in symbols:
        usages = find_symbol_usages(symbol, args.search_dir)
        if usages:
            for usage_file in usages:
                results.append((address, symbol, usage_file))
        else:
            results.append((address, symbol, "(not found)"))
    
    # Output table
    if args.markdown:
        print("| Address | Symbol | File |")
        print("|---------|--------|------|")
        for address, symbol, file in results:
            print(f"| 0x{address} | {symbol} | {file} |")
    else:
        # Calculate column widths
        addr_width = max(len("Address"), max(len(f"0x{r[0]}") for r in results))
        sym_width = max(len("Symbol"), max(len(r[1]) for r in results))
        file_width = max(len("File"), max(len(r[2]) for r in results))
        
        # Print header
        header = f"{'Address':<{addr_width}}  {'Symbol':<{sym_width}}  {'File':<{file_width}}"
        print(header)
        print("-" * len(header))
        
        # Print rows
        for address, symbol, file in results:
            print(f"0x{address:<{addr_width-2}}  {symbol:<{sym_width}}  {file:<{file_width}}")
    
    # Summary
    print()
    found_count = sum(1 for r in results if r[2] != "(not found)")
    not_found = sum(1 for r in results if r[2] == "(not found)")
    print(f"Summary: {found_count} usages found, {not_found} symbols not referenced")
    
    return 0


if __name__ == '__main__':
    exit(main())
