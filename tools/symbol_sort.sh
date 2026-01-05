#!/bin/bash

# Check if filename is provided as argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename>"
    echo "Example: $0 symbols.txt"
    exit 1
fi

input_file="$1"
output_file="${input_file%.txt}_sorted.txt"

# Check if file exists
if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found!"
    exit 1
fi

# Sort by hex address using sort's ability to handle hex numbers
sort -t'=' -k2,2g "$input_file" > "$output_file"

echo "Symbols sorted by address and saved to: $output_file"