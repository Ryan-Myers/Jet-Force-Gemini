#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viChangeMode.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viReset.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viAllocateZBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viFreeZBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetTiming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetCurrentSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viConvertXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetScaleXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viFrameRateReset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viFrameSync.s")

s32 viGetVideoMode(void) {
    return D_800FF988 & 3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetWideAdjust.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetWideAdjust.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viChangeBuffers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viNoClear.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viDisplayingScreen0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055D44.s")

//get_osViMode
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055DA8.s")

//swap_framebuffer_pointers
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055E68.s")

//osVimode_copy
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055F4C.s")
