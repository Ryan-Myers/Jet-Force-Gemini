#include "common.h"

typedef struct runlinkModule {
    s32 unk0;
    u8 pad4[25];
} runlinkModule;
extern runlinkModule *D_800FF780;

#if 0
extern u32 D_1B94430[], D_1B96910[]; // Linker symbols for the start of two lookup tables.
// D_1B94430 is a 4 byte value used as an offset in the list symbols for the start 
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

#if 0

typedef struct UnkRunLink800534B4 {
    s32 unk0;
    union {
        struct  {
            s32 unk4_0 : 4;
            s32 unk4_6 : 22;
            s32 unk4_24 : 4;
        };
        struct {            
            u8 unk4;
            u8 unk5;
            u8 unk6;
            u8 unk7_0 : 4;
            u8 unk7_4 : 4;
        };
    };
} UnkRunLink800534B4;

typedef struct func_800534B4_arg3 {
    s32 unk0_6 : 26;
    s32 unk0_0 : 6;
} func_800534B4_arg3;

extern s32 *D_800B16B0;
extern s32 *D_800FF788;
extern u32 *D_800FF7B4;
extern void TrapDanglingJump(void);
extern void amSetMuteMode(void); // Start of functions Address
extern s32 tuneSeqPlayer; // Start of Data Address

void *func_800534B4(s32 arg0, s32 arg1, UnkRunLink800534B4 *arg2, func_800534B4_arg3 *arg3) {
    s32 *var_v1;
    s32 *temp_a0;
    s32 temp_t8;
    s32 var_t1;
    u32 temp_t0;
    u32 temp_t4;
    u32 var_v1_2;

    temp_t8 = arg2->unk4_24;
    var_t1 = 0;
    temp_t0 = &D_800FF788[arg0];//*((arg0 * 4) + D_800FF788);
    var_v1_2 = temp_t0 >> 20;
    switch (temp_t8) {                              /* irregular */
    case 0:
        switch (var_v1_2) {                         /* switch 1; irregular */
        case 0xFFD:                                 /* switch 1 */
            var_v1_2 = 0;
            var_t1 = (u32)&tuneSeqPlayer - (u32)&amSetMuteMode;
            break;
        case 0xFFE:                                 /* switch 1 */
            var_v1_2 = 0;
            var_t1 = (u32)&tuneSeqPlayer - (u32)&amSetMuteMode;
            break;
        case 0xFFF:                                 /* switch 1 */
            var_v1_2 = 0;
            var_t1 = (u32)&D_800B16B0 - (u32)&amSetMuteMode;
            break;
        }
        temp_a0 = (s32 *)(((D_800FF780[0].unk0) + (var_v1_2 << 5)));
        if (temp_a0 == 0) {
            temp_t4 = arg2->unk7_0;
            if ((temp_t4 == 4) || (temp_t4 == 2)) {
                return &TrapDanglingJump;
            }
            return &D_800FF7B4;
        }
        return temp_a0 + (temp_t0 & 0xFFFFF) + var_t1;
    case 1:
        var_v1 = (s32 *)((D_800FF780[arg1].unk0) + arg2->unk0);
        if ((arg2->unk7_0) == 2) {
            var_v1 += arg3->unk0_0;
        }
        return var_v1;
    case 2:
        return (s32 *)((D_800FF780[arg1].unk0) + ((arg3->unk0_6)));
    default:
        return NULL;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800534B4.s")
#endif

#if 0
typedef struct Unk80053640_arg0 {
    union {
        s32 unk0_s32;
        struct {
            s16 unk0;
            s16 unk2;
        };
    };
} Unk80053640_arg0;
void func_80053640(Unk80053640_arg0 *arg0, u32 arg1, u8 arg2) {
    s32 test;

    if (arg2 != 2) {
        if (arg2 != 4) {
            switch (arg2) {
            case 5:
                arg1 >>= 16;
                if (arg1 & 0x8000) 
                {
                    arg1++;
                }
            case 6:
                arg0->unk0_s32 = arg1;
                break;
            }
        } else {
            test = ((((((arg1 >> 2) & 0x03FFFFFF) ^ arg0->unk0_s32) << 6) >> 6));
            if (((!arg0) && (!arg0)) && (!arg0)){}
            arg0->unk2 = test ^ arg0->unk0_s32;
        }
    } else {
        arg0->unk2 = arg1;
    }
    osWritebackDCache(arg0, sizeof(Unk80053640_arg0));
    osInvalICache(arg0, sizeof(Unk80053640_arg0));
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80053640.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800536F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkDownloadCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

s32 runlinkIsModuleLoaded(s32 module) {
    return D_800FF780[module].unk0;
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

extern s32 D_800FF78C;
extern LowMemoryStruct *D_800FF838;

void runlinkLowMemoryPanic(void) {
    s32 var_s1;
    u32 temp_v0;
    LowMemoryStruct *temp_s0;

    var_s1 = D_800FF78C;
    while (var_s1--) {
        temp_s0 = &D_800FF838[var_s1];
        temp_v0 = temp_s0->unk0;
        if ((temp_v0 >> 6) != 0) {
            if (!(temp_v0 & 0x3F)) {
                runlinkFreeCode(var_s1);
                temp_s0->unk0 = (u16) (temp_s0->unk0 & 0x3F);
                temp_s0->unk0_1 = (u8) (temp_s0->unk0_1 & 0xFFC0);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkGetAddressInfo.s")
