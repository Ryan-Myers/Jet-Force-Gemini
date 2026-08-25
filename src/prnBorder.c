#include "common.h"
#include "gameVi.h"
#include "main.h"
#include "textures.h"

Gfx D_800A3540_A4140[] = {
    gsDPPipeSync(),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_ZBUFFER | G_FOG),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsDPSetOtherMode(DKR_OMH_1CYC_POINT_NOPERSP, DKR_OML_COMMON | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
UNUSED Gfx D_800A3588_A4188[] = { gsSPNoOp() };
UNUSED s32 D_800A3590_A4190 = 0x20000;
UNUSED Gfx D_800A3598_A4198[] = { gsSPNoOp(), gsSPNoOp() };

extern s32 frontGet2PlayerSplit(void);
extern s32 squadsLowMemoryPanic(void);

void prnBorder(Gfx **dList) {
    s32 numCameras;
    s32 videoMode;
    u32 width;
    u32 height;
    u32 halfWidth;
    u32 halfHeight;
    Gfx *gfx;

    if (!squadsLowMemoryPanic()) {
        return;
    }

    numCameras = mainGetNumberOfCameras();
    videoMode = viGetVideoMode();
    if (numCameras < 2) {
        return;
    }

    gfx = *dList;
    viGetCurrentSize((s32 *) &width, (s32 *) &height);
    halfWidth = width >> 1;
    halfHeight = height >> 1;

    gSPDisplayList(gfx++, D_800A3540_A4140);

    if ((numCameras >= 3) || ((numCameras == 2) && !(videoMode & 1) && (frontGet2PlayerSplit() == 0))) {
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, 0, halfHeight - 2, width, halfHeight + 2);
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
        gDPFillRectangle(gfx++, 0, halfHeight - 1, width, halfHeight + 1);
        gDPPipeSync(gfx++);
    }

    if ((numCameras >= 3) || ((numCameras == 2) && ((videoMode & 1) || frontGet2PlayerSplit()))) {
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, halfWidth - 2, 0, halfWidth + 2, height);
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
        gDPFillRectangle(gfx++, halfWidth - 1, 0, halfWidth + 1, height);
        gDPPipeSync(gfx++);
    }

    *dList = gfx;
    texDPInit(dList);
}
