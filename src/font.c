#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontSetWindow0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontSetButtonMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontSetWindowNoise.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontUseFont.s")

void fontColour(s32 red, s32 green, s32 blue, s32 alpha, s32 opacity) {
    Window.textColourR = red;
    Window.textColourG = green;
    Window.textColourB = blue;
    Window.textColourA = alpha;
    Window.opacity = opacity;
}

void fontBackground(s32 red, s32 green, s32 blue, s32 alpha) {
    Window.textBGColourR = red;
    Window.textBGColourG = green;
    Window.textBGColourB = blue;
    Window.textBGColourA = alpha;
}

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontPrintXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontPrintWindowXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80070518_71118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringWidth.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowUseFont.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFontColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFontBackground.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowAddStringXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFlushStrings.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowEnable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowDisable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringAddNumber.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowsDraw.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071564_72164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowDraw.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071A0C_7260C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071B08_72708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontCreateDisplayList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontConvertString.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontGetLine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontYSpacing.s")
