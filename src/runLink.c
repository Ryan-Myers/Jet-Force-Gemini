#include "runLink.h"
#include "common.h"
#include "memory.h"
#include "mips.h"
#include "pi.h"

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
 * The index value next to it is 0x69E (1694)
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
 * So according to our symbols, overlay 3 at function offset 0x15E34 is cloneTasksQueueAndWait
 *
 * It triggers the dynamic linker to load overlay 3 (if not already loaded)
 * The actual function called is cloneTasksQueueAndWait at offset 0x15E34 within that overlay
 */

typedef struct RelocTableEntry {
    union {
        u32 bytes;
        struct {
            u32 functionAddress : 24; // This is the address of the calling function less 0x80000450 (0x8004DD50 -
                                      // 0x80000450 = 0x4D900)
            u32 unknown : 8;          // Unknown, almost always seems to be 0x40
        };
    } entry;
    /* 0x04 */ u32 overlayIndex; // This is an index into overlayRomTable
} RelocTableEntry;
extern RelocTableEntry D_1ECF220[];     // mainRelocTable ROM address
extern RelocTableEntry *mainRelocTable; // mainRelocTable RAM pointer

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
    /* 0x14 */ u16 RelocationTableSize; // This relocation stays in memory after the overlay is loaded, so that other
                                        // overlays can reference it
    /* 0x16 */ u16 SecondaryRelocationTableSize; // This relocation is freed after the overlay is loaded
    /* 0x18 */ s32 InitFunction;                 // -1 if none, offset from VramBase
    /* 0x1C */ s32 ResumeFunction;               // -1 if none, offset from VramBase
} OverlayHeader;
extern OverlayHeader D_1ED2780[];   // overlayTable
extern OverlayHeader *overlayTable; // overlayTable

extern s32 mainRelocCount;
extern s32 overlayCount;
extern void amSetMuteMode(s32 behaviour); // 0x80000450 Start of .text
extern void *tuneSeqPlayer;               // 0x800A0660 Start of .data

// Placeholder address returned when a symbol cannot be resolved (overlay not loaded)
extern u32 gUnresolvedSymbolAddr;

// typedef struct runlinkModule {
//     s32 unk0;
//     u8 pad4[25];
// } runlinkModule;
// extern runlinkModule *overlayTable;

#ifdef NON_EQUIVALENT
// ROM addresses for symbol name lookup tables
extern u32 D_1FEB040[]; // Symbol offset table (4 bytes per entry)
extern u8 D_1FED550[];  // Symbol string table base

/**
 * Retrieves a symbol name from ROM given its index.
 * @param symbolIndex Index into the symbol offset table
 * @return Pointer to the symbol name string (in stack buffer - use immediately!)
 */
char *GetSymbolName(u32 symbolIndex) {
    char stringBuffer[96]; // Buffer for string data
    u32 offsetAddr;
    u32 stringOffset;
    u32 stringAddr;
    u32 bufferOffset;
    u32 offsetTableEntry[2]; // 8-byte aligned buffer for offset table read

    // Calculate ROM address of offset table entry
    offsetAddr = (u32) &D_1FEB040[symbolIndex];

    // Read 8 bytes aligned (ROM requires 8-byte aligned reads)
    romCopy(offsetAddr & ~7, (u32) offsetTableEntry, 8);

    // Extract the 4-byte offset value using low bits to index into buffer
    stringOffset = ((((*((u32 *) (((u8 *) offsetTableEntry) + (offsetAddr & 7)))))) & 0xFFFFFFFFFFFFFFFF);

    // Calculate string ROM address
    stringAddr = stringOffset + (u32) D_1FED550;

    // Save offset within aligned block
    bufferOffset = stringAddr & 7;
    if (((!stringBuffer) && (!stringBuffer)) && (!stringBuffer)) {}
    if (!bufferOffset) {}
    // Read 96 bytes of string data (aligned)
    romCopy(stringAddr & ~7, (u32) stringBuffer, 96);

    // Return pointer to string within buffer
    return stringBuffer + bufferOffset;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/GetSymbolName.s")
#endif

/**
 * Relocation entry format used by the overlay dynamic linker.
 * Each entry is 8 bytes and describes how to patch an address reference.
 */
typedef struct RelocationEntry {
    /* 0x00 */ u32 symbolIndex; // Index into overlayRomTable, OR local offset for type 1
    /* 0x04 */ union {
        u32 info;
        struct {
            u32 targetOffset : 24; // Offset into section where relocation should be applied
            u32 patchOperation : 4;
            u32 relocType : 4; // Relocation type (R_MIPS_32=0, LOCAL=1, R_MIPS_26=2, SPECIAL=3, HI16=5, LO16=6)
        };
        struct {
            u8 typeByte; // Low nibble: relocType, High nibble written back after processing
            u8 offsetHi; // Upper bits of targetOffset
            u8 offsetMid;
            union {
                u8 flags;
                struct {
                    u8 patchOperation2 : 4; // 2=R_MIPS_32, 4=JAL, 5=HI16, 6=LO16
                    u8 resolveType : 4;     // 0=EXTERNAL, 1=LOCAL, 2=JUMP, 3=DATA
                };
            };
        };
    };
} RelocationEntry; /* 8 bytes */

extern void *__BSS_SECTION_START;
extern void *__BSS_SECTION_END;
extern void *__DATA_SECTION_START;
extern void *__CODE_SECTION_START;

void *ResolveRelocAddress(s32 ortIndex, s32 otIndex, RelocationEntry *relocEntry, MipsInstruction *patchLocation) {
    s32 address;
    s32 addressBase;
    s32 addressOffset;
    u32 overlayNumber;
    RomTableEntry *romTableEntry;

    romTableEntry = &overlayRomTable[ortIndex];
    overlayNumber = romTableEntry->entry.OverlayNumber;
    addressOffset = 0;
    switch (relocEntry->relocType) {
        case RELOC_TYPE_EXTERNAL: // R_MIPS_32: Absolute symbol reference
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
                if (relocEntry->patchOperation == RELOC_PATCH_JAL || relocEntry->patchOperation == RELOC_PATCH_WORD) {
                    return &TrapDanglingJump;
                } else {
                    return &gUnresolvedSymbolAddr;
                }
            }
            return (void *) (addressBase + (romTableEntry->entry.FunctionOffset) + addressOffset);
        case RELOC_TYPE_LOCAL: // Local offset relocation (relative to section base)
            address = overlayTable[otIndex].VramBase + relocEntry->symbolIndex;
            if (relocEntry->patchOperation == RELOC_PATCH_WORD) {
                address += patchLocation->word;
            }
            return (void *) address;
        case RELOC_TYPE_JUMP: // R_MIPS_26: Jump target relocation
            return (void *) ((patchLocation->jump.target << 2) + overlayTable[otIndex].VramBase);
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
        case RELOC_PATCH_WORD: // R_MIPS_32: Store full 32-bit address
            instr->word = address;
            break;
        case RELOC_PATCH_JAL: // R_MIPS_26: Patch jump target (preserve opcode)
            temp = (address >> 2) & 0x03FFFFFF;
            instrWord = instr->word;
            temp ^= instrWord;
            temp = (temp << 6) >> 6; // Clear upper 6 bits (opcode)
            temp ^= instrWord;
            instr->word = temp;
            break;
        case RELOC_PATCH_HI16: // R_MIPS_HI16: Patch upper 16 bits of address
            temp = address >> 16;
            if (address & 0x8000) {
                temp++; // Adjust for sign extension of LO16
            }
            instr->itype.upper = (u16) temp;
            break;
        case RELOC_PATCH_LO16: // R_MIPS_LO16: Patch lower 16 bits of address
            instr->itype.upper = (u16) address;
            break;
    }
    osWritebackDCache(instr, sizeof(MipsInstruction));
    osInvalICache(instr, sizeof(MipsInstruction));
}

// Could be NON_MATCHING, but not 100% sure.
#ifdef NON_EQUIVALENT
// Relocation section base addresses (set by runlinkDownloadCode when loading an overlay)
extern u8 *gRelocTextBase; // Base address of overlay's .text section being relocated
extern u8 *gRelocDataBase; // Base address for type-3 relocations (alternate section)

// Returns how many entries were consumed (important for the HI16/LO16 pair case where it processes 2 entries at once).
s32 ProcessRelocationEntry(RelocationEntry *relocEntry, s32 otIndex) {
    u32 combinedAddr;
    u8 *resolvedAddr;
    MipsInstruction *patchLocation;
    MipsInstruction *nextPatchLocation;
    s32 overlayNumber;
    s32 patchOperation;
    s32 resolveType;
    u32 nextLoImmediate;
    u32 currLoImmediate;

    patchOperation = relocEntry->patchOperation;
    resolveType = relocEntry->resolveType;
    if (relocEntry->relocType == RELOC_TYPE_DATA) {
        patchLocation = (MipsInstruction *) &gRelocDataBase[relocEntry->targetOffset];
        relocEntry->relocType = RELOC_TYPE_EXTERNAL; // Change to external so that it can be resolved normally
    } else {
        patchLocation = (MipsInstruction *) &gRelocTextBase[relocEntry->targetOffset];
    }
    resolvedAddr = ResolveRelocAddress(relocEntry->symbolIndex, otIndex, relocEntry, patchLocation);
    if (patchOperation == RELOC_PATCH_HI16) {
        overlayNumber = overlayRomTable[relocEntry->symbolIndex].entry.OverlayNumber;
        if (overlayNumber >= 0xFFC) {
            overlayNumber = 0;
        }
        if (relocEntry->relocType == RELOC_TYPE_EXTERNAL && (overlayTable[overlayNumber].VramBase == 0)) {
            resolvedAddr = (u32) &gUnresolvedSymbolAddr;
        }
        nextPatchLocation = (MipsInstruction *) &gRelocTextBase[relocEntry[1].targetOffset];
        currLoImmediate = patchLocation->itype.upper;
        nextLoImmediate = nextPatchLocation->itype.upper;
        if (nextLoImmediate & 0x8000) {
            nextLoImmediate |= 0xFFFF0000;
        }
        combinedAddr = (currLoImmediate << 16) + nextLoImmediate;
        // combinedAddr = ((currLoImmediate << 2) << 14) + nextLoImmediate;
        if (combinedAddr != (u32) &gUnresolvedSymbolAddr) {
            resolvedAddr += combinedAddr;
        }
        PatchInstruction(patchLocation, resolvedAddr, RELOC_PATCH_HI16);
        PatchInstruction(nextPatchLocation, resolvedAddr, RELOC_PATCH_LO16);
        relocEntry->resolveType = resolveType;
        return 2;
    }
    if (patchOperation == RELOC_PATCH_LO16) {
        overlayNumber = overlayRomTable[relocEntry->symbolIndex].entry.OverlayNumber;
        if (overlayNumber >= 0xFFC) {
            overlayNumber = 0;
        }
        if (relocEntry->relocType == RELOC_TYPE_EXTERNAL && (overlayTable[overlayNumber].VramBase == 0)) {
            resolvedAddr = (u32) &gUnresolvedSymbolAddr;
        }
        PatchInstruction(patchLocation, resolvedAddr + patchLocation->itype.upper, RELOC_PATCH_LO16);
        relocEntry->resolveType = resolveType;
        return 1;
    } else {
        PatchInstruction(patchLocation, resolvedAddr, patchOperation);
        relocEntry->resolveType = resolveType;
        return 1;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/ProcessRelocationEntry.s")
#endif

// Forward declarations
s32 ProcessRelocationEntry(RelocationEntry *relocEntry, s32 otIndex);
void runlinkResumeCode(s32 overlayIndex);

typedef struct RelocContext {
    u32 unk0;      // 0x00 - unused?
    u8 *textBase;  // 0x04 - gRelocTextBase
    u8 *dataBase;  // 0x08 - gRelocDataBase
    u8 *bssBase;   // 0x0C
    u8 *relocBase; // 0x10 - relocation table base
} RelocContext;
extern RelocContext gRelocContext;

typedef struct PendingOverlayLoad {
    u32 unk0;         // 0x00 - possibly status/flags
    s32 overlayIndex; // 0x04 - overlay number being loaded
} PendingOverlayLoad; // 8 bytes

extern PendingOverlayLoad gPendingOverlayLoads[16];
extern s32 mmColourTagUnk2;
extern s32 D_800B0B50_B1750;

/**
 * Downloads and links an overlay module into memory.
 * @param overlayIndex Index of the overlay to load
 * @return 1 on success, 0 on failure (already loading or allocation failed)
 */
s32 runlinkDownloadCode(s32 overlayIndex) {
    OverlayHeader *overlay;
    RelocationEntry *relocTable;
    RelocationEntry *relocEntry;
    s32 savedDelay;
    PendingOverlayLoad *overlayLoad;
    s32 relocCount;
    s32 otherIndex;
    s32 overlayNum;

    overlay = &overlayTable[overlayIndex];
    overlayLoad = gPendingOverlayLoads;
    relocTable = NULL;

    // Already loaded - return success
    if (overlay->VramBase != 0) {
        return 1;
    }

    // Check if this overlay is already being loaded (re-entrancy check)
    relocCount = ARRAY_COUNT(gPendingOverlayLoads);
    if (relocCount != 0) { // check count *just in case*
        while (relocCount--) {
            if (overlayIndex == overlayLoad->overlayIndex) {
                return 0; // Already in progress
            }
            overlayLoad++;
        }
    }

    // Mark this overlay as being loaded (for memory manager colour tagging)
    mmColourTagUnk2 = overlayIndex;

    // Allocate memory for overlay (text + data + rodata + relocation table)
    overlay->VramBase = (s32) mmAlloc(
        overlay->TextSize + overlay->DataSize + overlay->RodataSize + overlay->RelocationTableSize, COLOUR_TAG_GREY);

    mmColourTagUnk2 = -1;

    if (overlay->VramBase == 0) {
        return 0; // Allocation failed
    }

    // If there's a secondary relocation table, allocate and load it
    if (overlay->SecondaryRelocationTableSize) {
        relocTable = (RelocationEntry *) mmAlloc(overlay->SecondaryRelocationTableSize, COLOUR_TAG_GREY);
        if (relocTable == NULL) {
            mmFree((void *) overlay->VramBase);
            return 0;
        }
        // Load secondary relocation table from ROM
        romCopy(overlay->RomAddress + overlay->TextSize + overlay->DataSize + overlay->RelocationTableSize,
                (u32) relocTable, overlay->SecondaryRelocationTableSize);
    }

    // Set up relocation context with section base addresses
    gRelocContext.textBase = (u8 *) overlay->VramBase;
    gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
    gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
    gRelocContext.relocBase = (u8 *) gRelocContext.bssBase + overlay->RodataSize;

    // Load overlay code and data from ROM
    if (overlay->RodataSize == 0) {
        // No BSS - copy everything including relocation table
        romCopy(overlay->RomAddress, overlay->VramBase,
                overlay->TextSize + overlay->DataSize + overlay->RelocationTableSize);
    } else {
        // Has BSS - copy text and data only
        romCopy(overlay->RomAddress, overlay->VramBase, overlay->TextSize + overlay->DataSize);

        // Zero out BSS section
        {
            s32 *bssPtr = (s32 *) gRelocContext.bssBase;
            relocCount = (u32) overlay->RodataSize >> 2;
            while (relocCount--) {
                *bssPtr++ = 0;
            }
        }

        // Load relocation table after BSS
        romCopy(overlay->RomAddress + overlay->TextSize + overlay->DataSize, (u32) gRelocContext.relocBase,
                overlay->RelocationTableSize);
    }

    // Invalidate instruction cache for the new code
    osInvalICache((void *) overlay->VramBase, overlay->TextSize);

    // Process secondary relocation table (if present)
    if (relocTable != NULL) {
        savedDelay = mmGetDelay();
        relocCount = (u32) overlay->SecondaryRelocationTableSize >> 3;
        relocEntry = relocTable;

        while (relocCount-- > 0) {
            if (ProcessRelocationEntry(relocEntry, overlayIndex) == 2) {
                relocCount--;
                relocEntry++;
            }
            relocEntry++;
        }

        mmSetDelay(0);
        mmFree(relocTable);
        mmSetDelay(savedDelay);
    }

    // Process main relocation table
    relocCount = overlay->RelocationTableSize;
    relocCount = ((u32) relocCount >> 3);
    relocEntry = (RelocationEntry *) gRelocContext.relocBase;

    while (relocCount-- > 0) {
        if (ProcessRelocationEntry(relocEntry, overlayIndex) == 2) {
            relocCount--;
            relocEntry++;
        }
        relocEntry++;
    }

    // Update other loaded overlays that reference this newly loaded overlay
    overlay = overlayTable;
    for (otherIndex = 0; otherIndex < overlayCount; otherIndex++) {
        if (overlay->VramBase != 0 && otherIndex != overlayIndex) {
            if (otherIndex == 0) {
                // Main module - use special relocation context
                gRelocContext.textBase = (u8 *) &__CODE_SECTION_START; // Start of .text
                gRelocContext.dataBase = (u8 *) &__DATA_SECTION_START; // Start of .data
                gRelocContext.bssBase = (u8 *) &__BSS_SECTION_START;
                gRelocContext.relocBase = (u8 *) mainRelocTable;
                relocEntry = (RelocationEntry *) mainRelocTable;
                relocCount = mainRelocCount;
            } else {
                // Other overlay - set up context for it
                gRelocContext.textBase = (u8 *) overlay->VramBase;
                gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
                gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
                gRelocContext.relocBase = (u8 *) gRelocContext.bssBase + overlay->RodataSize;
                relocEntry = (RelocationEntry *) gRelocContext.relocBase;
                relocCount = (u32) overlay->RelocationTableSize >> 3;
            }

            while (relocCount-- > 0) {
                // Check if this relocation references the newly loaded overlay
                overlayNum = overlayRomTable[relocEntry->symbolIndex].entry.OverlayNumber;
                if (overlayNum > 0xFFB) {
                    overlayNum = 0;
                }

                if (overlayNum == overlayIndex) {
                    if (relocEntry->relocType == RELOC_TYPE_EXTERNAL || relocEntry->relocType == RELOC_TYPE_DATA) {
                        if (ProcessRelocationEntry(relocEntry, otherIndex) == 2) {
                            relocCount--;
                            relocEntry++;
                        }
                    }
                }

                relocEntry++;
            }
        }
        overlay++;
    }

    // Call overlay's init function if present
    overlay = &overlayTable[overlayIndex];
    if (overlay->InitFunction != -1) {
        ((void (*)(void))(overlay->VramBase + overlay->InitFunction))();
    }

    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

s32 runlinkIsModuleLoaded(s32 module) {
    return overlayTable[module].VramBase;
}

/**
 * Ensures an overlay is loaded/resumed and calls its ResumeFunction if present.
 * If the overlay is in the pending load list, resumes it first.
 * @param overlayIndex Index of the overlay
 */
void runlinkCallResumeFunction(s32 overlayIndex) {
    OverlayHeader *overlay;
    PendingOverlayLoad *pendingLoad;
    s32 relocCount;

    overlay = &overlayTable[overlayIndex];

    // If no resume function defined, nothing to do
    if (overlay->ResumeFunction == -1) {
        return;
    }

    pendingLoad = gPendingOverlayLoads;

    // If overlay not loaded, check if it's in pending list and resume it
    if (overlay->VramBase == 0) {
        relocCount = ARRAY_COUNT(gPendingOverlayLoads);
        while (relocCount--) {
            if (overlayIndex == pendingLoad->overlayIndex) {
                runlinkResumeCode(overlayIndex);
                break;
            }
            pendingLoad++;
        }
    }

    // If overlay is now loaded, call its resume function
    if (overlay->VramBase != 0) {
        ((void (*)(void))(overlay->VramBase + overlay->ResumeFunction))();
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkFreeCode.s")

/**
 * Timer/state entry for overlay self-destruct system.
 * gSelfDestructTimers points to an array indexed by overlay number.
 *
 * The 16-bit word layout:
 *   - selfDestructTimer (10 bits, bits 6-15): Ticks until overlay auto-unloads, 0 = disabled
 *   - refCount (6 bits, bits 0-5): Reference counter or usage flags
 *
 * runlinkTick() decrements both timers each frame.
 * When selfDestructTimer reaches 0 and refCount is also 0, runlinkFreeCode() is called.
 *
 * runlinkSetDestructTimer(overlayIndex, selfDestructTimer, refCount) sets both fields.
 */
typedef struct OverlayTimerEntry {
    union {
        u16 packed; // Full 16-bit access: selfDestructTimer[9:0] << 6 | refCount[5:0]
        struct {
            u16 selfDestructTimer : 10;
            u16 refCount : 6;
        };
    };
} OverlayTimerEntry;

extern OverlayTimerEntry *gSelfDestructTimers;

/**
 * Unloads an overlay and patches all references back to TrapDanglingJump.
 * @param overlayIndex Index of the overlay to unload
 */
void runlinkUnloadOverlay(s32 overlayIndex) {
    OverlayHeader *overlay;
    PendingOverlayLoad *pendingLoad;
    RelocationEntry *relocEntry;
    MipsInstruction *patchLocation;
    s32 overlayNum;
    s32 loadedAddress;
    s32 relocType;
    s32 found;
    s32 i;
    s32 patchOperation;
    u32 address;

    overlay = &overlayTable[overlayIndex];
    runlinkCallResumeFunction(overlayIndex);
    loadedAddress = overlay->VramBase;
    address = loadedAddress;

    if (address == 0) {
        found = FALSE;
        // Overlay not loaded - check if it's in pending list
        pendingLoad = gPendingOverlayLoads;
        i = ARRAY_COUNT(gPendingOverlayLoads);
        while (i--) {
            if (overlayIndex == pendingLoad->overlayIndex) {
                found = TRUE;
                break;
            }
            pendingLoad++;
        }

        if (found == FALSE) {
            return;
        }

        // Free the pending load's memory (base + textSize offset)
        mmFree((void *) (pendingLoad->unk0 + overlay->TextSize));
        pendingLoad->overlayIndex = 0xFFB; // Mark slot as unused
        return;
    }

    // Overlay is loaded - free it
    mmFree((void *) address);
    overlay->VramBase = 0;

    // Clear overlay timer entry - reset selfDestructTimer and refCount
    gSelfDestructTimers[overlayIndex].selfDestructTimer = 0;
    gSelfDestructTimers[overlayIndex].refCount = 0;

    // Iterate through mainRelocTable and patch entries referencing this overlay
    relocEntry = (RelocationEntry *) mainRelocTable;
    i = mainRelocCount;
    while (i--) {
        relocType = relocEntry->relocType;
        overlayNum = overlayRomTable[relocEntry->symbolIndex].entry.OverlayNumber;

        if (overlayNum >= 0xFFC) {
            overlayNum = 0;
        }

        if (overlayNum == overlayIndex) {
            // This entry references the overlay being unloaded
            if (relocEntry->relocType == RELOC_TYPE_DATA) {
                // Type 3: data section relocation
                patchLocation = (MipsInstruction *) ((u8 *) &__DATA_SECTION_START + (relocEntry->targetOffset));
                relocEntry->relocType = RELOC_TYPE_EXTERNAL;
            } else {
                // Other types: text section relocation
                patchLocation = (MipsInstruction *) ((u8 *) &__CODE_SECTION_START + (relocEntry->targetOffset));
            }

            // Get patchOperation and determine what to patch
            patchOperation = relocEntry->patchOperation;
            if ((patchOperation ^ 0) == RELOC_PATCH_JAL) { // FAKE MATCH
                // Patch back to TrapDanglingJump
                address = (u32) TrapDanglingJump;
            } else {
                // Clear the reference
                address = NULL;
            }
            PatchInstruction(patchLocation, address, patchOperation);
        }

        relocEntry->relocType = relocType;

        relocEntry++;
    }
}

void runlinkFlushModules(void) {
    PendingOverlayLoad *pendingLoad;
    s32 overlayIndex;

    pendingLoad = gPendingOverlayLoads;

    for (overlayIndex = overlayCount - 1; overlayIndex > 0; overlayIndex--) {
        runlinkUnloadOverlay(overlayIndex);
    }

    for (overlayIndex = ARRAY_COUNT(gPendingOverlayLoads); overlayIndex--;) {
        if (pendingLoad->overlayIndex != 0xFFB) {
            mmFree((void *) (overlayTable[pendingLoad->overlayIndex].TextSize + pendingLoad->unk0));
            pendingLoad->overlayIndex = 0xFFB;
        }

        pendingLoad++;
    }
}

extern s32 D_800A38F4_A44F4; // Some flag cleared at init
extern s32 D_800A38F8_A44F8; // Symbol table size (D_1FED550 - D_1FEB040)

// ROM addresses for runlink tables
extern u8 symbolsTable_offsets_ROM_START[];
extern u8 symbolsTable_offsets_ROM_END[];
extern u8 overlayRomTable_ROM_START[];
extern u8 overlayRomTable_ROM_END[];
extern u8 overlayTable_ROM_START[];
extern u8 overlayTable_ROM_END[];
extern u8 mainRelocTable_ROM_START[];
extern u8 mainRelocTable_ROM_END[];
extern u8 overlayCode_ROM_START[];
extern u8 overlayCode_ROM_END[];

#ifdef NON_EQUIVALENT
/**
 * Initializes the runtime linker system.
 * - Allocates and copies overlayTable, overlayRomTable, and mainRelocTable from ROM
 * - Sets up the main module (overlay 0) with section base addresses
 * - Initializes the pending overlay load slots
 * - Allocates the overlay timer entry array (gSelfDestructTimers)
 * - Converts relative ROM addresses in overlay headers to absolute
 */
void runlinkInitialise(void) {
    s32 overlayTableSize;
    s32 tableSize;
    PendingOverlayLoad *pendingSlot;
    OverlayHeader *overlayEntry;
    OverlayTimerEntry *timerArray;
    s32 i;

    // Store symbol table size for GetSymbolName
    D_800A38F8_A44F8 = symbolsTable_offsets_ROM_END - symbolsTable_offsets_ROM_START;

    // Allocate and copy overlayTable from ROM
    // Extra 0x20 bytes at start for main module header (overlay 0)
    overlayTableSize = overlayTable_ROM_END - overlayTable_ROM_START;
    overlayTable = (OverlayHeader *) mmAlloc(overlayTableSize + sizeof(OverlayHeader), COLOUR_TAG_WHITE);
    romCopy((u32) &overlayTable_ROM_START, (u32) (overlayTable + 1), overlayTableSize);

    // Allocate and copy overlayRomTable from ROM
    tableSize = overlayRomTable_ROM_END - overlayRomTable_ROM_START;
    overlayRomTable = (RomTableEntry *) mmAlloc(tableSize, COLOUR_TAG_WHITE);
    romCopy((u32) &overlayRomTable_ROM_START, (u32) overlayRomTable, tableSize);

    // Allocate and copy mainRelocTable from ROM
    // First word contains the count, actual table starts at +4
    tableSize = mainRelocTable_ROM_END - mainRelocTable_ROM_START;
    mainRelocTable = (RelocTableEntry *) mmAlloc(tableSize, COLOUR_TAG_WHITE);
    romCopy((u32) &mainRelocTable_ROM_START, (u32) mainRelocTable, tableSize);

    // Extract mainRelocCount from first word, then advance pointer past it
    mainRelocCount = mainRelocTable->entry.bytes;
    mainRelocTable = &mainRelocTable[1];

    // Calculate overlay count from table size (each entry is 0x20 bytes)
    // +1 for the main module (overlay 0)
    overlayCount = (overlayTableSize / sizeof(OverlayHeader)) + 1;

    // Initialize pending overlay load slots to unused (0xFFB)
    // Loop from end to start
    pendingSlot = &gPendingOverlayLoads[ARRAY_COUNT(gPendingOverlayLoads) - 1];
    i = ARRAY_COUNT(gPendingOverlayLoads);
    while (i--) {
        pendingSlot->overlayIndex = 0xFFB;
        pendingSlot--;
    }

    // Allocate and zero the overlay timer array
    timerArray = (OverlayTimerEntry *) mmAlloc(overlayCount * sizeof(OverlayTimerEntry), COLOUR_TAG_WHITE);
    gSelfDestructTimers = timerArray;
    bzero(timerArray, overlayCount * sizeof(OverlayTimerEntry));

    // Initialize main module header (overlay 0) with section addresses
    overlayTable->VramBase = (s32) &__CODE_SECTION_START;
    overlayTable->RomAddress = 0;
    overlayTable->TextSize = (s32) ((u8 *) &__DATA_SECTION_START - (u8 *) &__CODE_SECTION_START);
    overlayTable->DataSize = (s32) ((u8 *) &__BSS_SECTION_START - (u8 *) &__DATA_SECTION_START);
    overlayTable->RodataSize = (s32) ((u8 *) &__BSS_SECTION_END - (u8 *) &__BSS_SECTION_START);
    overlayTable->RelocationTableSize = mainRelocCount * sizeof(RelocTableEntry);
    overlayTable->SecondaryRelocationTableSize = 0;

    // Convert relative ROM addresses to absolute for all overlay entries
    overlayEntry = overlayTable + 1; // Start from overlay 1
    i = overlayCount - 1;
    while (i--) {
        overlayEntry->RomAddress = (s32) (overlayCode_ROM_START + overlayEntry->RomAddress);
        overlayEntry++;
    }

    D_800A38F4_A44F4 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkInitialise.s")
#endif

void runlinkSuspendCode(s32 overlayIndex) {
    OverlayHeader *overlay;
    PendingOverlayLoad *pendingLoad;
    s32 remaining;
    s32 savedDelay;

    overlay = &overlayTable[overlayIndex];
    if (overlay->VramBase != 0) {
        pendingLoad = gPendingOverlayLoads;
        remaining = ARRAY_COUNT(gPendingOverlayLoads) - 1;
        do {
            if (pendingLoad->overlayIndex == 0xFFB) {
                savedDelay = mmGetDelay();
                pendingLoad->unk0 = overlay->VramBase;
                pendingLoad->overlayIndex = overlayIndex;
                mmSetDelay(0);
                D_800A38F0_A44F0 = TRUE;
                runlinkFreeCode(overlayIndex);
                D_800A38F0_A44F0 = FALSE;
                mmSetDelay(savedDelay);
                mmAllocAtAddr(overlay->DataSize + overlay->RodataSize + overlay->RelocationTableSize,
                              (void *) (pendingLoad->unk0 + overlay->TextSize), COLOUR_TAG_GREY);
                return;
            }
            pendingLoad++;
        } while (remaining--);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeAll.s")

void runlinkSetDestructTimer(s32 index, u16 selfDestructTimer, u16 refCount) {
    gSelfDestructTimers[index].selfDestructTimer = selfDestructTimer;
    gSelfDestructTimers[index].refCount = refCount;
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkTick.s")

extern s32 overlayCount;

/**
 * Called when memory is low - frees overlays that have no references.
 * Iterates through all overlays and unloads any that have:
 *   - A non-zero selfDestructTimer
 *   - A zero refCount
 */
void runlinkLowMemoryPanic(void) {
    s32 overlayIndex;
    OverlayTimerEntry *timerEntry;

    overlayIndex = overlayCount;
    while (overlayIndex--) {
        timerEntry = &gSelfDestructTimers[overlayIndex];
        if (timerEntry->selfDestructTimer != 0) { // Has selfDestructTimer set?
            if (timerEntry->refCount == 0) {      // No references
                runlinkFreeCode(overlayIndex);
                timerEntry->selfDestructTimer = 0;
                timerEntry->refCount = 0;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkGetAddressInfo.s")
