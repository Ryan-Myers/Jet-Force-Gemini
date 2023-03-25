#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amCreateAudioMgr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/__amMain.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amGo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amAudioMgrGetNextFrameCount.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/__amHandleFrameMsg.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/__amHandleDoneMsg.s")

//__amDma
#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/func_80002764_3364.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/__amDmaNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/__clearAudioDMA.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amAudioMgrSetScheduleMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amAudioMgrHaveRunALargeFrame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audiomgr/amAudioMgrHaveFreedLargeBuffers.s")
