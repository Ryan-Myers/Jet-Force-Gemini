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

// .data

#ifdef VERSION_kiosk
u8 D_800A3720 = 0;
#endif

s32 D_800A2ED0_A3AD0 = 0;

#define DEFAULT_SCREEN_VIEWPORT \
    { 0,                        \
      0,                        \
      SCREEN_WIDTH,             \
      SCREEN_HEIGHT,            \
      SCREEN_WIDTH / 2,         \
      SCREEN_HEIGHT / 2,        \
      SCREEN_WIDTH,             \
      SCREEN_HEIGHT,            \
      0,                        \
      0,                        \
      SCREEN_WIDTH - 1,         \
      SCREEN_HEIGHT - 1,        \
      0 }
ScreenViewport D_800A2ED4_A3AD4[4] = { DEFAULT_SCREEN_VIEWPORT, DEFAULT_SCREEN_VIEWPORT, DEFAULT_SCREEN_VIEWPORT,
                                       DEFAULT_SCREEN_VIEWPORT };
#undef DEFAULT_SCREEN_VIEWPORT

s32 D_800A2FA4_A3BA4 = 0;
UNUSED s32 D_800A2FA8_A3BA8 = 0;
UNUSED s32 D_800A2FAC_A3BAC = 0x0000FFFF;
UNUSED u32 D_800A2FB0_A3BB0 = 0xFFFF0000;

#ifndef VERSION_kiosk
UNUSED s32 D_800A2FB4_A3BB4 = 0;
#endif

#define CAM_EMPTY_VP                               \
    {                                              \
        { { 0, 0, 0x1FF, 0 }, { 0, 0, 0x1FF, 0 } } \
    }
Vp D_800A2FB8_A3BB8[20] = {
    CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP,
    CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP,
    CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP, CAM_EMPTY_VP,
};
#undef CAM_EMPTY_VP

ObjectTransform D_800A30F8_A3CF8 = {
    { { 0, 0, 0 } }, /* rotation: y, x, z */
    0,               /* flags */
    1.0f,            /* scale */
    0.0f,            /* x_position */
    0.0f,            /* y_position */
    -281.0f,         /* z_position */
};

UNUSED ObjectTransform D_800A3110_A3D10 = {
    { { 0, 0, 0 } }, 0, 1.0f, 0.0f, 0.0f, 0.0f,
};

Matrix D_800A3128_A3D28 = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, SCREEN_WIDTH / 2 },
};

f32 D_800A3168_A3D68 = 1.0f;
u8 D_800A316C_A3D6C = 0;
u8 D_800A3170_A3D70[4] = { 0, 0, 0, 0 };
u8 D_800A3174_A3D74[4] = { 0, 0, 0, 0 };
u8 D_800A3178_A3D78[4] = { 0, 0, 0, 0 };
f32 D_800A317C_A3D7C[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
UNUSED s32 D_800A318C_A3D8C = 0;

// .bss
Camera cameraActorArray[4];
CameraShake D_800FA600_B1880[4];
s32 D_800FA630_B18B0; /* mode */
s32 D_800FA634_B18B4; /* active camera */
ObjectTransform D_800FA638_B18B8;
s8 D_800FA650_B18D0;
UNUSED s32 pad_b18d4;
ConvListEntry D_800FA658_B18D8[128];
s32 D_800FAA58_B5298; /* shake enable */
s32 D_800FAA5C_B1CDC;
f32 D_800FAA60_B1CE0;
f32 D_800FAA64_B1CE4;
u16 D_800FAA68_B1CE8;
UNUSED s32 pad_aa6c;
Matrix D_800FAA70_B1CF0; /* perspective */
Matrix D_800FAAB0_B1D30; /* current model */
Matrix D_800FAAF0_B1D70; /* view * persp */
Mtx *D_800FAB30_B1DB0;
Mtx D_800FAB38_B1DB8[16];
Mtx D_800FAF38_B21B8;
Matrix D_800FAF78_B21F8; /* view (XYZ_YPR) */
Matrix D_800FAFB8_B2238; /* inverse */
Matrix D_800FAFF8_B2278;
Matrix D_800FB038_B22B8;
f32 D_800FB078_B22F8;
UNUSED s32 pad_b22fc;

extern f32 aspectRatioFloat;

extern void conv_mult_matrix(Matrix mf, ConvListEntry *list);
extern s32 frontGet2PlayerSplit(void);
extern u8 levelGetType(void);
void sprDPset(Gfx **dlist, Sprite *sprite, s32 flags, s32 arg3, u8 arg4);

void camCopyOrthoMatrix(Matrix dest);
void camSetFOV(f32, s32);
void camSetProjMtx(Gfx **dlist, Mtx **mtx);
void camSetViewport(Gfx **dlist, s32 halfWidth, s32 halfHeight, s32 centerX, s32 centerY);
void camStopShakes(void);
void func_8003FAEC_406EC(s32 x, s32 y, s32 z, s32 zRot, s32 xRot, s32 yRot);

void camInit(s32 arg0) {
    s32 i;
#ifdef VERSION_kiosk
    vu32 *piStatus;
    u32 status;
    vu32 *cartWord;
#endif

    D_800FA634_B18B4 = 0;
    D_800FA630_B18B0 = 0;
    D_800FA650_B18D0 = 0;

    for (i = 0; i < 4; i++) {
        D_800FA634_B18B4 = i;
        func_8003FAEC_406EC(200, 200, 200, 0, 0, 180);
        cameraActorArray[D_800FA634_B18B4].fov = CAMERA_DEFAULT_FOV;
    }

    camStopShakes();

#ifdef VERSION_kiosk
    {
        piStatus = (vu32 *) PHYS_TO_K1(PI_STATUS_REG);
        if (arg0) {
            D_800A2ED0_A3AD0 = 0;
        }
        D_800A3720 = 0;
        status = *piStatus;
        cartWord = (vu32 *) 0xB0000000;
        while (status & 3) {
            status = *piStatus;
        }
        if ((cartWord[0x15E] & 0xFFFF) != 0x8965) {
            D_800A3720 = 1;
        }
    }
#else
    if (arg0) {
        D_800A2ED0_A3AD0 = 0;
    }
#endif

    D_800FB078_B22F8 = CAMERA_DEFAULT_FOV;
    guPerspectiveF(D_800FAA70_B1CF0, &D_800FAA68_B1CE8, D_800FB078_B22F8, CAMERA_ASPECT, CAMERA_NEAR, CAMERA_FAR,
                   CAMERA_SCALE);
    D_800A316C_A3D6C = (D_800A316C_A3D6C + 1) & 0xF;
    mathMtxF2L(D_800FAA70_B1CF0, &D_800FAB38_B1DB8[D_800A316C_A3D6C]);
}

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

void camSetFOV(f32 fov, s32 force) {
    f32 temp;
    s32 camNo;
    s32 videoMode;
    s32 type;
    u8 idx;

    camNo = D_800FA634_B18B4;
    cameraActorArray[camNo].fov = fov;
    if (D_800A3174_A3D74[camNo] != 0) {
        temp = D_800A317C_A3D7C[camNo] - 1.0f;
        fov *= 1.0f / ((0.25f * temp) + 1.0f);
    }
    if (fov > CAMERA_MIN_FOV && CAMERA_MAX_FOV > fov) {
        if ((force != 0) || (fov != D_800FB078_B22F8)) {
            D_800FB078_B22F8 = fov;
            guPerspectiveF(D_800FAA70_B1CF0, &D_800FAA68_B1CE8, D_800FB078_B22F8, CAMERA_ASPECT, CAMERA_NEAR,
                           CAMERA_FAR, 1 /*.0f*/);
            D_800A3168_A3D68 = D_800FAA70_B1CF0[0][0] / 1.817394f;
            videoMode = viGetVideoMode();
            if (D_800FAA5C_B1CDC != 0) {
                D_800FAA70_B1CF0[0][0] = D_800FAA70_B1CF0[0][0] * D_800FAA60_B1CE0;
                D_800FAA70_B1CF0[1][1] *= D_800FAA64_B1CE4;
                D_800FAA5C_B1CDC = 0;
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
            D_800A316C_A3D6C = (D_800A316C_A3D6C + 1) & 0xF;
            mathMtxF2L(D_800FAA70_B1CF0, &D_800FAB38_B1DB8[D_800A316C_A3D6C]);
        }
    }
}

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

void func_8003FAEC_406EC(s32 x, s32 y, s32 z, s32 zRot, s32 xRot, s32 yRot) {
    Camera *cam = &cameraActorArray[D_800FA634_B18B4];
    f32 fx;
    f32 fy;
    f32 fz;
    f32 f24;
    f32 f28;

    cam->trans.rotation.z_rotation = zRot * 0xB6;
    cam->trans.rotation.x_rotation = xRot * 0xB6;
    fx = x;
    fy = y;
    fz = z;
    f24 = 128.0f;
    f28 = 32.0f;
    cam->trans.rotation.y_rotation = yRot * 0xB6;
    cam->unk4A = 0;
    cam->trans.x_position = fx;
    cam->trans.y_position = fy;
    cam->trans.z_position = fz;
    cam->cam_unk_18 = fx;
    cam->boomLength = fy;
    cam->unk20 = fz;
    cam->unk3C = 0;
    cam->shake.x = 0.0f;
    cam->shake.y = 0.0f;
    cam->shake.z = 0.0f;
    cam->unk24 = f24;
    cam->unk28 = f28;
    cam->unk3D = D_800A3170_A3D70[D_800FA634_B18B4];
    cam->unk3E = -1;
    cam->unk40 = 0.0f;
}

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

void camUserViewTick(void) {
    s32 width;
    s32 height;
    s32 port;
    s32 yPos;
    s32 xPos;
    int i;

    D_800A2FA4_A3BA4 = 1 - D_800A2FA4_A3BA4;
    for (i = 0; i < 4; i++) {
        if (D_800A2ED4_A3AD4[i].flags & 4) {
            D_800A2ED4_A3AD4[i].flags &= ~1;
        } else if (D_800A2ED4_A3AD4[i].flags & 2) {
            D_800A2ED4_A3AD4[i].flags |= 1;
        }
        D_800A2ED4_A3AD4[i].flags &= ~(2 | 4);

        if (D_800A2ED4_A3AD4[i].flags & 1) {
            if (!(D_800A2ED4_A3AD4[i].flags & 8)) {
                xPos = (((D_800A2ED4_A3AD4[i].x2 - D_800A2ED4_A3AD4[i].x1) + 1) << 1) + (D_800A2ED4_A3AD4[i].x1 * 4);
            } else {
                xPos = D_800A2ED4_A3AD4[i].posX;
                xPos *= 4;
            }
            if (!(D_800A2ED4_A3AD4[i].flags & 0x10)) {
                yPos = (((D_800A2ED4_A3AD4[i].y2 - D_800A2ED4_A3AD4[i].y1) + 1) << 1) + (D_800A2ED4_A3AD4[i].y1 * 4);
            } else {
                yPos = D_800A2ED4_A3AD4[i].posY;
                yPos *= 4;
            }
            if (!(D_800A2ED4_A3AD4[i].flags & 0x20)) {
                width = D_800A2ED4_A3AD4[i].x2 - D_800A2ED4_A3AD4[i].x1;
                width += 1;
                width *= 2;
            } else {
                width = D_800A2ED4_A3AD4[i].width;
                width *= 2;
            }
            if (!(D_800A2ED4_A3AD4[i].flags & 0x40)) {
                height = (D_800A2ED4_A3AD4[i].y2 - D_800A2ED4_A3AD4[i].y1) + 1;
                height *= 2;
            } else {
                height = D_800A2ED4_A3AD4[i].height;
                height *= 2;
            }
            port = (i + D_800A2FA4_A3BA4 * 5) + 10;
            D_800A2FB8_A3BB8[port].vp.vtrans[0] = xPos;
            D_800A2FB8_A3BB8[port].vp.vtrans[1] = yPos;
            D_800A2FB8_A3BB8[port].vp.vscale[0] = width;
            D_800A2FB8_A3BB8[port].vp.vscale[1] = height;
        }
    }
}

void camEnableUserView(s32 camNo, s32 immediate) {
    ScreenViewport *vp;

    if (immediate != 0) {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags |= 1;
    } else {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags |= 2;
    }
    vp->flags &= ~4;
}

void camDisableUserView(s32 camNo, s32 immediate) {
    ScreenViewport *vp;

    if (immediate != 0) {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags &= ~1;
    } else {
        vp = &D_800A2ED4_A3AD4[camNo];
        vp->flags |= 4;
    }
    vp->flags &= ~2;
}

s32 camIsUserView(s32 camNo) {
    return D_800A2ED4_A3AD4[camNo].flags & 1;
}

void camSetUserView(s32 camNo, s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 temp;
    u32 width;
    u32 height;
    viGetCurrentSize((s32 *) (&width), (s32 *) (&height));
    if (x2 < x1) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y2 < y1) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if ((((x1 >= width) || (x2 < 0)) || (y1 >= height)) || (y2 < 0)) {
        D_800A2ED4_A3AD4[camNo].scissorX1 = 0;
        D_800A2ED4_A3AD4[camNo].scissorY1 = 0;
        D_800A2ED4_A3AD4[camNo].scissorX2 = 0;
        D_800A2ED4_A3AD4[camNo].scissorY2 = 0;
    } else {
        if (x1 < 0) {
            D_800A2ED4_A3AD4[camNo].scissorX1 = 0;
        } else {
            D_800A2ED4_A3AD4[camNo].scissorX1 = x1;
        }
        if (y1 < 0) {
            D_800A2ED4_A3AD4[camNo].scissorY1 = 0;
        } else {
            D_800A2ED4_A3AD4[camNo].scissorY1 = y1;
        }
        if (x2 >= width) {
            D_800A2ED4_A3AD4[camNo].scissorX2 = width - 1;
        } else {
            D_800A2ED4_A3AD4[camNo].scissorX2 = x2;
        }
        if (y2 >= height) {
            D_800A2ED4_A3AD4[camNo].scissorY2 = height - 1;
        } else {
            D_800A2ED4_A3AD4[camNo].scissorY2 = y2;
        }
    }
    D_800A2ED4_A3AD4[camNo].x2 = x2;
    D_800A2ED4_A3AD4[camNo].y1 = y1;
    D_800A2ED4_A3AD4[camNo].x1 = x1;
    D_800A2ED4_A3AD4[camNo].y2 = y2;
}

void camSetUserViewSpecial(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ScreenViewport *vp;

    if (arg1 != 0x8000) {
        vp = &D_800A2ED4_A3AD4[arg0];
        vp->posX = arg1;
        vp->flags |= 8;
    } else {
        vp = &D_800A2ED4_A3AD4[arg0];
        vp->flags &= ~8;
    }
    if (arg2 != 0x8000) {
        vp->posY = arg2;
        vp->flags |= 0x10;
    } else {
        vp->flags &= ~0x10;
    }
    if (arg3 != 0x8000) {
        vp->width = arg3;
        vp->flags |= 0x20;
    } else {
        vp->flags &= ~0x20;
    }
    if (arg4 != 0x8000) {
        vp->height = arg4;
        vp->flags |= 0x40;
        return;
    }
    vp->flags &= ~0x40;
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

void camGetUserView(s32 camNo, s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
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

    gDPSetScissor((*dlist)++, G_SC_NON_INTERLACE, win.ulx, win.uly, win.lrx, win.lry);
    camSetViewport(dlist, halfWidth, halfHeight, (win.lrx + win.ulx) >> 1, (win.lry + win.uly) >> 1);

    if (mtx != NULL) {
        camSetProjMtx(dlist, mtx);
    }
}

void camSetScissor(Gfx **dlist) {
    struct {
        u32 lry;
        u32 lrx;
        u32 uly;
        u32 ulx;
    } win;

    camGetWindowLimits(D_800FA630_B18B0, D_800FA634_B18B4, &win.ulx, &win.uly, &win.lrx, &win.lry);
    gDPSetScissor((*dlist)++, G_SC_NON_INTERLACE, win.ulx, win.uly, win.lrx, win.lry);
}

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
    D_800A3128_A3D28[1][1] = arg0;
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
        gDPSetScissor((*arg0)++, G_SC_NON_INTERLACE, 0, 0, width - 1, height - 1);
        camSetViewport(arg0, width >> 1, height >> 1, width >> 1, height >> 1);
    } else {
        camSetScissor(arg0);
        camSetViewport(arg0, 0, 0, 0, 0);
    }
    D_800FA634_B18B4 = 0;
}

void camOffsetZero(Gfx **dlist, Mtx **mtx) {
}

void camDoSprite(Gfx **dlist, Mtx **mtx, Vertex **vtx, ObjectSegment1 *segment, Sprite *sprite, s32 flags, u8 arg6) {
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

    rot = cameraActorArray[D_800FA634_B18B4].trans.rotation.z_rotation + segment->trans.rotation.z_rotation;
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
    sprDPset(dlist, sprite, flags & 0xF, *(s32 *) &segment->unk28, arg6);

    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
    gDkrDisableBillboard((*dlist)++);
}

void camDoSpriteDirect(Gfx **dlist, Mtx **mtx, Vertex **vtx, Sprite *sprite, s16 x, s16 y, s16 z, s16 angle, f32 scale,
                       s32 arg9, s32 flags, u8 arg11) {
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

    rot = cameraActorArray[D_800FA634_B18B4].trans.rotation.z_rotation + angle;
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

void camDo2DSprite(Gfx **dlist, Mtx **mtx, Vertex **vtx, ObjectSegment *segment, Sprite *sprite, s32 flags, u8 arg6) {
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
        cameraActorArray[D_800FA634_B18B4].trans.rotation.z_rotation + segment->trans.rotation.z_rotation;
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
    sprDPset(dlist, sprite, flags, *(s32 *) &segment->unk28, arg6);
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
    gSPMatrix((*arg0)++, OS_PHYSICAL_TO_K0(*arg1), G_MTX_DKR_INDEX_1);
    (*arg1)++;
}

void camScaleModelMtx(Gfx **dlist, Mtx **mtx, f32 scale) {
    if (scale != 1.0f) {
        matrixScale(scale, scale, scale, D_800FAAB0_B1D30);
        mathMtxCatF(D_800FAAB0_B1D30, D_800FAAF0_B1D70, D_800FB038_B22B8);
        mathMtxF2L(D_800FB038_B22B8, *mtx);
        D_800FAB30_B1DB0 = *mtx;
        gSPMatrix((*dlist)++, OS_PHYSICAL_TO_K0(*mtx), G_MTX_DKR_INDEX_1);
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

s32 camProjectPoint(f32 x, f32 y, f32 z, f32 *outX, f32 *outY, u8 transform) {
    s32 ret;
    f32 scaleX;
    f32 scaleY;
    f32 projX;
    f32 projY;
    f32 clipZ;
    Vp *vp;

    ret = 0;
    if (transform) {
        mathMtxXFMF(D_800FAF78_B21F8, x, y, z, &x, &y, &z);
    }

    projX = D_800FAA70_B1CF0[0][0] * x;
    projY = D_800FAA70_B1CF0[1][1] * y;
    clipZ = -(D_800FAA70_B1CF0[2][3] * z);
#ifdef VERSION_kiosk
    if (clipZ < 0.0f) {
#else
    if (clipZ < -2.0f) {
#endif
        ret = D_800FA634_B18B4;
        vp = &D_800A2FB8_A3BB8[ret];
        if (D_800A2ED4_A3AD4[ret].flags & 1) {
            vp += D_800A2FA4_A3BA4 * 5 + 10;
            ret = 1;
        }
        scaleX = vp->vp.vscale[0] >> 2;
        scaleY = vp->vp.vscale[1] >> 2;
        *outX = (f32) (vp->vp.vtrans[0] >> 2) - (projX * scaleX) / clipZ;
        *outY = (f32) (vp->vp.vtrans[1] >> 2) + (projY * scaleY) / clipZ;
        ret = 1;
    }
    return ret;
}

void camReversePoint(f32 x, f32 y, f32 *outX, f32 *outY, f32 *z, u8 transform) {
    f32 transX;
    f32 scaleY;
    f32 scaleX;
    f32 transY;
    f32 clipZ;
    Vp *vp;

    clipZ = ((*z) * D_800FAA70_B1CF0[2][2]) * D_800FAA70_B1CF0[2][3];
    transX = D_800A2FB8_A3BB8[D_800FA634_B18B4].vp.vtrans[0] >> 2;
    scaleX = D_800A2FB8_A3BB8[D_800FA634_B18B4].vp.vscale[0] >> 2;
    scaleY = D_800A2FB8_A3BB8[D_800FA634_B18B4].vp.vscale[1] >> 2;
    transX = D_800A2FB8_A3BB8[D_800FA634_B18B4].vp.vtrans[0] >> 2;
    transY = D_800A2FB8_A3BB8[D_800FA634_B18B4].vp.vtrans[1] >> 2;
    *outX = ((transX - x) * clipZ) / (D_800FAA70_B1CF0[0][0] * scaleX);
    *outY = ((y - transY) * clipZ) / (D_800FAA70_B1CF0[1][1] * scaleY);

    if (transform) {
        mathMtxXFMF(D_800FAFB8_B2238, *outX, *outY, *z, outX, outY, z);
    }
}

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

void camCopyOrthoMatrix(Matrix dest) {
    s32 i;
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);
    for (i = 0; i < 15; i++) {
        ((f32 *) dest)[i] = ((f32 *) D_800A3128_A3D28)[i];
    }
    dest[3][3] = (u32) width >> 1;
}

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

void camScreenShake(f32 x, f32 y, f32 z, f32 dist, f32 magnitude) {
    f32 dx;
    f32 distance;
    f32 dz;
    f32 dy;
    s32 i;

    for (i = 0; i <= D_800FA630_B18B0; i++) {
        dx = x - cameraActorArray[i].trans.x_position;
        dy = y - cameraActorArray[i].trans.y_position;
        dz = z - cameraActorArray[i].trans.z_position;
        distance = sqrtf(((dx * dx) + (dy * dy)) + (dz * dz));
        if (distance < dist) {
            distance = ((dist - distance) * magnitude) / dist;
            if (cameraActorArray[i].shake.y < distance) {
                cameraActorArray[i].shake.y = distance;
            }
        }
    }
}

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

void camTick(s32 updateRate) {
    CameraShake *shake;
    Camera *cam;
    s32 i;
    s32 timer;
    s32 sustainEnd;
    s32 mag;

    D_800FA658_B18D8[D_800A2ED0_A3AD0].count = -1;
    conv_mult_matrix(D_800FAAF0_B1D70, D_800FA658_B18D8);
    D_800A2ED0_A3AD0 = 0;

    cam = cameraActorArray;
    shake = D_800FA600_B1880;
    D_800FAA58_B5298 = 0;

    for (i = 4; i--; cam++, shake++) {
        D_800A3174_A3D74[i] = 0;
        D_800A3178_A3D78[i] = 1;
        cam->shake.f[0] = 0.0f;
        cam->shake.f[1] = 0.0f;
        cam->shake.f[2] = 0.0f;
        if (shake->magnitude != 0) {
            shake->timer += updateRate;
            if (shake->timer >= shake->totalEnd) {
                shake->magnitude = 0;
            } else {
                D_800FAA58_B5298 = 1;
                timer = shake->timer;
                sustainEnd = shake->sustainEnd;
                mag = shake->magnitude;
                if (sustainEnd < timer) {
                    mag = ((shake->totalEnd - timer) * mag) / (shake->totalEnd - sustainEnd);
                } else if (timer < shake->attackEnd) {
                    mag = (timer * mag) / shake->attackEnd;
                }
                cam->shake.y = mathRnd(0, mag);
            }
        }
    }
}
