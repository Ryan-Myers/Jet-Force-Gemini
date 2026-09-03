#ifndef _FX_H_
#define _FX_H_

#include <PR/gbi.h>
#include <PR/ultratypes.h>

typedef struct UnkScreenStruct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
} UnkScreenStruct;

extern s32 D_800A6784_A7384;

s32 fxFadeOn(void);
void fxFreeNightVision(s32); // guess
void fxClearLevelEffects(void);
void fxCpuTextureFlush(void);
void setupClearScreen(UnkScreenStruct *);
void fxInitNightVision(s32);

void fxDrawNightVision(Gfx **);
void fxSetClipWindow(s32);
void fxUpdateLevelEffects(s32);
void fxSPDPRipple(Gfx **, s32, s32, s32, s32, s32);

#endif
