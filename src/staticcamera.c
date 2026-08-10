#include "common.h"
#include "math/math.h"


typedef struct LobbyCamStruct {
    /* 0x00 */ Vec3s rotation;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x2C */ f32 unk2C;
} LobbyCamStruct;

extern LobbyCamStruct *gLobbyCam;
extern s32 gStaticCam;

void staticcamFree(void) {
    gStaticCam = 0;
}

void staticcamAdd(s32 arg0) {
    gStaticCam = arg0;
}

void staticcamDelete(s32 arg0) {
    if (arg0 == gStaticCam) {
        gStaticCam = 0;
    }
}

LobbyCamStruct *lobbycamInuse(void) {
    return gLobbyCam;
}

s32 staticcamInuse(void) {
    return gStaticCam;
}

void lobbycamGetFocalPoint(LobbyCamStruct *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    Vec3f direction;
    direction.x = arg0->unk14;
    direction.y = arg0->unk18;
    direction.z = arg0->unk1C;
    if (arg0->unk6 != 0) {
        mathOneFloatYPR(&arg0->rotation, &direction);
    } else {
        mathOneFloatRPY(&arg0->rotation, &direction);
    }
    *arg1 = direction.f[0] + arg0->unk8;
    *arg2 = direction.f[1] + arg0->unkC;
    *arg3 = direction.f[2] + arg0->unk10;
}

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/func_800427E8_433E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/lobbycamPosition.s")

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/staticcamPosition.s")
