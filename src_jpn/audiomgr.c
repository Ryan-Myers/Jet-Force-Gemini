#include "common.h"

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amCreateAudioMgr.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/__amMain.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amGo.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amStop.s")

s32 amAudioMgrGetNextFrameCount(void) {
    return nextFrameCount;
}

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/__amHandleFrameMsg.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/__amHandleDoneMsg.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/func_800024C4_30C4.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/__amDmaNew.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/__clearAudioDMA.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amAudioMgrSetScheduleMode.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amAudioMgrHaveRunALargeFrame.s")

#pragma GLOBAL_ASM("asm_jpn/nonmatchings/audiomgr/amAudioMgrHaveFreedLargeBuffers.s")
