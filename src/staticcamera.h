#ifndef _STATICCAMERA_H_
#define _STATICCAMERA_H_

#include "camera.h"
#include "structs.h"
#include <ultra64.h>

// Seems to be different from the regular camera struct because it's loading floats in the first few positions.
typedef struct LobbyCamStruct {
    Vec3f unk0;
    f32 unkC;
    u8 pad10[0x4];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
} LobbyCamStruct;

void staticcamFree(void);
void staticcamAdd(Camera *cam);
void staticcamDelete(Camera *cam);
LobbyCamStruct *lobbycamInuse(void);
Camera *staticcamInuse(void);
void lobbycamGetFocalPoint(Camera *cam, f32 *arg1, f32 *arg2, f32 *arg3);
void lobbycamPosition(Camera *cam1, Camera *cam2, s32 exp, s32 smoothed);
void staticcamPosition(Camera *cam);

#endif
