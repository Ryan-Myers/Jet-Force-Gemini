#include "common.h"

const char D_800AFB30[] = "Before Level Number %d (%d)\n";
const char D_800AFB50[] = "After  Level Number %d\n";

extern s32 *D_80105498;
extern u8 * D_8010549C;
extern u32 D_801054A0;

void func_800767A0(s32 *arg0) {
    D_80105498 = arg0;
    D_8010549C = *arg0;
    D_801054A0 = 0x80;
}

s32 func_800767C4(u32 arg0) {
    s32 ret;

    ret = 0;
    if (arg0 != 0) {
        arg0 = 1 << (arg0 + 0x1F);
        do {
            if (D_801054A0 == 0) {
                D_8010549C++;
                D_801054A0 = 0x80;
            }
            if (*D_8010549C & D_801054A0) {
                ret |= arg0;
            }
            arg0 >>= 1;
            D_801054A0 >>= 1;
        } while (arg0 != 0);
    }
    return ret;
}

s32 func_80076840(u32 arg0) {
    u32 temp_v0;
    s32 ret;

    ret = 0;
    if (arg0 != 0) {
        temp_v0 = 0xFFFFFFFF << (arg0 - 1);
        arg0 = 1 << (arg0 - 1);
        do {
            if (D_801054A0 == 0) {
                D_8010549C++;
                D_801054A0 = 0x80;
            }
            if (*D_8010549C & D_801054A0) {
                ret |= arg0;
            }
            arg0 >>= 1;
            D_801054A0 >>= 1;
        } while (arg0 != 0);
        if (ret & temp_v0) {
            ret |= temp_v0;
        }
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800768D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076918.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076968.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076A20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076A70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInitPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqResetPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqStartPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqStopPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqHoldPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLockPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqUnLockPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSndPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSndStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800772C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLinkNodes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80077784.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqFreeLevelData.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLoadLevelData.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqFreeGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSetupGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInitGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqResetGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80077BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqProcessCommandList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqCamera.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqGetPathNumbers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqWhichPathHasObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqGetFirstFreeIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqPause.s")
