#include "audio.h"
#include "common.h"
#include "gsSnd.h"
#include "gameVi.h"
#include "functions.h"
#include "font.h"

extern u16 SFXVolume;
extern u16 musicVolume;
extern s8 widescreenVOffset;
extern u8 D_800A51A0_A5DA0;
extern u8 D_800A51A4_A5DA4;
extern u8 D_800A51A8_A5DA8;
extern u8 D_800A51B0_A5DB0;
extern u8 multiGameType;

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/initFront.s")


void frontFreeMode(void) {
    if (runlinkIsModuleLoaded(0xC) != 0) {
        TrapDanglingJump();
    }
    if (D_800A51A0_A5DA0 != 0) {
        switch (D_800A51B0_A5DB0) {
        case 0:
            fontWindowFlushStrings(1);
            break;
        case 2:
            TrapDanglingJump();
            break;
        case 3:
            TrapDanglingJump();
            break;
        case 4:
            TrapDanglingJump();
            break;
        case 5:
            TrapDanglingJump();
            break;
        case 6:
            TrapDanglingJump();
            break;
        case 8:
            TrapDanglingJump();
            break;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            TrapDanglingJump();
            break;
        case 24:
            TrapDanglingJump();
            break;
        case 16:
            #ifdef VERSION_us
            if (((multiPlayerGame != 0) && (multiGameType == 4)) || (racingInGame != 0)) {
                TrapDanglingJump();
            } else if (numberOfPlayers == 1) {
                TrapDanglingJump();
            } else {
                TrapDanglingJump();
            }
            #endif
            #ifdef VERSION_kiosk
            if ((multiGameType == 4) || (racingInGame != 0)) {
                TrapDanglingJump();
            } else if (numberOfPlayers == 1) {
                TrapDanglingJump();
            } else {
                TrapDanglingJump();
            }
            #endif
            break;
        case 17:
            TrapDanglingJump();
            runlinkFreeCode(9);
            break;
        }
        D_800A51A0_A5DA0 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontInitMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDemoMessage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontInstruments.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontUpdateTimer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontPrintNum.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDrawRectangles.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDrawRectangle.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontFlashScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontPlayerScreenLimits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/func_80059A04_5A604.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/func_80059A98_5A698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/freeFrontEndList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/freeFrontEndItem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/loadFrontEndList.s")

const char D_800AD390_ADF90[] = "loadFrontEndItem() - Item no %d out of range 0-%d\n";

#pragma GLOBAL_ASM("asm/nonmatchings/menu/loadFrontEndItem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setupFrontEndList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setupFrontEndObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDrawObj.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetupMultiPickup.s")

s32 frontGameSelected(void) {
    return 0;
}

s32 frontGetWorldLevel(void) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetWorldName.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetScreenMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetScreenMode.s")

void frontStoreScreenMode(void) {
    D_800A51A4_A5DA4 = D_800A51A8_A5DA8;
}

u8 frontRecallScreenMode(void) {
    return D_800A51A4_A5DA4;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetLevelScreenMode.s")


s8 frontGetWideAdjust(void) {
    return widescreenVOffset;
}

void frontSetWideAdjust(s32 arg0) {
    viSetWideAdjust(arg0);
    widescreenVOffset = viGetWideAdjust();
}

extern u8 speakerSetting;
u8 frontGetStereoMode(void) {
    return speakerSetting;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetStereoMode.s")

u16 frontGetSfxVolume(void) {
    return SFXVolume;
}

void frontSetSfxVolume(s32 volume) {
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 0x100) {
        volume = 0x100;
    }
    SFXVolume = volume;
    gsSndpSetGlobalVolume(volume);
}

u16 frontGetBgmVolume(void) {
    return musicVolume;
}

void frontSetBgmVolume(s32 volume) {
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 0x100) {
        volume = 0x100;
    }
    musicVolume = volume;
    amTuneSetGlobalVolume(volume);
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGet2PlayerSplit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSet2PlayerSplit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetTargetControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetTargetControl.s")

#ifdef VERSION_us
extern s8 charselquitmode;
void frontCharSelectSetQuitMode(s32 arg0) {
    charselquitmode = arg0;
}
#endif
