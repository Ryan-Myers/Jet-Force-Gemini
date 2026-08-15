#include "camera.h"
#include "common.h"
#include "fx.h"
#include "gameVi.h"
#include "math.h"
#include "overlays/overlay1.h"
#include "overlays/overlay10.h"
#include "overlays/overlay39.h"
#include "weather.h"

typedef struct {
    u8 pad0[0x69];
    s8 unk69;
    u8 pad6A[0x6C - 0x6A];
    s8 unk6C;
    u8 pad6D[0xA0 - 0x6D];
    s16 unkA0;
    u8 padA2[0xB2 - 0xA2];
    s8 unkB2;
    s8 unkB3;
    TextureHeader *unkB4;
    s16 unkB8;
    s16 unkBA;
    PulsatingLightData *unkBC;
    u8 padC0[0xCD - 0xC0];
    s8 unkCD;
} TrackLevel;

typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    u16 unk14;
} WaterEffectTexture;

typedef struct {
    u8 pad0[0x1E];
    s16 unk1E;
} Track;

// .bss
extern s16 fadeA; // 0x801046EE
extern Gfx *D_800F2F70_B1750;
extern Mtx *D_800F2F74_B1754;
extern Vertex *D_800F2F78_B1758;
extern Triangle *D_800F2F7C_B175C;
// missing D_800F2F80_B1760
extern s32 D_800F2F84_B1764;
extern s32 D_800F2F8C_B176C;
// missing D_800F2F90_B1770
// missing D_800F2F94_B1774
extern s32 D_800F2F98_B1778;
// missing D_800F2F9C_B177C
extern s32 globflags; // 0x800F2FA0
extern s32 skyframe;  // 0x800F2FA4
extern u32 skyflags;  // 0x800F2FA8

// .data
extern s8 D_800A0CD0_A18D0;
extern u8 beamScrollFlag;                      // 0x800A0CE0
extern Track *track;                           // 0x800A0D60
extern TrackLevel *level;                      // 0x800A0D64
extern WaterEffectTexture *watereffecttexture; // 0x800A0D68
extern s32 watereffectframe;                   // 0x800A0D6C

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
void shadowGenerate(s32, s32);

// forward declarations
void func_800127A4_133A4(s32);
void func_800129AC_135AC(s32);
void func_80013454_14054(void);
void func_80013820_14420(s32, s32);
void func_800158BC_164BC(void);
void func_8001BE04_1CA04(s32, s32);
void func_8001BF9C_1CB9C(s32);

// either func_80012BAC_137AC or func_800136B8_142B8 take an argument, probably not both
void func_80012BAC_137AC(void);
void func_800136B8_142B8(s32);

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

void trackDraw(Gfx **arg0, Mtx **arg1, Vertex **arg2, Triangle **arg3, s32 arg4) {
    PulsatingLightData *temp_a0;
    s32 temp_s2;
    s32 var_s3;
    s32 var_v0;

    temp_s2 = mainGetNumberOfCameras();
    if (TrapDanglingJump() == 0) {
        return;
    }

    D_800F2F70_B1750 = *arg0;
    D_800F2F74_B1754 = *arg1;
    D_800F2F78_B1758 = *arg2;
    D_800F2F7C_B175C = *arg3;
    diRcpTrace(D_800F2F70_B1750, "track/track.c", 504);
    camSetNo(0);
    D_800F2F98_B1778 = 1;
    D_800F2F8C_B176C = 0;
    if (mainGetPauseMode() != 0) {
        var_s3 = 0;
    } else {
        var_s3 = arg4;
    }
    if (watereffecttexture != NULL) {
        var_v0 = watereffectframe;
        var_v0 += (watereffecttexture->unk14 * var_s3);
        while (var_v0 >= watereffecttexture->unk12) {
            var_v0 -= watereffecttexture->unk12;
        }
        watereffectframe = var_v0;
    }
    shadowGenerate(1, arg4);
    levelUpdateColourCycling(var_s3);
    temp_a0 = level->unkBC;
    // Cursed
    if (temp_a0 != (PulsatingLightData *) -1) {
        updateMixCycle(temp_a0, var_s3);
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
        level->unkB8 = (level->unkB8 + (level->unkB2 * var_s3)) & var_v0;
        var_v0 = ((level->unkB4->height << 9) - 1);
        level->unkBA = (level->unkBA + (level->unkB3 * var_s3)) & var_v0;
        texAnimateTexture(level->unkB4, &skyflags, &skyframe, var_s3);
    }
    texDPInit(&D_800F2F70_B1750);

    // some borked version of gSPNumLights?
    gMoveWd(D_800F2F70_B1750++, G_MW_NUMLIGHT, 0, 0);
    gSPClearGeometryMode(D_800F2F70_B1750++, G_CULL_FRONT);
    gDPSetBlendColor(D_800F2F70_B1750++, 0x00, 0x00, 0x00, 0x64);
    gDPSetPrimColor(D_800F2F70_B1750++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(D_800F2F70_B1750++, 0xFF, 0xFF, 0xFF, 0);
    rainSetFog();
    func_8001BE04_1CA04(temp_s2, var_s3);
    if (track->unk1E > 0) {
        func_800129AC_135AC(var_s3);
    }
    if (beamScrollFlag != 0) {
        TrapDanglingJump(var_s3);
    }
    fxUpdateLevelEffects(var_s3);
    if (gameInWindow != 0 && temp_s2 == 1) {
        camEnableUserView(0, 1);
        camUserViewTick();
    }
    for (D_800F2F84_B1764 = 0; D_800F2F84_B1764 < temp_s2; D_800F2F84_B1764++) {
        func_8001BF9C_1CB9C(D_800F2F84_B1764);
        gDPPipeSync(D_800F2F70_B1750++);
        camSetNo(D_800F2F84_B1764);
        camSetView(&D_800F2F70_B1750, &D_800F2F74_B1754);
        func_800158BC_164BC();
        objClearFlashes(1);
        fxSetClipWindow(D_800F2F84_B1764);
        if (temp_s2 < 2) {
            if (level->unk69 == -1) {
                func_80012BAC_137AC();
            } else {
                func_800136B8_142B8(arg4);
            }
        } else {
            func_80013454_14054();
        }
        diRcpTrace(D_800F2F70_B1750, "track/track.c", 637);
        gDPPipeSync(D_800F2F70_B1750++);
        func_80013820_14420(temp_s2, arg4);
        setWeatherLimits(-1, -0x200);
        if (level->unkA0 > 0 && temp_s2 < 2) {
            doWeather(&D_800F2F70_B1750, &D_800F2F74_B1754, &D_800F2F78_B1758, &D_800F2F7C_B175C, var_s3);
        }
        if (temp_s2 < 2) {
            camlightUpdateAll();
            camlightVisibilityCheck();
            camlightDraw(&D_800F2F70_B1750, &D_800F2F74_B1754, &D_800F2F78_B1758);
            func_800127A4_133A4(var_s3);
            fxDrawNightVision(&D_800F2F70_B1750);
        }
    }
    if (numanimlockons > 0) {
        TrapDanglingJump(&D_800F2F70_B1750);
    }
    TrapDanglingJump(&D_800F2F70_B1750);
    if (fadeA != 0) {
        TrapDanglingJump(&D_800F2F70_B1750);
    }
    camDisableUserView(0, 1);
    camResetView(&D_800F2F70_B1750);
    gDPPipeSync(D_800F2F70_B1750++);
    // some borked version of gSPNumLights?
    gMoveWd(D_800F2F70_B1750++, G_MW_NUMLIGHT, 0, 0);
    shadowChangeBuffer();
    *arg0 = D_800F2F70_B1750;
    *arg1 = D_800F2F74_B1754;
    *arg2 = D_800F2F78_B1758;
    *arg3 = D_800F2F7C_B175C;
}

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800127A4_133A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800129AC_135AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/initSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackSkySet.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80012BAC_137AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80013454_14054.s")

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
