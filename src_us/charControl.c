#include "common.h"

const char D_800AC5E0[] = "charAnimSoundTick: Illegal soundtype\n";
const char D_800AC608[] = "The maximum number of camera objects has been exceeded.\n";
const char D_800AC644[] = "Cannot delete camera object, it is not in list.\n";
#ifdef VERSION_kiosk
const char D_800AC678[] = "grenade\n";
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/charSpeak.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/charPlaySoundEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/charAnimSoundTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/cameraGetBlend.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002B418_2C018.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002B960_2C560.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002BD04_2C904.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002C070_2CC70.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002C078.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002C908_2D508.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002C9A0_2D5A0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002CC70_2D870.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002CF6C_2DB6C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/cameraTopDown.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/init_800f6e58_array.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/cameraAddOverrideObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/cameraDeleteOverrideObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002F0DC_2FCDC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002F2B0_2FEB0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8002F450_30050.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_setting_floyd_height.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/sidekickInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/sidekickControl.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80031090_31C90.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_800319C0_325C0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80031A80_32680.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80031D00_32900.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlClearAimlocks.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80031F9C_32B9C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerReInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlDecapitatePlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlFrozen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlRestartPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlFadePlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlWalkingBack.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80033ED4_34AD4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlThrowWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/dAngle.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlMakeGravity.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlMakeV.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlFSUvels.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlUpdateJetFlames.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_800344B4_350B4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlHalfTurn.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlUpdateBreathing.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGroundHits.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSetTransition.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80035628_36228.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlatform.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlAttitude.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80035B30_36730.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlHangOK.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGrabOK.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSquashCheckPrior.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSquashCheckPost.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_800370D0_37CD0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlCeiling.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlDisableJoypad.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlReadJoypad.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSetRumble.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGetMaxCharge.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_80037420_38020.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/can_fire_weapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlUpdateWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSelectNextWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSelectWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSelectPreviousWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlFireDummyShot.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shooting_func.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlFreePlayersStuff.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_plasmashot.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_default.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_TriRocket.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_HomingMissle.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_Shocker.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_FishFood.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_Grenades.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_shuriken.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_Mines.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_Flares.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/shoot_clusterBombs.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlUpdatePlayerAim.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGetManualAim.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerGunWeight.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerStopAim.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerStopAimAndLook.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGetGunBarrelPos.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerTiltList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSidekickTiltList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGravity.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGetWeaponDef.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/func_8003B51C_3C11C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlAlterMood.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlSetPlayerSetup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlGetPlayerSetup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlClearPlayerSetup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerOpenChest.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlPlayerOpeningChest.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/charControl/controlEmptyPlayersHand.s")
