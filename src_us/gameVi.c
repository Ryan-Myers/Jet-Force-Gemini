#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viChangeMode.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viReset.s")
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viAllocateZBuffer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viFreeZBuffer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viSetTiming.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viGetCurrentSize.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viConvertXY.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viGetScaleXY.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viFrameRateReset.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viFrameSync.s")

s32 viGetVideoMode(void) {
    return D_800FF988 & 3;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viGetWideAdjust.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viSetWideAdjust.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viSetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viGetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viChangeBuffers.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viNoClear.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/viDisplayingScreen0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/func_80055D44.s")

//get_osViMode
#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/func_80055DA8.s")

//swap_framebuffer_pointers
#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/func_80055E68.s")

//osVimode_copy
#pragma GLOBAL_ASM("asm_us/nonmatchings/gameVi/func_80055F4C.s")
