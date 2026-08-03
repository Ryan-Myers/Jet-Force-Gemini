#include "models.h"
#include "common.h"

const char D_800AC900[] = "Error: Model no. out of range on load. !!\n";
const char D_800AC92C[] = "TEXTURE ERROR!!\n%d,%d\n";
const char D_800AC944[] = "Error: Model table overflow!!\n";
const char D_800AC964[] = "CREATE LOD MODEL :: null model pointer!";
const char D_800AC98C[] = "WARNING :: createModelInstance called with NULL pointer\n";
const char D_800AC9C8[] = "MODELS Error: Tryed to deallocate non-existent model!\n";
const char D_800ACA00[] = "MODELS Error : cannot free NULL model instance pointer!!\n";
const char D_800ACA3C[] = "modLoadAnim: Overflowed AnimTab!\n";
const char D_800ACA60[] = "modFreeAnim : NULL anim!!\n";
const char D_800ACA7C[] = "Anim Error: Tryed to deallocate non-existent anim!!\n";
const char D_800ACAB4[] = "modLoadAnimEvent: Overflowed AnimEventTab!\n";
const char D_800ACAE0[] = "modFreeAnimEvents : NULL anim event table!!\n";
const char D_800ACB10[] = "Anim Event Error: Tryed to deallocate non-existent anim event!\n";

/**
 * Two Byte memcpy
 */
void func_8003B640(u16 *src, u16 *dest, s32 len) {
    len = (len + 1) >> 1;
    while (len--) {
        *dest++ = *src++;
    }
}

#ifdef VERSION_kiosk
#define SIZEOF_gModelCache 0x230
#else
#define SIZEOF_gModelCache 0x2A8
#endif

void modInitModels(void) {
    s32 *temp;

    gModelCache = (s32 *) mmAlloc(SIZEOF_gModelCache, COLOUR_TAG_GREEN);
    D_800F6F18_B1758 = (s32 *) mmAlloc(0x190, COLOUR_TAG_GREEN);
    gModelCacheCount = 0;
    D_800F6F24_B1764 = 0;
    D_800F6F58_B1798 = (s32 *) mmAlloc(0x2000, COLOUR_TAG_GREEN);
    gObjectModelTable = (s32 *) piRomLoad(0x26);
    gNumModelIDs = 0;
    while (gObjectModelTable[gNumModelIDs] != -1) {
        gNumModelIDs++;
    }
    gNumModelIDs--;
    temp = (s32 *) mmAlloc(0xA0, COLOUR_TAG_GREEN);
    D_800F6F28_B1768 = temp;
    D_800F6F38_B1778 = D_800F6F2C_B176C = (s32 *) ((u8 *) temp + 0x80);
    D_800F6F34_B1774 = (s32 *) ((u8 *) temp + 0x90);
    D_800F6F30_B1770 = (s32 *) mmAlloc(0x800, COLOUR_TAG_GREEN);
    D_800F6F3C_B177C = (s32 *) mmAlloc(0x100, COLOUR_TAG_GREEN);
    D_800F6F40_B1780 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/models/modLoadModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003BE68.s")

void *func_8003BED0(struct_8003BED0_arg0 *arg0) {
    u16 *temp_a1;
    struct_8003BED0_alloc *temp_v0;

    temp_v0 = (struct_8003BED0_alloc *) mmAlloc2(arg0->unk12 * 0xA + 0x10, COLOUR_TAG_BLUE);
    if (temp_v0 != NULL) {
        temp_a1 = (u16 *) ((u8 *) temp_v0 + 0x10);
        temp_v0->unk0 = arg0;
        temp_v0->unk4 = temp_a1;
        temp_v0->unkA = 2;
        temp_v0->unkB = 0;
        temp_v0->unkC = 0;
        func_8003B640(arg0->unk1C, temp_a1, arg0->unk12 * 0xA);
    }
    return temp_v0;
}

void modFreeAnim(s8 *arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003BF58.s")

typedef struct ObjectModel_JFG {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 numberOfTextures;
    /* 0x11 */ u8 pad11[0x18 - 0x11];
    /* 0x18 */ TextureInfo *textures;
    /* 0x1C */ u8 pad1C[0x28 - 0x1C];
    /* 0x28 */ void *unk28;
    /* 0x2C */ u8 pad2C[0x4C - 0x2C];
    /* 0x4C */ s16 references;
    /* 0x4E */ s8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ s8 *unk50;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[0x74 - 0x65];
    /* 0x74 */ void *unk74;
    /* 0x78 */ void *unk78;
    /* 0x7C */ s32 numberOfAnimations;
    /* 0x7E */ u8 pad7E[0x84 - 0x7E];
    /* 0x84 */ ObjectModel_44 *animations;
} ObjectModel_JFG;

typedef struct ModInst_UnkC {
    u8 pad;
} ModInst_UnkC;

/* Size: 0x24 bytes */
typedef struct ModelInstance_JFG {
    /* 0x00 */ ObjectModel_JFG *objModel;
    /* 0x04 */ u8 pad4[0x8];
    /* 0x0C */ ModInst_UnkC *unkC;
    /* 0x10 */ u8 unk10[0x70];
    /* 0x80 */ s8 *unk80[2];
} ModelInstance_JFG;

void func_8003C6D0(ObjectModel_JFG *mdl);

void modFreeModel(ModelInstance_JFG *modInst) {
    ObjectModel_JFG *model;
    s32 i;
    s32 modelIndex;

    if (modInst != NULL) {
        model = modInst->objModel;
        if (modInst->unkC != NULL) {
            mmFree(modInst->unkC);
        }

        if (model->unk64) {
            for (i = 0; i < 2; i++) {
                if (modInst->unk80[i] != NULL) {
                    modFreeAnim(modInst->unk80[i]);
                }
            }
        }
        mmFree(modInst);
        model->references--;
        if (model->references <= 0) {
            i = 0;
            modelIndex = -1;
            while (i < gModelCacheCount) {
                if (model == (ObjectModel_JFG *) gModelCache[ASSETCACHE_PTR(i)]) {
                    modelIndex = i;
                }
                i++;
            }

            if (modelIndex != -1) {
                func_8003C6D0(model);
                D_800F6F18_B1758[D_800F6F24_B1764] = modelIndex;
                D_800F6F24_B1764++;
                gModelCache[ASSETCACHE_ID(modelIndex)] = -1;
                gModelCache[ASSETCACHE_PTR(modelIndex)] = -1;
            }
        }
    }
}

#if 0
void texFreeTexture(TextureHeader *tex);
void func_8003C6D0(ObjectModel_JFG *mdl) {
    s32 sp2C;
    TextureHeader *temp_a0;
    s32 temp_s0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_v0;
    u8 numTextures;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a0_5;
    void *temp_a0_6;
    s32 i;

    numTextures = mdl->numberOfTextures;
    if (numTextures > 0) {
        s32 texturesFreed = 0;
        s32 textureIndex = 0;
        do {
            TextureHeader *header = mdl->textures[textureIndex].texture;
            if (header != NULL) {
                texFreeTexture(header);
                numTextures = mdl->numberOfTextures;
            }
            texturesFreed++;
            textureIndex++;
        } while (texturesFreed < numTextures);
    }

    if (mdl->unk60 != NULL) {
        mmFree(mdl->unk60);
    }

    if (mdl->unk28 != NULL) {
        mmFree(mdl->unk28);
    }

    if (mdl->unk74 != NULL) {
        mmFree(mdl->unk74);
    }

    if (mdl->unk78 != NULL) {
        mmFree(mdl->unk78);
    }
    if (mdl->unk4E != 0) {
        if (mdl->unk64 == 0) {
            var_s0_2 = 0;
            var_s1 = 0;
            if (mdl->unk50 != NULL) {
                do {
                    modFreeAnim(&mdl->unk50[var_s1]);
                    var_s0_2 += 1;
                    var_s1 += 4;
                } while (var_s0_2 < (s8) mdl->unk4E);
                mmFree(mdl->unk50);
            }
        }
        temp_a0_6 = mdl->unk5C;
        if (temp_a0_6 != NULL) {
            mmFree(temp_a0_6);
        }
        mmFree(mdl->unk58);
    }
        // free the animations
    if (mdl->animations != NULL) {
        s32 animsFreed = 0;
        s32 animIndex = 0;
        if (mdl->numberOfAnimations != 0) {
            do {
                mmFree(mdl->animations[animIndex].anim);
                animsFreed++;
                animIndex++;
            } while (animsFreed < mdl->numberOfAnimations);
            mmFree(mdl->animations);
        }
    }
    // if (mdl->animations != NULL) {
    //     temp_s0 = mdl->numberOfAnimations + 1;
    //     var_s0_3 = temp_s0 - 1;
    //     if (temp_s0 != 0) {
    //         var_s1_2 = var_s0_3 * 4;
    //         do {
    //             mmFree(mdl->animations[var_s1_2].anim);
    //             var_s1_2 -= 4;
    //             var_s0_3 -= 1;
    //         } while (var_s0_3 != 0);
    //     }
    //     mmFree(mdl->animations);
    // }
    mmFree(mdl);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C6D0.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C8A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CB50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CCC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGenAnimMatrices.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelSetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelGetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/makeModelGfx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E100.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E13C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modMakeLimbModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modLoadAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGetMakeBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modSetTextureFrame.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/models/modSuspendModelTextures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modResumeModelTextures.s")
#endif
