CC   = gcc

UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	$(error Native Windows is currently unsupported for building this repository, use WSL instead c:)
else ifeq ($(UNAME_S),Linux)
	DETECTED_OS := linux
else ifeq ($(UNAME_S),Darwin)
	DETECTED_OS := macos
endif

RECOMP_DIR := ido-recomp/$(DETECTED_OS)

default: all

all: n64cc recomp

n64cc:
	@$(CC) n64crc.c -o n64crc

recomp:
	@echo "Fetching Recomp..."
	wget https://github.com/decompals/ido-static-recomp/releases/download/v1.1/ido-5.3-recomp-${DETECTED_OS}.tar.gz
	mkdir -p $(RECOMP_DIR)
	tar xf ido-5.3-recomp-${DETECTED_OS}.tar.gz -C $(RECOMP_DIR)
	$(RM) ido-5.3-recomp-${DETECTED_OS}.tar.gz

clean:
	$(RM) n64crc
	$(RM) -rf $(RECOMP_DIR)

