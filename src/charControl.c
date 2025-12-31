#include "common.h"

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

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B8C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002BC64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002BFD0.s")

void func_8002C070(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C868.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C900.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CBD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CF78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraTopDown.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002EDA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraAddOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraDeleteOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F0E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F2BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F45C.s")

// func_setting_floyd_height
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F518.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003109C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031A44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031CC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlClearAimlocks.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031F60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerReInit.s")

void controlDecapitatePlayer(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFrozen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlRestartPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFadePlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlWalkingBack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80033EE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlThrowWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/dAngle.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlMakeGravity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlMakeV.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFSUvels.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateJetFlames.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800344C8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800370D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlCeiling.s")

void controlDisableJoypad(s32 arg0) {
    disablejoy = arg0;
}

void controlReadJoypad(s32 player) {
    if ((player >= 0) && (player < MAXCONTROLLERS) && (disablejoy == 0)) {
        controlXjoy = joyGetStickX(player);
        D_800F7874 = joyGetAbsX(player);
        controlYjoy = joyGetStickY(player);
        D_800F7878 = joyGetAbsY(player);
        controlKeys = joyGetButtons(player);
        controlDkeys = joyGetPressed(player);
        D_800F7868 = joyGetReleased(player);
    } else {
        controlXjoy = 0;
        D_800F7874 = 0;
        controlYjoy = 0;
        D_800F7878 = 0;
        controlKeys = 0;
        controlDkeys = 0;
        D_800F7868 = 0;
    }
}

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSetRumble.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetMaxCharge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80037380.s")

// can_fire_weapon
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80037428.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectNextWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectPreviousWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFireDummyShot.s")

// shooting_func
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038488.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFreePlayersStuff.s")

// shoot_plasmashot
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038D54.s")

// shoot_default
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038E70.s")

// shoot_TriRocket
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800390E0.s")

// shoot_HomingMissle
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003931C.s")

// shoot_Shocker
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800394A4.s")

// shoot_FishFood
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800395B8.s")

// shoot_Grenades
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800396F0.s")

// shoot_shuriken
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039810.s")

// shoot_Mines
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039970.s")

// shoot_Flares
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039AE0.s")

// shoot_clusterBombs
#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039CA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdatePlayerAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetManualAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerGunWeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAimAndLook.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetGunBarrelPos.s")

// TODO: This is clearly wrong, but it works.
s32 controlPlayerTiltList(Object *obj) {
    Object_Racer *player = &obj->racer[1];
    return &player->pad68[0x90];
}

// TODO: same as above
s32 controlSidekickTiltList(Object *obj) {
    Object_Racer *player = &obj->racer[0];
    return &player->pad68[0x2E];
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGravity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetWeaponDef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003B2E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlAlterMood.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetPlayerSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetPlayerSetup.s")

extern u8 D_800A3708[8];

void controlClearPlayerSetup(void) {
    D_800A3708[0] = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpenChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpeningChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlEmptyPlayersHand.s")
