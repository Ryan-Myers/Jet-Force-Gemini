#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxInitLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxAddLevelEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxClearLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxLevelFreeLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxLevelAllocateLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_800689B4_695B4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_800689BC_695BC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_80068A14_69614.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxUpdateLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawLevelEffects.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxFreeCone.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxAllocateCone.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_80069000_69C00.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_800691F0_69DF0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxMakeConeTextureCoords.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxMakeConeLength.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawCone.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_80069F04_6AB04.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006A0C0_6ACC0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/wakeSetupRipple.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006A894_6B494.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/wakeFreeRipple.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006A924_6B524.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/wakeUpdateRipple.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006B3CC_6BFCC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/wakeDrawRipple.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/disableClearScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/enableClearScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxFadeOn.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/setupClearScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/updateClearScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/drawClearScreen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006BE88_6CA88.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006BE9C_6CA9C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006BF50_6CB50.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006C124_6CD24.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006C230_6CE30.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006C77C_6D37C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CA30_6D630.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CB28_6D728.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CC20_6D820.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CD18_6D918.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CE78_6DA78.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006CFC0_6DBC0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxInitLines.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006D110_6DD10.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawLine.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawLineInWindow.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxSetClipWindow.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxTinyPrint.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxInttostr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawLine3D.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/PlotAddRGB.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/PlotAddRG.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006DB6C_6E76C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006DD20_6E920.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006DF90_6EB90.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawDigitalNumber.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxOutputLines.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxInitNightVision.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxFreeNightVision.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxNightVision.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxDrawNightVision.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxNightVisionSkip.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxSPDPRipple.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxQueueScreenEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxUnQueueScreenEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxScreenEffect.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/func_8006F874_70474.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxGenerateTextures.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxCpuTextureRequired.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/fx/fxCpuTextureFlush.s")
