#include "common.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_HEIGHT_PAL (SCREEN_HEIGHT + 44)
#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)
#define SCREEN_HEIGHT_HALF (SCREEN_HEIGHT / 2)
#define SCREEN_HEIGHT_HALF_PAL (SCREEN_HEIGHT_HALF + 12)

#define SCREEN_WIDTH_FLOAT ((float) (SCREEN_WIDTH))
#define SCREEN_HEIGHT_FLOAT ((float) (SCREEN_HEIGHT))
#define SCREEN_WIDTH_FLOAT_HALF ((float) SCREEN_WIDTH / 2)
#define SCREEN_HEIGHT_FLOAT_HALF ((float) SCREEN_HEIGHT / 2)

#define CAMERA_ASPECT (SCREEN_WIDTH_FLOAT / SCREEN_HEIGHT_FLOAT)
#define CAMERA_FAR 15000.0f
#define CAMERA_NEAR 10.0f
#define CAMERA_DEFAULT_FOV 60.0f
#define CAMERA_MIN_FOV 0.0f
#define CAMERA_MAX_FOV 90.0f
#define CAMERA_SCALE 1.0f

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
        func_8003F66C(200, 200, 200, 0, 0, 180);
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

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camUseShake.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camIgnoreShake.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetFOV.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camOverrideProjScales.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetFOV.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDistance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/func_8003F66C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetNo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetNo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetWaterLine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetWaterLine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camUserViewTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camEnableUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDisableUserView.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camIsUserView.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camOffsetZero.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDoSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDoSpriteDirect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camDo2DSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushFloatModelMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushMuzzleMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camScaleModelMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camPushModelMtx.s")

void camRestoreModelMtx(Gfx **dlist) {
    gDkrInsertMatrix((*dlist)++, G_MWO_MATRIX_XX_XY_I, G_MTX_DKR_INDEX_0);
}

void camPopModelMtx(Gfx **dlist) {
    gDkrInsertMatrix((*dlist)++, G_MWO_MATRIX_XX_XY_I, G_MTX_DKR_INDEX_0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetPtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetListPtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetInvProjMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjOrgMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjectionMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetRotationMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camProjectPoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camReversePoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camGetProjZ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camCopyOrthoMatrix.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camStartShake.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camStopShakes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camScreenShake.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camSetZoom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camConvertMatrixList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/camera/camTick.s")
