#include "common.h"

#ifdef VERSION_us
const char D_800AEF90_AFB90[] = "Path %d, Obj %d\n";
#endif
const char D_800AFB30[] = "Before Level Number %d (%d)\n";
const char D_800AFB50[] = "After  Level Number %d\n";

extern s32 *D_801047C8_B19B8;
extern u8 *D_801047CC_B19BC;
extern u32 D_801047D0_B19C0;

void func_80076020_76C20(s32 *arg0) {
    D_801047C8_B19B8 = arg0;
    D_801047CC_B19BC = *arg0;
    D_801047D0_B19C0 = 0x80;
}

s32 func_80076044_76C44(u32 arg0) {
    s32 ret;

    ret = 0;
    if (arg0 != 0) {
        arg0 = 1 << (arg0 + 0x1F);
        do {
            if (D_801047D0_B19C0 == 0) {
                D_801047CC_B19BC++;
                D_801047D0_B19C0 = 0x80;
            }
            if (*D_801047CC_B19BC & D_801047D0_B19C0) {
                ret |= arg0;
            }
            arg0 >>= 1;
            D_801047D0_B19C0 >>= 1;
        } while (arg0 != 0);
    }
    return ret;
}

s32 func_800760C0_76CC0(u32 arg0) {
    u32 temp_v0;
    s32 ret;

    ret = 0;
    if (arg0 != 0) {
        temp_v0 = 0xFFFFFFFF << (arg0 - 1);
        arg0 = 1 << (arg0 - 1);
        do {
            if (D_801047D0_B19C0 == 0) {
                D_801047CC_B19BC++;
                D_801047D0_B19C0 = 0x80;
            }
            if (*D_801047CC_B19BC & D_801047D0_B19C0) {
                ret |= arg0;
            }
            arg0 >>= 1;
            D_801047D0_B19C0 >>= 1;
        } while (arg0 != 0);
        if (ret & temp_v0) {
            ret |= temp_v0;
        }
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076154_76D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076198_76D98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800761C8_76DC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800761E8_76DE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800762A0_76EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_800762F0_76EF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInitPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqResetPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqStartPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqStopPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqHoldPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLockPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqUnLockPath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSndPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSndStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80076B44_77744.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLinkNodes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80077004_77C04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqFreeLevelData.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqLoadLevelData.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqFreeGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqSetupGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqInitGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqResetGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/func_80077468_78068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqProcessCommandList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqCamera.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqGetPathNumbers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqWhichPathHasObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqGetFirstFreeIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/anim/animseqPause.s")
