#include "common.h"
#include "sched.h"

#ifdef VERSION_kiosk
const char D_800ACDB0[] = "1.1723";
const char D_800ACDB8[] = "19/08/99 15:26";
const char D_800ACDC8[] = "jpegg";
const char D_800ACDD0[] = "null";
const char D_800ACDD8[] = "setting up";
const char D_800ACDE4[] = "freeing";
const char D_800ACDEC[] = "processing";
const char D_800ACDF8[] = "exploding";
const char D_800ACE04[] = "WARNING: couldn't find 'ra=0x666' in function %d\n";
const char D_800ACE38[] = "main/main.c";
const char D_800ACE44[] = "main/main.c";
const char D_800ACE50[] = "main/main.c";
const char D_800ACE5C[] = "main/main.c";
const char D_800ACE68[] = "main/main.c";
const char D_800ACE74[] = "main/main.c";
const char D_800ACE80[] = "Change: L %d, R %d >> L %d, R %d  (%d)\n";
const char D_800ACEA8[] = "SYNC\n";
const char D_800ACEB0[] = "Char Flag %d\n";
const char D_800ACEC0[] = "Char Flag %d\n";
const char D_800ACED0[] = "Saving Game %d\n";
const char D_800ACEE0[] = "Canyon";
const char D_800ACEE8[] = "Asteroid";
const char D_800ACEF4[] = "Mizar";
const char D_800ACEFC[] = "Ichor";
const char D_800ACF04[] = "Alien Cave";
const char D_800ACF10[] = "Swamp";
const char D_800ACF18[] = "Sekhmet Battleship";
const char D_800ACF2C[] = "Dune";
const char D_800ACF34[] = "SS Anubis";
const char D_800ACF40[] = "Spawn Ship";
const char D_800ACF4C[] = "Goldwood Forest";
const char D_800ACF5C[] = "Goldwood Quarry";
const char D_800ACF6C[] = "Spacestation";
const char D_800ACF7C[] = "Water ruin";
const char D_800ACF88[] = "Walkway";
const char D_800ACF90[] = "Multi";
const char D_800ACF98[] = "Old";
const char D_800ACF9C[] = "Baddy Test";
const char D_800ACFA8[] = "Canyon Mine";
const char D_800ACFB4[] = "Sub-Worlds";
const char D_800ACFC0[] = "GIRL";
const char D_800ACFC8[] = "BOY";
const char D_800ACFCC[] = "DOG";
const char D_800ACFD0[] = "GIRL HOVERSHIP";
const char D_800ACFE0[] = "POWERGIRL";
const char D_800ACFEC[] = "POWERBOY";
const char D_800ACFF8[] = "POWERDOG";
const char D_800AD004[] = "ANT HOVERSHIP";
const char D_800AD014[] = "CHEATS OFF";
const char D_800AD020[] = "CHEATS ON";
const char D_800AD02C[] = "GOLDWOOD Area 1";
const char D_800AD03C[] = "SS ANUBIS Maze";
const char D_800AD04C[] = "SS ANUBIS Floyd";
const char D_800AD05C[] = "CERULEAN Colony";
const char D_800AD06C[] = "ICHOR Military Base";
const char D_800AD080[] = "MIZARS PALACE";
const char D_800AD090[] = "WALKWAY Sequence";
const char D_800AD0A4[] = "ALIEN CAVES";
const char D_800AD0B0[] = "WATER RUIN";
const char D_800AD0BC[] = "PISTOL";
const char D_800AD0C4[] = "HOMING MISSILE";
const char D_800AD0D4[] = "MACHINE GUN";
const char D_800AD0E0[] = "SHOT GUN";
const char D_800AD0EC[] = "SHRINK BEAM";
const char D_800AD0F8[] = "ROCKET";
const char D_800AD100[] = "FLAMETHROWER";
const char D_800AD110[] = "GRENADE";
const char D_800AD118[] = "SHIRUKEN";
const char D_800AD124[] = "FISH FOOD";
const char D_800AD130[] = "PROXIMITY MINE";
const char D_800AD140[] = "TIMED MINE";
const char D_800AD14C[] = "REMOTE MINE";
const char D_800AD158[] = "FLARE";
const char D_800AD160[] = "CLUSTER BOMB";
// const char D_800AD170[] = "%5d  %5d  %5d\n";
// const char D_800AD180[] = "FREE %d";
// const char D_800AD188[] = "%d K";
#else
const char D_800AC4D0_AD0D0[] = "1.1848";
const char D_800AC4D8_AD0D8[] = "04/09/99 20:40";
const char D_800AC4E8_AD0E8[] = "jpegg";
const char D_800AC4F0_AD0F0[] = "null";
const char D_800AC4F8_AD0F8[] = "setting up";
const char D_800AC504_AD104[] = "freeing";
const char D_800AC50C_AD10C[] = "processing";
const char D_800AC518_AD118[] = "exploding";
const char D_800AC524_AD124[] = "%d\n";
const char D_800AC528_AD128[] = "WARNING: couldn't find 'ra=0x666' in function %d\n";
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainThread.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainPreNMI.s")
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/RevealReturnAddresses.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainInitGame.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_800448B0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80044938.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainAddZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainUpdateZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetZBCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainCPUeffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetGameWindow.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_8004552C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGameWindowChanging.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGameWindowSize.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_800456F8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_800457F4.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainValidateRestart.s")
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainRestartAfterDeath.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80045D44.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainChangeLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainSetAnimGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetAnimGroup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainChangeCameras.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNextCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/mainGetNextLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/main/func_80046070.s")

#ifdef VERSION_kiosk
void mainSetAutoSave(s32 autoSave) {
    mainGameAutoSave = autoSave;
}
#endif

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

#ifdef VERSION_kiosk

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGameChanged.s")
#endif

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
