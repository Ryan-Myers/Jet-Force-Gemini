BASENAME  = jfg
VERSION  := kiosk

# Colors

NO_COL  := \033[0m
RED     := \033[0;31m
RED2    := \033[1;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
PINK    := \033[0;35m
CYAN    := \033[0;36m

# Directories

BIN_DIRS  = assets

ifeq ($(VERSION),kiosk)
BUILD_DIR = build
SRC_DIR   = src
ASM_DIRS  = asm asm/data asm/libultra #For libultra handwritten files
else
BUILD_DIR = build_$(VERSION)
SRC_DIR   = src_$(VERSION)
ASM_DIRS  = asm_$(VERSION) asm_$(VERSION)/data asm_$(VERSION)/libultra #For libultra handwritten files
endif

LIBULTRA_SRC_DIRS = $(SRC_DIR)/libultra

DEFINE_SRC_DIRS  = $(SRC_DIR) $(SRC_DIR)/core $(LIBULTRA_SRC_DIRS)
SRC_DIRS = $(DEFINE_SRC_DIRS)

TOOLS_DIR = tools

# Files

S_FILES         = $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
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
PYTHON   = python3
GCC      = gcc

XGCC     = mips64-elf-gcc

GREP     = grep -rl

#Options
CC       = $(TOOLS_DIR)/ido-static-recomp/build/5.3/out/cc
SPLAT    = $(TOOLS_DIR)/splat/split.py
CRC      = @tools/n64crc $(BUILD_DIR)/$(BASENAME).$(VERSION).z64

OPT_FLAGS      = -O2
LOOP_UNROLL    =

MIPSISET       = -mips1 -32

INCLUDE_CFLAGS = -I . -I include/libc  -I include/PR -I include/sys -I include -I assets -I $(SRC_DIR)/os 

ASFLAGS        = -EB -mtune=vr4300 -march=vr4300 -mabi=32 -I include
OBJCOPYFLAGS   = -O binary

# Files requiring pre/post-processing
GLOBAL_ASM_C_FILES := $(shell $(GREP) GLOBAL_ASM $(SRC_DIR) </dev/null 2>/dev/null)
GLOBAL_ASM_O_FILES := $(foreach file,$(GLOBAL_ASM_C_FILES),$(BUILD_DIR)/$(file).o)


DEFINES := -D_LANGUAGE_C -D_FINALROM -DWIN32 -DNDEBUG -DTARGET_N64 -D__sgi


DEFINES += -DVERSION_$(VERSION)

VERIFY = verify

#Soon
#ifeq ($(NON_MATCHING),1)
#DEFINES += -DNON_MATCHING
#VERIFY = no_verify
#PROGRESS_NONMATCHING = --non-matching
#endif

CFLAGS := -Wab,-r4300_mul -non_shared -G 0 -Xcpluscomm -fullwarn -nostdinc -G 0
CFLAGS += $(DEFINES)
# ignore compiler warnings about anonymous structs
CFLAGS += -woff 649,838
CFLAGS += $(INCLUDE_CFLAGS)

CHECK_WARNINGS := -Wall -Wextra -Wno-format-security -Wno-unknown-pragmas -Wunused-function -Wno-unused-parameter -Wno-unused-variable -Wno-missing-braces -Wno-int-conversion
CC_CHECK := $(GCC) -fsyntax-only -fno-builtin -funsigned-char -std=gnu90 -m32 $(CHECK_WARNINGS) $(INCLUDE_CFLAGS) $(DEFINES)

GCC_FLAGS := $(INCLUDE_CFLAGS) $(DEFINES)
GCC_FLAGS += -G 0 -mno-shared -march=vr4300 -mfix4300 -mabi=32 -mhard-float
GCC_FLAGS += -mdivide-breaks -fno-stack-protector -fno-common -fno-zero-initialized-in-bss -fno-PIC -mno-abicalls -fno-strict-aliasing -fno-inline-functions -ffreestanding -fwrapv
GCC_FLAGS += -Wall -Wextra -Wno-missing-braces

TARGET     = $(BUILD_DIR)/$(BASENAME).$(VERSION)
LD_SCRIPT  = $(BASENAME).$(VERSION).ld

LD_FLAGS   = -T $(LD_SCRIPT) -T undefined_funcs_auto.$(VERSION).txt  -T undefined_syms_auto.$(VERSION).txt -T libultra_undefined_syms.$(VERSION).txt
LD_FLAGS  += -Map $(TARGET).map --no-check-sections

ifeq ($(VERSION),kiosk)
LD_FLAGS_EXTRA  =
LD_FLAGS_EXTRA += $(foreach sym,$(UNDEFINED_SYMS),-u $(sym))
else
LD_FLAGS_EXTRA  =
LD_FLAGS_EXTRA += $(foreach sym,$(UNDEFINED_SYMS),-u $(sym))
endif

ASM_PROCESSOR_DIR := $(TOOLS_DIR)/asm-processor
ASM_PROCESSOR      = $(PYTHON) $(ASM_PROCESSOR_DIR)/asm_processor.py

### Optimisation Overrides
# $(BUILD_DIR)/$(SRC_DIR)/os/%.c.o: OPT_FLAGS := -O1
# $(BUILD_DIR)/$(SRC_DIR)/os/audio/%.c.o: OPT_FLAGS := -O2
# $(BUILD_DIR)/$(SRC_DIR)/os/libc/%.c.o: OPT_FLAGS := -O3
# $(BUILD_DIR)/$(SRC_DIR)/os/gu/%.c.o: OPT_FLAGS := -O3

### Targets

default: all

all: $(VERIFY)

ldflags:
	@printf "[$(PINK) LDFLAGS $(NO_COL)]: $(LD_FLAGS)\n[$(PINK) EXTRA $(NO_COL)]: $(LD_FLAGS_EXTRA)\n"

dirs:
	$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(BIN_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

check: .baserom.$(VERSION).ok

verify: $(TARGET).z64
	@sha1sum -c $(BASENAME).$(VERSION).sha1

no_verify: $(TARGET).z64
	@echo "Skipping SHA1SUM check!"

splat: $(SPLAT)

extract: splat tools
	$(PYTHON) $(SPLAT) $(BASENAME).$(VERSION).yaml

extractall: splat tools
	$(PYTHON) $(SPLAT) $(BASENAME).kiosk.yaml
	$(PYTHON) $(SPLAT) $(BASENAME).us.yaml
	$(PYTHON) $(SPLAT) $(BASENAME).pal.yaml
	$(PYTHON) $(SPLAT) $(BASENAME).jpn.yaml

dependencies: tools
	@make -C tools
	@$(PYTHON) -m pip install -r tools/splat/requirements.txt #Installing the splat dependencies

clean:
	rm -rf $(BUILD_DIR)
	
cleanall:
	rm -rf build
	rm -rf build_us
	rm -rf build_pal
	rm -rf build_jpn


distclean: clean
	rm -rf $(ASM_DIRS)
	rm -rf assets
	rm -f *auto.kiosk.txt
	rm -f *auto.us.txt
	rm -f *auto.pal.txt
	rm -f *auto.jpn.txt
	rm -f $(LD_SCRIPT)

distcleanall: clean
	rm -rf $(ASM_DIRS)
	rm -rf assets
	rm -f *auto.$(VERSION).txt
	rm -f $(LD_SCRIPT)

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

.baserom.$(VERSION).ok: baserom.$(VERSION).z64
	@echo "$$(cat $(BASENAME).$(VERSION).sha1)  $<" | sha1sum --check
	@touch $@

$(TARGET).elf: dirs $(LD_SCRIPT) $(BUILD_DIR)/$(LIBULTRA) $(O_FILES) $(LANG_RNC_O_FILES) $(IMAGE_O_FILES)
	@$(LD) $(LD_FLAGS) $(LD_FLAGS_EXTRA) -o $@
	@printf "[$(PINK) Linker $(NO_COL)]  $<\n"

ifndef PERMUTER
$(GLOBAL_ASM_O_FILES): $(BUILD_DIR)/%.c.o: %.c  include/variables.h include/structs.h
	@$(CC_CHECK) $<
	@printf "[$(YELLOW) check $(NO_COL)] $<\n"
	@$(ASM_PROCESSOR) $(OPT_FLAGS) $< > $(BUILD_DIR)/$<
	@$(CC) -c $(CFLAGS) $(OPT_FLAGS) $(LOOP_UNROLL) $(MIPSISET) -o $@ $(BUILD_DIR)/$<
	@$(ASM_PROCESSOR) $(OPT_FLAGS) $< --post-process $@ \
		--assembler "$(AS) $(ASFLAGS)" --asm-prelude $(ASM_PROCESSOR_DIR)/prelude.inc
	@printf "[$(GREEN) ido5.3 $(NO_COL)]  $<\n"
endif

# non asm-processor recipe
$(BUILD_DIR)/%.c.o: %.c
#	@$(CC_CHECK) $<
	@$(CC) -c $(CFLAGS) $(OPT_FLAGS) $(LOOP_UNROLL) $(MIPSISET) -o $@ $<
	@printf "[$(GREEN) ido5.3 $(NO_COL)]  $<\n"



$(BUILD_DIR)/$(LIBULTRA): $(LIBULTRA)
	@mkdir -p $$(dirname $@)

$(BUILD_DIR)/%.s.o: %.s
	@$(AS) $(ASFLAGS) -o $@ $<
	@printf "[$(GREEN)  ASSEMBLER   $(NO_COL)]  $<\n"

$(BUILD_DIR)/%.bin.o: %.bin
	@$(LD) -r -b binary -o $@ $<
	@printf "[$(PINK) Linker $(NO_COL)]  $<\n"

$(TARGET).bin: $(TARGET).elf
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
	@printf "[$(CYAN) Objcopy $(NO_COL)]  $<\n"

$(TARGET).z64: $(TARGET).bin
	@printf "[$(BLUE) CopyRom $(NO_COL)]  $<\n"
	@tools/CopyRom.py $< $@ #Mask
	@printf "[$(GREEN) CRC $(NO_COL)]  $<\n"
	@$(CRC)

# fake targets for better error handling
$(SPLAT):
	$(info Repo cloned without submodules, attempting to fetch them now...)
	@which git >/dev/null || echo "ERROR: git binary not found on PATH"
	@which git >/dev/null
	git submodule update --init --recursive

baserom.$(VERSION).z64:
	$(error Place the Jet Force Gemini $(VERSION) ROM, named '$@', in the root of this repo and try again.)

### Settings
.SECONDARY:
.PHONY: all clean default
SHELL = /bin/bash -e -o pipefail
