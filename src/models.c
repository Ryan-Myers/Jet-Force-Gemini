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

extern s32 *gObjectModelTable;
extern s32 *gModelCache; // Size: 0x230 in kiosk, 0x2A8 in other versions
extern s32 *D_800F6F18_B1758;
extern s32 gModelCacheCount;
extern s32 gNumModelIDs;
extern s32 D_800F6F24_B1764;
extern s32 *D_800F6F28_B1768;
extern s32 *D_800F6F2C_B176C;
extern s32 *D_800F6F30_B1770;
extern s32 *D_800F6F34_B1774;
extern s32 *D_800F6F38_B1778;
extern s32 *D_800F6F3C_B177C;
extern s32 D_800F6F40_B1780;
extern s32 *D_800F6F58_B1798;

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

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[6];
    u16 *unk1C;
} struct_8003BED0_arg0;

typedef struct {
    struct_8003BED0_arg0 *unk0;
    u16 *unk4;
    u8 pad8[2];
    u8 unkA;
    u8 unkB;
    s32 unkC;
} struct_8003BED0_alloc;

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

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeModel.s")

//void free_model_data(ObjectModel *mdl);
#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C6D0.s")

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
