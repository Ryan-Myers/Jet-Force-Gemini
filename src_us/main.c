#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainThread.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPreNMI.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/RevealReturnAddresses.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/init_heap_scheduler.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80044F24_45B24.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC4D0_AD0D0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC4D8_AD0D8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC4E8_AD0E8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC4F0_AD0F0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC4F8_AD0F8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC504_AD104.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC50C_AD10C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/D_800AC518_AD118.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/main_loop.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainAddZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainUpdateZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainCPUeffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetGameWindow.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80045C38_46838.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGameWindowChanging.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGameWindowSize.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80045E04_46A04.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80045F00_46B00.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainValidateRestart.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainRestartAfterDeath.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80046550_47150.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainChangeLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetAnimGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetAnimGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainChangeCameras.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNextCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNextLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_800468EC_474EC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSyncNextLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainTitlePageInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainFrontInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainStartGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetDefaultGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainIncreaseWeaponKills.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainIncreaseWeaponShots.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainIncreaseWeaponHits.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainInitMultiPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainInitMultiPlayerPoints.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainRefreshMultiPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetDefaultCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetGameCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPlayerCollected.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPlayerDropped.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPlayerHas.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainAnyoneHas.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPlayerHasWeapon.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetGameFlagGamePtr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetGameFlag.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetGameFlag.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetTribalStatus.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetTribalStatus.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetLevelWorld.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetPauseMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetPauseMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainResetPressed.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetCurrentLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainLoadGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainLoadAllGames.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetGameArrayPtr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSaveGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSaveGame2.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainDeleteGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainResetSaveGames.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainEraseFlashRom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainLoadGlobalFlags.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSaveGlobalFlags.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPauseDisable.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainDemoOnly.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNumberOfCameras.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNumberOfPlayers.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetPlayerCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainResetRegion.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainToggleDebug.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/debug_print_memory.s")
