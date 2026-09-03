#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "structs.h"
#include <PR/gbi.h>
#include <PR/ultratypes.h>

typedef struct TextureSprite {
    /* 0x00 */ s16 baseTextureId;
    /* 0x02 */ s16 numberOfFrames; // 1 means static texture
    /* 0x04 */ s16 numberOfInstances;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ TextureHeader **frames;
    union {
        /* 0x0C */ u8 val[1];  // Actual size varies.
        /* 0x0C */ u8 *ptr[1]; // Display list?
    } unkC;
} TextureSprite;

extern Gfx D_800A3F90_A4B90[11][2]; // unknown number of graphics commands. This is a best guess.

extern u8 blinkMode;
extern u32 D_800A4E60_A5A60; // gTexColourTag = COLOUR_TAG_MAGENTA;
extern s32 D_800A4E68_A5A68; // D_80126378
extern u8 D_800A4E6C_A5A6C;
extern s32 D_800FED14_B9554;     // D_80126354 in DKR
extern s32 *D_800FECE0_B9520[2]; // D_80126320 in DKR
extern s32 D_800FED30_B9570;
extern s32 D_800FED34_B9574;
extern s32 D_800FED38_B9578;
extern s32 D_800FED3C_B957C;
extern s32 D_800FED40_B9580;
extern s32 D_800FED44_B9584;
extern s32 D_800A4E7C_A5A7C; // A store of the texture currently loading?
extern s32 D_800A4E74_A5A74; // IA2ColOverride?
extern u8 D_800FED48_B9588;
extern u8 D_800FED49_B9589;
extern u8 D_800FED4A_B958A;
extern u8 D_800FED4B_B958B;
extern u8 D_800FED4C_B958C;
extern u8 D_800FED4D_B958D;
extern s32 D_800A4E64_A5A64; // textureFilter

void texDPTextureX(Gfx **dList, TextureHeader *texhead, s32 flags, s32 texOffset);
TextureSprite *texLoadSprite(s32 id, s32 arg1);
void texFreeSprite(TextureSprite *);
void texFreeTexture(TextureHeader *tex);
void updateColourCycle(void *, s32);
void texDPInit(Gfx **);
TextureHeader *texLoadTexture(s32 arg0);
void resetMixCycle(PulsatingLightData *data);
void updateMixCycle(PulsatingLightData *data, s32 timeDelta);
void resetColourCycle(unkResetColourCycle *arg0);
void initColourCycle(unkResetColourCycle *arg0, s32 arg1);
void texAnimateTexture(TextureHeader *texture, u32 *triangleBatchInfoFlags, s32 *arg2, s32 updateRate);
void setTexMemColour(s32 tagId);
void func_800570D8_57CD8(TextureHeader *tex, Gfx *_dlist); // build_tex_display_list in DKR
void func_8005719C_57D9C(Gfx **dlist, TextureHeader *tex, s32 rtile, s32 tmem);
void sprSetIA2ColOverride(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5);
void sprClearIA2ColOverride(void);
void sprSetTextureFilter(s32 arg0);

#endif
