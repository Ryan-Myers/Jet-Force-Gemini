#include "track.h"
#include "camera.h"
#include "common.h"
#include "fx.h"
#include "gameVi.h"
#include "math/math.h"
#include "objects.h"
#include "overlays/overlay1.h"
#include "overlays/overlay10.h"
#include "overlays/overlay39.h"
#include "textures.h"
#include "weather.h"

// LevelHeader in DKR
typedef struct {
    // 0x10 offset added here somewhere?
    u8 pad0[0x69];
    s8 unk69;
    u8 pad6A[0x6C - 0x6A];
    s8 unk6C;
    u8 pad6D[0xA0 - 0x6D];
    s16 unkA0;
    u8 padA2[0xB0 - 0xA2];
    u8 unkB0;
    u8 unkB1;
    s8 unkB2;
    s8 unkB3;
    TextureHeader *unkB4;
    s16 unkB8;
    s16 unkBA;
    PulsatingLightData *unkBC;
    u8 padC0[0xCD - 0xC0];
    s8 unkCD;
    /* 0xCE */ u8 BGColourBottomR;
    /* 0xCF */ u8 BGColourBottomG;
    /* 0xD0 */ u8 BGColourBottomB;
    /* 0xD1 */ u8 BGColourTopR;
    /* 0xD2 */ u8 BGColourTopG;
    /* 0xD3 */ u8 BGColourTopB;
} TrackLevel;

typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    u16 unk14;
} WaterEffectTexture;

/* Size: 0x8 Bytes */
typedef struct {
    TextureHeader *unk0;
    u32 pad4;
} Track_Unk0;

/* Size: 0x10 Bytes */
typedef struct {
    u8 unk0;
    u8 pad1[0xA - 0x1];
    u16 unkA;
    u32 unkC;
} Track_Unk4_UnkC;

// LevelModelSegment in DKR
/* Size: 0x48 Bytes */
typedef struct {
    u8 pad0[0xC];
    Track_Unk4_UnkC *batches;
    u8 pad10[0x28 - 0x10];
    s16 unk28;
    u8 pad2A[0x48 - 0x2A];
} Track_Unk4;

typedef struct {
    Track_Unk0 *unk0; // textures? in DKR
    Track_Unk4 *segments;
    u8 pad8[0x1A - 0x8];
    s16 unk1A;
    u16 pad1C;
    s16 unk1E;
} Track;

// .bss
extern s16 fadeA; // 0x801046EE
extern Gfx *gTrackDL;
extern Mtx *gTrackMtxPtr;
extern Vertex *gTrackVtxPtr;
extern Triangle *gTrackTriPtr;
// missing D_800F2F80_B1760
extern s32 D_800F2F84_B1764;
extern Object *D_800F2F88_B1768;
extern s32 D_800F2F8C_B176C;
// missing D_800F2F90_B1770
// missing D_800F2F94_B1774
extern s32 gSceneRenderSkyDome;
// missing D_800F2F9C_B177C
extern s32 globflags; // 0x800F2FA0
extern s32 skyframe;  // 0x800F2FA4
extern u32 skyflags;  // 0x800F2FA8
extern s16 D_800F2FAC_B178C;

// .data
extern s8 D_800A0CD0_A18D0;
extern u8 beamScrollFlag;                      // 0x800A0CE0
extern Track *track;                           // 0x800A0D60
extern TrackLevel *level;                      // 0x800A0D64
extern WaterEffectTexture *watereffecttexture; // 0x800A0D68
extern s32 watereffectframe;                   // 0x800A0D6C
extern u8 D_800A0D90_A1990[];

// misc
extern u8 D_A0DA8[];

// main .data
extern u8 gameInWindow; // 0x800A321C

// anim .data
extern s32 numanimlockons; // 0x800A74E0

// TODO: Move to .h files
void diRcpTrace(Gfx *, const char *, s32);
void levelUpdateColourCycling(s32);
s32 mainGetPauseMode(void);
void objClearFlashes(s32);
void shadowChangeBuffer(void);
void shadowGenerate(s32, s32 updateRate);
Object *objSetupObject(LevelObjectEntryCommon *, s32);
void camOffsetZero(Gfx **dlist, Mtx **mtx);

// forward declarations
void func_800127A4_133A4(s32);
void func_800129AC_135AC(s32 updateRate);
void func_80013454_14054(void);
void func_80013820_14420(s32, s32 updateRate);
void func_800158BC_164BC(void);
void func_8001BE04_1CA04(s32, s32);
void func_8001BF9C_1CB9C(s32);

// either func_80012BAC_137AC or func_800136B8_142B8 take an argument, probably not both
void func_80012BAC_137AC(void);
void func_800136B8_142B8(s32 updateRate);

void trackUpdateFX(s32 arg0) {
    if (runlinkIsModuleLoaded(1) != 0) {
        dropletUpdateAll_Trap(arg0);
    }
    if (runlinkIsModuleLoaded(8) != 0) {
        bloodSpurtUpdateAll_Trap(arg0);
    }
    if (runlinkIsModuleLoaded(10) != 0) {
        sparkUpdate_Trap(arg0);
    }
}

void trackDraw(Gfx **dList, Mtx **mtx, Vertex **vtx, Triangle **tris, s32 updateRate) {
    PulsatingLightData *temp_a0;
    s32 temp_s2;
    s32 targetUpdateRate;
    s32 var_v0;

    temp_s2 = mainGetNumberOfCameras();
    if (TrapDanglingJump() == 0) {
        return;
    }

    gTrackDL = *dList;
    gTrackMtxPtr = *mtx;
    gTrackVtxPtr = *vtx;
    gTrackTriPtr = *tris;
    diRcpTrace(gTrackDL, "track/track.c", 504);
    camSetNo(0);
    gSceneRenderSkyDome = 1;
    D_800F2F8C_B176C = 0;
    if (mainGetPauseMode() != 0) {
        targetUpdateRate = 0;
    } else {
        targetUpdateRate = updateRate;
    }
    if (watereffecttexture != NULL) {
        var_v0 = watereffectframe;
        var_v0 += (watereffecttexture->unk14 * targetUpdateRate);
        while (var_v0 >= watereffecttexture->unk12) {
            var_v0 -= watereffecttexture->unk12;
        }
        watereffectframe = var_v0;
    }
    shadowGenerate(1, updateRate);
    levelUpdateColourCycling(targetUpdateRate);
    temp_a0 = level->unkBC;
    // Cursed
    if (temp_a0 != (PulsatingLightData *) -1) {
        updateMixCycle(temp_a0, targetUpdateRate);
    }
    if (level->unk6C == 2) {
        D_800A0CD0_A18D0 = 0;
    } else {
        D_800A0CD0_A18D0 = 1;
    }
    if (level->unk6C == 1 || level->unk6C == 2 || level->unkCD != 0) {
        globflags = 1;
    }
    if (level->unk69 == -1) {
        var_v0 = ((level->unkB4->width << 9) - 1);
        level->unkB8 = (level->unkB8 + (level->unkB2 * targetUpdateRate)) & var_v0;
        var_v0 = ((level->unkB4->height << 9) - 1);
        level->unkBA = (level->unkBA + (level->unkB3 * targetUpdateRate)) & var_v0;
        texAnimateTexture(level->unkB4, &skyflags, &skyframe, targetUpdateRate);
    }
    texDPInit(&gTrackDL);

    gDkrDisableBillboard(gTrackDL++);
    gSPClearGeometryMode(gTrackDL++, G_CULL_FRONT);
    gDPSetBlendColor(gTrackDL++, 0x00, 0x00, 0x00, 0x64);
    gDPSetPrimColor(gTrackDL++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(gTrackDL++, 0xFF, 0xFF, 0xFF, 0);
    rainSetFog();
    func_8001BE04_1CA04(temp_s2, targetUpdateRate);
    if (track->unk1E > 0) {
        func_800129AC_135AC(targetUpdateRate);
    }
    if (beamScrollFlag != 0) {
        TrapDanglingJump(targetUpdateRate);
    }
    fxUpdateLevelEffects(targetUpdateRate);
    if (gameInWindow != 0 && temp_s2 == 1) {
        camEnableUserView(0, 1);
        camUserViewTick();
    }
    for (D_800F2F84_B1764 = 0; D_800F2F84_B1764 < temp_s2; D_800F2F84_B1764++) {
        func_8001BF9C_1CB9C(D_800F2F84_B1764);
        gDPPipeSync(gTrackDL++);
        camSetNo(D_800F2F84_B1764);
        camSetView(&gTrackDL, &gTrackMtxPtr);
        func_800158BC_164BC();
        objClearFlashes(1);
        fxSetClipWindow(D_800F2F84_B1764);
        if (temp_s2 < 2) {
            if (level->unk69 == -1) {
                func_80012BAC_137AC();
            } else {
                func_800136B8_142B8(updateRate);
            }
        } else {
            func_80013454_14054();
        }
        diRcpTrace(gTrackDL, "track/track.c", 637);
        gDPPipeSync(gTrackDL++);
        func_80013820_14420(temp_s2, updateRate);
        setWeatherLimits(-1, -0x200);
        if (level->unkA0 > 0 && temp_s2 < 2) {
            doWeather(&gTrackDL, &gTrackMtxPtr, &gTrackVtxPtr, &gTrackTriPtr, targetUpdateRate);
        }
        if (temp_s2 < 2) {
            camlightUpdateAll();
            camlightVisibilityCheck();
            camlightDraw(&gTrackDL, &gTrackMtxPtr, &gTrackVtxPtr);
            func_800127A4_133A4(targetUpdateRate);
            fxDrawNightVision(&gTrackDL);
        }
    }
    if (numanimlockons > 0) {
        TrapDanglingJump(&gTrackDL);
    }
    TrapDanglingJump(&gTrackDL);
    if (fadeA != 0) {
        TrapDanglingJump(&gTrackDL);
    }
    camDisableUserView(0, 1);
    camResetView(&gTrackDL);
    gDPPipeSync(gTrackDL++);
    gDkrDisableBillboard(gTrackDL++);
    shadowChangeBuffer();
    *dList = gTrackDL;
    *mtx = gTrackMtxPtr;
    *vtx = gTrackVtxPtr;
    *tris = gTrackTriPtr;
}

void func_800127A4_133A4(s32 arg0) {
    s32 var_s0;
    s32 sp60;
    s32 sp5C;
    f32 sp58;
    f32 sp54;
    Camera *camera;
    s32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 temp_f0;
    f32 var_f20;
    s32 pad;

    camera = camGetPtr();
    if (camGetWaterLine(camGetNo()) == 0) {
        return;
    }

    sp58 = camera->trans.position.f[0] - (Sinf(camera->trans.rotation.s[0] + 0x8000) * 10.0f);
    var_f20 = camera->trans.position.f[2] - (Cosf(camera->trans.rotation.s[0] + 0x8000) * 10.0f);
    if ((trackPolyHeight(sp58, var_f20, &sp54, 0x2000) & 0x2000) == 0) {
        return;
    }

    if ((camera->trans.position.f[1] - 16.0f) < sp54) {
        viGetCurrentSize(&sp60, &sp5C);
        sp44 = Sinf(camera->trans.rotation.s[1]);
        temp_f0 = Cosf(camera->trans.rotation.s[1]);
        sp54 = sp54 - camera->trans.position.f[1];
        if (sp54 >= 10.0f) {
            fxSPDPRipple(&gTrackDL, 0, 0, sp60, sp5C, arg0);
            D_801008B4_BB0F4 = 0;
            return;
        }

        var_f20 = (sp54 * sp44) - (10.0f * temp_f0);
        sp54 = (sp54 * temp_f0) + (10.0f * sp44);
        if (var_f20 > 0.0f) {
            var_f20 = -var_f20;
        }
        if (camProjectPoint(0, sp54, var_f20, &sp4C, &sp48, 0) != 0) {
            var_s0 = sp48;
            if (var_s0 < sp5C) {
                if (var_s0 < 0) {
                    var_s0 = 0;
                }
                fxSPDPRipple(&gTrackDL, 0, var_s0, sp60, sp5C, arg0);
                D_801008B4_BB0F4 = var_s0;
            }
        }
    }
}

// track_tex_anim in DKR
void func_800129AC_135AC(s32 updateRate) {
    s32 segmentNumber;
    TextureHeader *temp_a0;
    s32 batchNumber;
    Track_Unk4_UnkC *batch;
    Track_Unk4 *segments;
    s32 sp58;

    segments = track->segments;
    for (segmentNumber = 0; segmentNumber < track->unk1A; segmentNumber++) {
        batch = segments[segmentNumber].batches;
        for (batchNumber = 0; batchNumber < segments[segmentNumber].unk28; batchNumber++) {
            if (batch[batchNumber].unkC & 0x10000) {
                if (batch[batchNumber].unk0 != 0xFF) {
                    temp_a0 = track->unk0[batch[batchNumber].unk0].unk0;
                    if ((temp_a0->numOfTextures != 0x100) && (temp_a0->frameAdvanceDelay != 0)) {
                        sp58 = batch[batchNumber].unkA;
                        texAnimateTexture(temp_a0, &batch[batchNumber].unkC, &sp58, updateRate);
                        batch[batchNumber].unkA = sp58;
                    }
                }
            }
        }
    }
}

void initSky(s32 arg0) {
    LevelObjectEntryCommon spawnObject;

    if (arg0 == -1) {
        D_800F2F88_B1768 = NULL;
        D_800F2FAC_B178C = arg0;
    } else {
        spawnObject.x = 0;
        spawnObject.y = 0;
        spawnObject.z = 0;
        spawnObject.size = sizeof(LevelObjectEntryCommon);
        spawnObject.objectID = arg0;
        D_800F2F88_B1768 = objSetupObject(&spawnObject, OBJECT_SPAWN_UNK02);
        D_800F2FAC_B178C = arg0;
        if (D_800F2F88_B1768 != NULL) {
            D_800F2F88_B1768->segment.unk3C = 0;
            D_800F2F88_B1768->unk4A = -1;
        }
    }
}

void trackSkySet(s32 skyDome) {
    gSceneRenderSkyDome = skyDome;
}

// trackbg_render_flashy in DKR
void func_80012BAC_137AC(void) {
    Triangle *tris;
    Vertex *verts;
    s32 vCoordMask;
    s32 uCoordMask;
    f32 scaledXSin;
    f32 scaledXCos;
    f32 var_f16;
    s16 uCoords[9];
    s16 vCoords[9];
    f32 xCos;
    f32 xSin;
    f32 pad_sp108;
    Camera *camera;
    UNUSED f32 pad_sp100;
    f32 xPositions[9];
    f32 zPositions[9];
    Vec3f pos;
    s32 i;
    s32 var_v0;
    s32 var_v1;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    u8 *var_v0_3;
    f32 var_f14;
    s16 vertY;
    s16 vTempCoord;
    s16 uTempCoord;
    LevelHeader_70 *pad2;
    LevelHeader_70 *var_t2;
    LevelHeader_70 *levelHeader;
    TextureHeader *texHeader;

    verts = gTrackVtxPtr;
    tris = gTrackTriPtr;

    camera = camGetPtr();
    texHeader = level->unkB4;
    D_800F2FAC_B178C = -1;

    uCoordMask = (texHeader->width << 5) - 1;
    vCoordMask = (texHeader->height << 5) - 1;
    xSin = Sinf(-camera->trans.rotation.s[0]);
    xCos = Cosf(-camera->trans.rotation.s[0]);

    scaledXSin = xSin * 1280.0f;
    scaledXCos = xCos * 1280.0f;
    pad_sp100 = 2.0f * scaledXSin;
    xPositions[0] = -scaledXCos - (xSin * 1280.0f);
    zPositions[0] = -scaledXCos + (xSin * 1280.0f);
    xPositions[1] = scaledXCos - (xSin * 1280.0f);
    zPositions[1] = -scaledXCos - (xSin * 1280.0f);
    xPositions[2] = scaledXCos + scaledXSin;
    zPositions[2] = scaledXCos - (xSin * 1280.0f);
    xPositions[3] = -scaledXCos + (xSin * 1280.0f);
    zPositions[3] = scaledXCos + (xSin * 1280.0f);
    xPositions[4] = 0.0f;
    zPositions[4] = 0.0f;

    xPositions[5] = -(xCos * 1280.0f) - (2.0f * scaledXSin);
    zPositions[5] = scaledXSin + -(2.0f * (xCos * 1280.0f));
    xPositions[6] = (xCos * 1280.0f) - (2.0f * scaledXSin);
    zPositions[6] = -(2.0f * (xCos * 1280.0f)) - scaledXSin;
    xPositions[7] = (xCos * 1280.0f) + (2.0f * scaledXSin);
    zPositions[7] = (2.0f * (xCos * 1280.0f)) - scaledXSin;
    xPositions[8] = -(xCos * 1280.0f) + (2.0f * scaledXSin);
    zPositions[8] = (2.0f * (xCos * 1280.0f)) + scaledXSin;

    scaledXCos = 1280.0f;
    var_f14 = scaledXCos * 0.25f;

    var_a1 = texHeader->width * 16 * level->unkB0;
    var_a2 = texHeader->height * 16 * level->unkB1;

    var_v0 = ((s32) (camera->trans.x_position * ((scaledXCos * 0.25f) / var_a1)) + (level->unkB8 >> 4)) & uCoordMask;
    var_v1 = ((s32) (camera->trans.z_position * ((scaledXCos * 0.25f) / var_a2)) + (level->unkBA >> 4)) & vCoordMask;

    var_f14 = var_a1 * xCos;
    pos.z = var_a1 * xCos;
    pos.x = var_a1 * xCos;
    var_f16 = var_a2 * xSin;
    xCos = var_f16;
    pad_sp108 = var_f16;

    // @fake
    var_a2 = texHeader->height * 16 * level->unkB1;

    uCoords[0] = (s16) (-var_f14 - pad_sp108) + var_v0;
    vCoords[0] = (s16) (var_f16 - var_f14) + var_v1;
    uCoords[1] = (s16) (var_f14 - pad_sp108) + var_v0;
    vCoords[1] = (s16) (-var_f14 - var_f16) + var_v1;
    uCoords[2] = (s16) (var_f14 + var_f16) + var_v0;
    vCoords[2] = (s16) (var_f14 - var_f16) + var_v1;
    uCoords[3] = (s16) (var_f16 - var_f14) + var_v0;
    vCoords[3] = (s16) (var_f14 + var_f16) + var_v1;

    uCoords[4] = var_v0;
    vCoords[4] = var_v1;

    uCoords[5] = (s16) (-var_f14 - (2.0f * xCos)) + var_v0;
    vCoords[5] = (s16) (var_f16 - (2.0f * var_f14)) + var_v1;
    uCoords[6] = (s16) (var_f14 - (2.0f * xCos)) + var_v0;
    vCoords[6] = (s16) ((-(2.0f * var_f14)) - var_f16) + var_v1;
    uCoords[7] = (s16) (pos.f[2] + (2.0f * xCos)) + var_v0;
    vCoords[7] = (s16) ((2.0f * pos.x) - var_f16) + var_v1;
    uCoords[8] = (s16) ((2.0f * xCos) - pos.z) + var_v0;
    vCoords[8] = (s16) ((2.0f * pos.x) + var_f16) + var_v1;

    camOffsetZero(&gTrackDL, &gTrackMtxPtr);
    texDPTextureX(&gTrackDL, texHeader, 0x10, skyframe << 8);
    gDPSetPrimColor(gTrackDL++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(gTrackDL++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPVertexJFG(gTrackDL++, OS_K0_TO_PHYSICAL(gTrackVtxPtr), 9, 0);
    gSPPolygon(gTrackDL++, OS_K0_TO_PHYSICAL(gTrackTriPtr), 8, 1);
    gDPPipeSync(gTrackDL++);

    vertY = camera->trans.position.f[1] + 192.0f;
    for (i = 0; i < 9; i++) {
        verts->x = camera->trans.position.f[0] + xPositions[i];
        verts->y = vertY;
        verts->z = camera->trans.position.f[2] + zPositions[i];
        verts->r = 0xFF;
        verts->g = 0xFF;
        verts->b = 0xFF;
        verts->a = (i <= 4) ? (255) : (0);
        verts++;
    }

    var_v0_3 = D_800A0D90_A1990;
    for (i = 0; i < 8; i++) {
        tris->flags = BACKFACE_DRAW;
        tris->vi0 = *var_v0_3;
        tris->uv0.u = uCoords[*var_v0_3];
        tris->uv0.v = vCoords[*var_v0_3];
        var_v0_3 += 1;
        tris->vi1 = *var_v0_3;
        tris->uv1.u = uCoords[*var_v0_3];
        tris->uv1.v = vCoords[*var_v0_3];
        var_v0_3 += 1;
        tris->vi2 = *var_v0_3;
        tris->uv2.u = uCoords[*var_v0_3];
        tris->uv2.v = vCoords[*var_v0_3];
        var_v0_3 += 1;
        tris++;
    }
    gTrackVtxPtr = verts;
    gTrackTriPtr = tris;
}

// trackbg_render_gradient in DKR
void func_80013454_14054(void) {
    s32 width;
    s32 height;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    u8 topR;
    u8 topG;
    u8 topB;
    u8 bottomR;
    u8 bottomG;
    u8 bottomB;
    Vertex *verts;

    verts = gTrackVtxPtr;
    D_800F2FAC_B178C = -1;
    camStandardOrtho(&gTrackDL, &gTrackMtxPtr);
    texDPInit(&gTrackDL);
    texDPTextureX(&gTrackDL, NULL, 8, 0);
    gSPVertexJFG(gTrackDL++, OS_K0_TO_PHYSICAL(verts), 4, 0);
    gSPPolygon(gTrackDL++, D_A0DA8, 2, 0);
    camSetView(&gTrackDL, &gTrackMtxPtr);
    topR = level->BGColourTopR;
    topG = level->BGColourTopG;
    topB = level->BGColourTopB;
    bottomR = level->BGColourBottomR;
    bottomG = level->BGColourBottomG;
    bottomB = level->BGColourBottomB;
    viGetCurrentSize(&width, &height);
    camGetWindowLimits(camGetMode(), camGetNo(), &sp54, &sp50, &sp4C, &sp48);
    width = (u32) width >> 1;
    height = (u32) height >> 1;
    verts->x = sp54 - ((u32) width);
    verts->y = ((u32) height) - sp48;
    verts->z = 0x10;
    verts->r = topR;
    verts->g = topG;
    verts->b = topB;
    verts->a = 0xFF;
    verts++;

    verts->x = sp4C - ((u32) width);
    verts->y = ((u32) height) - sp48;
    verts->z = 0x10;
    verts->r = topR;
    verts->g = topG;
    verts->b = topB;
    verts->a = 0xFF;
    verts++;

    verts->x = sp54 - ((u32) width);
    verts->y = ((u32) height) - sp50;
    verts->z = 0x10;
    verts->r = bottomR;
    verts->g = bottomG;
    verts->b = bottomB;
    verts->a = 0xFF;

    verts++;

    verts->x = sp4C - ((u32) width);
    verts->y = ((u32) height) - sp50;
    verts->z = 0x10;
    verts->r = bottomR;
    verts->g = bottomG;
    verts->b = bottomB;
    verts->a = 0xFF;
    verts++;

    gTrackVtxPtr = verts;
}

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800136B8_142B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80013820_14420.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackAddTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackUpdateTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightAllocate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightFreeMem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightAdd.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightMove.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001400C_14C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackUpdateLighting.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001436C_14F6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001448C_1508C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80014EBC_15ABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015228_15E28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015444_16044.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetBlock.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetCubeBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackBlockDim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800158BC_164BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015B3C_1673C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015E3C_16A3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015ED8_16AD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetPlayerIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80017094_17C94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80017794_18394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackCylinderIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackSphereIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800184B4_190B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackNearestIntersection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackClip3D.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80019518_1A118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackPolyHeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetLedgeCrossed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetAdjacentLedge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackCylinderHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001AB84_1B784.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetNearestPoly.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetTrack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackMakeAbsolute.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001B8E0_1C4E0.s")

// In this struct, data is rightshifted 16 bytes, so make the smooth transition more precise.
typedef struct EnvironmentFog {
    /* 0x00 */ s32 r;
    /* 0x04 */ s32 g;
    /* 0x08 */ s32 b;
    /* 0x0C */ s32 near;
    /* 0x10 */ s32 far;
} EnvironmentFog;

typedef struct EnvironmentFogCompact {
    /* 0x30 */ u8 r;
    /* 0x31 */ u8 g;
    /* 0x32 */ u8 b;
    /* 0x33 */ s8 unk33;
    /* 0x34 */ s16 near;
    /* 0x36 */ s16 far;
} EnvironmentFogCompact;

/* Size: 0x40 bytes */
typedef struct FogData {
    EnvironmentFog fog; // Current fog properties. What you'll actually see ingame.
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x20 */ EnvironmentFog
        addFog; // Fog override. This will apply over the current fog to give a smooth transition effect.
    EnvironmentFogCompact intendedFog; // Fog properties the game will try to be if the switch timer is 0.
    /* 0x38 */ s32 switchTimer;
    /* 0x3C */ Object *fogChanger;
} FogData;
extern FogData D_800F3260_F3E60[4];

void trackSetFog(s32 fogIdx, s16 near, s16 far, s16 arg3, u8 red, u8 green, u8 blue, s8 arg7) {
    s32 tempNear;
    FogData *fogData;

    fogData = &D_800F3260_F3E60[fogIdx];

    if (far < near) {
        tempNear = near;
        near = far;
        far = tempNear;
    }

    if (far > 1023) {
        far = 1023;
    }
    if (near >= far - 5) {
        near = far - 5;
    }

    fogData->addFog.near = 0;
    fogData->addFog.far = 0;
    fogData->addFog.r = 0;
    fogData->addFog.g = 0;
    fogData->addFog.b = 0;
    fogData->fog.r = red << 16;
    fogData->fog.g = green << 16;
    fogData->fog.b = blue << 16;
    fogData->fog.near = near << 16;
    fogData->fog.far = far << 16;
    fogData->unk14 = near << 16;
    fogData->unk18 = arg3 << 16;
    fogData->intendedFog.unk33 = arg7;
    fogData->intendedFog.near = near;
    fogData->intendedFog.far = far;
    fogData->switchTimer = 0;
    fogData->fogChanger = NULL;
    fogData->intendedFog.r = red;
    fogData->intendedFog.g = green;
    fogData->intendedFog.b = blue;
}

void trackGetFog(s32 playerID, s16 *near, s16 *far, s16 *unk18, u8 *r, u8 *g, u8 *b, s8 *unk33) {
    FogData *fogData;

    if (runlinkIsModuleLoaded(39) != 0) {
        mantisLightingGetFog_Trap(near, far, r, g, b);
        *unk18 = 0;
        *unk33 = 0;
    } else if (runlinkIsModuleLoaded(23) != 0) {
        dayGetFog_Trap(near, far, r, g, b);
        *unk18 = 0;
        *unk33 = 0;
    } else {
        fogData = &D_800F3260_F3E60[playerID];
        *near = fogData->fog.near >> 16;
        *far = fogData->fog.far >> 16;
        *unk18 = fogData->unk18 >> 16;
        *r = fogData->fog.r >> 16;
        *g = fogData->fog.g >> 16;
        *b = fogData->fog.b >> 16;
        *unk33 = fogData->intendedFog.unk33 & 0x7F;
    }
    if (runlinkIsModuleLoaded(29) != 0) {
        girlMagicFog_Trap(r, g, b, near, far, unk33);
    }
}

void trackSetFogOff(s32 playerID) {
    D_800F3260_F3E60[playerID].addFog.near = 0;
    D_800F3260_F3E60[playerID].addFog.far = 0;
    D_800F3260_F3E60[playerID].addFog.r = 0;
    D_800F3260_F3E60[playerID].addFog.g = 0;
    D_800F3260_F3E60[playerID].addFog.b = 0;
    D_800F3260_F3E60[playerID].fog.near = 1018 << 16;
    D_800F3260_F3E60[playerID].fog.far = 1023 << 16;
    D_800F3260_F3E60[playerID].intendedFog.r = D_800F3260_F3E60[playerID].fog.r >> 16;
    D_800F3260_F3E60[playerID].intendedFog.g = D_800F3260_F3E60[playerID].fog.g >> 16;
    D_800F3260_F3E60[playerID].intendedFog.b = D_800F3260_F3E60[playerID].fog.b >> 16;
    D_800F3260_F3E60[playerID].intendedFog.near = 1018;
    D_800F3260_F3E60[playerID].intendedFog.far = 1023;
    D_800F3260_F3E60[playerID].switchTimer = 0;
    D_800F3260_F3E60[playerID].fogChanger = NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001BE04_1CA04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001BF9C_1CB9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackChangeFog.s")

void trackFadeFog(s32 fogIdx, s32 red, s32 green, s32 blue, s32 near, s32 far, f32 timer) {
    s32 temp;
    s32 switchTimer;
    FogData *fogData;

    fogData = &D_800F3260_F3E60[fogIdx];

    if (osTvType == OS_TV_TYPE_PAL) {
        switchTimer = (timer * 50.0f);
    } else {
        switchTimer = (timer * 60.0f);
    }

    if (far < near) {
        temp = near;
        near = far;
        far = temp;
    }

    if (far > 1023) {
        far = 1023;
    }
    if (near >= far - 5) {
        near = far - 5;
    }

    fogData->intendedFog.r = red;
    fogData->intendedFog.g = green;
    fogData->intendedFog.b = blue;
    fogData->intendedFog.near = near;
    fogData->intendedFog.far = far;

    if (switchTimer > 0) {
        fogData->switchTimer = switchTimer;
        fogData->addFog.r = ((red << 16) - fogData->fog.r) / switchTimer;
        fogData->addFog.g = ((green << 16) - fogData->fog.g) / switchTimer;
        fogData->addFog.b = ((blue << 16) - fogData->fog.b) / switchTimer;
        fogData->addFog.near = ((near << 16) - fogData->fog.near) / switchTimer;
        fogData->addFog.far = ((far << 16) - fogData->fog.far) / switchTimer;
    } else {
        fogData->switchTimer = 0;
        fogData->fog.r = red << 16;
        fogData->fog.g = green << 16;
        fogData->fog.b = blue << 16;
        fogData->fog.near = near << 16;
        fogData->fog.far = far << 16;
    }
    fogData->fogChanger = NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C650_1D250.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C758_1D358.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C778_1D378.s")
