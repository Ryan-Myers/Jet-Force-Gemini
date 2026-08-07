#include "common.h"
#include "gsSnd.h"
#include "audio.h"

extern u16 SFXVolume;
extern u16 musicVolume;

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/initFront.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontFreeMode.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontStoreScreenMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontRecallScreenMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetLevelScreenMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontGetWideAdjust.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetWideAdjust.s")

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
    if (volume >= 0x101) {
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
    if (volume >= 0x101) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontCharSelectSetQuitMode.s")
#endif
