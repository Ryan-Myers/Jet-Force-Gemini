#include "camera.h"
#include "common.h"

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

extern u8 D_800A3178_A3D78[];
extern Camera cameraActorArray[];
extern ScreenViewport D_800A2ED4_A3AD4[];
extern CameraShake D_800FA600_B1880[];
extern s32 D_800A2ED0_A3AD0;
extern ConvListEntry D_800FA658_B18D8[];
extern Matrix D_800FAFB8_B2238;
extern s32 D_800FA630_B18B0;
extern s32 D_800FA634_B18B4;
extern s32 D_800FAA58_B5298;
extern f32 D_800FB078_B22F8;
extern s32 D_800FAA5C_B1CDC;
extern f32 D_800FAA60_B1CE0;
extern f32 D_800FAA64_B1CE4;
extern u8 D_800A3174_A3D74[4];
extern f32 D_800A317C_A3D7C[4];

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

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetFOV.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDistance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/func_8003FAEC_406EC.s")

s32 camGetMode(void) {
    return D_800FA630_B18B0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetMode.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camEnableUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDisableUserView.s")

s32 camIsUserView(s32 arg0) {
    return D_800A2ED4_A3AD4[arg0].flags & 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetUserViewSpecial.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetVisibleUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetWindowLimits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetScissor.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetPlayerProjMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetProjMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camOrthoYAspect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camStandardOrtho.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camStandardPersp.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetViewport.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camResetView.s")

void camOffsetZero(Gfx **dlist, Mtx **mtx) {}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDoSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDoSpriteDirect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDo2DSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushFloatModelMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushMuzzleMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camScaleModelMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushModelMtx.s")

void camRestoreModelMtx(Gfx **dlist) {
    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
}

void camPopModelMtx(Gfx **dlist) {
    gSPSelectMatrixDKR((*dlist)++, G_MTX_DKR_INDEX_0);
}

Camera *camGetPtr(void) {
    return (Camera *)((u8 *)cameraActorArray + D_800FA634_B18B4 * 0x4C);
}

Camera *camGetListPtr(void) {
    return cameraActorArray;
}

Matrix *camGetInvProjMtx(void) {
    return &D_800FAFB8_B2238;
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjOrgMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjectionMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetRotationMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camProjectPoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camReversePoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjZ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camCopyOrthoMatrix.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camStartShake.s")

void camStopShakes(void) {
    s32 i;
    Camera* cam;
    CameraShake* shake;

    D_800FAA58_B5298 = 0;
    cam = cameraActorArray;
    shake = D_800FA600_B1880;

    /* IDO: line-join for regalloc */
    for (i = 4; i--; cam++, shake++) { \
        cam->shake.x = 0.0f; \
        cam->shake.y = 0.0f; \
        cam->shake.z = 0.0f; \
        shake->magnitude = 0; \
    } \
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
