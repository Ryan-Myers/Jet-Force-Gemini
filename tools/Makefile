CC := gcc
CFLAGS := -I . -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -O2 -s
LDFLAGS := -lm
PROGRAMS := n64crc

UNAME_S := $(shell uname -s)

# MacOS has a slightly different name for grep, so check to see if it needs to be used.
ifeq ($(UNAME_S),Windows_NT)
	$(error Native Windows is currently unsupported for building this repository, use WSL instead c:)
else ifeq ($(UNAME_S),Darwin)
	DETECTED_OS := macos
	LDFLAGS += -I/opt/homebrew/lib -lpcre2-8
else ifeq ($(UNAME_S),Linux)
	DETECTED_OS := linux
	UNAME_M := $(shell uname -m)
# If we detect arm as a platform for linux, but not mac, we'll make this -arm to download that version (Mostly used for raspi)
	ifneq ($(filter aarch%,$(UNAME_M)),)
		DETECTED_OS := linux-arm
	endif
endif

RECOMP_DIR := ido-recomp/$(DETECTED_OS)

default: all

n64crc_SOURCES := n64crc.c

all: $(PROGRAMS) recomp

clean:
	$(RM) -Rf $(PROGRAMS)
	$(RM) -rf $(RECOMP_DIR)

distclean: clean

n64crc: n64crc.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

recomp:
	@echo "Fetching Recomp..."
	wget https://github.com/decompals/ido-static-recomp/releases/download/v1.2/ido-5.3-recomp-${DETECTED_OS}.tar.gz
	mkdir -p $(RECOMP_DIR)
	tar xf ido-5.3-recomp-${DETECTED_OS}.tar.gz -C $(RECOMP_DIR)
	$(RM) ido-5.3-recomp-${DETECTED_OS}.tar.gz

.PHONY: all clean distclean default
