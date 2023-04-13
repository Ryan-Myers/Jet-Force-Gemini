#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpFast3d.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpWaitDP.s")

/**
 * Sets the primitive colour for the cyclemode fillrect background.
 */
void rcpSetScreenColour(u8 red, u8 green, u8 blue) {
    sBackgroundPrimColourR = red;
    sBackgroundPrimColourG = green;
    sBackgroundPrimColourB = blue;
}

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpSetBorderColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpClearZBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpInitDp.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpInitDpNoSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpInitSp.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpMosaicClear.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpCheckClear.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpTileWrite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpTileWriteX.s")
