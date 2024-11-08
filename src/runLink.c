#include "common.h"

#if 0
//extern u8 __SYMBOLS_LUT_START[], __SYMBOLS_LUT_EMD[];
extern u32 D_1B94430[], D_1B96910[];

char *GetSymbolName(s32 arg0) {
    u32 *ramAddress;
    u8 *secondRomOffset;
    u32 *romOffset;
    romOffset = D_1B94430[arg0] & ~7;
    romCopy((u32) (romOffset), (u32) ramAddress, 8);
    secondRomOffset = *(&ramAddress + ((s32) romOffset & 7)) + *D_1B96910;
    romCopy((u32) (*secondRomOffset & ~7), (u32) ramAddress, 96);
    return &ramAddress + ((u32)secondRomOffset & 7);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/runLink/GetSymbolName.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800534B4_540B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80053640_54240.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800536F8_542F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkDownloadCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

typedef struct runlinkModule {
    s32 unk0;
    u8 pad4[25];
} runlinkModule;
extern runlinkModule *D_800FF780_B2380;
s32 runlinkIsModuleLoaded(s32 module) {
    return D_800FF780_B2380[module].unk0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80053FC8_54BC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkFreeCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80054368_54F68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkFlushModules.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkInitialise.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkSuspendCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkResumeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkSetDestructTimer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkLowMemoryPanic.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkGetAddressInfo.s")
