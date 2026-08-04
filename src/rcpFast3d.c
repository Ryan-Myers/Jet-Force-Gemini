#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/rcpFast3d/rcpFast3d.s")

void cloneTasksQueueAndWait_Trap(void);

s32 rcpWaitDP(void) {
    s32 *unkMsg = NULL;
    s32 *refractDoneMsg = NULL;
    s32 *blurDoneMsg = NULL;

    if (D_800A35E4_A41E4 == FALSE) {
        return 0;
    }
    osRecvMesg(&D_800FE4B8_B8CF8, (OSMesg) &unkMsg, OS_MESG_BLOCK);
    if (blurTaskActive) {
        osRecvMesg(&D_800FE918_B9158, (OSMesg) &blurDoneMsg, OS_MESG_BLOCK);
        blurTaskActive = FALSE;
    }
    if (refractTaskActive) {
        osRecvMesg(&refractDoneMsgQueue, (OSMesg) &refractDoneMsg, OS_MESG_BLOCK);
        refractTaskActive = FALSE;
    }
    if (cloneTaskActive) {
        cloneTasksQueueAndWait_Trap();
        cloneTaskActive = FALSE;
    }
    D_800A35E4_A41E4 = FALSE;
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
