#include "common.h"

/**
 * Return the serial interface message queue.
 */
OSMesgQueue *joyMessageQ(void) {
    return &joyMessageQueue;
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyRead.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyResetMap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyDisable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyEnable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyCreateMap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetController.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetButtons.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetPressed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetReleased.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetStickX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetAbsX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetStickY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetAbsY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/func_800431B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joySetSecurity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyCharVal.s")
