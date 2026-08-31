#ifndef _RUNLINK_H_
#define _RUNLINK_H_

#include <PR/ultratypes.h>
#include "mips.h"

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
 * Relocation entry format used by the overlay dynamic linker.
 * Each entry is 8 bytes and describes how to patch an address reference.
 */
typedef struct RelocationEntry {
    u32 symbolIndex; // Index into overlayRomTable, OR local offset for type 1
    union {
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

typedef struct RelocTableEntry {
    union {
        u32 bytes;
        struct {
            u32 functionAddress : 24; // This is the address of the calling function less 0x80000450 (0x8004DD50 -
                                      // 0x80000450 = 0x4D900)
            u32 unknown : 8;          // Unknown, almost always seems to be 0x40
        };
    } entry;
    u32 overlayIndex; // This is an index into overlayRomTable
} RelocTableEntry; /* 8 bytes */

typedef struct RomTableEntry {
    union {
        u32 bytes;
        struct {
            u32 OverlayNumber : 12;
            u32 FunctionOffset : 20;
        };
    } entry;
} RomTableEntry; /* 4 bytes */

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
} OverlayHeader; /* 0x20 bytes */

typedef struct RelocContext {
    union {
        u8 *bases[5];      // An array of base addresses for different sections: unused, text, data, bss, reloc
        struct {
            u8 *unused;    // 0x00 - Unknown
            u8 *textBase;  // 0x04 - gRelocTextBase
            u8 *dataBase;  // 0x08 - gRelocDataBase
            u8 *bssBase;   // 0x0C
            u8 *relocBase; // 0x10 - relocation table base
        };
    };
} RelocContext; /* 0x14 bytes */

typedef struct PendingOverlayLoad {
    u32 unk0;         // 0x00 - possibly status/flags
    s32 overlayIndex; // 0x04 - overlay number being loaded
} PendingOverlayLoad; // 8 bytes

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

extern OverlayHeader D_1ED2780[];   // overlayTable
extern RomTableEntry D_1ED0270[]; // overlayRomTable
extern RelocTableEntry D_1ECF220[];     // mainRelocTable ROM address

extern void amSetMuteMode(s32 behaviour); // 0x80000450 Start of .text
extern void *tuneSeqPlayer;               // 0x800A0660 Start of .data
extern s32 D_800B0B50_B1750; // 0x800B0B50 Start of .bss

extern void *__BSS_SECTION_START;
extern void *__BSS_SECTION_END;
extern void *__DATA_SECTION_START;
extern void *__CODE_SECTION_START;

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
extern u8 overlayData_ROM_START[];


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
 * |Overlay number	   | 3 (bits 31-20: 0x003)   |
 * |Function offset	   | 0x15E34 (bits 19-0)     |
 * |_____________________________________________|
 *
 * So according to our symbols, overlay 3 at function offset 0x15E34 is cloneTasksQueueAndWait
 *
 * It triggers the dynamic linker to load overlay 3 (if not already loaded)
 * The actual function called is cloneTasksQueueAndWait at offset 0x15E34 within that overlay
 */


// This function is unique in that it has no specific limit on arguments, 
// and they can change even within the same function call it.
// This empty signature seems to be the way to handle it.
extern s32 TrapDanglingJump();

char *GetSymbolName(u32 symbolIndex);
void *ResolveRelocAddress(s32 ortIndex, s32 otIndex, RelocationEntry *relocEntry, MipsInstruction *patchLocation);
void PatchInstruction(MipsInstruction *instr, u32 address, u8 relocType);
s32 ProcessRelocationEntry(RelocationEntry *relocEntry, s32 otIndex);
s32 runlinkDownloadCode(s32);
s32 runlinkEnsureJumpIsValid(void **jumpAddress);
s32 runlinkIsModuleLoaded(s32 module);
void runlinkCallResumeFunction(s32 overlayIndex);
void runlinkFreeCode(s32 overlayIndex);
void runlinkUnloadOverlay(s32 overlayIndex);
void runlinkFlushModules(void);
void runlinkInitialise(void);
void runlinkSuspendCode(s32 overlayIndex);
void runlinkResumeCode(s32 overlayIndex);
void runlinkResumeAll(void);
void runlinkSetDestructTimer(s32 index, u16 selfDestructTimer, u16 refCount);
void runlinkTick(void);
void runlinkLowMemoryPanic(void);
s32 runlinkGetAddressInfo(u32 address, s32 *moduleId, s32 *moduleAddress, char **symbolName);

#endif
