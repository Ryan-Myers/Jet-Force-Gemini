#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/setLanguage.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/initFront.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontFreeMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontInitMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontUpdate.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontDemoMessage.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontInstruments.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontUpdateTimer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontPrintNum.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontDrawRectangles.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontDrawRectangle.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontFlashScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontPlayerScreenLimits.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/func_8005A4B4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/func_8005A548.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/freeFrontEndList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/freeFrontEndItem.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/loadFrontEndList.s")

const char D_800ADF40[] = "loadFrontEndItem() - Item no %d out of range 0-%d\n";

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/loadFrontEndItem.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/setupFrontEndList.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/setupFrontEndObject.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontDrawObj.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetupMultiPickup.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGameSelected.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetWorldLevel.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetWorldName.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetLanguage.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetLanguage.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontStoreScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontRecallScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetLevelScreenMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetWideAdjust.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetWideAdjust.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetStereoMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetStereoMode.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetSfxVolume.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetSfxVolume.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetBgmVolume.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetBgmVolume.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGet2PlayerSplit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSet2PlayerSplit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontGetTargetControl.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontSetTargetControl.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/menu/frontCharSelectSetQuitMode.s")
#endif
