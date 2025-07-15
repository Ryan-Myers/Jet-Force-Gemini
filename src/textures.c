#include "common.h"

enum TriangleBatchFlags {
    BATCH_FLAGS_NONE = 0,
    BATCH_FLAGS_BIT0 = (1 << 0),
    BATCH_FLAGS_BIT1 = (1 << 1),
    BATCH_FLAGS_BIT2 = (1 << 2),
    BATCH_FLAGS_BIT3 = (1 << 3),
    BATCH_FLAGS_BIT4 = (1 << 4),
    BATCH_FLAGS_BIT5 = (1 << 5),
    BATCH_FLAGS_BIT6 = (1 << 6),
    BATCH_FLAGS_BIT7 = (1 << 7),
    BATCH_FLAGS_BIT8 = (1 << 8),
    BATCH_FLAGS_BIT9 = (1 << 9),
    BATCH_FLAGS_BIT10 = (1 << 10),
    BATCH_FLAGS_BIT11 = (1 << 11),
    BATCH_FLAGS_BIT12 = (1 << 12),
    BATCH_FLAGS_BIT13 = (1 << 13),
    BATCH_FLAGS_BIT14 = (1 << 14),
    BATCH_FLAGS_BIT15 = (1 << 15),
    BATCH_FLAGS_BIT16 = (1 << 16),
    BATCH_FLAGS_BIT17 = (1 << 17),
    BATCH_FLAGS_BIT18 = (1 << 18),
    BATCH_FLAGS_BIT19 = (1 << 19),
    BATCH_FLAGS_BIT20 = (1 << 20),
    BATCH_FLAGS_BIT21 = (1 << 21),
    BATCH_FLAGS_BIT22 = (1 << 22),
    BATCH_FLAGS_BIT23 = (1 << 23),
    BATCH_FLAGS_BIT24 = (1 << 24),
    BATCH_FLAGS_BIT25 = (1 << 25),
    BATCH_FLAGS_BIT26 = (1 << 26),
    BATCH_FLAGS_BIT27 = (1 << 27),
    BATCH_FLAGS_BIT28 = (1 << 28),
    BATCH_FLAGS_BIT29 = (1 << 29),
    BATCH_FLAGS_BIT30 = (1 << 30),
    BATCH_FLAGS_BIT31 = (1 << 31)
};

void texFreeTexture(TextureHeader *tex); // Non Matching

typedef struct TextureCacheEntry {
    s32 id;
    TextureHeader *texture;
} TextureCacheEntry;

typedef struct Sprite {
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
} Sprite;

typedef struct SpriteCacheEntry {
    s32 id;
    Sprite *sprite;
} SpriteCacheEntry;

extern SpriteCacheEntry *D_800FF9EC;
extern s32 D_800FF9F8;

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texInitTextures.s")

void texDisableModes(s32 flags) {
    D_800A5838 |= flags;
}

void texEnableModes(s32 flags) {
    D_800A5838 &= ~flags;
}

void texModelTextureLoad(u8 arg0) {
    D_800A583C = arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texLoadTexture.s")

#ifdef NON_EQUIVALENT
extern s32 D_800FF9D0;
extern TextureCacheEntry *D_800FF9C8;
void texFreeTexture(TextureHeader *tex) {
    s32 i;
    s32 j;
    s32 texId;

    if (tex != NULL) {
        tex->numberOfInstances--;
        if (tex->numberOfInstances <= 0) {
            for (i = 0; i < D_800FF9D0; i++) {
                j = i << 1;
                if (tex == D_800FF9C8[j].texture) {
                    texId = -1;
                    mmFree(tex);
                    D_800FF9C8[j].id = texId;
                    D_800FF9C8[j].texture = (TextureHeader *) texId;
                    break;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/textures/texFreeTexture.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texGetTextureNum.s")

void texLoadTextureAddr(s32 arg0, s32 arg1) {
    D_800A584C = arg1;
    texLoadTexture(arg0);
    D_800A584C = 0;
}

void setTexMemColour(s32 tagId) {
    D_800A5830 = tagId;
}

typedef struct Struct_Unk_8007B46C {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[2];
    s16 unk16;
    u8 pad17[8];
} Struct_Unk_8007B46C;

TextureHeader *texFrame(TextureHeader *arg0, s32 arg1) {
    TextureHeader *ret = arg0 + 1;
    if ((arg1 > 0) && (arg1 < arg0->numOfTextures << 8)) {
        ret = (TextureHeader *) (((u8 *) arg0) + ((arg1 >> 16) * arg0->textureSize)) + 1;
    }
    return ret;
}

/**
 * Resets all render settings to the default state.
 * The next draw call will be forced to apply all settings instead of skipping unecessary steps.
 */
void texDPInit(Gfx **dlist) {
    D_800A5838 = 0;
    D_800FFA14 = 0;
    D_800FFA18 = 0;
    D_800FFA10 = 0;
    D_800FFA1C = 0;
    D_800FFA20 = 1;
    D_800FFA24 = 1;
    if (dlist != NULL) {
        gDPPipeSync((*dlist)++);
        gSPSetGeometryMode((*dlist)++, G_FOG | G_SHADING_SMOOTH | G_SHADE | G_ZBUFFER);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texDPTextureX.s")

void sprSetTextureFilter(s32 arg0) {
    D_800A5834 = arg0;
}

void sprSetIA2ColOverride(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    D_800FFA28 = arg0;
    D_800FFA29 = arg1;
    D_800FFA2A = arg2;
    D_800FFA2B = arg3;
    D_800FFA2C = arg4;
    D_800FFA2D = arg5;
    D_800A5844 = 1;
}

void sprClearIA2ColOverride(void) {
    D_800A5844 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/textures/sprDPset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texLoadSprite.s")

#ifdef NON_MATCHING

void texFreeSprite(Sprite *sprite) {
    s32 i;
    s32 frame;
    s32 spriteId;

    if (sprite != 0) {
        sprite->numberOfInstances--;
        if (sprite->numberOfInstances <= 0) {
            for (i = 0; i < D_800FF9F8; i++) {
                if (sprite == D_800FF9EC[i].sprite) {

                    if (D_800FF9EC[i].sprite == 0) {}

                    for (frame = 0; frame < sprite->numberOfFrames; frame++) {
                        texFreeTexture(sprite->frames[frame]);
                    }
                    spriteId = -1;
                    mmFree(sprite);
                    D_800FF9EC[i].id = spriteId;
                    D_800FF9EC[i].sprite = (Sprite *) spriteId;
                    break;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/textures/texFreeSprite.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/textures/func_800577D8.s")

// builD_tex_list in DKR
void func_80057B8C(TextureHeader *tex, Gfx *_dlist) {
    Gfx *dlist = _dlist;
    if (tex) {}
    tex->cmd = dlist;
    func_80057C50(&dlist, tex, 0, 0);
    // tex->flags & 0x40 - U clamp flag. Wrap
    if (tex->unk1B < 2 && tex->flags & 0x40) {
        if (!(tex->format & 0xF)) {
            func_80057C50(&dlist, tex, 1, (0x1000 - tex->textureSize) >> 3);
        } else {
            func_80057C50(&dlist, tex, 1, 0x100);
        }
    }
    tex->numberOfCommands = dlist - tex->cmd;
}

// Shrunk builD_tex_list
#ifdef NON_EQUIVALENT
void func_80057C50(Gfx **dlist, TextureHeader *tex, s32 rtile, s32 tmem) {
    s32 tileImgSiz;
    s32 imgSiz;
    s32 imgSizIncr;
    s32 imgSizShift;
    s32 imgFmt;
    s32 tileLine;
    s32 texFlags;
    s32 line;
    s32 imgSizTileBytes;
    s32 i;
    s32 var_v1;
    u32 texFormat;
    u8 texHeight;
    u8 texWidth;

    texFormat = tex->format & 0xF;
    texFlags = (tex->format >> 4) & 0xF;
    texHeight = tex->height;
    texWidth = tex->width;
    switch (texFormat) {
        case 0:
            tileImgSiz = G_IM_SIZ_32b;
            imgSiz = 3;
            imgSizIncr = G_IM_SIZ_32b_INCR;
            imgSizShift = G_IM_SIZ_32b_SHIFT;
            imgSizTileBytes = G_IM_SIZ_32b_TILE_BYTES;
            break;
        case 1:
        case 4:
            tileImgSiz = G_IM_SIZ_16b;
            imgSiz = 2;
            imgSizIncr = G_IM_SIZ_16b_INCR;
            imgSizShift = G_IM_SIZ_16b_SHIFT;
            imgSizTileBytes = G_IM_SIZ_16b_TILE_BYTES;
            break;
        case 2:
        case 5:
            tileImgSiz = G_IM_SIZ_8b;
            imgSiz = 2;
            imgSizIncr = G_IM_SIZ_8b_INCR;
            imgSizShift = G_IM_SIZ_8b_SHIFT;
            imgSizTileBytes = G_IM_SIZ_8b_TILE_BYTES;
            break;
        default:
            tileImgSiz = G_IM_SIZ_4b;
            imgSiz = 2;
            imgSizIncr = G_IM_SIZ_4b_INCR;
            imgSizShift = G_IM_SIZ_4b_SHIFT;
            imgSizTileBytes = G_IM_SIZ_4b_TILE_BYTES;
            break;
    }
    switch (texFormat) {
        case 0:
        case 1:
            imgFmt = G_IM_FMT_RGBA;
            if ((texFlags == 0) || (texFlags == 2)) {
                tex->flags |= 4;
            }
            break;
        case 4:
        case 5:
        case 6:
            imgFmt = G_IM_FMT_IA;
            tex->flags |= 4;
            break;
        default:
            imgFmt = G_IM_FMT_I;
            break;
    }
    if (tileImgSiz == 0) {
        line = texWidth * 2;
    } else {
        line = texWidth * imgSizTileBytes;
    }
    if (tex->unk1B >= 2) {
        var_v1 = 0;
        for (i = 0; i < tex->unk1B; i++) {
            var_v1 += (texWidth >> i) * (texHeight >> i);
        }
        gDPSetTextureImage((*dlist)++, imgFmt, imgSiz, 1, OS_PHYSICAL_TO_K0(tex + 1));
        // gDPSetTile - Maybe not? arg3 is just using the first 13 bits, 0-12
        gDPSetTile((*dlist)++, imgFmt, imgSiz, 0, tmem & 0x1FF, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
        gDPLoadSync((*dlist)++);
        gDPLoadBlock((*dlist)++, G_TX_LOADTILE, 0, 0, (((s32) (var_v1 + imgSizIncr) >> imgSizShift) - 1), 0);
        gDPPipeSync((*dlist)++);
        i = 0;
        while (i < tex->unk1B) {
            tileLine = ((line) + 7) >> 3;
            gDPSetTile((*dlist)++, imgFmt, tileImgSiz, tileLine, tmem & 0x1FF, rtile, 0, tex->unk1E, (tex->unk1F - i),
                       i, tex->unk1C, (tex->isCompressed - i), i);
            gDPSetTileSize((*dlist)++, rtile, 0, 0, ((texWidth) -1) << G_TEXTURE_IMAGE_FRAC,
                           ((texHeight) -1) << G_TEXTURE_IMAGE_FRAC);
            i++;
            tmem += tileLine * texHeight;
            rtile++;
            texWidth = (u8) ((s32) texWidth >> 1);
            texHeight = (u8) ((s32) texHeight >> 1);
            line >>= 1;
        }
        gSPTexture((*dlist)++, 0, 0, (tex->unk1B - 1), 0, 1);
        gSPEndDisplayList((*dlist)++);
    } else {
        gDkrDPLoadMultiBlockS((*dlist)++,                 // pkt
                              OS_PHYSICAL_TO_K0(tex + 1), // timg
                              tmem,                       // tmem
                              rtile,                      // rtile
                              imgFmt,                     // fmt
                              imgSiz,                     // sizblk
                              tileImgSiz,                 // siztile
                              imgSizIncr,                 // sizincr
                              0,                          // sizbytes
                              imgSizShift,                // sizshift
                              line,                       // line
                              texWidth,                   // width
                              texHeight,                  // height
                              0,                          // pal
                              tex->unk1C,                 // cms
                              tex->unk1E,                 // cmt
                              tex->isCompressed,          // masks
                              tex->unk1F,                 // maskt
                              0,                          // shifts
                              0                           // shiftt
        );
        // //gDPLoadTextureBlock
        // {
        //     gDPSetTextureImage((*dlist)++, imgFmt, imgSiz, 1, OS_PHYSICAL_TO_K0(tex + 1));
        //     gDPSetTile((*dlist)++, imgFmt, imgSiz, 0, arg3 & 0x1FF, G_TX_LOADTILE, 0, tex->unk1E, tex->unk1F, 0,
        //     tex->unk1C, tex->isCompressed, 0); gDPLoadSync((*dlist)++); gDPLoadBlock((*dlist)++, G_TX_LOADTILE, 0, 0,
        //     (((s32) ((texWidth * texHeight) + imgSizIncr) >> imgSizShift) - 1), 0); gDPPipeSync((*dlist)++);
        //     gDPSetTile(
        //         (*dlist)++,//pkt
        //         imgFmt,//fmt
        //         tileImgSiz,//siz
        //         (((s32) (line + 7) >> 3) & 0x1FF),//line
        //         (arg3 & 0x1FF),//tmem
        //         arg2,//tile
        //         0,//palette
        //         tex->unk1E,//cmt
        //         tex->unk1F,//maskt
        //         0,//shiftt
        //         tex->unk1C,//cms
        //         tex->isCompressed,//masks
        //         0//shifts
        //     );
        //     gDPSetTileSize(
        //         (*dlist)++,//pkt
        //         arg2,//tile
        //         0,//uls
        //         0,//ult
        //         ((texWidth)-1) << G_TEXTURE_IMAGE_FRAC,//lrs
        //         ((texHeight)-1) << G_TEXTURE_IMAGE_FRAC//lrt
        //     );
        // }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/textures/func_80057C50.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texAnimateSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texAnimateTexSprite.s")

void texAnimateTexture(TextureHeader *texture, u32 *triangleBatchInfoFlags, s32 *arg2, s32 updateRate) {
    s32 breakVar;
    u16 *frameAdvanceDelay;
    u8 blink;
    s32 arg2Temp = *arg2;
    s32 flags = *triangleBatchInfoFlags;

    if (flags & BATCH_FLAGS_BIT17) {
        blink = blinkMode;
        if (!(flags & BATCH_FLAGS_BIT18)) {
            if (blink == 0) {
                if (mathRnd(0, 1000) > 985) {
                    flags &= ~BATCH_FLAGS_BIT19;
                    flags |= BATCH_FLAGS_BIT18;
                }
            } else if (blink != 2) {
                flags &= ~BATCH_FLAGS_BIT19;
                flags |= BATCH_FLAGS_BIT18;
            }
        } else if (!(flags & BATCH_FLAGS_BIT19)) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
            if (arg2Temp >= texture->numOfTextures) {
                if (blink == 3) {
                    arg2Temp = texture->numOfTextures - 1;
                } else {
                    arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                    if (arg2Temp < 0) {
                        arg2Temp = 0;
                        flags &= ~(BATCH_FLAGS_BIT19 | BATCH_FLAGS_BIT18);
                    } else {
                        flags |= BATCH_FLAGS_BIT19;
                    }
                }
            }
        } else {
            arg2Temp -= texture->frameAdvanceDelay * updateRate;
            if (arg2Temp < 0) {
                arg2Temp = 0;
                flags &= ~(BATCH_FLAGS_BIT19 | BATCH_FLAGS_BIT18);
            }
        }
        blinkMode = 0;
    } else if (flags & BATCH_FLAGS_BIT18) {
        if (!(flags & BATCH_FLAGS_BIT19)) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
        } else {
            // Probably a fake match var, but it works.
            frameAdvanceDelay = &texture->frameAdvanceDelay;
            arg2Temp -= (*frameAdvanceDelay) * updateRate;
        }
        do {
            breakVar = FALSE;
            if (arg2Temp < 0) {
                arg2Temp = -arg2Temp;
                flags &= ~BATCH_FLAGS_BIT19;
                breakVar = TRUE;
            }
            if (arg2Temp >= texture->numOfTextures) {
                arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                flags |= BATCH_FLAGS_BIT19;
                breakVar = TRUE;
            }
        } while (breakVar);
    } else if (!(flags & BATCH_FLAGS_BIT19)) {
        arg2Temp += texture->frameAdvanceDelay * updateRate;
        while (arg2Temp >= texture->numOfTextures) {
            arg2Temp -= texture->numOfTextures;
        }
    } else {
        arg2Temp -= texture->frameAdvanceDelay * updateRate;
        while (arg2Temp < 0) {
            arg2Temp += texture->numOfTextures;
        }
    }
    *arg2 = arg2Temp;
    *triangleBatchInfoFlags = flags;
}

void initColourCycle(unkResetColourCycle *arg0, s32 arg1) {
    arg0->unkC = (unkResetColourCycle *) objGetTable(arg1);
    resetColourCycle(arg0);
}

void resetColourCycle(unkResetColourCycle *arg0) {
    unkResetColourCycle *temp_v0;

    temp_v0 = arg0->unkC;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = temp_v0->unk8;
    arg0->unk9 = temp_v0->unk9;
    arg0->unkA = temp_v0->unkA;
    arg0->unkB = temp_v0->unkB;
}

#pragma GLOBAL_ASM("asm/nonmatchings/textures/updateColourCycle.s")

void resetMixCycle(PulsatingLightData *data) {
    s32 i;
    data->currentFrame = 0;
    data->time = 0;
    data->totalTime = 0;
    data->outColorValue = data->frames[0].value;
    for (i = 0; i < data->numberFrames; i++) {
        data->totalTime += data->frames[i].time;
    }
}

void updateMixCycle(PulsatingLightData *data, s32 timeDelta) {
    s32 thisFrameIndex, nextFrameIndex;

    if (data->numberFrames > 1) {
        data->time += timeDelta;
        while (data->time >= data->totalTime) {
            data->time -= data->totalTime;
        }
        while (data->time >= data->frames[data->currentFrame].time) {
            data->time -= data->frames[data->currentFrame].time;
            data->currentFrame++;
            if (data->currentFrame >= data->numberFrames) {
                data->currentFrame = 0;
            }
        }
        thisFrameIndex = data->currentFrame;
        nextFrameIndex = thisFrameIndex + 1;
        if (nextFrameIndex >= data->numberFrames) {
            nextFrameIndex = 0;
        }

        data->outColorValue = data->frames[thisFrameIndex].value +
                              ((data->frames[nextFrameIndex].value * data->time) / data->frames[thisFrameIndex].time);
    }
}
