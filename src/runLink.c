#include "common.h"

const char D_800ADC90[] = "WARNING: Unimplemented linkage operation %d\n";
const char D_800ADCC0[] = "ERROR:MIPS_HI16 without matching MIPS_LO16\n";
#ifdef VERSION_us
const char D_800AD12C[] = "REALLOC: %08x (%d)\n";
#endif


/**
 * Overlays work by having functions that call the overlay actually load TrapDanglingJump
 * That function then uses the overlayRomTable to figure out which overlay to load
 * and where the function is within that overlay. It then uses runlink to load the overlay
 * into memory if it is not already loaded, and then jumps to the function within that overlay.
 * 
 * When a function calls TrapDanglingJump, the return address is shifted back by 8 bytes 
 * to point to the exact VRAM address of the instruction that called the function.
 * That function address is used to lookup in mainRelocTable and see if it can find that address.
 * If it finds it, it then loads the index value next to it, which is an index into overlayRomTable.
 * When it finds the entry in overlayRomTable, it reads the overlay number and function offset
 * within that overlay. It then calls runlink to load that overlay if it is not already loaded,
 * and then jumps to the function offset within that overlay.
 */


 /**
  * Complete Analysis: How rcpWaitDP Uses TrapDanglingJump
  * 
  * At VRAM address 0x8004DD50, rcpWaitDP has a jal TrapDanglingJump instruction.
  * TrapDanglingJump has the ra register set to ra = 0x8004DD58
  * It then stores the address of the calling jal to t5 with: t5 = ra - 8 = 0x8004DD50
  * Then is substracted by the base address the start of the text segment: 0x8004DD50 - 0x80000450 = 0x4D900
  * 
  * Searches assets/mainRelocTable.bin for an entry matching 0x4D900, which it finds at: offset 0xD00
  * The index value next to it is 0x69E (1684)
  * That index is then used to lookup in assets/overlayRomTable.bin which is 4 bytes per entry.
  * So it reads the entry at offset 0x69E * 4 = 0x1A78 and finds 0x00315E34
  * 
  * ______________________________________________
  * |Field              | Value                   |
  * |_____________________________________________|
  * |Overlay entry      | 0x00315E34              |
  * |Overlay number	    | 3 (bits 31-20: 0x003)   |
  * |Function offset	| 0x15E34 (bits 19-0)     |
  * |_____________________________________________|
  * 
  * So according to our symbols, overlay 3 at function offset 0x15E34 is BindRegionsToObjects
  * 
  * It triggers the dynamic linker to load overlay 3 (if not already loaded)
  * The actual function called is BindRegionsToObjects at offset 0x15E34 within that overlay
  */

typedef struct RelocTableEntry {
    /* 0x00 */ u32 functionAddress; // This is the address of the calling function less 0x80000450 (0x8004DD50 - 0x80000450 = 0x4D900)
    /* 0x04 */ u32 overlayIndex;    // This is an index into overlayRomTable
} RelocTableEntry;
extern RelocTableEntry D_1ECF220[]; // mainRelocTable
extern RelocTableEntry mainRelocTable[]; // mainRelocTable

typedef struct RomTableEntry {
    union {
        u32 bytes;
        struct {
            u32 OverlayNumber : 12;
            u32 FunctionOffset : 20;
        };
    } entry;
} RomTableEntry;

extern RomTableEntry *overlayRomTable;
extern RomTableEntry D_1ED0270[]; // overlayRomTable

typedef struct OverlayHeader {
    /* 0x00 */ s32 VramBase; // (0 if not loaded, set after alloc)
    /* 0x04 */ s32 RomAddress;
    /* 0x08 */ s32 TextSize;
    /* 0x0C */ s32 DataSize;
    /* 0x10 */ s32 RodataSize;
    /* 0x14 */ u16 RelocationTableSize;
    /* 0x16 */ u16 SecondaryRelocationTableSize; // ?
    /* 0x18 */ s32 InitFunction; // -1 if none
    /* 0x1C */ u8 reserved[4];
} OverlayHeader;
extern OverlayHeader D_1ED2780[]; // overlayTable
extern OverlayHeader *overlayTable; // overlayTable


extern s32 mainRelocCount;
extern s32 overlayCount;
extern void amSetMuteMode(s32 behaviour); // 0x80000450 Start of .text
extern void *tuneSeqPlayer; // 0x800A0660 Start of .data
extern u32 *D_800FF7B4;

// Relocation section base addresses (set by runlinkDownloadCode when loading an overlay)
extern u8 *gRelocTextBase;  // Base address of overlay's .text section being relocated
extern u8 *gRelocDataBase;  // Base address for type-3 relocations (alternate section)

// typedef struct runlinkModule {
//     s32 unk0;
//     u8 pad4[25];
// } runlinkModule;
// extern runlinkModule *overlayTable;

#if 0
extern u32 D_1B94430[], D_1B96910[]; // Linker symbols for the start of two lookup tables.
// D_1B94430 is a 4 byte value used as an offset in the list symbols for overlayRomTable the start 
// D_1B96910 is just a list of symbol names as ascii strings with null byte seperators

char *GetSymbolName(u32 arg0) {
    char symbolName[96];
    u32 secondRomOffset;
    u32 symbolBytes;
    u32 romOffset;
    romOffset = &D_1B94430[arg0];
    romCopy((u32) romOffset, &symbolBytes, 8);
    secondRomOffset = *(&symbolBytes + ((u32) &romOffset & 7)) + (u32) D_1B96910;
    symbolBytes = ((u32) secondRomOffset & 7);
    romCopy(((u32) secondRomOffset & ~7), &symbolName, 96);
    return symbolName + symbolBytes;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/GetSymbolName.s")
#endif

/**
 * Relocation entry format used by the overlay dynamic linker.
 * Each entry is 8 bytes and describes how to patch an address reference.
 */
typedef struct RelocationEntry {
    /* 0x00 */ u32 symbolIndex;    // Index into overlayRomTable, OR local offset for type 1
    /* 0x04 */ union {
        u32 info;
        struct {
            u32 targetOffset : 24; // Offset into section where relocation should be applied
            u32 unused : 4;
            u32 relocType : 4;     // Relocation type (R_MIPS_32=0, LOCAL=1, R_MIPS_26=2, SPECIAL=3, HI16=5, LO16=6)
        };
        struct {
            u8 typeByte;           // Low nibble: relocType, High nibble written back after processing
            u8 offsetHi;           // Upper bits of targetOffset
            u8 offsetMid;
            union {
                u8 flags;          // Upper nibble: additional flags (2=addend, 4=stub if unloaded)
                struct {
                    u8 flagsHi : 4;
                    u8 flagsLo : 4;
                };
            };
        };
    };
} RelocationEntry; /* 8 bytes */

/**
 * MIPS instruction word layout (big-endian).
 * Used for runtime instruction patching during relocation.
 */
typedef union MipsInstruction {
    u32 word;
    struct {
        u32 opcode : 6;     // Instruction opcode (bits 26-31)
        u32 target : 26;    // J/JAL target address >> 2 (bits 0-25)
    } jump;
    struct {
        u16 immediate;      // immediate value (bits 0-15)
        u16 upper;          // rs/rt/opcode (bits 16-31)
    } itype;
} MipsInstruction; /* 4 bytes */

extern void *__BSS_SECTION_START;
extern void *__DATA_SECTION_START;
extern void *__CODE_SECTION_START;

void *ResolveRelocAddress(s32 ortIndex, s32 otIndex, RelocationEntry *relocEntry, MipsInstruction *patchLocation) {
    s32 var_v1;
    s32 addressBase;
    s32 addressOffset;
    u32 overlayNumber;
    RomTableEntry *romTableEntry;

    romTableEntry = &overlayRomTable[ortIndex];
    overlayNumber = romTableEntry->entry.OverlayNumber;
    addressOffset = 0;
    switch (relocEntry->relocType) { // Extract relocType from low 4 bits
        case 0: // R_MIPS_32: Absolute symbol reference
            switch (overlayNumber) {
                case 0xFFD: // Data section
                    overlayNumber = 0;
                    addressOffset = (u32) &__DATA_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
                case 0xFFE: // Data section
                    overlayNumber = 0;
                    addressOffset = (u32) &__DATA_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
                case 0xFFF: // BSS section
                    overlayNumber = 0;
                    addressOffset = (u32) &__BSS_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
            }
            addressBase = overlayTable[overlayNumber].VramBase;
            if (addressBase == 0) {
                // Overlay not loaded - check if caller wants stub or trap
                if (relocEntry->flagsHi == 4 || relocEntry->flagsHi == 2) {
                    return &TrapDanglingJump;
                } else {
                    return &D_800FF7B4;
                }
            }
            return addressBase + (romTableEntry->entry.FunctionOffset) + addressOffset;
        case 1: // Local offset relocation (relative to section base)
            var_v1 = overlayTable[otIndex].VramBase + relocEntry->symbolIndex;
            if (relocEntry->flagsHi == 2) {
                var_v1 += patchLocation->word;
            }
            return var_v1;
        case 2: // R_MIPS_26: Jump target relocation
            return (patchLocation->jump.target << 2) + overlayTable[otIndex].VramBase;
        default:
            return NULL;
    }
}

/**
 * Patches a MIPS instruction with a relocated address.
 * @param instr     Pointer to the instruction to patch
 * @param address   The resolved address to patch in
 * @param relocType The relocation type (2=R_MIPS_32, 4=R_MIPS_26, 5=HI16, 6=LO16)
 */
void PatchInstruction(MipsInstruction *instr, u32 address, u8 relocType) {
    u32 instrWord;
    u32 temp;

    switch (relocType) {
        case 2: // R_MIPS_32: Store full 32-bit address
            instr->word = address;
            break;
        case 4: // R_MIPS_26: Patch jump target (preserve opcode)
            temp = (address >> 2) & 0x03FFFFFF;
            instrWord = instr->word;
            temp ^= instrWord;
            temp = (temp << 6) >> 6; // Clear upper 6 bits (opcode)
            temp ^= instrWord;
            instr->word = temp;
            break;
        case 5: // R_MIPS_HI16: Patch upper 16 bits of address
            temp = address >> 16;
            if (address & 0x8000) {
                temp++; // Adjust for sign extension of LO16
            }
            instr->itype.upper = (u16) temp;
            break;
        case 6: // R_MIPS_LO16: Patch lower 16 bits of address
            instr->itype.upper = (u16) address;
            break;
    }
    osWritebackDCache(instr, sizeof(MipsInstruction));
    osInvalICache(instr, sizeof(MipsInstruction));
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800536F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkDownloadCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

s32 runlinkIsModuleLoaded(s32 module) {
    return overlayTable[module].VramBase;
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80053FC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkFreeCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80054368.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkFlushModules.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkInitialise.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkSuspendCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkSetDestructTimer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkTick.s")

typedef struct LowMemoryStruct {
    union {
        u16 unk0;
        struct {
            u8 unk0_0;
            u8 unk0_1;
        };
    };
} LowMemoryStruct;

extern s32 overlayCount;
extern LowMemoryStruct *D_800FF838;

void runlinkLowMemoryPanic(void) {
    s32 overlayIndex;
    u32 temp_v0;
    LowMemoryStruct *temp_s0;

    overlayIndex = overlayCount;
    while (overlayIndex--) {
        temp_s0 = &D_800FF838[overlayIndex];
        temp_v0 = temp_s0->unk0;
        if ((temp_v0 >> 6) != 0) {
            if (!(temp_v0 & 0x3F)) {
                runlinkFreeCode(overlayIndex);
                temp_s0->unk0 = (u16) (temp_s0->unk0 & 0x3F);
                temp_s0->unk0_1 = (u8) (temp_s0->unk0_1 & 0xFFC0);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkGetAddressInfo.s")
