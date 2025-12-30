#include "common.h"

const char D_800AAA80[] = "audio: ai out of samples\n";
const char D_800AAA9C[] = "OH DEAR - No audio DMA buffers left\n";
const char D_800AAAC4[] = "Dma not done\n";

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amCreateAudioMgr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/__amMain.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amGo.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amStop.s")

s32 amAudioMgrGetNextFrameCount(void) {
    return nextFrameCount;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/__amHandleFrameMsg.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/__amHandleDoneMsg.s")

//__amDma
#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/func_80002764.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/__amDmaNew.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/__clearAudioDMA.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amAudioMgrSetScheduleMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amAudioMgrHaveRunALargeFrame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/audiomgr/amAudioMgrHaveFreedLargeBuffers.s")
