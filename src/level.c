#include "common.h"
#include "audio.h"
#include "camlight.h"
#include "fx.h"
#include "hit.h"
#include "memory.h"
#include "rcpFast3d.h"
#include "runLink.h"
#include "textures.h"
#include "track.h"
#include "weather.h"

extern LevelHeader *D_800FB118_B5958;
typedef struct {
    u8 unk0;
    s8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} Level_B176C;

typedef struct {
    s32 unk0;
    u8 pad4[0x10 - 0x4];
} Unk_800FB170;


const char D_800ACD20[] = "LOADLEVEL Error: Level out of range\n";
const char D_800ACD48[] = "levelGetRegionFlags: Ran out of levelRegionFlag structures!!\n";
const char D_800ACD88[] = "levelGetObjectID - Out of level flags\n";

extern s32 D_800A089C_A149C;
extern u8* D_800A31A0_A3DA0;

extern u32* D_800FB110_B1750;
extern s32 D_800FB114_B1754; // gLevelNumber
extern s32 D_800FB124_B1764;
extern Level_B176C* D_800FB12C_B176C[];
extern Unk_800FB170 D_800FB170_B17B0[];

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

void levelTunePlay(f32 tempo) {
    if (D_800FBBD8->seqNum != 0) {
        if (D_800FBBD8->seqNum != amTuneGetSeqNo()) {
            amTuneResetChls();
            amTunePlay(D_800FBBD8->seqNum);
            amTuneResetFade();
            amTuneScaleTempo(tempo);
            amTuneSetChlMask(D_800FBBD8->chlMask);
        }
    } else {
        amTuneStop();
    }
}

void levelUpdateColourCycling(s32 arg0) {
    s32 i;

    for (i = 0; i < 7; i++) {
        // TODO: array on `weatherType`?
        if ((&D_800FBBD8->weatherType)[i] != -1) {
            updateColourCycle(&D_800FB170_B17B0[i], arg0);
        }
    }
}

Unk_800FB170* levelGetColourCycling(void) {
    return D_800FB170_B17B0;
}

s32 levelGetNumber(void) {
    return D_800FB114_B1754;
}

u8 levelGetType(void) {
    return D_800FBBD8->unk6C;
}

u8 levelGetCamera(void) {
    if (D_800FBBD8 != NULL) {
        // TODO: make struct bigger instead of [1]?
        return D_800FBBD8[1].unk16[9];
    }
    return 0;
}

LevelHeader *levelGetLevel(void) {
    return D_800FB118_B5958;
}

u8* levelGetName(s32 arg0) {
    *D_800A31A0_A3DA0 = 0;
    if (arg0 < D_800FB124_B1764) {
        D_800FB110_B1750 = piRomLoad(0x1E);
        if (D_800FB110_B1750 != NULL) {
            piRomLoadSection(0x1F, (u32) D_800A31A0_A3DA0, (s32) D_800FB110_B1750[arg0], 0x20);
            mmFree(D_800FB110_B1750);
        }
    }
    return D_800A31A0_A3DA0;
}

void levelFreeAll(void) {
    fxFreeNightVision(1);
    fxClearLevelEffects();
    rcpSetScreenColour(0, 0, 0);
    mmFree(D_800FBBD8);
    amAmbientStop();
    freeLights();
    camlightFlush();
    trackFreeAll();
    hitFree();
    amResetAudioMap();
    D_800A089C_A149C = 0;
    amSetMuteMode(0);
    if (D_800FBBD8->unkA0 > 0) {
        freeWeather();
    }
    if (D_800FBBD8->unk69 == -1) {
        texFreeTexture(D_800FBBD8->unkB4_ptr);
    }
    runlinkFlushModules();
    fxCpuTextureFlush();
}

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetNextOfWorld.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetPrevOfWorld.s")

// The reference to gfxBase is false, as it's checking the end of the D_800FBCA0 array
#pragma GLOBAL_ASM("asm/nonmatchings/level/levelInitRegionFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetRegionFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelGetObjectID.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelSetObjectFlag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/level/levelObjectFlagSet.s")
