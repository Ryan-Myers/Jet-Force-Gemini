#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackUpdateFX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackDraw.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80012564.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001276C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/initSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackSkySet.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001296C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80013214.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80013478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800135E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackAddTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackUpdateTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightAllocate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightFreeMem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightAdd.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightMove.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80013DCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackUpdateLighting.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001412C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001424C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80014B6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800150A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800152C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetBlock.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetCubeBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackBlockDim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015738.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800159B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015CB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80015D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetPlayerIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80016EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800175A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackCylinderIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackSphereIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_800182C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackNearestIntersection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackClip3D.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_80019324.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackPolyHeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetLedgeCrossed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetAdjacentLedge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackCylinderHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001A990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetNearestPoly.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetTrack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackMakeAbsolute.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001B6D8.s")

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
 /* 0x33 */ u8 unk33;
 /* 0x34 */ s16 near;
 /* 0x36 */ s16 far;
} EnvironmentFogCompact;

/* Size: 0x40 bytes */
typedef struct FogData {
    EnvironmentFog fog;                 // Current fog properties. What you'll actually see ingame.
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x20 */ EnvironmentFog addFog;              // Fog override. This will apply over the current fog to give a smooth transition effect.
    EnvironmentFogCompact intendedFog;  // Fog properties the game will try to be if the switch timer is 0.
    /* 0x38 */ s32 switchTimer;
    /* 0x3C */ Object *fogChanger;
} FogData;
extern FogData D_800F3D20[4];

void trackSetFog(s32 fogIdx, s16 near, s16 far, s16 arg3, u8 red, u8 green, u8 blue, s8 arg7) {    
    s32 tempNear;
    FogData *fogData;

    fogData = &D_800F3D20[fogIdx];

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

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackGetFog.s")

void trackSetFogOff(s32 playerID) {
    D_800F3D20[playerID].addFog.near = 0;
    D_800F3D20[playerID].addFog.far = 0;
    D_800F3D20[playerID].addFog.r = 0;
    D_800F3D20[playerID].addFog.g = 0;
    D_800F3D20[playerID].addFog.b = 0;
    D_800F3D20[playerID].fog.near = 1018 << 16;
    D_800F3D20[playerID].fog.far = 1023 << 16;
    D_800F3D20[playerID].intendedFog.r = D_800F3D20[playerID].fog.r >> 16;
    D_800F3D20[playerID].intendedFog.g = D_800F3D20[playerID].fog.g >> 16;
    D_800F3D20[playerID].intendedFog.b = D_800F3D20[playerID].fog.b >> 16;
    D_800F3D20[playerID].intendedFog.near = 1018;
    D_800F3D20[playerID].intendedFog.far = 1023;
    D_800F3D20[playerID].switchTimer = 0;
    D_800F3D20[playerID].fogChanger = NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001BBFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001BD94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackChangeFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackFadeFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/func_8001C570.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackMakePolylist.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/getXZCompareMask.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/getYCompareMask.s")

#pragma GLOBAL_ASM("asm/nonmatchings/track/trackLightAsm.s")
