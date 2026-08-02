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

void func_8003B640(u16 *src, u16 *dest, s32 len);
void *func_8003BED0(struct_8003BED0_arg0 *arg0);
void func_8003C6D0(ObjectModel *mdl);

#endif
