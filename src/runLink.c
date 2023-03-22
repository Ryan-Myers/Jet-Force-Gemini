#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/GetSymbolName.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800534B4_540B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_80053640_54240.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/func_800536F8_542F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkDownloadCode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

typedef struct runlinkModule {
	s32 unk0;
	u8 pad4[25];
} runlinkModule;
extern runlinkModule *D_800FF780_100380;
s32 runlinkIsModuleLoaded(s32 module) {
    return D_800FF780_100380[module].unk0;
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
