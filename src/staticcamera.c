#include "common.h"
#include "math/math.h"

// Probably just Camera struct from camera.h
typedef struct StaticCam {
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
} StaticCam;

StaticCam *gLobbyCam;
StaticCam *gStaticCam;

void staticcamFree(void) {
    gStaticCam = NULL;
}

void staticcamAdd(StaticCam *cam) {
    gStaticCam = cam;
}

void staticcamDelete(StaticCam *cam) {
    if (cam == gStaticCam) {
        gStaticCam = NULL;
    }
}

StaticCam *lobbycamInuse(void) {
    return gLobbyCam;
}

StaticCam *staticcamInuse(void) {
    return gStaticCam;
}

void lobbycamGetFocalPoint(StaticCam *cam, f32 *arg1, f32 *arg2, f32 *arg3) {
    Vec3f direction;
    direction.x = cam->unk14;
    direction.y = cam->unk18;
    direction.z = cam->unk1C;
    if (cam->unk6 != 0) {
        mathOneFloatYPR(&cam->rotation, &direction);
    } else {
        mathOneFloatRPY(&cam->rotation, &direction);
    }
    *arg1 = direction.f[0] + cam->unk8;
    *arg2 = direction.f[1] + cam->unkC;
    *arg3 = direction.f[2] + cam->unk10;
}

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/func_800427E8_433E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/lobbycamPosition.s")

void staticcamPosition(StaticCam *cam) {
    if (gStaticCam != NULL) {
        cam->unkC = gStaticCam->unkC;
        cam->unk10 = gStaticCam->unk10;
        cam->unk14 = gStaticCam->unk14;
        cam->rotation.z = gStaticCam->rotation.z;
        cam->rotation.y = -gStaticCam->rotation.y;
        cam->rotation.x = 0x8000 - gStaticCam->rotation.x;
    }
}
