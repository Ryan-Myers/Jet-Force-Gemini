#include "menu.h"
#include "audio.h"
#include "common.h"
#include "font.h"
#include "gameVi.h"
#include "gsSnd.h"
#include "overlays/overlay12.h"
#include "overlays/overlay14.h"
#include "overlays/overlay31.h"
#include "overlays/overlay37.h"
#include "overlays/overlay40.h"
#include "overlays/overlay42.h"
#include "overlays/overlay57.h"
#include "overlays/overlay58.h"
#include "overlays/overlay61.h"
#include "overlays/overlay62.h"
#include "overlays/overlay63.h"
#include "overlays/overlay9.h"
#include "PR/n_libaudio.h"
#include "runLink.h"

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/initFront.s")

void frontFreeMode(void) {
    if (runlinkIsModuleLoaded(12) != 0) {
        frontFreeMenuFrame();
    }
    if (D_800A51A0_A5DA0 != 0) {
        switch (frontEndMode) {
            case 0:
                fontWindowFlushStrings(1);
                break;
            case 2:
                frontCleanupRarepage();
                break;
            case 3:
                frontCleanupStartScreen();
                break;
            case 4:
                frontCleanupOptionsPage();
                break;
            case 5:
                frontCleanupCharSelect();
                break;
            case 6:
                frontCleanupMultiSelect();
                break;
            case 8:
                frontCleanupMultiModeSelect();
                break;
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                frontCleanupMultiStats();
                break;
            case 24:
                frontKeyboardCleanup();
                break;
            case 16:
                if (
#ifdef VERSION_kiosk
                    multiGameType == 4 || racingInGame
#else
                    (multiPlayerGame && multiGameType == 4) || racingInGame
#endif
                ) {
                    sprintFreeInstruments();
                } else if (numberOfPlayers == 1) {
                    frontCleanupInstruments();
                } else {
                    frontCleanupMultiInstruments();
                }
                break;
            case 17:
                frontCleanupMap();
                runlinkFreeCode(9);
                break;
        }
        D_800A51A0_A5DA0 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontInitMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetMode.s")

u8 frontGetMode(void) {
    return frontEndMode;
}

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

s32 frontGetLanguage(void) {
    return D_800FF386_B1D86;
}

void frontSetLanguage(s32 language) {
    D_800FF386_B1D86 = language;
    setLanguage(language);
}

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

void frontSetWideAdjust(s32 offset) {
    viSetWideAdjust(offset);
    widescreenVOffset = viGetWideAdjust();
}

u8 frontGetStereoMode(void) {
    return speakerSetting;
}

void frontSetStereoMode(s32 mode) {
    if (mode < 0) {
        mode = 0;
    }
    if (mode >= 4) {
        mode = 3;
    }
    speakerSetting = mode;
    alSurround_OutputType(D_800A5938_A6538[mode]);
}

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

u8 frontGetTargetControl(s32 mode) {
    return selectedControlModes[mode & 3];
}

void frontSetTargetControl(s32 mode, s32 value) {
    selectedControlModes[mode & 3] = value & 1;
}

#ifdef VERSION_us
void frontCharSelectSetQuitMode(s32 mode) {
    charselquitmode = mode;
}
#endif
