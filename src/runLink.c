#include "runLink.h"
#include "common.h"
#include "memory.h"
#include "mips.h"
#include "pi.h"
#include "types.h"

// .data
s32 gStopCallResumeFunction = FALSE;
s32 gRunlinkNeedsInit = TRUE; // Set to FALSE once the init function has completed.
s32 gNumberOfSymbols = 0;
s32 AllowSelfDestructing = TRUE;

// .bss
OverlayHeader *gOverlayTable;
RelocationEntry *gMainRelocTable;
RomTableEntry *gOverlayRomTable;
s32 gOverlayCount;
s32 gMainRelocCount;
UNUSED s32 D_800FEAB4_Pad;
RelocContext gRelocContext;
UNUSED s32 D_800FEACC_Pad;
UNUSED s32 D_800FEAD0_Pad;
u32 gUnresolvedSymbolAddr; // Placeholder address returned when a symbol cannot be resolved (overlay not loaded)
PendingOverlayLoad gPendingOverlayLoads[16];
OverlayTimerEntry *gSelfDestructTimers;

/**
 * Retrieves a symbol name from ROM given its index.
 */
char *GetSymbolName(u32 symbolIndex) {
    u32 stringOffset;
    u32 romOffset;
    char buffer[96];

    // Calculate ROM address of offset table entry
    romOffset = (u32) ((u32 *) symbolsTable_offsets_ROM_START + symbolIndex);

    // Read 8 bytes aligned (ROM requires 8-byte aligned reads)
    romCopy(romOffset & ~7, buffer, 8);

    // Extract the 4-byte offset value using low bits to index into buffer
    stringOffset = (*(u32 *) &buffer[romOffset & 7]);

    // Calculate string ROM address
    stringOffset += (u32) symbolsTable_symbol_names_ROM_START;

    romOffset = stringOffset & 7;
    stringOffset = stringOffset & ~7;

    // Read 96 bytes of string data (aligned)
    romCopy(stringOffset, buffer, sizeof(buffer));

    // Return pointer to string within buffer
    return &buffer[romOffset];
}

void *ResolveRelocAddress(s32 ortIndex, s32 otIndex, RelocationEntry *relocEntry, MipsInstruction *patchLocation) {
    s32 address;
    s32 addressBase;
    s32 addressOffset;
    u32 overlayNumber;
    RomTableEntry *romTableEntry;

    romTableEntry = &gOverlayRomTable[ortIndex];
    overlayNumber = romTableEntry->OverlayNumber;
    addressOffset = 0;
    switch (relocEntry->relocType) {
        case RELOC_TYPE_EXTERNAL: // R_MIPS_32: Absolute symbol reference
            switch (overlayNumber) {
                case OVERLAY_SECTION_DATA_1: // Data section
                    overlayNumber = 0;
                    addressOffset = (u32) &__DATA_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
                case OVERLAY_SECTION_DATA_2: // Data section
                    overlayNumber = 0;
                    addressOffset = (u32) &__DATA_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
                case OVERLAY_SECTION_BSS: // BSS section
                    overlayNumber = 0;
                    addressOffset = (u32) &__BSS_SECTION_START - (u32) &__CODE_SECTION_START;
                    break;
            }
            addressBase = (u32) gOverlayTable[overlayNumber].VramBase;
            if (addressBase == 0) {
                // Overlay not loaded - check if caller wants stub or trap
                if (relocEntry->patchOperation == RELOC_PATCH_JAL || relocEntry->patchOperation == RELOC_PATCH_WORD) {
                    return &TrapDanglingJump;
                } else {
                    return &gUnresolvedSymbolAddr;
                }
            }
            return (void *) (addressBase + (romTableEntry->FunctionOffset) + addressOffset);
        case RELOC_TYPE_LOCAL: // Local offset relocation (relative to section base)
            address = (u32) gOverlayTable[otIndex].VramBase + relocEntry->symbolIndex;
            if (relocEntry->patchOperation == RELOC_PATCH_WORD) {
                address += patchLocation->word;
            }
            return (void *) address;
        case RELOC_TYPE_JUMP: // R_MIPS_26: Jump target relocation
            return (void *) ((patchLocation->jump.target << 2) + (u32) gOverlayTable[otIndex].VramBase);
        default:
            return NULL;
    }
}

/**
 * Patches a MIPS instruction with a relocated address.
 * @param instr     Pointer to the instruction to patch
 * @param address   The resolved address to patch in
 * @param patchOperation The patch operation type
 */
void PatchInstruction(MipsInstruction *instr, u32 address, u8 patchOperation) {
    u32 instrWord;
    u32 temp;

    switch (patchOperation) {
        case RELOC_PATCH_WORD: // Store full 32-bit address
            instr->word = address;
            break;
        case RELOC_PATCH_JAL: // Patch jump target (preserve opcode)
            temp = (address >> 2) & 0x03FFFFFF;
            instrWord = instr->word;
            temp ^= instrWord;
            temp = (temp << 6) >> 6; // Clear upper 6 bits (opcode)
            temp ^= instrWord;
            instr->word = temp;
            break;
        case RELOC_PATCH_HI16: // Patch upper 16 bits of address
            temp = address >> 16;
            if (address & 0x8000) {
                temp++; // Adjust for sign extension of LO16
            }
            instr->itype.immediate = temp & 0xFFFF;
            break;
        case RELOC_PATCH_LO16: // Patch lower 16 bits of address
            instr->itype.immediate = address & 0xFFFF;
            break;
        default:
            stubbed_printf("WARNING: Unimplemented linkage operation %d\n", patchOperation);
            break;
    }
    osWritebackDCache(instr, sizeof(MipsInstruction));
    osInvalICache(instr, sizeof(MipsInstruction));
}

// Returns how many entries were consumed (important for the HI16/LO16 pair case where it processes 2 entries at once).
s32 ProcessRelocationEntry(RelocationEntry *relocEntry, s32 otIndex) {
    MipsInstruction *patchLocation;
    s32 patchOperation;
    u32 resolvedAddr;
    MipsInstruction *nextPatchLocation;
    UNUSED s32 pad;
    s32 overlayNumber;
    s32 relocType;
    u32 nextLoImmediate;
    u32 currLoImmediate;

    patchOperation = relocEntry->patchOperation;
    relocType = relocEntry->relocType;
    if (relocEntry->relocType == RELOC_TYPE_DATA) {
        patchLocation = (MipsInstruction *) &gRelocContext.bases[RELOC_BASE_DATA][relocEntry->targetOffset];
        relocEntry->relocType = RELOC_TYPE_EXTERNAL; // Change to external so that it can be resolved normally
    } else {
        patchLocation = (MipsInstruction *) &gRelocContext.bases[RELOC_BASE_TEXT][relocEntry->targetOffset];
    }

    resolvedAddr = (u32) ResolveRelocAddress(relocEntry->symbolIndex, otIndex, relocEntry, patchLocation);

    if (patchOperation == RELOC_PATCH_HI16) {
        if (relocEntry[1].patchOperation != RELOC_PATCH_LO16) {
            stubbed_printf("ERROR:MIPS_HI16 without matching MIPS_LO16\n");
        }

        overlayNumber = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
        if (overlayNumber > OVERLAY_SECTION_UNUSED) {
            overlayNumber = OVERLAY_SECTION_MAIN;
        }
        if (relocEntry->relocType == RELOC_TYPE_EXTERNAL && (gOverlayTable[overlayNumber].VramBase == NULL)) {
            resolvedAddr = (u32) &gUnresolvedSymbolAddr;
        }
        nextPatchLocation = (MipsInstruction *) &gRelocContext.bases[RELOC_BASE_TEXT][relocEntry[1].targetOffset];
        currLoImmediate = patchLocation->itype.immediate;
        nextLoImmediate = nextPatchLocation->itype.immediate;
        if (nextLoImmediate & 0x8000) {
            nextLoImmediate |= 0xFFFF0000;
        }
        currLoImmediate = (currLoImmediate << 16) + nextLoImmediate;
        if (currLoImmediate != (u32) &gUnresolvedSymbolAddr) {
            resolvedAddr += currLoImmediate;
        }
        PatchInstruction(patchLocation, resolvedAddr, RELOC_PATCH_HI16);
        PatchInstruction(nextPatchLocation, resolvedAddr, RELOC_PATCH_LO16);
        relocEntry->relocType = relocType;
        return 2;
    } else if (patchOperation == RELOC_PATCH_LO16) {
        overlayNumber = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
        if (overlayNumber > OVERLAY_SECTION_UNUSED) {
            overlayNumber = OVERLAY_SECTION_MAIN;
        }
        if (relocEntry->relocType == RELOC_TYPE_EXTERNAL && (gOverlayTable[overlayNumber].VramBase == NULL)) {
            resolvedAddr = (u32) &gUnresolvedSymbolAddr;
        }

        resolvedAddr += patchLocation->itype.immediate;

        PatchInstruction(patchLocation, resolvedAddr, RELOC_PATCH_LO16);
        relocEntry->relocType = relocType;
        return 1;
    } else {
        PatchInstruction(patchLocation, resolvedAddr, patchOperation);
        relocEntry->relocType = relocType;
        return 1;
    }
}

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

    overlay = &gOverlayTable[overlayIndex];
    overlayLoad = gPendingOverlayLoads;
    relocTable = NULL;

    // Already loaded - return success
    if (overlay->VramBase != NULL) {
        return TRUE;
    }

    // Check if this overlay is already being loaded (re-entrancy check)
    relocCount = ARRAY_COUNT(gPendingOverlayLoads);
    if (relocCount != 0) { // check count *just in case*
        while (relocCount--) {
            if (overlayIndex == overlayLoad->overlayIndex) {
                return FALSE; // Already in progress
            }
            overlayLoad++;
        }
    }

    // Mark this overlay as being loaded (for memory manager colour tagging)
    mmColourTagOverlayIndex = overlayIndex;

    // Allocate memory for overlay (text + data + rodata + relocation table)
    overlay->VramBase = mmAlloc(overlay->TextSize + overlay->DataSize + overlay->BssSize + overlay->RelocationTableSize,
                                COLOUR_TAG_GREY);

    // Reset the colour tag back to the deault untagged.
    mmColourTagOverlayIndex = INDEX_NOT_SET;

    if (overlay->VramBase == NULL) {
        return FALSE; // Allocation failed
    }

    // If there's a secondary relocation table, allocate and load it
    if (overlay->SecondaryRelocationTableSize) {
        relocTable = (RelocationEntry *) mmAlloc(overlay->SecondaryRelocationTableSize, COLOUR_TAG_GREY);
        if (relocTable == NULL) {
            mmFree(overlay->VramBase);
            return FALSE;
        }
        // Load secondary relocation table from ROM
        romCopy(overlay->RomAddress + overlay->TextSize + overlay->DataSize + overlay->RelocationTableSize, relocTable,
                overlay->SecondaryRelocationTableSize);
    }

    // Set up relocation context with section base addresses
    gRelocContext.textBase = (u8 *) overlay->VramBase;
    gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
    gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
    gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);

    // Load overlay code and data from ROM
    if (overlay->BssSize == 0) {
        // No BSS - copy everything including relocation table
        romCopy(overlay->RomAddress, overlay->VramBase,
                overlay->TextSize + overlay->DataSize + overlay->RelocationTableSize);
    } else {
        // Has BSS - copy text and data only
        romCopy(overlay->RomAddress, overlay->VramBase, overlay->TextSize + overlay->DataSize);

        // Zero out BSS section
        {
            u32 *bssPtr = (u32 *) gRelocContext.bssBase;
            relocCount = overlay->BssSize / sizeof(u32 *);
            while (relocCount--) {
                *bssPtr++ = 0;
            }
        }

        // Load relocation table after BSS
        romCopy(overlay->RomAddress + overlay->TextSize + overlay->DataSize, gRelocContext.relocBase,
                overlay->RelocationTableSize);
    }

    // Invalidate instruction cache for the new code
    osInvalICache(overlay->VramBase, overlay->TextSize);

    // Process secondary relocation table (if present)
    if (relocTable != NULL) {
        savedDelay = mmGetDelay();
        relocCount = (u32) overlay->SecondaryRelocationTableSize / sizeof(RelocationEntry);
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
    relocCount = (u32) relocCount / sizeof(RelocationEntry);
    relocEntry = gRelocContext.relocBase;

    while (relocCount-- > 0) {
        if (ProcessRelocationEntry(relocEntry, overlayIndex) == 2) {
            relocCount--;
            relocEntry++;
        }
        relocEntry++;
    }

    // Update other loaded overlays that reference this newly loaded overlay
    overlay = gOverlayTable;
    for (otherIndex = 0; otherIndex < gOverlayCount; otherIndex++) {
        if (overlay->VramBase != NULL && otherIndex != overlayIndex) {
            if (otherIndex == 0) {
                // Main module - use special relocation context
                gRelocContext.textBase = (u8 *) &__CODE_SECTION_START; // Start of .text
                gRelocContext.dataBase = (u8 *) &__DATA_SECTION_START; // Start of .data
                gRelocContext.bssBase = (u8 *) &__BSS_SECTION_START;
                gRelocContext.relocBase = gMainRelocTable;
                relocEntry = gMainRelocTable;
                relocCount = gMainRelocCount;
            } else {
                // Other overlay - set up context for it
                gRelocContext.textBase = (u8 *) overlay->VramBase;
                gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
                gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
                gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);
                relocEntry = gRelocContext.relocBase;
                relocCount = overlay->RelocationTableSize / sizeof(RelocationEntry);
            }

            while (relocCount-- > 0) {
                // Check if this relocation references the newly loaded overlay
                overlayNum = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
                if (overlayNum > OVERLAY_SECTION_UNUSED) {
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
    overlay = &gOverlayTable[overlayIndex];
    if (overlay->InitFunction != -1) {
        ((void (*)(void))((u32) overlay->VramBase + overlay->InitFunction))();
    }

    return TRUE;
}

s32 runlinkEnsureJumpIsValid(void **jumpAddress) {
    OverlayHeader *overlay;
    RelocationEntry *relocEntry;
    s32 relocCount;
    s32 overlayIndex;
    s32 section;
    s32 overlayNumber;

    if (*jumpAddress != TrapDanglingJump) {
        return FALSE;
    }

    if (gOverlayCount) {}
    if (1) {}
    if (1) {}

    overlay = gOverlayTable;
    for (overlayIndex = 0; overlayIndex < gOverlayCount; overlayIndex++) {
        if (overlay->VramBase != NULL) {
            if (overlayIndex == OVERLAY_SECTION_MAIN) {
                // Main module - use special relocation context
                gRelocContext.textBase = (u8 *) &__CODE_SECTION_START; // Start of .text
                gRelocContext.dataBase = (u8 *) &__DATA_SECTION_START; // Start of .data
                gRelocContext.bssBase = (u8 *) &__BSS_SECTION_START;
                gRelocContext.relocBase = gMainRelocTable;
                relocEntry = gMainRelocTable;
                relocCount = gMainRelocCount;
            } else {
                // Other overlay - set up context for it
                gRelocContext.textBase = (u8 *) overlay->VramBase;
                gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
                gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
                gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);
                relocEntry = gRelocContext.relocBase;
                relocCount = overlay->RelocationTableSize / sizeof(RelocationEntry);
            }
            while (relocCount--) {
                switch (relocEntry->relocType) {
                    case RELOC_TYPE_DATA:
                        section = RELOC_BASE_DATA;
                        break;
                    default:
                        section = RELOC_BASE_TEXT;
                        break;
                }

                if ((u8 *) jumpAddress == (gRelocContext.bases[section] + relocEntry->targetOffset)) {
                    overlayNumber = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
                    if (overlayNumber > OVERLAY_SECTION_UNUSED) {
                        overlayNumber = 0;
                    }
                    runlinkDownloadCode(overlayNumber);
                    return TRUE;
                }
                relocEntry++;
            }
        }
        overlay++;
    }

#ifdef VERSION_us
    return FALSE;
#endif
}

/**
 * Checks if a given overlay module is currently loaded in memory.
 * When a module is loaded, it will have a non-zero VramBase.
 */
s32 runlinkIsModuleLoaded(s32 module) {
    return (u32) gOverlayTable[module].VramBase;
}

/**
 * Ensures an overlay is loaded/resumed and calls its ResumeFunction if present.
 * If the overlay is in the pending load list, resumes it first.
 */
void runlinkCallResumeFunction(s32 overlayIndex) {
    OverlayHeader *overlay;
    PendingOverlayLoad *pendingLoad;
    s32 relocCount;

    overlay = &gOverlayTable[overlayIndex];

    // If no resume function defined, nothing to do
    if (overlay->ResumeFunction == -1) {
        return;
    }

    pendingLoad = gPendingOverlayLoads;

    // If overlay not loaded, check if it's in pending list and resume it
    if (overlay->VramBase == NULL) {
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
    if (overlay->VramBase != NULL) {
        ((void (*)(void))((u32) overlay->VramBase + overlay->ResumeFunction))();
    }
}

void runlinkFreeCode(s32 overlayIndex) {
    OverlayHeader *overlay;
    RelocationEntry *relocEntry;
    MipsInstruction *instr;
    s32 overlayNum;
    s32 i;
    void *loadedAddress;
    s32 relocType;
    s32 found;
    s32 relocCount;
    u32 address;

    overlay = &gOverlayTable[overlayIndex];
    if (gStopCallResumeFunction == FALSE) {
        runlinkCallResumeFunction(overlayIndex);
    }

    loadedAddress = overlay->VramBase;
    if (loadedAddress == NULL) {
        // Overlay not loaded - check if it's in pending list
        found = FALSE;
        relocCount = ARRAY_COUNT(gPendingOverlayLoads);
        for (i = 0; relocCount--; i++) {
            if (overlayIndex == gPendingOverlayLoads[i].overlayIndex) {
                found = TRUE;
                break;
            }
        }

        if (found) {
            // Free the pending load's memory (base + TextSize offset)
            mmFree((void *) ((u32) gPendingOverlayLoads[i].VramBase + overlay->TextSize));
            gPendingOverlayLoads[i].overlayIndex = OVERLAY_SECTION_UNUSED; // Mark slot as unused
        }
        return;
    }

    // Overlay is loaded - free it
    mmFree(loadedAddress);
    overlay->VramBase = NULL;

    gSelfDestructTimers[overlayIndex].selfDestructTimer = 0;
    gSelfDestructTimers[overlayIndex].refCount = 0;

    overlay = gOverlayTable;
    for (i = 0; i < gOverlayCount; i++, overlay++) {
        loadedAddress = overlay->VramBase;
        if (loadedAddress != NULL && i != overlayIndex) {
            if (i == OVERLAY_SECTION_MAIN) {
                // Main module - use special relocation context
                gRelocContext.textBase = (u8 *) &__CODE_SECTION_START; // Start of .text
                gRelocContext.dataBase = (u8 *) &__DATA_SECTION_START; // Start of .data
                gRelocContext.bssBase = (u8 *) &__BSS_SECTION_START;
                gRelocContext.relocBase = gMainRelocTable;
                relocEntry = gMainRelocTable;
                relocCount = gMainRelocCount;
            } else {
                // Other overlay - set up context for it
                gRelocContext.textBase = (u8 *) loadedAddress;
                gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
                gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
                gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);
                relocEntry = gRelocContext.relocBase;
                relocCount = overlay->RelocationTableSize / sizeof(RelocationEntry);
            }

            // Iterate through reloc and patch entries referencing this overlay
            while (relocCount--) {
                // Save the original relocation type before potentially modifying it
                relocType = relocEntry->relocType;

                overlayNum = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
                // Treat all overlay numbers used for data/bss as the main overlay
                if (overlayNum > OVERLAY_SECTION_UNUSED) {
                    overlayNum = OVERLAY_SECTION_MAIN;
                }

                // Patch the section being unloaded only
                if (overlayNum == overlayIndex) {
                    if (relocEntry->relocType == RELOC_TYPE_DATA) {
                        instr = (MipsInstruction *) &gRelocContext.bases[RELOC_BASE_DATA][relocEntry->targetOffset];
                        relocEntry->relocType = RELOC_TYPE_EXTERNAL;
                    } else {
                        instr = (MipsInstruction *) &gRelocContext.bases[RELOC_BASE_TEXT][relocEntry->targetOffset];
                    }

                    // Patch the instruction to point to TrapDanglingJump if it's a JAL instuction
                    if (relocEntry->patchOperation == RELOC_PATCH_JAL) {
                        address = (u32) &TrapDanglingJump;
                    } else {
                        address = NULL;
                    }

                    PatchInstruction(instr, address, relocEntry->patchOperation);
                }

                // Reset the relocation entry to its original type if it was modified.
                relocEntry->relocType = relocType;
                relocEntry++;
            }
        }
    }
}

/**
 * Unloads an overlay and patches all references back to TrapDanglingJump.
 * @param overlayIndex Index of the overlay to unload
 */
void runlinkUnloadOverlay(s32 overlayIndex) {
    OverlayHeader *overlay;
    PendingOverlayLoad *pendingLoad;
    RelocationEntry *relocEntry;
    MipsInstruction *instr;
    s32 overlayNum;
    void *loadedAddress;
    s32 relocType;
    s32 found;
    s32 i;
    s32 patchOperation;
    void *address;

    overlay = &gOverlayTable[overlayIndex];
    runlinkCallResumeFunction(overlayIndex);
    loadedAddress = overlay->VramBase;
    address = loadedAddress;

    if (address == NULL) {
        // Overlay not loaded - check if it's in pending list
        found = FALSE;
        pendingLoad = gPendingOverlayLoads;
        i = ARRAY_COUNT(gPendingOverlayLoads);
        while (i--) {
            if (overlayIndex == pendingLoad->overlayIndex) {
                found = TRUE;
                break;
            }
            pendingLoad++;
        }

        if (found) {
            // Free the pending load's memory (base + TextSize offset)
            mmFree((void *) ((u32) pendingLoad->VramBase + overlay->TextSize));
            pendingLoad->overlayIndex = OVERLAY_SECTION_UNUSED; // Mark slot as unused
        }
        return;
    }

    // Overlay is loaded - free it
    mmFree(address);
    overlay->VramBase = NULL;

    gSelfDestructTimers[overlayIndex].selfDestructTimer = 0;
    gSelfDestructTimers[overlayIndex].refCount = 0;

    // Iterate through mainRelocTable and patch entries referencing this overlay
    relocEntry = gMainRelocTable;
    i = gMainRelocCount;
    while (i--) {
        // Save the original relocation type before potentially modifying it
        relocType = relocEntry->relocType;

        overlayNum = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;

        // Treat all overlay numbers used for data/bss as the main overlay
        if (overlayNum > OVERLAY_SECTION_UNUSED) {
            overlayNum = OVERLAY_SECTION_MAIN;
        }

        // Patch the section being unloaded only
        if (overlayNum == overlayIndex) {
            if (relocEntry->relocType == RELOC_TYPE_DATA) {
                instr = (MipsInstruction *) ((u8 *) &__DATA_SECTION_START + (relocEntry->targetOffset));
                relocEntry->relocType = RELOC_TYPE_EXTERNAL;
            } else {
                instr = (MipsInstruction *) ((u8 *) &__CODE_SECTION_START + (relocEntry->targetOffset));
            }

            // Patch the instruction to point to TrapDanglingJump if it's a JAL instuction
            if (relocEntry->patchOperation == RELOC_PATCH_JAL) {
                address = TrapDanglingJump;
            } else {
                address = NULL;
            }
            PatchInstruction(instr, (u32) address, relocEntry->patchOperation);
        }

        // Reset the relocation entry to its original type if it was modified.
        relocEntry->relocType = relocType;
        relocEntry++;
    }
}

void runlinkFlushModules(void) {
    PendingOverlayLoad *pendingLoad;
    s32 overlayIndex;

    pendingLoad = gPendingOverlayLoads;

    for (overlayIndex = gOverlayCount - 1; overlayIndex > 0; overlayIndex--) {
        runlinkUnloadOverlay(overlayIndex);
    }

    for (overlayIndex = ARRAY_COUNT(gPendingOverlayLoads); overlayIndex--;) {
        if (pendingLoad->overlayIndex != OVERLAY_SECTION_UNUSED) {
            mmFree((void *) ((u32) gOverlayTable[pendingLoad->overlayIndex].TextSize + (u32) pendingLoad->VramBase));
            pendingLoad->overlayIndex = OVERLAY_SECTION_UNUSED;
        }

        pendingLoad++;
    }
}

/**
 * Initializes the runtime linker system.
 * - Allocates and copies overlayTable, overlayRomTable, and mainRelocTable from ROM
 * - Sets up the main module (overlay 0) with section base addresses
 * - Initializes the pending overlay load slots
 * - Allocates the overlay timer entry array (gSelfDestructTimers)
 * - Converts relative ROM addresses in overlay headers to absolute
 */
void runlinkInitialise(void) {
    OverlayHeader *overlayEntry;
    s32 i;

    // This is the number of of symbols stored in the ROM
    gNumberOfSymbols = SYMBOLS_TABLE_OFFSETS_SIZE;

    // Allocate and copy overlayTable from ROM
    // Extra 0x20 bytes at start for main module header (overlay 0)
    gOverlayTable = (OverlayHeader *) mmAlloc(OVERLAY_TABLE_SIZE + sizeof(OverlayHeader), COLOUR_TAG_WHITE);
    romCopy((u32) &overlayTable_ROM_START, (gOverlayTable + 1), OVERLAY_TABLE_SIZE);

    // Allocate and copy overlayRomTable from ROM
    gOverlayRomTable = (RomTableEntry *) mmAlloc(OVERLAY_ROM_TABLE_SIZE, COLOUR_TAG_WHITE);
    romCopy((u32) &overlayRomTable_ROM_START, gOverlayRomTable, OVERLAY_ROM_TABLE_SIZE);

    // Allocate and copy mainRelocTable from ROM
    // First word contains the count, actual table starts at +4
    gMainRelocTable = (RelocationEntry *) mmAlloc(MAIN_RELOC_TABLE_SIZE, COLOUR_TAG_WHITE);
    romCopy((u32) &mainRelocTable_ROM_START, gMainRelocTable, MAIN_RELOC_TABLE_SIZE);

    // Extract mainRelocCount from first word, then advance pointer past it
    gMainRelocCount = *((u32 *) gMainRelocTable);
    gMainRelocTable = (RelocationEntry *) ((u8 *) gMainRelocTable + 4);

    // Calculate overlay count from table size (each entry is 0x20 bytes)
    // +1 for the main module (overlay 0)
    gOverlayCount = (OVERLAY_TABLE_SIZE / sizeof(OverlayHeader)) + 1;

    // Initialize pending overlay load slots to unused
    // Loop from end to start
    i = ARRAY_COUNT(gPendingOverlayLoads);
    while (i--) {
        gPendingOverlayLoads[i].overlayIndex = OVERLAY_SECTION_UNUSED;
    }

    // Allocate and zero the overlay timer array
    gSelfDestructTimers = (OverlayTimerEntry *) mmAlloc(gOverlayCount * sizeof(OverlayTimerEntry), COLOUR_TAG_WHITE);
    bzero(gSelfDestructTimers, gOverlayCount * sizeof(OverlayTimerEntry));

    // Initialize main module header (overlay 0) with section addresses
    gOverlayTable->VramBase = &__CODE_SECTION_START;
    gOverlayTable->RomAddress = 0;
    gOverlayTable->TextSize = ((u32) &__CODE_SECTION_END - (u32) &__CODE_SECTION_START);
    gOverlayTable->DataSize = ((u32) &__DATA_SECTION_END - (u32) &__DATA_SECTION_START);
    gOverlayTable->BssSize = ((u32) &__BSS_SECTION_END - (u32) &__BSS_SECTION_START);
    gOverlayTable->RelocationTableSize = gMainRelocCount * sizeof(RelocationEntry);
    gOverlayTable->SecondaryRelocationTableSize = 0;

    // Convert relative ROM addresses to absolute for all overlay entries
    overlayEntry = gOverlayTable + 1; // Start from overlay 1 to skip the main overlay
    i = gOverlayCount - 1;
    while (i--) {
        overlayEntry->RomAddress = ((u32) overlayCode_ROM_START + overlayEntry->RomAddress);
        overlayEntry++;
    }

    // Mark initialization as complete
    gRunlinkNeedsInit = FALSE;
}

void runlinkSuspendCode(s32 overlayIndex) {
    OverlayHeader *overlay;
#ifdef VERSION_kiosk
    s32 pad;
#endif
    PendingOverlayLoad *pendingLoad;
    s32 remaining;
#ifdef VERSION_us
    s32 savedDelay;
#endif

    overlay = &gOverlayTable[overlayIndex];
    if (overlay->VramBase != NULL) {
        pendingLoad = gPendingOverlayLoads;
        remaining = ARRAY_COUNT(gPendingOverlayLoads) - 1;
        do {
            if (pendingLoad->overlayIndex == OVERLAY_SECTION_UNUSED) {
#ifdef VERSION_us
                savedDelay = mmGetDelay();
#endif
                pendingLoad->VramBase = overlay->VramBase;
                pendingLoad->overlayIndex = overlayIndex;
                mmSetDelay(0);
                gStopCallResumeFunction = TRUE;
                runlinkFreeCode(overlayIndex);
                gStopCallResumeFunction = FALSE;
#ifdef VERSION_kiosk
                mmSetDelay(2);
#else
                mmSetDelay(savedDelay);
#endif
                mmAllocAtAddr(overlay->DataSize + overlay->BssSize + overlay->RelocationTableSize,
                              (void *) ((u32) pendingLoad->VramBase + overlay->TextSize), COLOUR_TAG_GREY);
                return;
            }
            pendingLoad++;
        } while (remaining--);
    }
}

// Not sure where to put this RODATA string.
// It could be referring to reallocating something in the runLinkSuspendCode.
#ifdef VERSION_us
const char D_800AD12C_ADD2C[] = "REALLOC: %08x (%d)\n";
#endif

void runlinkResumeCode(s32 overlayIndex) {
    OverlayHeader *overlay;
    UNUSED s32 pad;
    PendingOverlayLoad *pendingLoad;
    s32 savedDelay;
    s32 relocSavedDelay;
    RelocationEntry *relocTable;
    RelocationEntry *relocEntry;
    s32 relocCount;
    s32 otherIndex;
    s32 overlayNumber;
    s32 found;

    overlay = &gOverlayTable[overlayIndex];
    pendingLoad = gPendingOverlayLoads;
    found = FALSE;
    relocTable = NULL;
    relocCount = ARRAY_COUNT(gPendingOverlayLoads);
    while (relocCount--) {
        if (overlayIndex == pendingLoad->overlayIndex) {
            found = TRUE;
            break;
        }
        pendingLoad++;
    }

    if (found) {
#ifdef VERSION_us
        savedDelay = mmGetDelay();
#endif
        mmSetDelay(0);
        mmFree((void *) ((u32) pendingLoad->VramBase + overlay->TextSize));
#ifdef VERSION_kiosk
        mmSetDelay(2);
#else
        mmSetDelay(savedDelay);
#endif
        overlay->VramBase = (void *) mmAllocAtAddr(overlay->TextSize + overlay->DataSize + overlay->BssSize +
                                                       overlay->RelocationTableSize,
                                                   pendingLoad->VramBase, COLOUR_TAG_GREY);
        if (overlay->VramBase == NULL) {
            return;
        }

        if (overlay->SecondaryRelocationTableSize) {
            relocTable = (RelocationEntry *) mmAlloc(overlay->SecondaryRelocationTableSize, COLOUR_TAG_GREY);
            if (relocTable == NULL) {
                mmFree(overlay->VramBase);
                return;
            }
            romCopy(overlay->RomAddress + overlay->TextSize + overlay->DataSize + overlay->RelocationTableSize,
                    relocTable, overlay->SecondaryRelocationTableSize);
        }

        gRelocContext.textBase = (u8 *) overlay->VramBase;
        gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
        gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
        gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);
        romCopy(overlay->RomAddress, overlay->VramBase, overlay->TextSize);

        if (relocTable != NULL) {
#ifdef VERSION_us
            relocSavedDelay = mmGetDelay();
#endif
            relocCount = overlay->SecondaryRelocationTableSize / sizeof(RelocationEntry);
            relocEntry = relocTable;
            while (relocCount-- > 0) {
                if ((relocEntry->targetOffset) < overlay->TextSize &&
                    ProcessRelocationEntry(relocEntry, overlayIndex) == 2) {
                    relocCount--;
                    relocEntry++;
                }
                relocEntry++;
            }
            mmSetDelay(0);
            mmFree(relocTable);
#ifdef VERSION_kiosk
            mmSetDelay(2);
#else
            mmSetDelay(relocSavedDelay);
#endif
        }

        relocCount = overlay->RelocationTableSize / sizeof(RelocationEntry);
        relocEntry = gRelocContext.relocBase;
        while (relocCount-- > 0) {
            if ((relocEntry->targetOffset) < overlay->TextSize &&
                ProcessRelocationEntry(relocEntry, overlayIndex) == 2) {
                relocCount--;
                relocEntry++;
            }
            relocEntry++;
        }

        overlay = gOverlayTable;
        for (otherIndex = 0; otherIndex < gOverlayCount; otherIndex++) {
            if (overlay->VramBase != NULL && otherIndex != overlayIndex) {
                if (otherIndex == 0) {
                    // Main module - use special relocation context
                    gRelocContext.textBase = (u8 *) &__CODE_SECTION_START; // Start of .text
                    gRelocContext.dataBase = (u8 *) &__DATA_SECTION_START; // Start of .data
                    gRelocContext.bssBase = (u8 *) &__BSS_SECTION_START;
                    gRelocContext.relocBase = gMainRelocTable;
                    relocEntry = gMainRelocTable;
                    relocCount = gMainRelocCount;
                } else {
                    // Other overlay - set up context for it
                    gRelocContext.textBase = (u8 *) overlay->VramBase;
                    gRelocContext.dataBase = (u8 *) gRelocContext.textBase + overlay->TextSize;
                    gRelocContext.bssBase = (u8 *) gRelocContext.dataBase + overlay->DataSize;
                    gRelocContext.relocBase = (RelocationEntry *) ((u8 *) gRelocContext.bssBase + overlay->BssSize);
                    relocEntry = gRelocContext.relocBase;
                    relocCount = overlay->RelocationTableSize / sizeof(RelocationEntry);
                }

                while (relocCount-- > 0) {
                    overlayNumber = gOverlayRomTable[relocEntry->symbolIndex].OverlayNumber;
                    if (overlayNumber > OVERLAY_SECTION_UNUSED) {
                        overlayNumber = 0;
                    }
                    if (overlayNumber == overlayIndex && ((relocEntry->relocType) == RELOC_TYPE_EXTERNAL ||
                                                          (relocEntry->relocType) == RELOC_TYPE_DATA)) {
                        if (ProcessRelocationEntry(relocEntry, otherIndex) == 2) {
                            relocCount--;
                            relocEntry++;
                        }
                    }
                    relocEntry++;
                }
            }
            overlay++;
        }

        pendingLoad->overlayIndex = OVERLAY_SECTION_UNUSED;
    }
}

void runlinkResumeAll(void) {
    PendingOverlayLoad *pendingLoad;
    s32 overlayIndex;
    s32 remaining;

    pendingLoad = gPendingOverlayLoads;
    remaining = ARRAY_COUNT(gPendingOverlayLoads);
    while (remaining--) {
        if (pendingLoad->overlayIndex != OVERLAY_SECTION_UNUSED) {
            overlayIndex = pendingLoad->overlayIndex;
            runlinkResumeCode(overlayIndex);
        }
        pendingLoad++;
    }
}

void runlinkSetDestructTimer(s32 index, u16 selfDestructTimer, u16 refCount) {
    gSelfDestructTimers[index].selfDestructTimer = selfDestructTimer;
    gSelfDestructTimers[index].refCount = refCount;
}

void runlinkTick(void) {
    OverlayTimerEntry *timerEntry;
    s32 overlayIndex;

    overlayIndex = gOverlayCount;
    if (AllowSelfDestructing) {
        while (overlayIndex--) {
            timerEntry = &gSelfDestructTimers[overlayIndex];
            if (timerEntry->refCount != 0) {
                timerEntry->refCount--;
            }
            if (timerEntry->selfDestructTimer != 0) {
                if (--timerEntry->selfDestructTimer == 0) {
                    runlinkFreeCode(overlayIndex);
                }
            }
        }
    }
}

/**
 * Called when memory is low - frees overlays that have no references.
 * Iterates through all overlays and unloads any that have:
 *   - A non-zero selfDestructTimer
 *   - A zero refCount
 */
void runlinkLowMemoryPanic(void) {
    s32 overlayIndex;
    OverlayTimerEntry *timerEntry;

    overlayIndex = gOverlayCount;
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

/**
 * Retrieves information about a given address.
 *
 * @param address The address to query.
 * @param moduleId Overlay Number containing the address.
 * @param moduleAddress Address relative to the overlay's VramBase.
 * @param symbolName Name of the symbol corresponding to the address (if available).
 * @return TRUE if the address belongs to a loaded module, FALSE otherwise.
 */
s32 runlinkGetAddressInfo(u32 address, s32 *moduleId, s32 *moduleAddress, char **symbolName) {
    u32 overlayVram;
    RomTableEntry *romEntry;
    OverlayHeader *overlayTable;
    OverlayHeader *overlay;
    s32 count;
    s32 symbolIndex;
    u32 bestAddress;
    u32 symbolAddress;
    u32 symbolOffset;

    romEntry = gOverlayRomTable;
    *moduleId = 0;
    *moduleAddress = 0;
    bestAddress = 0;

    if (symbolName != NULL) {
        *symbolName = "unknown";
    }

    // If the runlink system needs initialization, return a default module address.
    if (gRunlinkNeedsInit) {
        *moduleAddress = address - CODE_SECTION_VRAM_START;
        return TRUE;
    }

    if (symbolName != NULL) {
        count = gNumberOfSymbols;

        // Iterate through every symbol entry looking for the best candidate
        while (count--) {
            overlayTable = gOverlayTable; // Assign the global to a local first for some reason

            overlay = &overlayTable[romEntry->OverlayNumber];
            overlayVram = (u32) overlay->VramBase; // Get the address as an integer so we can add to it.
            if (overlayVram != NULL) {
                symbolOffset = romEntry->FunctionOffset;
                symbolAddress = overlayVram + symbolOffset;
                if (overlay->TextSize >= symbolOffset && address >= symbolAddress && bestAddress < symbolAddress) {
                    bestAddress = symbolAddress;
                    symbolIndex = romEntry - gOverlayRomTable;
                }
            }
            romEntry++;
        }
        if (bestAddress != 0) {
            *symbolName = GetSymbolName(symbolIndex);
        }
    }

    overlayTable = gOverlayTable;
    overlay = overlayTable;
    count = gOverlayCount;
    while (count--) {
        if (address >= (u32) overlay->VramBase && address <= ((u32) overlay->VramBase + overlay->TextSize)) {
            *moduleId = overlay - overlayTable;
            *moduleAddress = address - (u32) overlay->VramBase;
            return TRUE;
        }
        overlay++;
    }
    return FALSE;
}
