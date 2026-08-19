#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <PR/ultratypes.h>
#include "structs.h"

typedef struct {
    u8 unk0;
    s8 unk1;           // world
    u8 unk2;           // region
    u8 gfxIndex : 3;   // unk3 bits 7:5
    u8 blur : 2;       // unk3 bits 4:3
    u8 screenMode : 3; // unk3 bits 2:0
#ifdef VERSION_us
    u8 unk4;
#endif
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

void levelGetCounts(void);
s32 levelNGetType(s32 arg0);
s32 levelGetTune(s32 arg0);
s32 levelGetWorld(s32 arg0);
s32 levelGetRegionNo(s32 arg0);
s32 levelGetScreenMode(s32 arg0);
s32 levelGetBlurEffect(s32 arg0);
u32 levelGetGfxIndex(s32 arg0);
s32 levelGetWorldRegions(s32 arg0, u8 *arg1);
void levelInit(s32 lvlIdx, s32 arg1, s32 arg2, s32 arg3);
void levelTunePlay(f32 tempo);
void levelUpdateColourCycling(s32 arg0);
Unk_800FB170 *levelGetColourCycling(void);
s32 levelGetNumber(void);
u8 levelGetType(void);
u8 levelGetCamera(void);
LevelHeader *levelGetLevel(void);
u8 *levelGetName(s32 arg0);
void levelFreeAll(void);
s32 levelGetNextOfWorld(s32 arg0, s8 arg1);
s32 levelGetPrevOfWorld(s32 arg0, s8 arg1);
void levelInitRegionFlags(void);
void levelGetRegionFlags(void);
s32 levelGetObjectID(s32 arg0);
void levelSetObjectFlag(s32 indexFlag);
s32 levelObjectFlagSet(s32 arg0);

#endif
