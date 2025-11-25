#!/bin/bash
# Usage: ./strip_comments.sh src/hasm/math_util.s > src/hasm/math_util_stripped.s
# Removes leading whitespace, comments (/* ... */), and hex addresses before instructions

sed -E 's/^[[:space:]]*\/\*[^*]*\*\/\s\s/    /g' "$1"
