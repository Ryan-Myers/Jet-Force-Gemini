#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainThread.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/RevealReturnAddresses.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainInitGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800448B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80044938.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainAddZBCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainUpdateZBCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetZBCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainCPUeffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetGameWindow.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004552C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGameWindowChanging.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGameWindowSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800456F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800457F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainRestartAfterDeath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80045D44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainChangeLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetAnimGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetAnimGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainChangeCameras.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNextCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNextLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80046070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetAutoSave.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSyncNextLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainTitlePageInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainFrontInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainStartGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetDefaultGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainIncreaseWeaponKills.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainIncreaseWeaponShots.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainIncreaseWeaponHits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainInitMultiPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainInitMultiPlayerPoints.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainRefreshMultiPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetDefaultCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetGameCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainPlayerCollected.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainPlayerDropped.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainPlayerHas.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainAnyoneHas.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainPlayerHasWeapon.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetGameFlagGamePtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetGameFlag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetGameFlag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetTribalStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetTribalStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetLevelWorld.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetPauseMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetPauseMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainResetPressed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetCurrentLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGameChanged.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainLoadGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainLoadAllGames.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetGameArrayPtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSaveGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSaveGame2.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainDeleteGame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainResetSaveGames.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainEraseFlashRom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainLoadGlobalFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSaveGlobalFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainPauseDisable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainDemoOnly.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNumberOfCameras.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNumberOfPlayers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetPlayerCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80047EE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80047FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004809C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainResetRegion.s")

void mainToggleDebug(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80048B38.s")
