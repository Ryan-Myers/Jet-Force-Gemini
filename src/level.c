#include "common.h"

extern LevelHeader *D_800FB118_B5958;
typedef struct {
    u8 unk0;
    s8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} Level_B176C;

const char D_800ACD20[] = "LOADLEVEL Error: Level out of range\n";
const char D_800ACD48[] = "levelGetRegionFlags: Ran out of levelRegionFlag structures!!\n";
const char D_800ACD88[] = "levelGetObjectID - Out of level flags\n";

extern s32 D_800FB124_B1764;
extern Level_B176C* D_800FB12C_B176C[];

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetCounts.s")

s32 levelNGetType(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        return D_800FB12C_B176C[0][arg0].unk0;
    }
    return -1;
}

#ifdef VERSION_us
s32 levelGetTune(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        return D_800FB12C_B176C[0][arg0].unk4;
    }
    return -1;
}
#endif

s32 levelGetWorld(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        return D_800FB12C_B176C[0][arg0].unk1;
    }
    return 0;
}

s32 levelGetRegionNo(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        return D_800FB12C_B176C[0][arg0].unk2;
    }
    return 0;
}

s32 levelGetScreenMode(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        return D_800FB12C_B176C[0][arg0].unk3 & 7;
    }
    return 0;
}

s32 levelGetBlurEffect(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        // TODO: weird casting and shifting to get instructions. might be bitfields?
        return ((u32)D_800FB12C_B176C[0][arg0].unk3 << 0x1B) >> 0x1E;
    }
    return 0;
}

u32 levelGetGfxIndex(s32 arg0) {
    u32 temp_t0;
    u32 var_v0;

    var_v0 = mainGetNumberOfCameras() - 1;
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        temp_t0 = (u32)D_800FB12C_B176C[0][arg0].unk3 >> 5;
        if (temp_t0 != 0) {
            var_v0 = temp_t0;
        }
    }
    return var_v0;
}

s32 levelGetWorldRegions(s32 arg0, u8* arg1) {
    s32 found;
    s32 j;
    s32 i;
    s32 var_v1;

    var_v1 = 0;
    for (i = 0; i < D_800FB124_B1764; i++) {
        if (arg0 == D_800FB12C_B176C[0][i].unk1) {
            if (D_800FB12C_B176C[0][i].unk2 != 0) {
                found = FALSE;
                if (D_800FB12C_B176C[0][i].unk2 != 0xFF) {
                    for(j = 0; j < var_v1; j++) {
                        if (arg1[j] == D_800FB12C_B176C[0][i].unk2) {
                            found = TRUE;
                            j = var_v1;
                        }
                    }
                    if (!found) {
                        arg1[var_v1] = D_800FB12C_B176C[0][i].unk2;
                        var_v1++;
                    }
                }
            }
        }
    }
    return var_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelTunePlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelUpdateColourCycling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetColourCycling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetNumber.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetType.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetCamera.s")

LevelHeader *levelGetLevel(void) {
    return D_800FB118_B5958;
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
