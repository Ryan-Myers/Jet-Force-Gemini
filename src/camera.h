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

/* Size: 0x4C (DKR copy was 0x44; stride from camGetPtr / cam++) */
typedef struct Camera {
    /* 0x00 */ ObjectTransform trans;
    /* 0x18 */ f32 cam_unk_18; /* names kept for staticcamera.c */
    /* 0x1C */ f32 boomLength;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 fov;
    /* 0x30 */ Vec3f shake;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[6];
    /* 0x4A */ s16 unk4A; /* added to trans.rotation.x */
} Camera;

Camera *camGetPtr(void);
Matrix *camGetRotationMtx(void);
Mtx *camGetProjOrgMtx(void);
void camDoSprite(Gfx**, Mtx**, Vertex**, void*, Sprite*, s32, s32);
void camDisableUserView(s32, s32);
void camEnableUserView(s32, s32);
void camResetView(Gfx**);
void camSetNo(s32);
void camSetView(Gfx**, Mtx**);
void camUserViewTick(void);
void camlightDraw(Gfx**, Mtx**, Vertex**);
void camlightUpdateAll(void);
void camlightVisibilityCheck(void);

#endif
