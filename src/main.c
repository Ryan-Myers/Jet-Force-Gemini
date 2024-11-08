#include "common.h"
#include "sched.h"

void mainThread(UNUSED void *arg0) {
    //Anti Piracy - This will zero out all RAM if this is a PAL console.
    if (osTvType == OS_TV_TYPE_PAL) {
        s32 i = 0;
        while(1) { ((vu32 *)(RAM_END))[--i] = (u32)0; }
    }
    D_800A3B70_A4770 = osBootRamTest1_6105();
    D_800A3B74_A4774 = osBootRamTest2_6105();
    mainInitGame();
    load_save_flags = joyRead(load_save_flags, 0);
    D_800FE280_FEE80 = 0;
    mainGameMode = 6;
    mainChangeLevel(0, D_800A3AB0_A46B0, 0, 0, 1, 0);
    func_80046070_46C70(0x1E);
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
        func_80044938_45538();
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
    D_800FE26C_FEE6C = 0;

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

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800448B0_454B0.s")

//main_game_loop in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80044938_45538.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainAddZBCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainUpdateZBCheck.s")

s8 mainGetZBCheck(s32 arg0) {
    if ((arg0 < 0) || (arg0 >= 8)) {
        return 1;
    }
    return D_800FE217_FEE17[arg0].ZBCheck;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainCPUeffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetGameWindow.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004552C_4612C.s")

s16 mainGameWindowChanging(void) {
    return D_800A3A80_A4680;
}

void mainGameWindowSize(s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    *x1 = mainGameWindowSizeX1;
    *y1 = mainGameWindowSizeY1;
    *x2 = mainGameWindowSizeX2;
    *y2 = mainGameWindowSizeY2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800456F8_462F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800457F4_463F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainRestartAfterDeath.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80045D44_46944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainChangeLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetAnimGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetAnimGroup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainChangeCameras.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNextCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetNextLevel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80046070_46C70.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80047EE8_48AE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80047FC0_48BC0.s")

//Main debug menu
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8004809C_48C9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainResetRegion.s")

void mainToggleDebug(void) {
    //debugMenuEnable ^= 1;
}

//Draw debug menu Lower Right section
#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80048B38_49738.s")
