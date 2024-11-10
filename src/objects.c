#include "common.h"
#include "math.h"

void resetVars(void) {
    D_800F3860_F4460 = 1;
    D_800F386C_F446C = 0;
    D_800F3870_F4470 = 0;
    D_800F3908_F4508 = 0;
    D_800F3910_F4510 = 0;
    D_800F391C_F451C = 0;
    ObjListCount = 0;
    D_800F38AC_F44AC = 0;
    D_800F38B8_F44B8 = 0;
    D_800F38BC_F44BC = 0;
    D_800F38C4_F44C4 = 0;
    D_800F38C0_F44C0 = 0;
    D_800F38C2_F44C2 = 0;
    D_800F3948_F4548 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objFreeAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetClosestControlType.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetClosestClass.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objGetObjdef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objects/objFreeObjdef.s")

s32 objTvTimes(s32 timer) {
    if (osTvType != OS_TV_TYPE_PAL || timer < 0) {
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

void objMoveXYZnocheck(void) {
    D_800A1208_A1E08 = 1;
}

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

#ifdef NON_EQUIVALENT
void objDoPlayerTumble(Object *this) {
    UNUSED s32 unused1;
    Object_Racer *sp_20;
    f32 sp_1c;
    f32 tmp_f0;
    if (this->behaviorId == 1) {
        sp_20 = this->racer;
        this->segment.trans.y_rotation += sp_20->y_rotation_offset;
        this->segment.trans.x_rotation += sp_20->x_rotation_offset;
        this->segment.trans.z_rotation += sp_20->z_rotation_offset;
        sp_1c = Cosf(sp_20->z_rotation_offset);
        tmp_f0 = Cosf(sp_20->x_rotation_offset - sp_20->unk146) * sp_1c;
        tmp_f0 = ((tmp_f0 < 0.0f) ? 0.0f : tmp_f0 * tmp_f0);
        tmp_f0 = ((1.0f - tmp_f0)) * 24.0f + sp_20->unk64;
        this->segment.trans.y_position += tmp_f0;
        D_800F3A20_F4620 = tmp_f0;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/objects/objDoPlayerTumble.s")
#endif

void objUndoPlayerTumble(Object *obj) {
    if (obj->behaviorId == 1) {
        Object_Racer *racer = obj->racer;
        obj->segment.trans.y_rotation -= racer->y_rotation_offset;
        obj->segment.trans.x_rotation -= racer->x_rotation_offset;
        obj->segment.trans.z_rotation -= racer->z_rotation_offset;
        obj->segment.trans.y_position -= D_800F3A20_F4620;
    }
}

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
