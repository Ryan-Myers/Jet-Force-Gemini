#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include <PR/sp.h>
#include "structs.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_HEIGHT_PAL (SCREEN_HEIGHT + 44)
#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)
#define SCREEN_HEIGHT_HALF (SCREEN_HEIGHT / 2)
#define SCREEN_HEIGHT_HALF_PAL (SCREEN_HEIGHT_HALF + 12)

#define SCREEN_WIDTH_FLOAT ((float)(SCREEN_WIDTH))
#define SCREEN_HEIGHT_FLOAT ((float)(SCREEN_HEIGHT))
#define SCREEN_WIDTH_FLOAT_HALF ((float)SCREEN_WIDTH / 2)
#define SCREEN_HEIGHT_FLOAT_HALF ((float)SCREEN_HEIGHT / 2)

#define CAMERA_ASPECT (SCREEN_WIDTH_FLOAT / SCREEN_HEIGHT_FLOAT)
#define CAMERA_FAR 15000.0f
#define CAMERA_NEAR 10.0f
#define CAMERA_DEFAULT_FOV 60.0f
#define CAMERA_MIN_FOV 0.0f
#define CAMERA_MAX_FOV 90.0f
#define CAMERA_SCALE 1.0f

/* Size: 0x44 bytes */
// Copied from DKR
typedef struct Camera {
    /* 0x0000 */ ObjectTransform trans;
    /* 0x0018 */ f32 cam_unk_18;
    /* 0x001C */ f32 boomLength;
    /* 0x0020 */ f32 cam_unk_20;
    /* 0x0024 */ f32 x_velocity;
    /* 0x0028 */ f32 y_velocity;
    /* 0x002C */ f32 z_velocity;
    /* 0x0030 */ f32 shakeMagnitude;
    /* 0x0034 */ s16 cameraSegmentID;
    /* 0x0036 */ s16 mode;
    /* 0x0038 */ s16 pitch;
    /* 0x003A */ s8 shakeTimer;
    /* 0x003B */ u8 zoom;
    /* 0x003C */ u8 unk3C;
    /* 0x003D */ u8 unk3D;
    /* 0x003E */ u8 unk3E;
    /* 0x003F */ u8 unk3F;
    /* 0x0040 */ ObjectHeader *header;
  } Camera;

s32 camGetPtr(void);
s32 camGetRotationMtx(void);
Mtx *camGetProjOrgMtx(void);
void camDoSprite(Gfx**, Mtx**, Vertex**, void*, Sprite*, s32, s32); /* extern */

#endif
