#include "camera.h"
#include "common.h"
#include "gameVi.h"
#include "math/math.h"

const char D_800ACC60[] = "Camera Error: Illegal mode!\n";
const char D_800ACC80[] = "Illegal CAMERA number %d\n";
const char D_800ACC9C[] = "%f\n";
const char D_800ACCA0[] = "\nCam do 2D sprite called with NULL pointer!";
const char D_800ACCCC[] = "CAM Error!! Convlist overflow.\n";

typedef struct {
    /* 0x00 */ Matrix *mtx;
    /* 0x04 */ s16 count;
} ConvListEntry;

typedef struct {
    /* 0x0 */ s16 attackEnd;
    /* 0x2 */ s16 sustainEnd;
    /* 0x4 */ s16 totalEnd;
    /* 0x6 */ s16 timer;
    /* 0x8 */ s32 magnitude;
} CameraShake;

extern s8 D_800FA650_B18D0;
extern s16 D_800FA4D4_B1754;
extern s32 D_800A2ED0_A3AD0;
extern ScreenViewport D_800A2ED4_A3AD4[];
extern s32 D_800A2FA4_A3BA4;
extern Vp D_800A2FB8_A3BB8[];
extern ObjectTransform D_800A30F8_A3CF8;
extern f32 D_800A313C_A3D3C;
extern u8 D_800A316C_A3D6C;
extern u8 D_800A3174_A3D74[4];
extern u8 D_800A3178_A3D78[];
extern f32 D_800A317C_A3D7C[4];
extern Camera cameraActorArray[];
extern CameraShake D_800FA600_B1880[];
extern s32 D_800FA630_B18B0;
extern s32 D_800FA634_B18B4;
extern ConvListEntry D_800FA658_B18D8[];
extern s32 D_800FAA58_B5298;
extern s32 D_800FAA5C_B1CDC;
extern f32 D_800FAA60_B1CE0;
extern f32 D_800FAA64_B1CE4;
extern u16 D_800FAA68_B1CE8;
extern Matrix D_800FAA70_B1CF0;
extern Matrix D_800FAAF0_B1D70;
extern Matrix D_800FAAB0_B1D30;
extern Mtx *D_800FAB30_B1DB0;
extern Mtx D_800FAB38_B1DB8[];
extern Mtx D_800FAF38_B21B8;
extern Matrix D_800FAF78_B21F8;
extern Matrix D_800FAFB8_B2238;
extern Matrix D_800FAFF8_B2278;
extern Matrix D_800FB038_B22B8;
extern f32 D_800FB078_B22F8;
extern f32 D_800FA4FC_B177C;
extern f32 D_800A3168_A3D68;
extern f32 aspectRatioFloat;
extern ObjectTransform D_800FA638_B18B8;

extern s32 frontGet2PlayerSplit(void);
extern u8 levelGetType(void);
void sprDPset(Gfx **dlist, Sprite *sprite, s32 flags, s32 arg3, u8 arg4);

void camCopyOrthoMatrix(Matrix dest);
void camSetFOV(f32, s32);
void camSetProjMtx(Gfx **dlist, Mtx **mtx);
void camSetViewport(Gfx **dlist, s32 halfWidth, s32 halfHeight, s32 centerX, s32 centerY);

#if 0
extern s32 D_B0000578;
extern s32 D_800FB0F4; //gActiveCameraID;
extern Matrix D_800FB530; // gPerspectiveMatrixF;
extern MatrixS D_800FB5F8; // gProjectionMatrixS;
extern u16 D_800FB528; // perspNorm;
extern f32 D_800FBB38; // gCurCamFOV;
extern s8 D_800A3720; // AntiPiracyViewport = FALSE;
extern s32 D_800FB0F0; // gActiveCameraID;

void camInit(void) {
    s32 i;
    s32 j;
    u32 stat;

    // clang-format off
    // This section is highly suspicous to be undefined behaviour as we have it defined right now.
    for (i = 0; i < 5; i++) { gModelMatrixF[i] = (Matrix *) &D_80120DA0[(i << 1)]; }
    // clang-format on

    for (j = 0; j < 8; j++) {
        D_800FB0F4 = j;
        func_8003FAEC_406EC(200, 200, 200, 0, 0, 180);
    }

    gCutsceneCameraActive = FALSE;
    D_800FB0F4 = 0;
    gModelMatrixStackPos = 0;
    gCameraMatrixPos = 0;
    gNumberOfViewports = 0;
    gSpriteAnimOff = FALSE;
    D_80120D18 = 0;
    gAdjustViewportHeight = 0;
    D_800A3720 = FALSE;

    WAIT_ON_IOBUSY(stat);

    // 0xB0000578 is a direct read from the ROM as opposed to RAM
    if (((D_B0000578 & 0xFFFF) & 0xFFFF) != 0x8965) {
        D_800A3720 = TRUE;
    }

    guPerspectiveF(D_800FB530, &D_800FB528, CAMERA_DEFAULT_FOV, CAMERA_ASPECT, CAMERA_NEAR, CAMERA_FAR,
                   CAMERA_SCALE);
    f32_matrix_to_s16_matrix(&D_800FB530, &D_800FB5F8);
    D_800FBB38 = CAMERA_DEFAULT_FOV;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/camera/camInit.s")
#endif

void camUseShake(void) {
    D_800FAA58_B5298 = 1;
}

void camIgnoreShake(void) {
    D_800FAA58_B5298 = 0;
}

f32 camGetFOV(void) {
    return D_800FB078_B22F8;
}

void camOverrideProjScales(f32 scaleX, f32 scaleY) {
    D_800FAA60_B1CE0 = scaleX;
    D_800FAA64_B1CE4 = scaleY;
    D_800FAA5C_B1CDC = 1;
}

#if 0
void camSetFOV(f32 arg0, s32 arg1) {
    f32 temp;
    s32 camNo;
    s32 videoMode;
    s32 type;
    u8 idx;

    camNo = D_800FA634_B18B4;
    *(&D_800FA4FC_B177C + camNo * 0x13) = arg0;
    if (D_800A3174_A3D74[camNo] != 0) {
        temp = D_800A317C_A3D7C[camNo] - 1.0f;
        arg0 *= 1.0f / (0.25f * temp + 1.0f);
    }
    if (arg0 > 0.0f) {
        if (arg0 < 90.0f) {
            if ((arg1 != 0) || (camNo = (s32)&D_800FB078_B22F8, arg0 != D_800FB078_B22F8)) {
                D_800FB078_B22F8 = arg0;
                guPerspectiveF(D_800FAA70_B1CF0, &D_800FAA68_B1CE8, D_800FB078_B22F8, 1.3333334f, 10.0f, 15000.0f, 1.0f);
                D_800A3168_A3D68 = D_800FAA70_B1CF0[0][0] / 1.817394f;
                videoMode = viGetVideoMode();
                if (D_800FAA5C_B1CDC != 0) {
                    D_800FAA70_B1CF0[0][0] = D_800FAA70_B1CF0[0][0] * D_800FAA60_B1CE0;
                    D_800FAA5C_B1CDC = 0;
                    D_800FAA70_B1CF0[1][1] = D_800FAA70_B1CF0[1][1] * D_800FAA64_B1CE4;
                } else if (videoMode & 1) {
                    type = levelGetType();
                    if ((type == 1) || (type == 2)) {
                        D_800FAA70_B1CF0[1][1] *= 1.33f;
                    } else {
                        D_800FAA70_B1CF0[0][0] *= 0.75f;
                    }
                } else if ((D_800FA630_B18B0 == 1) && (frontGet2PlayerSplit() != 0)) {
                    D_800FAA70_B1CF0[0][0] *= 0.75f;
                    D_800FAA70_B1CF0[1][1] *= 0.75f;
                }
                idx = (D_800A316C_A3D6C + 1) & 0xF;
                D_800A316C_A3D6C = idx;
                mathMtxF2L(D_800FAA70_B1CF0, &D_800FAB38_B1DB8[idx & 0xFF]);
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetFOV.s")
#endif

f32 camDistance(f32 x, f32 y, f32 z) {
    Camera *cam;
    f32 dx;
    f32 dy;
    f32 dz;

    cam = &cameraActorArray[D_800FA634_B18B4];
    dx = x - cam->trans.x_position;
    dy = y - cam->trans.y_position;
    dz = z - cam->trans.z_position;
    return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/func_8003FAEC_406EC.s")

s32 camGetMode(void) {
    return D_800FA630_B18B0;
}

s32 camSetMode(s32 mode) {
    if (mode < 0 || mode >= 4) {
        mode = 0;
    }
    D_800FA630_B18B0 = mode;
    if (D_800FA630_B18B0 < D_800FA634_B18B4) {
        D_800FA634_B18B4 = 0;
    }
    return mode + 1;
}

s32 camGetNo(void) {
    return D_800FA634_B18B4;
}

void camSetNo(s32 camNo) {
    if ((camNo < 0) || (camNo >= 4)) {
        camNo = 0;
    }
    D_800FA634_B18B4 = camNo;
}

u8 camGetWaterLine(s32 arg0) {
    return D_800A3178_A3D78[arg0];
}

void camSetWaterLine(s32 camNo, s32 waterLine) {
    if ((camNo >= 0) && (camNo < 4)) {
        D_800A3178_A3D78[camNo] = waterLine;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camUserViewTick.s")

void camEnableUserView(s32 camNo, s32 immediate) {
    ScreenViewport *vp;

    if (immediate != 0) {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags = (s32) (vp->flags | 1);
    } else {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags = (s32) (vp->flags | 2);
    }
    vp->flags = (s32) (vp->flags & ~4);
}

void camDisableUserView(s32 camNo, s32 immediate) {
    ScreenViewport *vp;

    if (immediate != 0) {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags = (s32) (vp->flags & ~1);
    } else {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags = (s32) (vp->flags | 4);
    }
    vp->flags = (s32) (vp->flags & ~2);
}

s32 camIsUserView(s32 camNo) {
    return D_800A2ED4_A3AD4[camNo].flags & 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetUserView.s")

void camSetUserViewSpecial(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ScreenViewport* vp;

    if (arg1 != 0x8000) {
        vp = &D_800A2ED4_A3AD4[arg0];
        vp->posX = arg1;
        vp->flags = (s32) (vp->flags | 8);
    } else {
        vp = &D_800A2ED4_A3AD4[arg0];
        vp->flags = (s32) (vp->flags & ~8);
    }
    if (arg2 != 0x8000) {
        vp->posY = arg2;
        vp->flags = (s32) (vp->flags | 0x10);
    } else {
        vp->flags = (s32) (vp->flags & ~0x10);
    }
    if (arg3 != 0x8000) {
        vp->width = arg3;
        vp->flags = (s32) (vp->flags | 0x20);
    } else {
        vp->flags = (s32) (vp->flags & ~0x20);
    }
    if (arg4 != 0x8000) {
        vp->height = arg4;
        vp->flags = (s32) (vp->flags | 0x40);
        return;
    }
    vp->flags = (s32) (vp->flags & ~0x40);
}

s32 camGetVisibleUserView(s32 camNo, s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    ScreenViewport *vp = &D_800A2ED4_A3AD4[camNo];
    s32 y;

    *x1 = vp->scissorX1;
    *x2 = vp->scissorX2;
    *y1 = vp->scissorY1;
    *y2 = vp->scissorY2;
    if ((*x1 | *x2 | *y1 | *y2) == 0) {
        return 0;
    }
    return 1;
}

void camGetUserView(s32 camNo, s32* x1, s32* y1, s32* x2, s32* y2) {
    ScreenViewport *vp = &D_800A2ED4_A3AD4[camNo];
    *x1 = vp->x1;
    *y1 = vp->y1;
    *x2 = vp->x2;
    *y2 = vp->y2;
}

void camGetWindowLimits(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, u32 *arg4, u32 *arg5) {
    s32 videoMode;
    u32 halfW;
    u32 halfH;

    if (D_800A2ED4_A3AD4[arg1].flags & 1) {
        *arg2 = D_800A2ED4_A3AD4[arg1].scissorX1;
        *arg3 = D_800A2ED4_A3AD4[arg1].scissorY1;
        *arg4 = D_800A2ED4_A3AD4[arg1].scissorX2;
        *arg5 = D_800A2ED4_A3AD4[arg1].scissorY2;
        return;
    }

    viGetCurrentSize(arg4, arg5);
    videoMode = viGetVideoMode();
    halfW = *arg4 / 2;
    halfH = *arg5 / 2;
    *arg2 = 0;
    *arg3 = 0;

    switch (arg0) {
    case 1:
        if ((videoMode & 1) || frontGet2PlayerSplit()) {
            if (arg1 == 0) {
                *arg4 = halfW;
            } else {
                *arg2 = halfW;
            }
        } else {
            if (arg1 == 0) {
                *arg5 = halfH;
            } else {
                *arg3 = halfH;
            }
        }
        break;
    case 2: /* fallthrough */
    case 3:
        if (arg1 & 1) {
            *arg2 = halfW;
        } else {
            *arg4 = halfW;
        }
        if (arg1 & 2) {
            *arg3 = halfH;
        } else {
            *arg5 = halfH;
        }
        break;
    case 0:
        break;
    }
}

void camSetView(Gfx **dlist, Mtx **mtx) {
    f32 new_var;
    u32 halfWidth;
    struct {
        u32 lry;
        u32 lrx;
        u32 uly;
        u32 ulx;
    } win;
    u32 halfHeight;
    s32 videoMode;

    camGetWindowLimits(D_800FA630_B18B0, D_800FA634_B18B4, &win.ulx, &win.uly, &win.lrx, &win.lry);

    videoMode = viGetVideoMode();
    if ((videoMode == 2) || (videoMode == 3)) {
        halfWidth = 224;
        halfHeight = 168;
    } else {
        halfWidth = 160;
        halfHeight = 120;
    }

    if (D_800FA630_B18B0 >= 2) {
        halfWidth >>= 1;
        halfHeight >>= 1;
    }

    if (D_800A3174_A3D74[D_800FA634_B18B4] != 0) {
        new_var = D_800A317C_A3D7C[D_800FA634_B18B4] - 1.0f;
        halfWidth = (u32) ((f32) halfWidth * (0.25f * new_var + 1.0f));
        halfHeight = (u32) ((f32) halfHeight * (0.25f * new_var + 1.0f));
        gSPClipRatio((*dlist)++, FRUSTRATIO_1);
    }

    gDPSetScissor((*dlist)++, 0, win.ulx, win.uly, win.lrx, win.lry);
    camSetViewport(dlist, halfWidth, halfHeight, (win.lrx + win.ulx) >> 1, (win.lry + win.uly) >> 1);

    if (mtx != NULL) {
        camSetProjMtx(dlist, mtx);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetScissor.s")

void camGetPlayerProjMtx(s32 arg0, Matrix arg1) {
    mathMtxCatF(D_800FAF78_B21F8, D_800FAA70_B1CF0, arg1);
}

void camSetProjMtx(Gfx **dlist, Mtx **mtx) {
    Camera *camera = &cameraActorArray[D_800FA634_B18B4];

    if ((D_800FB078_B22F8 != camera->fov) || (D_800A3174_A3D74[D_800FA634_B18B4] != 0)) {
        camSetFOV(camera->fov, 0);
    }

    if (dlist != NULL) {
        gSPPerspNormalize((*dlist)++, D_800FAA68_B1CE8);
    }

    D_800FA638_B18B8.rotation.y_rotation = camera->trans.rotation.y_rotation + 0x8000;
    D_800FA638_B18B8.rotation.x_rotation = camera->trans.rotation.x_rotation + camera->unk4A;
    D_800FA638_B18B8.rotation.z_rotation = camera->trans.rotation.z_rotation;
    D_800FA638_B18B8.x_position = -camera->trans.x_position;
    D_800FA638_B18B8.y_position = -camera->trans.y_position;
    D_800FA638_B18B8.z_position = -camera->trans.z_position;
    if (D_800FAA58_B5298) {
        D_800FA638_B18B8.x_position -= camera->shake.x;
        D_800FA638_B18B8.y_position -= camera->shake.y;
        D_800FA638_B18B8.z_position -= camera->shake.z;
    }
    matrix_XYZ_YPR(&D_800FA638_B18B8, D_800FAF78_B21F8);
    mathMtxCatF(D_800FAF78_B21F8, D_800FAA70_B1CF0, D_800FAAF0_B1D70);

    if (dlist != NULL) {
        mathMtxF2L(D_800FAAF0_B1D70, *mtx);
        gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        (*mtx)++;
    }

    D_800FA638_B18B8.rotation.y_rotation = -0x8000 - camera->trans.rotation.y_rotation;
    D_800FA638_B18B8.rotation.x_rotation = -(camera->trans.rotation.x_rotation + camera->unk4A);
    D_800FA638_B18B8.rotation.z_rotation = -camera->trans.rotation.z_rotation;
    D_800FA638_B18B8.x_position = camera->trans.x_position;
    D_800FA638_B18B8.y_position = camera->trans.y_position;
    D_800FA638_B18B8.z_position = camera->trans.z_position;
    if (D_800FAA58_B5298) {
        D_800FA638_B18B8.x_position += camera->shake.x;
        D_800FA638_B18B8.y_position += camera->shake.y;
        D_800FA638_B18B8.z_position += camera->shake.z;
    }
    matrix_RPY_XYZ(&D_800FA638_B18B8, D_800FAFB8_B2238);
    mathMtxF2L(D_800FAFB8_B2238, &D_800FAF38_B21B8);
}

void camOrthoYAspect(f32 arg0) {
    D_800A313C_A3D3C = arg0;
}

void camStandardOrtho(Gfx **arg0, Mtx **arg1) {
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);

    D_800A2FB8_A3BB8[D_800FA634_B18B4 + 5].vp.vscale[0] = width * 2;
    D_800A2FB8_A3BB8[D_800FA634_B18B4 + 5].vp.vscale[1] = width * 2;
    D_800A2FB8_A3BB8[D_800FA634_B18B4 + 5].vp.vtrans[0] = width * 2;
    D_800A2FB8_A3BB8[D_800FA634_B18B4 + 5].vp.vtrans[1] = height * 2;

    gSPViewport((*arg0)++, OS_PHYSICAL_TO_K0(&D_800A2FB8_A3BB8[D_800FA634_B18B4 + 5]));

    camCopyOrthoMatrix(D_800FAAF0_B1D70);
    mathMtxF2L(D_800FAAF0_B1D70, *arg1);

    gSPMatrix((*arg0)++, OS_PHYSICAL_TO_K0(*arg1), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    (*arg1)++;
}

void camStandardPersp(Gfx **arg0, Mtx **arg1) {
    gSPPerspNormalize((*arg0)++, D_800FAA68_B1CE8);

    matrix_XYZ_YPR(&D_800A30F8_A3CF8, D_800FAFF8_B2278);
    mathMtxCatF(D_800FAFF8_B2278, D_800FAA70_B1CF0, D_800FAAF0_B1D70);
    mathMtxF2L(D_800FAAF0_B1D70, *arg1);

    gSPMatrix((*arg0)++, OS_PHYSICAL_TO_K0(*arg1), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    (*arg1)++;
}

void camSetViewport(Gfx **dlist, s32 halfWidth, s32 halfHeight, s32 centerX, s32 centerY) {
    s32 camNo;
    Vp *vp;

    camNo = D_800FA634_B18B4;
    if (!(D_800A2ED4_A3AD4[camNo].flags & 1)) {
        vp = &D_800A2FB8_A3BB8[camNo];
        vp->vp.vtrans[0] = centerX * 4;
        vp->vp.vtrans[1] = centerY * 4;
        vp->vp.vscale[0] = halfWidth * 4;
        vp->vp.vscale[1] = halfHeight * 4;
    } else {
        camNo += D_800A2FA4_A3BA4 * 5;
        camNo += 10;
        vp = &D_800A2FB8_A3BB8[camNo];
    }
    gSPViewport((*dlist)++, OS_K0_TO_PHYSICAL(vp));
}

void camResetView(Gfx **arg0) {
    u32 height;
    u32 width;

    D_800FA634_B18B4 = 4;
    viGetCurrentSize((s32 *) &width, (s32 *) &height);
    if (!(D_800A2ED4_A3AD4[D_800FA634_B18B4].flags & 1)) {
        gDPSetScissor((*arg0)++, 0, 0, 0, width - 1, height - 1);
        camSetViewport(arg0, width >> 1, height >> 1, width >> 1, height >> 1);
    } else {
        camSetScissor(arg0);
        camSetViewport(arg0, 0, 0, 0, 0);
    }
    D_800FA634_B18B4 = 0;
}

void camOffsetZero(Gfx **dlist, Mtx **mtx) {
}

void camDoSprite(Gfx **dlist, Mtx **mtx, Vertex **vtx, ObjectSegment *segment,
                 Sprite *sprite, s32 flags, u8 arg6) {
    s32 rot;
    f32 scale;
    f32 aspect;
    register Vertex *v;

    v = *vtx;
    v->x = segment->trans.x_position;
    v->y = segment->trans.y_position;
    v->z = segment->trans.z_position;
    v->r = 255;
    v->g = 255;
    v->b = 255;
    v->a = 255;
    gSPVertexJFG((*dlist)++, OS_PHYSICAL_TO_K0(*vtx), 1, 0);
    (*vtx)++;

    rot = *(s16 *)((u8 *)&D_800FA4D4_B1754 + D_800FA634_B18B4 * 0x4C)
        + segment->trans.rotation.z_rotation;
    scale = segment->trans.scale * D_800A3168_A3D68;
    aspect = aspectRatioFloat;
    if (viGetVideoMode() & 1) {
        scale *= 0.75f;
        aspect *= 1.3334f;
    }
    mathRSMtx(rot, scale, aspect, D_800FAAB0_B1D30);

    if (flags << 2 < 0) {
        matrixID(D_800FAFF8_B2278);
        D_800FAFF8_B2278[1][1] = -1.0f;
        mathMtxCatF(D_800FAAB0_B1D30, D_800FAFF8_B2278, D_800FAAB0_B1D30);
    }

    mathMtxF2L(D_800FAAB0_B1D30, *mtx);
    D_800FAB30_B1DB0 = *mtx;
    gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_2);
    (*mtx)++;
    gDkrEnableBillboard((*dlist)++);

    flags &= ~1;
    if (flags & 4) {
        flags |= 1;
    }
    sprDPset(dlist, sprite, flags & 0xF, *(s32 *)&segment->unk28, arg6);

    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
    gDkrDisableBillboard((*dlist)++);
}

void camDoSpriteDirect(Gfx **dlist, Mtx **mtx, Vertex **vtx, Sprite *sprite,
                       s16 x, s16 y, s16 z, s16 angle, f32 scale, s32 arg9,
                       s32 flags, u8 arg11) {
    s32 rot;
    f32 aspect;
    register Vertex *v;

    v = *vtx;
    v->x = x;
    v->y = y;
    v->z = z;
    v->r = 255;
    v->g = 255;
    v->b = 255;
    v->a = 255;
    gSPVertexJFG((*dlist)++, OS_PHYSICAL_TO_K0(*vtx), 1, 0);
    (*vtx)++;

    rot = *(s16 *)((u8 *)&D_800FA4D4_B1754 + D_800FA634_B18B4 * 0x4C) + angle;
    scale *= D_800A3168_A3D68;
    aspect = aspectRatioFloat;
    if (viGetVideoMode() & 1) {
        scale *= 0.75f;
        aspect *= 1.3334f;
    }
    mathRSMtx(rot, scale, aspect, D_800FAAB0_B1D30);

    if (flags << 2 < 0) {
        matrixID(D_800FAFF8_B2278);
        D_800FAFF8_B2278[1][1] = -1.0f;
        mathMtxCatF(D_800FAAB0_B1D30, D_800FAFF8_B2278, D_800FAAB0_B1D30);
    }

    mathMtxF2L(D_800FAAB0_B1D30, *mtx);
    D_800FAB30_B1DB0 = *mtx;
    gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_2);
    (*mtx)++;
    gDkrEnableBillboard((*dlist)++);

    flags &= ~1;
    if (flags & 4) {
        flags |= 1;
    }
    sprDPset(dlist, sprite, flags & 0xF, arg9, arg11);

    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
    gDkrDisableBillboard((*dlist)++);
}

void camDo2DSprite(Gfx **dlist, Mtx **mtx, Vertex **vtx, ObjectSegment *segment,
                   Sprite *sprite, s32 flags, u8 arg6) {
    Vertex *v;
    f32 scale;
    struct {
        s32 pad[2];
        Matrix scaleMtxF;
        Matrix aspectMtxF;
    } mf;

    if (sprite == NULL) {
        return;
    }

    v = *vtx;
    v->x = segment->trans.x_position;
    v->y = segment->trans.y_position;
    v->z = segment->trans.z_position;
    v->r = 255;
    v->g = 255;
    v->b = 255;
    v->a = 255;
    gSPVertexJFG((*dlist)++, OS_PHYSICAL_TO_K0(*vtx), 1, 0);
    (*vtx)++;

    D_800FA638_B18B8.rotation.y_rotation = -segment->trans.rotation.y_rotation;
    D_800FA638_B18B8.rotation.x_rotation = -segment->trans.rotation.x_rotation;
    D_800FA638_B18B8.rotation.z_rotation =
        *(s16 *)((u8 *)&D_800FA4D4_B1754 + D_800FA634_B18B4 * 0x4C)
        + segment->trans.rotation.z_rotation;
    D_800FA638_B18B8.x_position = 0.0f;
    D_800FA638_B18B8.y_position = 0.0f;
    D_800FA638_B18B8.z_position = 0.0f;

    if (D_800FA650_B18D0) {
        scale = segment->trans.scale;
        mathScaleMtx(mf.scaleMtxF, scale, scale, 1.0f);
        mathRSMtx(0, 1.0f, aspectRatioFloat, mf.aspectMtxF);
        mathMtxCatF(mf.aspectMtxF, mf.scaleMtxF, D_800FAFF8_B2278);
    } else {
        scale = segment->trans.scale;
        mathScaleMtx(D_800FAFF8_B2278, scale, scale, 1.0f);
    }

    matrix_XYZ_YPR(&D_800FA638_B18B8, mf.aspectMtxF);
    mathMtxCatF(D_800FAFF8_B2278, mf.aspectMtxF, D_800FAAB0_B1D30);
    mathMtxF2L(D_800FAAB0_B1D30, *mtx);
    D_800FAB30_B1DB0 = *mtx;
    gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_2);
    (*mtx)++;
    gDkrEnableBillboard((*dlist)++);
    sprDPset(dlist, sprite, flags, *(s32 *)&segment->unk28, arg6);
    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
    gDkrDisableBillboard((*dlist)++);
}

void camPushFloatModelMtx(Gfx **dlist, Mtx **mtx, Matrix mf) {
    s32 i;
    s32 j;

    i = 0;
    do {
        j = 0;
        do {
            D_800FAAB0_B1D30[i][j] = mf[i][j];
            j++;
        } while (j < 4);
        i++;
    } while (i < 4);

    mathMtxCatF(mf, D_800FAAF0_B1D70, D_800FB038_B22B8);
    mathMtxF2L(D_800FB038_B22B8, *mtx);
    D_800FAB30_B1DB0 = *mtx;
    gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_1);
    (*mtx)++;
}

void camPushMuzzleMtx(Gfx **arg0, Mtx **arg1, Vec3f *arg2, Matrix arg3) {
    f32 oz;
    f32 oy;
    f32 ox;
    Matrix temp;
    s32 i;
    s32 j;

    D_800FAAB0_B1D30[3][0] = 0.0f;
    D_800FAAB0_B1D30[3][1] = 0.0f;
    D_800FAAB0_B1D30[3][2] = 0.0f;
    mathMtxCatF(arg3, D_800FAAB0_B1D30, temp);
    i = 0;
    do {
        j = 0;
        do {
            D_800FAAB0_B1D30[i][j] = temp[i][j];
            j++;
        } while (j < 4);
        i++;
    } while (i < 4);
    mathMtxXFMF(D_800FAAB0_B1D30, arg2->x, arg2->y, arg2->z, &ox, &oy, &oz);
    D_800FAAB0_B1D30[3][0] = arg2->x;
    D_800FAAB0_B1D30[3][1] = arg2->y;
    D_800FAAB0_B1D30[3][2] = arg2->z;
    mathMtxCatF(D_800FAAB0_B1D30, D_800FAAF0_B1D70, D_800FB038_B22B8);
    mathMtxF2L(D_800FB038_B22B8, *arg1);
    D_800FAB30_B1DB0 = *arg1;
    {
        Gfx *g = (*arg0)++;
        g->words.w0 = 0x01010040;
        g->words.w1 = (u32)((char *)*arg1 + 0x80000000);
    }
    (*arg1)++;
}

void camScaleModelMtx(Gfx **dlist, Mtx **mtx, f32 scale) {
    if (scale != 1.0f) {
        matrixScale(scale, scale, scale, D_800FAAB0_B1D30);
        mathMtxCatF(D_800FAAB0_B1D30, D_800FAAF0_B1D70, D_800FB038_B22B8);
        mathMtxF2L(D_800FB038_B22B8, *mtx);
        D_800FAB30_B1DB0 = *mtx;
        {
            Gfx *g = (*dlist)++;
            g->words.w0 = 0x01010040;
            g->words.w1 = (u32)((char *)*mtx + 0x80000000);
        }
        (*mtx)++;
    }
}

void camPushModelMtx(Gfx **dList, Mtx **mtx, ObjectTransform *trans, f32 scale, f32 scaleY) {
    matrix_SCL_RPY_XYZ(trans, D_800FAAB0_B1D30);
    if (scaleY != 0.0f) {
        mathTransY(D_800FAAB0_B1D30, scaleY);
    }
    if (scale != 1.0f) {
        mathSquashY(D_800FAAB0_B1D30, scale);
    }
    mathMtxCatF(D_800FAAB0_B1D30, D_800FAAF0_B1D70, D_800FB038_B22B8);
    mathMtxF2L(D_800FB038_B22B8, *mtx);
    D_800FAB30_B1DB0 = *mtx;
    gSPMatrix((*dList)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_1);
    (*mtx)++;
}

void camRestoreModelMtx(Gfx **dlist) {
    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
}

void camPopModelMtx(Gfx **dlist) {
    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
}

Camera *camGetPtr(void) {
    return &cameraActorArray[D_800FA634_B18B4];
}

Camera *camGetListPtr(void) {
    return cameraActorArray;
}

Matrix *camGetInvProjMtx(void) {
    return &D_800FAFB8_B2238;
}

Mtx *camGetProjOrgMtx(void) {
    return &D_800FAB38_B1DB8[D_800A316C_A3D6C];
}

Matrix *camGetProjectionMtx(void) {
    return &D_800FAAF0_B1D70;
}

Matrix *camGetRotationMtx(void) {
    return &D_800FAF78_B21F8;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camProjectPoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camReversePoint.s")

f32 camGetProjZ(f32 x, f32 y, f32 z) {
    f32 temp;
    f32 out;
    
    temp = D_800FAF78_B21F8[3][2];

    out = temp + (z * D_800FAF78_B21F8[2][2] + (y * D_800FAF78_B21F8[1][2] + D_800FAF78_B21F8[0][2] * (temp = x)));
    temp = y * D_800FAF78_B21F8[1][2] + x * D_800FAF78_B21F8[0][2];
    temp = z * D_800FAF78_B21F8[2][2] + temp;
    out = D_800FAF78_B21F8[3][2] + temp;
    
    return out;
}

/* Needs camera .data migrated before this will match. */
#if 0
extern f32 D_800A3128_A3D28[16];

void camCopyOrthoMatrix(Matrix dest) {
    s32 i;
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);
    for (i = 0; i < 15; i++) {
        ((f32*)dest)[i] = D_800A3128_A3D28[i];
    }
    dest[3][3] = (u32)width >> 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/camera/camCopyOrthoMatrix.s")
#endif

void camStartShake(s32 camNo, f32 attack, f32 sustain, f32 decay, s32 magnitude) {
    CameraShake *shake;

    if ((camNo >= 0) && (camNo < 4)) {
        shake = &D_800FA600_B1880[camNo];
        shake->attackEnd = (s16) (s32) (attack * 60.0f);
        shake->sustainEnd = (s16) (shake->attackEnd + (s32) (sustain * 60.0f));
        shake->totalEnd = (s16) (shake->sustainEnd + (s32) (decay * 60.0f));
        shake->timer = 0;
        shake->magnitude = magnitude;
    }
}

void camStopShakes(void) {
    s32 i;
    Camera *cam;
    CameraShake *shake;

    D_800FAA58_B5298 = 0;
    cam = cameraActorArray;
    shake = D_800FA600_B1880;

    /* IDO: line-join for regalloc */
    // clang-format off
    for (i = 4; i--; cam++, shake++) { \
        cam->shake.x = 0.0f; \
        cam->shake.y = 0.0f; \
        cam->shake.z = 0.0f; \
        shake->magnitude = 0; \
    }
    // clang-format on
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camScreenShake.s")

void camSetZoom(s32 camNo, f32 zoom) {
    if ((camNo >= 0) && (camNo < 4)) {
        D_800A3174_A3D74[camNo] = 1;
        D_800A317C_A3D7C[camNo] = zoom;
    }
}

void camConvertMatrixList(Matrix *mtx, s32 count) {
    s32 temp = D_800A2ED0_A3AD0;
    ConvListEntry *entry = &D_800FA658_B18D8[temp];

    entry->mtx = mtx;
    D_800A2ED0_A3AD0 = temp + 1;
    entry->count = count;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camTick.s")
