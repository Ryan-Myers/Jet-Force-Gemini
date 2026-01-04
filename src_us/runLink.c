#include "common.h"

const char D_800ADC90[] = "WARNING: Unimplemented linkage operation %d\n";
const char D_800ADCC0[] = "ERROR:MIPS_HI16 without matching MIPS_LO16\n";
#ifdef VERSION_us
const char D_800AD12C[] = "REALLOC: %08x (%d)\n";
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/GetSymbolName.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/func_800534B4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/func_80053640.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/func_800536F8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkDownloadCode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkEnsureJumpIsValid.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkIsModuleLoaded.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/func_80053FC8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkFreeCode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/func_80054368.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkFlushModules.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkInitialise.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkSuspendCode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkResumeCode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkResumeAll.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkSetDestructTimer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkLowMemoryPanic.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/runLink/runlinkGetAddressInfo.s")
