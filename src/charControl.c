#include "charControl.h"
#include "common.h"
#include "math/math.h"
#include "objects.h"
#include "joy.h"

const char D_800AC5E0[] = "charAnimSoundTick: Illegal soundtype\n";
const char D_800AC608[] = "The maximum number of camera objects has been exceeded.\n";
const char D_800AC644[] = "Cannot delete camera object, it is not in list.\n";
#ifdef VERSION_kiosk
const char D_800AC678[] = "grenade\n";
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charSpeak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charPlaySoundEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charAnimSoundTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraGetBlend.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B418_2C018.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B960_2C560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002BD04_2C904.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C070_2CC70.s")

void func_8002C110_2CD10(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C118_2CD18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C908_2D508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C9A0_2D5A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CC70_2D870.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CF6C_2DB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraTopDown.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002ED94_2F994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraAddOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraDeleteOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F0DC_2FCDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F2B0_2FEB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F450_30050.s")

// func_setting_floyd_height
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F50C_3010C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031090_31C90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800319C0_325C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031A80_32680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031D00_32900.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlClearAimlocks.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031F9C_32B9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerReInit.s")

void controlDecapitatePlayer(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFrozen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlRestartPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFadePlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlWalkingBack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80033ED4_34AD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlThrowWeapon.s")

s16 dAngle(s16 arg0, s16 arg1, f32 arg2) {
    s32 temp_t1;
    s32 var_v1;

    var_v1 = (arg1 - arg0) & 0xFFFF;
    temp_t1 = (arg0 - arg1) & 0xFFFF;
    if (temp_t1 < var_v1) {
        var_v1 = -temp_t1;
    }
    return (s16) (arg0 + (s32) ((f32) var_v1 * arg2));
}

void controlMakeGravity(ControlPlayer *player) {
    s32 pad;
    f32 *table1;
    f32 *table2;

    table1 = (f32 *) objGetTable(1);
    table2 = (f32 *) objGetTable(2);
    controlchr_gravity = table1[player->gravityType & 3] * table2[player->gravityScaleIndex];
    if (player->gravityDisabled == TRUE) {
        controlchr_gravity = 0.0f;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlMakeV.s")

void controlFSUvels(s16 *rotation, ControlPlayer *player) {
    Vec3s sp18;

    sp18.x = rotation[0];
    sp18.y = rotation[1];
    sp18.z = 0;
    pointListRPY(3, &sp18, D_800A2DB4_A39B4, (f32 *) &player->unk18);
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateJetFlames.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800344B4_350B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlHalfTurn.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateBreathing.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGroundHits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetTransition.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80035628.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlatform.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlAttitude.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80035B30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlHangOK.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGrabOK.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSquashCheckPrior.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSquashCheckPost.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800370D0_37CD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlCeiling.s")

void controlDisableJoypad(s32 arg0) {
    disablejoy = arg0;
}

void controlReadJoypad(s32 player) {
    if (player >= 0 && player < MAXCONTROLLERS && !disablejoy) {
        controlXjoy = joyGetStickX(player);
        controlAbsXjoy = joyGetAbsX(player);
        controlYjoy = joyGetStickY(player);
        controlAbsYjoy = joyGetAbsY(player);
        controlKeys = joyGetButtons(player);
        controlDkeys = joyGetPressed(player);
        controlReleasedKeys = joyGetReleased(player);
    } else {
        controlXjoy = 0;
        controlAbsXjoy = 0;
        controlYjoy = 0;
        controlAbsYjoy = 0;
        controlKeys = 0;
        controlDkeys = 0;
        controlReleasedKeys = 0;
    }
}

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetRumble.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetMaxCharge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80037420_38020.s")

// can_fire_weapon
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800374C8_380C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectNextWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectPreviousWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFireDummyShot.s")

// shooting_func
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800386B0_392B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFreePlayersStuff.s")

// shoot_plasmashot
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038F7C_39B7C.s")

// shoot_default
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039098_39C98.s")

// shoot_TriRocket
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039308_39F08.s")

// shoot_HomingMissle
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039544_3A144.s")

// shoot_Shocker
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800396CC_3A2CC.s")

// shoot_FishFood
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800397E0_3A3E0.s")

// shoot_Grenades
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039918_3A518.s")

// shoot_shuriken
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039A38_3A638.s")

// shoot_Mines
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039B98_3A798.s")

// shoot_Flares
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039D08_3A908.s")

// shoot_clusterBombs
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039ECC_3AACC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdatePlayerAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetManualAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerGunWeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAimAndLook.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetGunBarrelPos.s")

// TODO: This is clearly wrong, but it works.
s16 *controlPlayerTiltList(Object *obj) {
    ControlPlayer *player = (ControlPlayer *) &obj->racer[1];
    return &player->playerTiltList;
}

// TODO: same as above
s16 *controlSidekickTiltList(Object *obj) {
    ControlPlayer *player = (ControlPlayer *) &obj->racer[0];
    return &player->sidekickTiltList;
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGravity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetWeaponDef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003B51C_3C11C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlAlterMood.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetPlayerSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetPlayerSetup.s")

extern u8 D_800A2EB0_A3AB0[8];

void controlClearPlayerSetup(void) {
    D_800A2EB0_A3AB0[0] = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpenChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpeningChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlEmptyPlayersHand.s")
