#!/usr/bin/env python3
import json
import os
from pathlib import Path

def generate_objdiff_config():
    units = []
    
    # Add src/*.c files
    src_dir = Path("src")
    for c_file in sorted(src_dir.rglob("*.c")):
        rel_path = c_file.relative_to(".")
        name = str(rel_path.with_suffix(""))
        units.append({
            "name": name,
            "target_path": f"expected/build/{rel_path}.o",
            "base_path": f"build/{rel_path}.o"
        })
    
    # Add libultra/*.c files
    libultra_dir = Path("libultra")
    for c_file in sorted(libultra_dir.rglob("*.c")):
        rel_path = c_file.relative_to(".")
        name = str(rel_path.with_suffix(""))
        units.append({
            "name": name,
            "target_path": f"expected/build/{rel_path}.o",
            "base_path": f"build/{rel_path}.o"
        })
    
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "make",
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.h", "*.s", "*.S", "*.inc"],
        "ignore_patterns": ["build/**/*", "expected/**/*"],
        "units": units
    }
    
    with open("objdiff.json", "w") as f:
        json.dump(config, f, indent=2)
    
    print(f"Generated objdiff.json with {len(units)} units")

if __name__ == "__main__":
    generate_objdiff_config()