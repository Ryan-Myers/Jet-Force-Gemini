#include "common.h"
#include "math.h"

#pragma GLOBAL_ASM("asm/nonmatchings/objects/resetVars.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetClosestControlType.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetClosestClass.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetObjdef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objFreeObjdef.s")

s32 objTvTimes(s32 timer) {
    if (osTvType != TV_TYPE_PAL || timer < 0) {
        return timer;
    } else {
        return (timer * 5) / 6;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800052B4_5EB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objLoadObjList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objSetupPlayers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80005DDC_69DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDeleteRomDef.s")

/**
 * Returns the object at the current offset by ID.
*/
Object *objGetObject(s32 index) {
    if (index < 0 || index >= ObjListCount) {
        return 0;
    }
    return ObjList[index];
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetObjList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetControlNo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objSetupObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80006FFC_7BFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000710C_7D0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007494_8094.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800075EC_81EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007648_8248.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007760_8360.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007968_8568.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007974_8574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007984_8584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objFreeObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDoFrees.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80007EA0_8AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80008F60_9B60.s")

void objObjectsPauseTick(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objObjectsTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objObjectsCutCameraTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAnimTextures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objResetAnimModels.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objMoveXYZnocheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objMoveXYZ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPrintShardCluster.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPrintSeveredLimb.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDrawSimpleObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDrawTracer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000A9C0_B5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPrintCustomObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDrawDoor.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000B3DC_BFDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objMakeGunMtx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000BDCC_C9CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000C028_CC28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDrawPlayerEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objSetFlashPartColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objClearFlashes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000C964_D564.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPrintModelObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPrintObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDoPlayerTumble.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objUndoPlayerTumble.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000DCE8_E8E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000DD1C_E91C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objScrollTexture.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objPartInv.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objZsortGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetPlayerlist.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAddAnimPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetAnimPlayerlist.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetPlayerNo.s")

void setObjectViewNormal(f32 x, f32 y, f32 z) {
    f32 vecLength = sqrtf((x * x) + (y * y) + (z * z));
    f32 normalizedLength;
    if (vecLength != 0.0f) {
        normalizedLength = -8192.0f / vecLength;
        x *= normalizedLength;
        y *= normalizedLength;
        z *= normalizedLength;
    }
    gEnvmapPos.x = x;
    gEnvmapPos.y = y;
    gEnvmapPos.z = z;
}

/**
 * Returns a pointer to the asset in the misc. section. If index is out of range, then this
 * function just returns the pointer to Ftables.
 */
s32 *objGetTable(s32 index) {
    if (index < 0 || index >= Fmax) {
        return Ftables;
    }
    return (s32 *)&Ftables[Findex[index]];
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objSetAnimGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000E5F0_F1F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000F8E8_104E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8000FB60_10760.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80010430_11030.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80011200_11E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAnimDframe.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAnimSetMove.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAnimSetFrame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objCreateBonusItem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/GetRomlistInfo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objRegisterDependancy.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objRemoveDependancy.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80011AD4_126D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objCutCameraActive.s")

f32 GetRange(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 temp_f0;
    f32 temp_f16;
    f32 temp_f2;

    temp_f0 = arg0 - arg3;
    temp_f2 = arg1 - arg4;
    temp_f16 = arg2 - arg5;
    return sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f16 * temp_f16));
}

f32 GetRangeSquared(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;

    temp_f2 = arg0 - arg3;
    temp_f16 = arg1 - arg4;
    temp_f18 = arg2 - arg5;
    return (temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18);
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objAddMine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDeleteMine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackUpdateFX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackDraw.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80012564_13164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001276C_1336C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/initSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackSkySet.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001296C_1356C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80013214_13E14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80013478_14078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetSky.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800135E0_141E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackAddTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackUpdateTextureScroll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightAllocate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightFreeMem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightAdd.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackLightMove.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80013DCC_149CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackUpdateLighting.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001412C_14D2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001424C_14E4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80014B6C_1576C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800150A4_15CA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800152C0_15EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetBlock.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetCubeBlockList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackBlockDim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80015738_16338.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800159B8_165B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80015CB8_168B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80015D54_16954.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetPlayerIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80016EA0_17AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800175A0_181A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackCylinderIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackSphereIntersect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_800182C0_18EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackNearestIntersection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackClip3D.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_80019324_19F24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackPolyHeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetLedgeCrossed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetAdjacentLedge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackCylinderHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001A990_1B590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetNearestPoly.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetTrack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackMakeAbsolute.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001B6D8_1C2D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackSetFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackGetFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackSetFogOff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001BBFC_1C7FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001BD94_1C994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackChangeFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/trackFadeFog.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001C448_1D048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001C550_1D150.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/func_8001C570_1D170.s")
