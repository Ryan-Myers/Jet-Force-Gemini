#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpFast3d.s")

s32 rcpWaitDP(void) {
    s32 *unkMsg = NULL;
    s32 *refractDoneMsg = NULL;
    s32 *blurDoneMsg = NULL;

    if (D_800A4034 == FALSE) {
        return 0;
    }
    osRecvMesg(&D_800FF1C8, (OSMesg) &unkMsg, OS_MESG_BLOCK);
    if (blurTaskActive) {
        osRecvMesg(&D_800FF628, (OSMesg) &blurDoneMsg, OS_MESG_BLOCK);
        blurTaskActive = FALSE;
    }
    if (refractTaskActive) {
        osRecvMesg(&refractDoneMsgQueue, (OSMesg) &refractDoneMsg, OS_MESG_BLOCK);
        refractTaskActive = FALSE;
    }
    if (cloneTaskActive) {
        TrapDanglingJump();
        cloneTaskActive = FALSE;
    }
    D_800A4034 = FALSE;
    return unkMsg[1];
}

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
