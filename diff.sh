#!/bin/sh

# Allows custom diff arguments for the version being diffed. (ex. --VERSION=us)
.venv/bin/python3 tools/asm-differ/diff.py "$@"