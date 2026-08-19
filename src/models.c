#include "models.h"
#include "common.h"
#include "memory.h"
#include "pi.h"

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
void func_8003B870_3C470(u16 *src, u16 *dest, s32 len) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C0C4_3CCC4.s")

void *func_8003C12C_3CD2C(struct_8003BED0_arg0 *arg0) {
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
        func_8003B870_3C470(arg0->unk1C, temp_a1, arg0->unk12 * 0xA);
    }
    return temp_v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C1B4_3CDB4.s")

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
                func_8003C92C_3D52C(model);
                D_800F6F18_B1758[D_800F6F24_B1764] = modelIndex;
                D_800F6F24_B1764++;
                gModelCache[ASSETCACHE_ID(modelIndex)] = -1;
                gModelCache[ASSETCACHE_PTR(modelIndex)] = -1;
            }
        }
    }
}

void func_8003C92C_3D52C(ObjectModel_JFG *mdl) {
    s32 animsFreed;
    s32 animIndex;

    if (mdl->numberOfTextures > 0) {
        animsFreed = 0;
        animIndex = 0;
        do {
            if (mdl->textures[animIndex].texture != NULL) {
                texFreeTexture(mdl->textures[animIndex].texture);
            }
            animsFreed++;
            animIndex++;
        } while (animsFreed < mdl->numberOfTextures);
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

    // free the animations
    if (mdl->unk4E != 0) {
        if (mdl->unk64 == 0) {
            if (mdl->unk50 != NULL) {
                animsFreed = 0;
                animIndex = 0;
                do {
                    modFreeAnim(mdl->unk50[animIndex]);
                    animsFreed++;
                    animIndex++;
                } while (animsFreed < mdl->unk4E);
                mmFree(mdl->unk50);
            }
        }
        if (mdl->unk5C != NULL) {
            mmFree(mdl->unk5C);
        }
        mmFree(mdl->unk58);
    }

    if (mdl->unk84 != NULL) {
        animsFreed = mdl->unk7C + 1;
        while (animsFreed--) {
            mmFree(mdl->unk84[animsFreed]);
        }
        mmFree(mdl->unk84);
    }
    mmFree(mdl);
}

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CB04_3D704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CDAC_3D9AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CF24_3DB24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CFCC_3DBCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGenAnimMatrices.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelSetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelGetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/makeModelGfx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E35C_3EF5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E398_3EF98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modMakeLimbModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modLoadAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGetMakeBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modSetTextureFrame.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/models/modSuspendModelTextures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modResumeModelTextures.s")
#endif
