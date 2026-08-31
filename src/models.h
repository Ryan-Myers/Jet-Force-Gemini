#ifndef _MODELS_H_
#define _MODELS_H_

#include "common.h"

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
    /* 0x50 */ s8 **unk50;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[0x74 - 0x65];
    /* 0x74 */ void *unk74;
    /* 0x78 */ void *unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x7E */ u8 pad80[0x84 - 0x80];
    /* 0x84 */ s8 **unk84;
} ObjectModel_JFG;

typedef struct ModInst_UnkC {
    u8 pad;
} ModInst_UnkC;

/* Size: 0x24 bytes */
typedef struct ModelInstance_JFG {
    /* 0x00 */ ObjectModel_JFG *objModel;
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8 pad8[0x3];
    /* 0x0B */ s8 unkB;
    /* 0x0C */ ModInst_UnkC *unkC;
    /* 0x10 */ u8 unk10[0x70];
    /* 0x80 */ s8 *unk80[2];
} ModelInstance_JFG;

void func_8003C92C_3D52C(ObjectModel_JFG *mdl);
void modFreeModel(ModelInstance_JFG *modInst);

void func_8003B870_3C470(u16 *src, u16 *dest, s32 len);
void *func_8003C12C_3CD2C(struct_8003BED0_arg0 *arg0);
void modFreeAnim(s8 *arg0);
void texFreeTexture(TextureHeader *tex);

#endif
