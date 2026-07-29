#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include "structs.h"


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

void texDPTextureX(Gfx **dList, TextureHeader *texhead, s32 flags, s32 texOffset);
TextureSprite* texLoadSprite(s32 id, s32 arg1);
void texFreeSprite(TextureSprite*);
void texFreeTexture(TextureHeader *tex);
void updateColourCycle(void*, s32);

#endif
