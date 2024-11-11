#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charSpeak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charPlaySoundEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/charAnimSoundTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraGetBlend.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B378_2BF78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002B8C0_2C4C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002BC64_2C864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002BFD0_2CBD0.s")

void func_8002C070_2CC70(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C078_2CC78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C868_2D468.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002C900_2D500.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CBD0_2D7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002CF78_2DB78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraTopDown.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002EDA0_2F9A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraAddOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/cameraDeleteOverrideObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F0E8_2FCE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F2BC_2FEBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F45C_3005C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8002F518_30118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/sidekickControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003109C_31C9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031984_32584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031A44_32644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031CC4_328C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlClearAimlocks.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80031F60_32B60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerReInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlDecapitatePlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFrozen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlRestartPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFadePlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlWalkingBack.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80033EE8_34AE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlThrowWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/dAngle.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlMakeGravity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlMakeV.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFSUvels.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateJetFlames.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800344C8_350C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlHalfTurn.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateBreathing.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGroundHits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetTransition.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80035628_36228.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlatform.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlAttitude.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80035B30_36730.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlHangOK.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGrabOK.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSquashCheckPrior.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSquashCheckPost.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800370D8_37CD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlCeiling.s")

void controlDisableJoypad(s32 arg0) {
    disablejoy = arg0;
}

void controlReadJoypad(s32 player) {
    if ((player >= 0) && (player < MAXCONTROLLERS) && (disablejoy == 0)) {
        controlXjoy = joyGetStickX(player);
        D_800F7874_F8474 = joyGetAbsX(player);
        controlYjoy = joyGetStickY(player);
        D_800F7878_F8478 = joyGetAbsY(player);
        controlKeys = joyGetButtons(player);
        controlDkeys = joyGetPressed(player);
        D_800F7868_F8468 = joyGetReleased(player);
    } else {
        controlXjoy = 0;
        D_800F7874_F8474 = 0;
        controlYjoy = 0;
        D_800F7878_F8478 = 0;
        controlKeys = 0;
        controlDkeys = 0;
        D_800F7868_F8468 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetMaxCharge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80037380_37F80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80037428_38028.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdateWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectNextWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSelectPreviousWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFireDummyShot.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038488_39088.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlFreePlayersStuff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038D54_39954.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80038E70_39A70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800390E0_39CE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003931C_39F1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800394A4_3A0A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800395B8_3A1B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_800396F0_3A2F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039810_3A410.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039970_3A570.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039AE0_3A6E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_80039CA4_3A8A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlUpdatePlayerAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetManualAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerGunWeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerStopAimAndLook.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetGunBarrelPos.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerTiltList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSidekickTiltList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGravity.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetWeaponDef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/func_8003B2E4_3BEE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlAlterMood.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlSetPlayerSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlGetPlayerSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlClearPlayerSetup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpenChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlPlayerOpeningChest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/charControl/controlEmptyPlayersHand.s")
