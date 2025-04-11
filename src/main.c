#include "common.h"
#include "sched.h"

const char D_800ACDB0[] = "1.1723";
const char D_800ACDB8[] = "19/08/99 15:26";
const char D_800ACDC8[] = "jpegg";
const char D_800ACDD0[] = "null";
const char D_800ACDD8[] = "setting up";
const char D_800ACDE4[] = "freeing";
const char D_800ACDEC[] = "processing";
const char D_800ACDF8[] = "exploding";
const char D_800ACE04[] = "WARNING: couldn't find 'ra=0x666' in function %d\n";
// const char D_800ACE38[] = "main/main.c";
// const char D_800ACE44[] = "main/main.c";
// const char D_800ACE50[] = "main/main.c";
// const char D_800ACE5C[] = "main/main.c";
// const char D_800ACE68[] = "main/main.c";
// const char D_800ACE74[] = "main/main.c";
// const char D_800ACE80[] = "Change: L %d, R %d >> L %d, R %d  (%d)\n";
// const char D_800ACEA8[] = "SYNC\n";
// const char D_800ACEB0[] = "Char Flag %d\n";
// const char D_800ACEC0[] = "Char Flag %d\n";
// const char D_800ACED0[] = "Saving Game %d\n";
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

void mainThread(UNUSED void *unused) {
    //Anti Piracy - This will zero out all RAM if this is a PAL console.
    if (osTvType == OS_TV_TYPE_PAL) {
        s32 i = 0;
        while(1) { ((vu32 *)(RAM_END))[--i] = (u32)0; }
    }
    D_800A3B70 = osBootRamTest1_6105();
    D_800A3B74 = osBootRamTest2_6105();
    mainInitGame();
    load_save_flags = joyRead(load_save_flags, 0);
    D_800FE280 = 0;
    mainGameMode = 6;
    mainChangeLevel(0, D_800A3AB0, 0, 0, 1, 0);
    func_80046070(0x1E);
    while (1) {
        if (mainResetPressed()) {
            rumbleKill();
            amStop();
            osViBlack(TRUE);
            __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | SP_CLR_YIELD | SP_CLR_YIELDED | SP_CLR_TASKDONE | SP_CLR_RSPSIGNAL
                            | SP_CLR_CPUSIGNAL | SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);
            osDpSetStatus(DPC_SET_XBUS_DMEM_DMA | DPC_CLR_FREEZE | DPC_CLR_FLUSH | DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR
                            | DPC_CLR_CMD_CTR | DPC_CLR_CMD_CTR);
            while (1){ } // Infinite loop
        }
        func_80044938();
        if (joyGetPressed(1) & L_TRIG) {
            mainSetGameFlag(0x58, 1);
        }
        if (joyGetPressed(1) & R_TRIG) {
            mainSetGameFlag(0x58, 0);
        }
        bootCheckStack();
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/RevealReturnAddresses.s")

void TrapDanglingJump(void);
void mainInitGame(void) {
    s32 viMode;

    RevealReturnAddresses();
    mmInit();
    securitybuffer = mmAlloc(16, COLOUR_TAG_GREY);
    rzipInit();
    D_800FE26C = 0;

    if (osTvType == OS_TV_TYPE_PAL) {
        viMode = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_TYPE_NTSC) {
        viMode = OS_VI_NTSC_LPN1;
    } else if (osTvType == OS_TV_TYPE_MPAL) {
        viMode = OS_VI_MPAL_LPN1;
    }
    osCreateScheduler(&sc, &Time, 13, viMode, 1);
    viInit(&sc);
    piInit();
    rcpInit(&sc);
    runlinkInitialise();
    TrapDanglingJump();
    runlinkFreeCode(0x24);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800448B0.s")

//main_game_loop in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80044938.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainAddZBCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainUpdateZBCheck.s")

s8 mainGetZBCheck(s32 arg0) {
    if ((arg0 < 0) || (arg0 >= 8)) {
        return 1;
    }
    return D_800FE217[arg0].ZBCheck;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainCPUeffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetGameWindow.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004552C.s")

s16 mainGameWindowChanging(void) {
    return D_800A3A80;
}

void mainGameWindowSize(s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    *x1 = mainGameWindowSizeX1;
    *y1 = mainGameWindowSizeY1;
    *x2 = mainGameWindowSizeX2;
    *y2 = mainGameWindowSizeY2;
}

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

void mainSetAutoSave(s32 autoSave) {
    mainGameAutoSave = autoSave;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSyncNextLevel.s")

void mainSetMode(s32 modeToSet) {
    mainGameMode = modeToSet;
}

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

extern OSMesgQueue resetMsgQueue;
extern s32 resetPressed;

s32 mainResetPressed(void) {
    if (resetPressed == 0) {
        resetPressed = (s32) ((osRecvMesg(&resetMsgQueue, NULL, OS_MESG_NOBLOCK) + 1) != 0);
    }
    return resetPressed;
}

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

//Main debug menu
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004809C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainResetRegion.s")

void mainToggleDebug(void) {
    //debugMenuEnable ^= 1;
}

//Draw debug menu Lower Right section
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80048B38.s")
