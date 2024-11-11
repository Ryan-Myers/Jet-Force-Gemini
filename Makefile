BASENAME  = jfg
VERSION  := kiosk
NON_MATCHING ?= 0
# Libultra version might be at least J, but still labeled in the header as G for some reason.
LIBULTRA_VERSION_DEFINE := -DBUILD_VERSION=4 -DBUILD_VERSION_STRING=\"2.0G\" -DRAREDIFFS -DJFGDIFFS

# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

PRINT = printf
GREP  = grep -rl

# Colors

# Whether to colorize build messages
COLOR ?= 1
GCC_COLOR := -fno-diagnostics-color

ifeq ($(COLOR),1)
NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
PINK    := \033[0;35m
CYAN    := \033[0;36m
COLORIZE := -c
GCC_COLOR :=
endif

# Common build print status function
define print
  @$(PRINT) "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)\n"
endef

# Directories
BIN_DIRS  = assets
ifeq ($(VERSION),kiosk)
BUILD_DIR = build
SRC_DIR   = src
OLD_LIBULTRA_DIR = $(SRC_DIR)/libultra
LIBULTRA_DIR = libultra
ASM_DIRS  = asm asm/data asm/nonmatchings
HASM_DIRS = $(SRC_DIR)/hasm $(LIBULTRA_DIR)/src/os $(LIBULTRA_DIR)/src/gu $(LIBULTRA_DIR)/src/libc $(OLD_LIBULTRA_DIR)
LIBULTRA_SRC_DIRS  = $(LIBULTRA_DIR) $(LIBULTRA_DIR)/src $(LIBULTRA_DIR)/src/audio $(LIBULTRA_DIR)/src/audio/mips1 
LIBULTRA_SRC_DIRS += $(LIBULTRA_DIR)/src/debug $(LIBULTRA_DIR)/src/gu $(LIBULTRA_DIR)/src/io
LIBULTRA_SRC_DIRS += $(LIBULTRA_DIR)/src/libc $(LIBULTRA_DIR)/src/os $(LIBULTRA_DIR)/src/sc
else
BUILD_DIR = build_$(VERSION)
SRC_DIR   = src_$(VERSION)
LIBULTRA_DIR = $(SRC_DIR)/libultra
ASM_DIRS  = asm_$(VERSION) asm_$(VERSION)/data asm_$(VERSION)/libultra
HASM_DIRS = $(SRC_DIR)/hasm $(LIBULTRA_DIR)/src/os $(LIBULTRA_DIR)/src/gu $(LIBULTRA_DIR)/src/libc
LIBULTRA_SRC_DIRS  = $(LIBULTRA_DIR) $(LIBULTRA_DIR)/src $(LIBULTRA_DIR)/src/audio $(LIBULTRA_DIR)/src/audio/mips1 
LIBULTRA_SRC_DIRS += $(LIBULTRA_DIR)/src/debug $(LIBULTRA_DIR)/src/gu $(LIBULTRA_DIR)/src/io
LIBULTRA_SRC_DIRS += $(LIBULTRA_DIR)/src/libc $(LIBULTRA_DIR)/src/os $(LIBULTRA_DIR)/src/sc
endif

# Files requiring pre/post-processing
GLOBAL_ASM_C_FILES := $(shell $(GREP) GLOBAL_ASM $(SRC_DIR) $(LIBULTRA_DIR) </dev/null 2>/dev/null)
GLOBAL_ASM_O_FILES := $(foreach file,$(GLOBAL_ASM_C_FILES),$(BUILD_DIR)/$(file).o)

SRC_DIRS = $(SRC_DIR) $(LIBULTRA_SRC_DIRS) $(OLD_LIBULTRA_DIR)
SYMBOLS_DIR = ver/symbols

TOOLS_DIR = tools

UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	$(error Native Windows is currently unsupported for building this repository, use WSL instead c:)
else ifeq ($(UNAME_S),Linux)
	DETECTED_OS := linux
else ifeq ($(UNAME_S),Darwin)
	DETECTED_OS := macos
endif

RECOMP_DIR := $(TOOLS_DIR)/ido-recomp/$(DETECTED_OS)

# Files

S_FILES         = $(foreach dir,$(ASM_DIRS) $(HASM_DIRS),$(wildcard $(dir)/*.s))
C_FILES         = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
BIN_FILES       = $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))

O_FILES := $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file).o) \
           $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file).o) \
           $(foreach file,$(BIN_FILES),$(BUILD_DIR)/$(file).o)


find-command = $(shell which $(1) 2>/dev/null)

# Tools

ifeq ($(shell type mips-linux-gnu-ld >/dev/null 2>/dev/null; echo $$?), 0)
  CROSS := mips-linux-gnu-
else ifeq ($(shell type mips64-linux-gnu-ld >/dev/null 2>/dev/null; echo $$?), 0)
  CROSS := mips64-linux-gnu-
else
  CROSS := mips64-elf-
endif

AS       = $(CROSS)as
CPP      = cpp
LD       = $(CROSS)ld
OBJCOPY  = $(CROSS)objcopy
VENV     = .venv
PYTHON   = $(VENV)/bin/python3
GCC      = gcc

XGCC     = mips64-elf-gcc

#Options
CC       = $(RECOMP_DIR)/cc
SPLAT    ?= $(PYTHON) -m splat split
CRC      = $(TOOLS_DIR)/n64crc $(BUILD_DIR)/$(BASENAME).$(VERSION).z64 $(COLORIZE)

OPT_FLAGS      = -O2

MIPSISET       = -mips1

DEFINES := _FINALROM NDEBUG TARGET_N64 F3DDKR_GBI
DEFINES += VERSION_$(VERSION)

VERIFY = verify

ifeq ($(NON_MATCHING),1)
	DEFINES += NON_MATCHING
	DEFINES += AVOID_UB
	VERIFY = no_verify
else
	DEFINES += ANTI_TAMPER
endif

C_DEFINES := $(foreach d,$(DEFINES),-D$(d)) $(LIBULTRA_VERSION_DEFINE) -D_MIPS_SZLONG=32
ASM_DEFINES = --defsym _MIPS_SIM=1 --defsym mips=1

INCLUDE_CFLAGS  = -I . -I include -I include/libc  -I include/PR -I include/sys -I $(BIN_DIRS) -I $(SRC_DIR) -I $(LIBULTRA_DIR)
INCLUDE_CFLAGS += -I $(LIBULTRA_DIR)/src/gu -I $(LIBULTRA_DIR)/src/libc -I $(LIBULTRA_DIR)/src/io  -I $(LIBULTRA_DIR)/src/sc 
INCLUDE_CFLAGS += -I $(LIBULTRA_DIR)/src/audio -I $(LIBULTRA_DIR)/src/os

ASFLAGS        = -march=vr4300 -32 -G0 $(ASM_DEFINES) $(INCLUDE_CFLAGS)
OBJCOPYFLAGS   = -O binary

#IDO Warnings to Ignore. These are coding style warnings we don't follow
CC_WARNINGS := -fullwarn -Xfullwarn -woff 838,649,624

CFLAGS := -G 0 -non_shared -verbose -Xcpluscomm -nostdinc -Wab,-r4300_mul
CFLAGS += $(C_DEFINES)
CFLAGS += $(INCLUDE_CFLAGS)


CHECK_WARNINGS := -Wall -Wextra -Wno-format-security -Wno-unknown-pragmas -Wunused-function -Wno-unused-parameter -Werror-implicit-function-declaration
CHECK_WARNINGS += -Werror-implicit-function-declaration -Wno-unused-variable -Wno-missing-braces -Wno-int-conversion -Wno-main
CHECK_WARNINGS += -Wno-builtin-declaration-mismatch -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-switch
CC_CHECK := $(GCC) -fsyntax-only -fno-builtin -funsigned-char -std=gnu90 -m32 -D_LANGUAGE_C $(CHECK_WARNINGS) $(INCLUDE_CFLAGS) $(C_DEFINES) $(GCC_COLOR)

TARGET     = $(BUILD_DIR)/$(BASENAME).$(VERSION)
LD_SCRIPT  = ver/$(BASENAME).$(VERSION).ld

LD_FLAGS   = -T $(LD_SCRIPT) -T $(SYMBOLS_DIR)/undefined_funcs_auto.$(VERSION).txt  -T $(SYMBOLS_DIR)/undefined_syms_auto.$(VERSION).txt -T $(SYMBOLS_DIR)/libultra_undefined_syms.$(VERSION).txt
LD_FLAGS  += -Map $(TARGET).map
LD_FLAGS_EXTRA  = $(foreach sym,$(UNDEFINED_SYMS),-u $(sym))

ASM_PROCESSOR_DIR := $(TOOLS_DIR)/asm-processor
ASM_PROCESSOR      = $(PYTHON) $(ASM_PROCESSOR_DIR)/build.py

### Optimisation Overrides
####################### LIBULTRA #########################

# $(BUILD_DIR)/$(LIBULTRA_DIR)/%.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/audio/%.c.o: OPT_FLAGS := -O3
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/audio/mips1/%.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/os/%.c.o: OPT_FLAGS := -O1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/%.c.o: OPT_FLAGS := -O1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/vimgr.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/pimgr.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/motor.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/xprintf.c.o : OPT_FLAGS := -O3
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/audio/env.c.o: OPT_FLAGS := -g
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/llcvt.c.o: OPT_FLAGS := -O1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/llcvt.c.o: MIPSISET := -mips3 -32
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/ll.c.o: OPT_FLAGS := -O1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/ll.c.o: MIPSISET := -mips3 -32
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/ldiv.c.o: OPT_FLAGS := -O3
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/ldiv.c.o: MIPSISET := -mips2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/xldtob.c.o: OPT_FLAGS := -O3
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/xldtob.c.o: MIPSISET := -mips2

# $(BUILD_DIR)/$(LIBULTRA_DIR)/%.c.o: MIPSISET := -mips2
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/audio/mips1/%.c.o: MIPSISET := -mips1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/pimgr.c.o: MIPSISET := -mips1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/sc/sched.c.o: MIPSISET := -mips1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/io/motor.c.o: MIPSISET := -mips1
# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/audio/env.c.o: MIPSISET := -mips1

#Ignore warnings for libultra files
$(BUILD_DIR)/$(LIBULTRA_DIR)/%.c.o: CC_WARNINGS := -w
$(BUILD_DIR)/$(LIBULTRA_DIR)/%.c.o: CC_CHECK := :

### Targets

default: all

all: $(VERIFY)

ldflags:
	$(V)printf "[$(PINK) LDFLAGS $(NO_COL)]: $(LD_FLAGS)\n[$(PINK) EXTRA $(NO_COL)]: $(LD_FLAGS_EXTRA)\n"

dirs:
	$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(HASM_DIRS) $(BIN_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

check: .baserom.$(VERSION).ok

verify: $(TARGET).z64
	$(V)$(CRC)
ifeq ($(NON_MATCHING),0)
	@(sha1sum -c --quiet ver/verification/$(BASENAME).$(VERSION).sha1 \
	&& $(PRINT) "$(GREEN)Verify:$(NO_COL)\
	 $(YELLOW)OK$(NO_COL)\n")
	$(V)$(PRINT) "$(YELLOW)    __\n .\`_  _\`.\n| | \`| | |\n| |_|._| |\n \`. __ .\'$(NO_COL)\n\n"
else
	$(V)$(PRINT) "$(GREEN)Build Complete!$(NO_COL)\n"
endif

no_verify: $(TARGET).z64
	$(V)$(PRINT) "$(GREEN)Build Complete!$(NO_COL)\n"

extract:
	$(SPLAT) ver/splat/$(BASENAME).$(VERSION).yaml

extractall: extract tools
	$(PYTHON) $(SPLAT) ver/splat/$(BASENAME).kiosk.yaml
	$(PYTHON) $(SPLAT) ver/splat/$(BASENAME).us.yaml
	$(PYTHON) $(SPLAT) ver/splat/$(BASENAME).pal.yaml
	$(PYTHON) $(SPLAT) ver/splat/$(BASENAME).jpn.yaml

dependencies: tools
	$(V)make -C $(TOOLS_DIR)
#Set up a python venv so we don't get warnings about breaking system packages.
	$(V)python3 -m venv $(VENV)
#Installing the splat dependencies
	$(V)$(PYTHON) -m pip install -r requirements.txt

clean:
	rm -rf $(BUILD_DIR)
	
cleanall:
	rm -rf build
	rm -rf build_us
	rm -rf build_pal
	rm -rf build_jpn
	rm -rf asm
	rm -rf asm_us
	rm -rf asm_pal
	rm -rf asm_jpn

distclean: clean
	rm -rf $(ASM_DIRS)
	rm -rf $(BIN_DIRS)
	rm -rf assets
	rm -f $(SYMBOLS_DIR)/*auto.$(VERSION).txt
	rm -f $(LD_SCRIPT)

distcleanall: clean cleanall
	rm -rf assets
	rm -f $(SYMBOLS_DIR)/*auto.kiosk.txt
	rm -f $(SYMBOLS_DIR)/*auto.us.txt
	rm -f $(SYMBOLS_DIR)/*auto.pal.txt
	rm -f $(SYMBOLS_DIR)/*auto.jpn.txt
	rm -f ver/jfg.kiosk.ld
	rm -f ver/jfg.us.ld
	rm -f ver/jfg.pal.ld
	rm -f ver/jfg.kiosk.ld
	rm -f ver/jfg.jpn.ld

#When you just need to wipe old symbol names and re-extract
cleanextract: distclean extract

#When you just need to wipe old symbol names and re-extract
cleanextractall: distcleanall extractall

#Put the build folder into expected for use with asm-differ. Only run this with a matching build.
expected: verify
	mkdir -p expected
	rm -rf expected/$(BUILD_DIR)
	cp -r $(BUILD_DIR)/ expected/

### Recipes

$(GLOBAL_ASM_O_FILES): CC := $(ASM_PROCESSOR) $(CC) -- $(AS) $(ASFLAGS) --

$(TARGET).elf: dirs $(LD_SCRIPT) $(O_FILES)
	@$(PRINT) "$(GREEN)Linking: $(BLUE)$@$(NO_COL)\n"
	$(V)$(LD) $(LD_FLAGS) $(LD_FLAGS_EXTRA) -o $@

ifndef PERMUTER
$(GLOBAL_ASM_O_FILES): $(BUILD_DIR)/%.c.o: %.c
	$(call print,Compiling:,$<,$@)
#	$(V)$(CC_CHECK) $<
	$(V)$(CC) -c $(CFLAGS) $(CC_WARNINGS) $(OPT_FLAGS) $(MIPSISET) -o $@ $<
endif

# non asm-processor recipe
$(BUILD_DIR)/%.c.o: %.c
	$(call print,Compiling:,$<,$@)
#	$(V)$(CC_CHECK) $<
	$(V)$(CC) -c $(CFLAGS) $(CC_WARNINGS) $(OPT_FLAGS) $(MIPSISET) -o $@ $<

# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/llcvt.c.o: $(LIBULTRA_DIR)/src/libc/llcvt.c
# 	$(call print,Compiling mips3:,$<,$@)
# 	@$(CC)  -c $(CFLAGS) $(CC_WARNINGS) $(OPT_FLAGS) $(MIPSISET) -o $@ $<
# 	$(V)$(PYTHON) tools/patchmips3.py $@ || rm $@

# $(BUILD_DIR)/$(LIBULTRA_DIR)/src/libc/ll.c.o: $(LIBULTRA_DIR)/src/libc/ll.c
# 	$(call print,Compiling mips3:,$<,$@)
# 	@$(CC)  -c $(CFLAGS) $(CC_WARNINGS) $(OPT_FLAGS) $(MIPSISET) -o $@ $<
# 	$(V)$(PYTHON) tools/patchmips3.py $@ || rm $@

$(BUILD_DIR)/%.s.o: %.s
	$(call print,Assembling:,$<,$@)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin
	$(call print,Linking Binary:,$<,$@)
	$(V)$(LD) -r -b binary -o $@ $<

$(TARGET).bin: $(TARGET).elf
	$(call print,Objcopy:,$<,$@)
	$(V)$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(TARGET).z64: $(TARGET).bin
	$(call print,CopyRom:,$<,$@)
	$(V)$(TOOLS_DIR)/CopyRom.py $< $@

### Settings
.SECONDARY:
.PHONY: all clean default
SHELL = /bin/bash -e -o pipefail
