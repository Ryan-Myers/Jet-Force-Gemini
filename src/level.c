#include "audio.h"
#include "camlight.h"
#include "common.h"
#include "fx.h"
#include "gameVi.h"
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
    s8 unk1; // world
    u8 unk2; // region
    u8 unk3; // screenmode
    u8 unk4;
} Level_B176C;

typedef struct {
    s32 unk0;
    s32 unk1;
    s32 unk2;
    s32 unk3;
} Unk_800FB170;

typedef struct {
    u8 unk0;
    u8 unk1[0x10];
} Unk_800FB1E0_B1820;

const char D_800ACD20[] = "LOADLEVEL Error: Level out of range\n";
const char D_800ACD48[] = "levelGetRegionFlags: Ran out of levelRegionFlag structures!!\n";
const char D_800ACD88[] = "levelGetObjectID - Out of level flags\n";

extern s32 D_800A089C_A149C;
extern u8 *D_800A31A0_A3DA0;
extern Unk_800FB1E0_B1820 *D_800A31C4_A3DC4;

extern s32 *D_800FB110_B1750; /* loaded ROM offset table, -1 terminated */
extern s32 D_800FB114_B1754;  // gLevelNumber
extern s32 D_800FB124_B1764;
extern Level_B176C *D_800FB12C_B176C[];
extern Unk_800FB170 D_800FB170_B17B0[];
extern Unk_800FB1E0_B1820 D_800FB1E0_B1820[0x20];

// new
extern u8 **D_800FB120_B1760;       /* level name pointer table (relocated) */
extern s32 D_800FB128_B1768;        /* world count = max(world index) + 1 */
extern s32 D_800FB130_B1770[0x10];  /* per-world level counts, 16 words (B130..B170) */

void levelGetCounts(void) {
    s32 i;
    LevelHeader *hdrBuf = (LevelHeader *) mmAlloc(0x114, 0xFFFF00FF);
    s32 count;
    u8 *nameData;

    D_800FB110_B1750 = (s32 *) piRomLoad(0x1E);

    i = 0;
    do {
        D_800FB130_B1770[i] = 0;
        i++;
    } while (i != 0x10);

    D_800FB124_B1764 = 0;
    while (D_800FB110_B1750[D_800FB124_B1764] != -1) {
        D_800FB124_B1764++;
    }
    D_800FB124_B1764--;

    *D_800FB12C_B176C = (Level_B176C *)mmAlloc(D_800FB124_B1764 * 5, 0xFFFF00FF);
    D_800FB128_B1768 = -1;

    D_800FB118_B5958 = hdrBuf;
    for (i = 0; i < D_800FB124_B1764; i++) {
        piRomLoadSection(0x1F, (u32) D_800FB118_B5958, D_800FB110_B1750[i], 0x114);

        if (D_800FB128_B1768 < D_800FB118_B5958->unk20) {
            D_800FB128_B1768 = D_800FB118_B5958->unk20;
        }

        if ((D_800FB118_B5958->levelType >= 0) && (D_800FB118_B5958->levelType < 0x10)) {
            D_800FB130_B1770[D_800FB118_B5958->levelType]++;
        }

        D_800FB12C_B176C[0][i].unk0 = D_800FB118_B5958->levelType;
        D_800FB12C_B176C[0][i].unk1 = (s8) D_800FB118_B5958->unk20;
        D_800FB12C_B176C[0][i].unk2 = (u8) D_800FB118_B5958->objectFlag;
        D_800FB12C_B176C[0][i].unk3 = (D_800FB118_B5958->unk23 << 5)         | (D_800FB12C_B176C[0][i].unk3 & 0xFF1F);
        D_800FB12C_B176C[0][i].unk3 = ((D_800FB118_B5958->unkC8 * 8) & 0x18) | (D_800FB12C_B176C[0][i].unk3 & 0xFFE7);
        D_800FB12C_B176C[0][i].unk3 = (D_800FB118_B5958->unkC9 & 7)          | (D_800FB12C_B176C[0][i].unk3 & 0xFFF8);
        D_800FB12C_B176C[0][i].unk4 = (u8) D_800FB118_B5958->seqNum;
    }

    D_800FB128_B1768++;
    mmFree(D_800FB110_B1750);
    mmFree(hdrBuf);

    /* --- level names --- */
    D_800A31A0_A3DA0 = (u8 *) mmAlloc(0x20, 0xFFFF00FF);
    D_800FB110_B1750 = (s32 *) piRomLoad(0x22);

    i = 0;
    while (D_800FB110_B1750[i] != -1) {
        i++;
    }
    i--;

    hdrBuf = (LevelHeader *) (D_800FB110_B1750[i] - D_800FB110_B1750[0]);
    nameData = (u8*)mmAlloc(mmAlign4((u8*)hdrBuf) + (i * 4), 0xFFFF00FF);
    D_800FB120_B1760 = mmAlign4((u8*)hdrBuf) + (u32)nameData;
    piRomLoadSection(0x23, (u32) nameData, 0, (s32) hdrBuf);

    for (count = 0; count < i; count++) {
        D_800FB120_B1760[count] = nameData + D_800FB110_B1750[count];
    }
    mmFree(D_800FB110_B1750);
}

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
        return ((u32) D_800FB12C_B176C[0][arg0].unk3 << 0x1B) >> 0x1E;
    }
    return 0;
}

u32 levelGetGfxIndex(s32 arg0) {
    u32 temp_t0;
    u32 var_v0;

    var_v0 = mainGetNumberOfCameras() - 1;
    if ((arg0 >= 0) && (arg0 < D_800FB124_B1764)) {
        temp_t0 = (u32) D_800FB12C_B176C[0][arg0].unk3 >> 5;
        if (temp_t0 != 0) {
            var_v0 = temp_t0;
        }
    }
    return var_v0;
}

s32 levelGetWorldRegions(s32 arg0, u8 *arg1) {
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
                    for (j = 0; j < var_v1; j++) {
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

void animseqSetupGroup(s32);      /* extern */
void camSetFOV(f32, s32);         /* extern */
void camSetNo(s32);               /* extern */
void fxInitNightVision(s32);      /* extern */
void gsSndpLimitVoices(s32 arg0); /* extern */
void hitReset();                  /* extern */
void levelGetRegionFlags(void);   /* extern */
void levelTunePlay(f32 tempo);    /* extern */
s32 mainGetNumberOfPlayers();     /* extern */
void objSetAnimGroup(s32);        /* extern */
void setWeatherLimits(s16 near, s16 far);
void setupLights(s32 count, s32 arg1, s32 arg2);                                         /* extern */
void setupWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6); /* extern */
void squadsInitialiseAfterObjects();                                                     /* extern */
void squadsInitialiseBeforeObjects();                                                    /* extern */
void dayInit_Trap(f32, s32);
void trackInit_Trap(s32, s32, s32, s32, s32, s32);
void refractInit_Trap();
void blurInit_Trap();
void underWaterLightsInit_Trap();
s32 osCartDmaTest4_6105_Trap();
extern SoundHandle D_800A31B0_A3DB0[3];
extern s16 D_800A31BC_A3DBC[3];
extern s32 D_800FB114_B1754;

void levelInit(s32 lvlIdx, s32 arg1, s32 arg2, s32 arg3) {
    s16 tune;
    s32 lvlStart;
    u32 lvlSize;
    s32 lvlCount;
    s32 j;
    s32 shouldPlay;
    s32 pad;
    s32 off;
    s32 freeSlot;

    #ifdef VERSION_kiosk
        rumbleKill();
    #else
        rumbleKill(1);
    #endif
    D_800FB110_B1750 = piRomLoad(0x1E);
    if (arg3 < 0) {
        arg3 = 0;
    }

    switch (mainGetNumberOfPlayers()) {
        case 4:
            gsSndpLimitVoices(0x10);
            break;
        case 3:
            gsSndpLimitVoices(0x10);
            break;
        case 2:
            gsSndpLimitVoices(0xC);
            break;
        default:
            gsSndpLimitVoices(8);
            break;
    }

    /* count entries in the -1-terminated ROM offset table */
    lvlCount = 0;
    while (D_800FB110_B1750[lvlCount] != -1) {
        lvlCount++;
    }
    lvlCount--;
    if (lvlIdx >= lvlCount) {
        lvlIdx = 0;
    }

    lvlStart = D_800FB110_B1750[lvlIdx];
    lvlSize = D_800FB110_B1750[lvlIdx + 1] - lvlStart;
    D_800FB118_B5958 = (LevelHeader *) mmAlloc(lvlSize, 0xFFFF00FF);
    piRomLoadSection(0x1F, (u32) D_800FB118_B5958, lvlStart, lvlSize);
    mainPreNMI();
    mmFree(D_800FB110_B1750);
    D_800FB114_B1754 = lvlIdx;
    levelGetRegionFlags();

    for (lvlStart = 0; lvlStart < 7; lvlStart++) {
        if ((&D_800FB118_B5958->weatherType)[lvlStart] != -1) {
            initColourCycle((unkResetColourCycle *) (&D_800FB170_B17B0[lvlStart]),
                            (&D_800FB118_B5958->weatherType)[lvlStart]);
        }
    }
    amTuneVoiceLimit(D_800FB118_B5958->BGColourTopB);
    amTuneResetFade();
    mainPreNMI();
    lvlCount = 8;
    setupLights(D_800FB118_B5958->light_count, lvlCount, 0x10);
    mainPreNMI();
    squadsInitialiseBeforeObjects();
    mainPreNMI();
    hitReset();
    objSetAnimGroup(arg3);
    mainPreNMI();
    trackInit_Trap(D_800FB118_B5958->instruments, D_800FB118_B5958->unk58, arg1, D_800FB118_B5958->unk56,
                   (s32) D_800FB118_B5958->unkCA, (s32) D_800FB118_B5958->unkE8);
    mainPreNMI();
    animseqSetupGroup(arg3);
    mainPreNMI();
    squadsInitialiseAfterObjects();

    if ((D_800FB118_B5958->fogNear2 == 0) && (D_800FB118_B5958->fogFar2 == 0) && (D_800FB118_B5958->fogR2 == 0) &&
        (D_800FB118_B5958->fogG2 == 0) && (D_800FB118_B5958->fogB2 == 0)) {
        for (lvlStart = 0; lvlStart < 4; lvlStart++) {
            trackSetFogOff(lvlStart);
        }
    } else {
        for (lvlStart = 0; lvlStart < 4; lvlStart++) {
            trackSetFog(lvlStart, D_800FB118_B5958->fogNear2, D_800FB118_B5958->fogFar2, D_800FB118_B5958->unk5E,
                        D_800FB118_B5958->fogR2, D_800FB118_B5958->fogG2, D_800FB118_B5958->fogB2,
                        D_800FB118_B5958->unk63);
        }
    }

    if (D_800FB118_B5958->unkA0 > 0) {
        setupWeather(D_800FB118_B5958->unkA3, D_800FB118_B5958->unkA0, D_800FB118_B5958->unkA6 << 8,
                     D_800FB118_B5958->unkA8 << 8, D_800FB118_B5958->unkAA << lvlCount,
                     D_800FB118_B5958->unkA4_b * 0x101, D_800FB118_B5958->unkA5 * 0x101);
        setWeatherLimits(-1, -0x200);
    }
    if (D_800FB118_B5958->unk69 == -1) {
        D_800FB118_B5958->unkB4_ptr = texLoadTexture(D_800FB118_B5958->unkB4);
        D_800FB118_B5958->unkB8 = 0;
        D_800FB118_B5958->unkBA = 0;
    }
    if (D_800FB118_B5958->unkBC != -1) {
        D_800FB118_B5958->unkBC_ptr = objGetTable(D_800FB118_B5958->unkBC);
        resetMixCycle((PulsatingLightData *) D_800FB118_B5958->unkBC);
    }
    rcpSetScreenColour(D_800FB118_B5958->screen_color_r, D_800FB118_B5958->screen_color_g,
                       D_800FB118_B5958->screen_color_b);
    viFrameRateReset();
    levelTunePlay(1.0f);

    for (lvlStart = 0; lvlStart < 4; lvlStart++) {
        camSetNo(lvlStart);
        camSetFOV((f32) D_800FB118_B5958->camera_fov, 1);
    }
    camSetNo(0);

    if (D_800FB118_B5958->unkE4 == -1) {
        D_800FB118_B5958->unkE4 = 0;
    } else {
        D_800FB118_B5958->unkE4_ptr = objGetTable(D_800FB118_B5958->unkE4);
    }
    mainPreNMI();

    if (D_800FB118_B5958->unk107 != 0) {
        fxInitNightVision(1);
    }
    if (D_800FB118_B5958->BGColourTopG != 0) {
        refractInit_Trap();
    }
    if (D_800FB118_B5958->unkC8 != 0) {
        blurInit_Trap();
    }
    if (D_800FB118_B5958->unkF7 != 0) {
        underWaterLightsInit_Trap(D_800FB118_B5958);
    }
    if (D_800FB118_B5958->unk101 != 0) {
        dayInit_Trap(12.0f, D_800FB118_B5958->unk101 * 0x3C);
    }
    mainPreNMI();
    runlinkFreeCode(0x18);
    runlinkFreeCode(0x1E);

    for (off = 0; off < 3; off++) {
        shouldPlay = 1;
        for (j = 0; j < 3; j++) {
            if (D_800FB118_B5958->tunes[j] == D_800A31BC_A3DBC[off]) {
                shouldPlay = 0;
            }
        }
        if (shouldPlay != 0) {
            if (D_800A31B0_A3DB0[off] != 0) {
                D_800A31BC_A3DBC[off] = -1;
                amSndStop(D_800A31B0_A3DB0[off]);
            }
        }
    }

    if (osCartDmaTest4_6105_Trap() == 0) {
        D_800FB118_B5958->fogNear2 = 0x384;
        D_800FB118_B5958->fogFar2 = 0x398;
    }

    /* start tunes the new level wants that aren't already playing */
    for (off = 0; off < 3; off++) {
        tune = 1;
        if (D_800FB118_B5958->tunes[off] != -1) {
            for (j = 0; j < 3; j++) {
                if (D_800FB118_B5958->tunes[off] == D_800A31BC_A3DBC[j]) {
                    tune = 0;
                } else if (D_800A31BC_A3DBC[j] == -1) {
                    freeSlot = j;
                }
            }
            if (tune != 0) {
                amSndPlay(D_800FB118_B5958->tunes[off], &D_800A31B0_A3DB0[freeSlot]);
                D_800A31BC_A3DBC[freeSlot] = D_800FB118_B5958->tunes[off];
            }
        }
    }
}

void levelTunePlay(f32 tempo) {
    if (D_800FB118_B5958->seqNum != 0) {
        if (D_800FB118_B5958->seqNum != amTuneGetSeqNo()) {
            amTuneResetChls();
            amTunePlay(D_800FB118_B5958->seqNum);
            amTuneResetFade();
            amTuneScaleTempo(tempo);
            amTuneSetChlMask(D_800FB118_B5958->chlMask);
        }
    } else {
        amTuneStop();
    }
}

void levelUpdateColourCycling(s32 arg0) {
    s32 i;

    for (i = 0; i < 7; i++) {
        // TODO: array on `weatherType`?
        if ((&D_800FB118_B5958->weatherType)[i] != -1) {
            updateColourCycle(&D_800FB170_B17B0[i], arg0);
        }
    }
}

Unk_800FB170 *levelGetColourCycling(void) {
    return D_800FB170_B17B0;
}

s32 levelGetNumber(void) {
    return D_800FB114_B1754;
}

u8 levelGetType(void) {
    return D_800FB118_B5958->levelType;
}

u8 levelGetCamera(void) {
    if (D_800FB118_B5958 != NULL) {
        return D_800FB118_B5958->cameraLevel;
    }
    return 0;
}

LevelHeader *levelGetLevel(void) {
    return D_800FB118_B5958;
}

u8 *levelGetName(s32 arg0) {
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
    mmFree(D_800FB118_B5958);
    amAmbientStop();
    freeLights();
    camlightFlush();
    trackFreeAll();
    hitFree();
    amResetAudioMap();
    D_800A089C_A149C = 0;
    amSetMuteMode(0);
    if (D_800FB118_B5958->unkA0 > 0) {
        freeWeather();
    }
    if (D_800FB118_B5958->unk69 == -1) {
        texFreeTexture(D_800FB118_B5958->unkB4_ptr);
    }
    runlinkFlushModules();
    fxCpuTextureFlush();
}

s32 levelGetNextOfWorld(s32 arg0, s8 arg1) {
    s32 var_v1;

    var_v1 = arg0 + 1;
    if (var_v1 >= D_800FB124_B1764) {
        var_v1 = 0;
    }
    while ((var_v1 != arg0) && (arg1 != (*D_800FB12C_B176C)[var_v1].unk1)) {
        var_v1 += 1;
        if (var_v1 >= D_800FB124_B1764) {
            var_v1 = 0;
        }
    }
    return var_v1;
}

s32 levelGetPrevOfWorld(s32 arg0, s8 arg1) {
    s32 var_v1;

    var_v1 = arg0 - 1;
    if (var_v1 < 0) {
        var_v1 = D_800FB124_B1764 - 1;
    }
    while ((var_v1 != arg0) && (arg1 != D_800FB12C_B176C[0][var_v1].unk1)) {
        var_v1 -= 1;
        if (var_v1 < 0) {
            var_v1 = D_800FB124_B1764 - 1;
        }
    }
    return var_v1;
}

void levelInitRegionFlags(void) {
    s32 j;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(D_800FB1E0_B1820); i++) {
        for (j = 0; j < ARRAY_COUNT(D_800FB1E0_B1820[i].unk1); j++) {
            D_800FB1E0_B1820[i].unk1[j] = 0;
        }
        D_800FB1E0_B1820[i].unk0 = 0;
    }
}

void levelGetRegionFlags(void) {
    Unk_800FB1E0_B1820 *temp_t9;
    u8 temp_v0;

    temp_v0 = D_800FB118_B5958->regionFlag;
    if (temp_v0 >= 0x20) {
        D_800A31C4_A3DC4 = NULL;
    } else {
        D_800A31C4_A3DC4 = &D_800FB1E0_B1820[temp_v0];
        D_800A31C4_A3DC4->unk0 = 0;
    }
}

s32 levelGetObjectID(s32 arg0) {
    s32 newId;
    u8 prevId;

    if (D_800A31C4_A3DC4 == NULL) {
        return -1;
    }
    prevId = D_800A31C4_A3DC4->unk0;
    newId = prevId + arg0;
    if (newId > 0x80) {
        return -1;
    }
    D_800A31C4_A3DC4->unk0 = (u8) newId;
    return prevId;
}

void levelSetObjectFlag(s32 indexFlag) {
    s32 index;
    s32 flag;

    index = indexFlag >> 3;
    flag = indexFlag & 0x7;
    if (D_800A31C4_A3DC4 != NULL) {
        D_800A31C4_A3DC4->unk1[index] |= 1 << flag;
    }
}

s32 levelObjectFlagSet(s32 arg0) {
    s32 index;
    s32 flag;

    if ((D_800FB118_B5958->objectFlag == 0) || (D_800FB118_B5958->objectFlag == 0xFF) || (D_800A31C4_A3DC4 == NULL)) {
        return 0;
    }
    index = arg0 >> 3;
    flag = arg0 & 7;
    return (D_800A31C4_A3DC4->unk1[index] & (1 << flag)) != 0;
}
