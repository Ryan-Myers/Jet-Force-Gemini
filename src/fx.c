#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxInitLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxAddLevelEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxClearLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxLevelFreeLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxLevelAllocateLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_800689B4_695B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_800689BC_695BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_80068A14_69614.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxUpdateLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawLevelEffects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxFreeCone.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxAllocateCone.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_80069000_69C00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_800691F0_69DF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxMakeConeTextureCoords.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxMakeConeLength.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawCone.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_80069F04_6AB04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeAllocate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeSetupRipple.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeFree.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeFreeRipple.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeUpdateRipple.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeDraw.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/wakeDrawRipple.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/disableClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/enableClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxFadeOn.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/setupClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/updateClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/drawClearScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006BE88_6CA88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006BE9C_6CA9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006BF50_6CB50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006C124_6CD24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006C230_6CE30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006C77C_6D37C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CA30_6D630.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CB28_6D728.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CC20_6D820.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CD18_6D918.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CE78_6DA78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006CFC0_6DBC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxInitLines.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006D110_6DD10.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawLine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawLineInWindow.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxSetClipWindow.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxTinyPrint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxInttostr.s")

#ifdef VERSION_kiosk
#pragma GLOBAL_ASM("asm/nonmatchings/fx/ClipPoint.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawLine3D.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/PlotAddRGB.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/PlotAddRG.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006DB6C_6E76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006DD20_6E920.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006DF90_6EB90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawDigitalNumber.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxOutputLines.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxInitNightVision.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxFreeNightVision.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxNightVision.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxDrawNightVision.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxNightVisionSkip.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxSPDPRipple.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxQueueScreenEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxUnQueueScreenEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxScreenEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/func_8006F874_70474.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxGenerateTextures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxCpuTextureRequired.s")

#pragma GLOBAL_ASM("asm/nonmatchings/fx/fxCpuTextureFlush.s")
