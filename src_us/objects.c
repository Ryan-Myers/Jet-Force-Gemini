#include "common.h"

const char D_800AAB50[] = "Objects out of ram(1) !!\n";
const char D_800AAB6C[] = "Char %d, Disguise %d\n";
const char D_800AAB84[] = "Found Boss Control At %d,%d,%d,%08x\n";
const char D_800AABAC[] = "ObjSetupObject(1) Memory fail!!\n";
const char D_800AABD0[] = "ObjSetupObject(2) Memory fail!!\n";
const char D_800AABF4[] = "ObjSetupObject(5) Memory fail!!\n";
const char D_800AAC18[] = "ObjSetupObject(6) Memory fail!!\n";
const char D_800AAC3C[] = "ObjSetupObject(3) Memory fail!!\n";
const char D_800AAC60[] = "ObjList Overflow %d!!!\n";
const char D_800AAC78[] = "NoAddObjList Overflow %d!!!\n";
const char D_800AAC98[] = "ObjSetupObject(4) Memory fail!!\n";
const char D_800AACBC[] = "objSetupObject: clone shadow set up failed\n";
const char D_800AACE8[] = "objSetupObject: clone blob shadow set up failed\n";
const char D_800AAD1C[] = "objSetupChild: memory fail\n";
#ifdef VERSION_kiosk
const char D_800AAD38[] = "stable\n";
#endif
const char D_800AAD40[] = "WARNING:%s (%x) already registered its dependancy with %s (%x)!\n";

void resetVars(void) {
    D_800F3860 = 1;
    D_800F386C = 0;
    D_800F3870 = 0;
    D_800F3908 = 0;
    D_800F3910 = 0;
    D_800F391C = 0;
    ObjListCount = 0;
    D_800F38AC = 0;
    D_800F38B8 = 0;
    D_800F38BC = 0;
    D_800F38C4 = 0;
    D_800F38C0 = 0;
    D_800F38C2 = 0;
    D_800F3948 = 0;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objFreeAll.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetClosestControlType.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetClosestClass.s")

#ifdef VERSION_us
extern s8 D_800A0898_A1498;

void objSetObjRegionOverflow(s8 arg0) {
    D_800A0898_A1498 = arg0;
}
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetObjdef.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objFreeObjdef.s")

s32 objTvTimes(s32 timer) {
    if (osTvType != OS_TV_TYPE_PAL || timer < 0) {
        return timer;
    } else {
        return (timer * 5) / 6;
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_800052B4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objLoadObjList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objSetupPlayers.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80005DDC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDeleteRomDef.s")

/**
 * Returns the object at the current offset by ID.
 */
Object *objGetObject(s32 index) {
    if (index < 0 || index >= ObjListCount) {
        return 0;
    }
    return ObjList[index];
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetObjList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetControlNo.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objSetupObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80006FFC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000710C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007494.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_800075EC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007648.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007760.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007968.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007974.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007984.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objFreeObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDoFrees.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80007EA0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80008F60.s")

void objObjectsPauseTick(void) {
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objObjectsTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objObjectsCutCameraTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAnimTextures.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objResetAnimModels.s")

void objMoveXYZnocheck(void) {
    D_800A1208 = 1;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objMoveXYZ.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPrintShardCluster.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPrintSeveredLimb.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDrawSimpleObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDrawTracer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000A9C0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPrintCustomObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDrawDoor.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000B3DC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objMakeGunMtx.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000BDCC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000C028.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDrawPlayerEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objSetFlashPartColour.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objClearFlashes.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000C964.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPrintModelObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPrintObject.s")

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
        D_800F3A20 = tmp_f0;
    }
}
#else
#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDoPlayerTumble.s")
#endif

void objUndoPlayerTumble(Object *obj) {
    if (obj->behaviorId == 1) {
        Object_Racer *racer = obj->racer;
        obj->segment.trans.y_rotation -= racer->y_rotation_offset;
        obj->segment.trans.x_rotation -= racer->x_rotation_offset;
        obj->segment.trans.z_rotation -= racer->z_rotation_offset;
        obj->segment.trans.y_position -= D_800F3A20;
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000DCE8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000DD1C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objScrollTexture.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objPartInv.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objZsortGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetPlayerlist.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAddAnimPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetAnimPlayerlist.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objGetPlayerNo.s")

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
    return (s32 *) &Ftables[Findex[index]];
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objSetAnimGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000E5F0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objSetup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000F8E8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_8000FB60.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80010430.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80011200.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAnimDframe.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAnimSetMove.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAnimSetFrame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objCreateBonusItem.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/GetRomlistInfo.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objRegisterDependancy.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objRemoveDependancy.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80011AD4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objCutCameraActive.s")

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

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objAddMine.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/objDeleteMine.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/objects/func_80011E74_12A74.s")
#endif
