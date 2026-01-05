#include "common.h"

const char D_800ACD20[] = "LOADLEVEL Error: Level out of range\n";
const char D_800ACD48[] = "levelGetRegionFlags: Ran out of levelRegionFlag structures!!\n";
const char D_800ACD88[] = "levelGetObjectID - Out of level flags\n";

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetCounts.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelNGetType.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetTune.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetWorld.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetRegionNo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetScreenMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetBlurEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetGfxIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetWorldRegions.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelTunePlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelUpdateColourCycling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetColourCycling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetNumber.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetType.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetCamera.s")

LevelHeader *levelGetLevel(void) {
    return D_800FBBD8;
}

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetName.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetNextOfWorld.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetPrevOfWorld.s")

// The reference to gfxBase is false, as it's checking the end of the D_800FBCA0 array
#pragma GLOBAL_ASM("asm/nonmatchings/level/levelInitRegionFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetRegionFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetObjectID.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelSetObjectFlag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelObjectFlagSet.s")
