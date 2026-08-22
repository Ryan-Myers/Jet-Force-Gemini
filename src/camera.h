#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include <PR/sp.h>
#include "gameVi.h"
#include "structs.h"
#include "types.h"

#define CAMERA_ASPECT ASPECT_RATIO_NTSC
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

extern ScreenViewport D_800A3728[4]; //gScreenViewports

void camPopModelMtx(Gfx **dlist);
void camRestoreModelMtx(Gfx **dlist);
void camPushModelMtx(Gfx **dList, Mtx **mtx, ObjectTransform *trans, f32 scale, f32 scaleY);
Matrix *camGetInvProjMtx(void);
s32 camProjectPoint(f32 x, f32 y, f32 z, f32 *outX, f32 *outY, u8 transform);
Camera *camGetPtr(void);
Matrix *camGetRotationMtx(void);
Mtx *camGetProjOrgMtx(void);
void camDoSprite(Gfx**, Mtx**, Vertex**, ObjectSegment*, Sprite*, s32, u8);
void camSetFOV(f32, s32);
void camSetNo(s32);
void camSetScissor(Gfx **dlist);
s32 camGetMode(void);
void camDisableUserView(s32, s32);
void camEnableUserView(s32, s32);
void camResetView(Gfx**);
void camSetView(Gfx**, Mtx**);
void camUserViewTick(void);
void camlightDraw(Gfx**, Mtx**, Vertex**);
void camlightUpdateAll(void);
void camlightVisibilityCheck(void);
s32 camGetNo(void);
u8 camGetWaterLine(s32 arg0);
s32 camProjectPoint(f32, f32, f32, void*, f32*, s32);
void camGetWindowLimits(s32, s32, s32*, s32*, u32*, u32*);
void camStandardOrtho(Gfx**, Mtx**);

#endif
