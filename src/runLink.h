#ifndef _RUNLINK_H_
#define _RUNLINK_H_

#include <PR/ultratypes.h>

extern s32 D_800A38F0_A44F0;

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

extern s32 overlayCount;
extern s32 AllowSelfDestructing;

// typedef struct runlinkModule {
//     s32 unk0;
//     u8 pad4[25];
// } runlinkModule;
// extern runlinkModule *overlayTable;

extern void *__BSS_SECTION_START;
extern void *__BSS_SECTION_END;
extern void *__DATA_SECTION_START;
extern void *__CODE_SECTION_START;

// This function is unique in that it has no specific limit on arguments, 
// and they can change even within the same function call it.
// This empty signature seems to be the way to handle it.
s32 TrapDanglingJump(); 

void runlinkFreeCode(s32 overlayIndex);
void runlinkInitialise(void);
s32 runlinkDownloadCode(s32);
void runlinkLowMemoryPanic(void);
s32 runlinkIsModuleLoaded(s32 module);
s32 runlinkGetAddressInfo(u32 address, s32 *moduleId, s32 *moduleAddress, char **symbolName);
void runlinkFlushModules(void);
char *GetSymbolName(u32 symbolIndex);

#endif
