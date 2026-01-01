#include "common.h"

const char D_800ACD20[] = "LOADLEVEL Error: Level out of range\n";
const char D_800ACD48[] = "levelGetRegionFlags: Ran out of levelRegionFlag structures!!\n";
const char D_800ACD88[] = "levelGetObjectID - Out of level flags\n";

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetCounts.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelNGetType.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetTune.s")
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetWorld.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetRegionNo.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetBlurEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetGfxIndex.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetWorldRegions.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelTunePlay.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelUpdateColourCycling.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetColourCycling.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetNumber.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetType.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetCamera.s")

LevelHeader *levelGetLevel(void) {
    return D_800FBBD8;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetName.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelFreeAll.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetNextOfWorld.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetPrevOfWorld.s")

// The reference to gfxBase is false, as it's checking the end of the D_800FBCA0 array
#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelInitRegionFlags.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetRegionFlags.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelGetObjectID.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelSetObjectFlag.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/level/levelObjectFlagSet.s")
