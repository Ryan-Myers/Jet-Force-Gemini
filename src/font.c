#include "common.h"

enum WindowFlags {
    DIALOGUE_BOX_UNUSED_01 = 0x0001,
    DIALOGUE_BOX_VERTS     = 0x4000,
    WINDOW_CLOSED    = 0x7FFF,
    WINDOW_OPEN      = 0x8000,
    DIALOGUE_BOX_UNK_02    = 0xBFFF,
    DIALOGUE_BOX_UNUSED_02 = 0xFFFE
};

void fontSetWindow0(s32 width, s32 height) {
    Window->x2 = width - 1;
    Window->y2 = height - 1;
    Window->width = width;
    Window->height = height;
}

void fontSetButtonMode(s32 mode) {
    buttonMode = mode;
}

void fontSetWindowNoise(u8 arg0, u8 arg1, u8 arg2) {
    D_800A7868_A8468 = arg0;
    D_800A786C_A846C = arg1;
    D_800A7870_A8470 = arg2;
}

void fontUseFont(s32 arg0) {
    fontInUse = arg0;
    D_80104C95_105895 = arg0;
}

void fontColour(s32 red, s32 green, s32 blue, s32 alpha, s32 opacity) {
    Window->textColourR = red;
    Window->textColourG = green;
    Window->textColourB = blue;
    Window->textColourA = alpha;
    Window->opacity = opacity;
}

void fontBackground(s32 red, s32 green, s32 blue, s32 alpha) {
    Window->textBGColourR = red;
    Window->textBGColourG = green;
    Window->textBGColourB = blue;
    Window->textBGColourA = alpha;
}

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontPrintXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontPrintWindowXY.s")

//render_text_string
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80070518_71118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringWidth.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowUseFont.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFontColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFontBackground.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowAddStringXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowFlushStrings.s")

void fontWindowEnable(s32 windowId) {
    Window[windowId].flags |= WINDOW_OPEN;
}

void fontWindowDisable(s32 windowId) {
    Window[windowId].flags &= WINDOW_CLOSED;
}

#ifdef NON_EQUIVALENT
/**
 * Convert an integer to a string.
 */
void fontStringAddNumber(unsigned char **outString, s32 number) {
    u8 digit;
    s32 i;
    s32 hasDigit; // boolean
    s32 div;
    s32 pow;
    unsigned char *ret = *outString;

    // Check for a negative number
    if (number < 0) {
        *ret = '-';
        ret++;
        number = -number;
    }

    // Loop through digit places.
    hasDigit = FALSE;
    for (i = 0; number < D_800A7874_A8474[i]; i++) {
        pow = D_800A7874_A8474[i];
        digit = '0';
        if (number >= pow) {
            div = number / pow;
            number -= div * pow;
            digit += div;
            hasDigit = TRUE;
        }
        if (hasDigit) {
            *ret = digit;
            ret++;
        }
    }

    // One's digit place
    *ret = '0' + number;
    ret++;
    *outString = ret;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringAddNumber.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowsDraw.s")

//render_fill_rectangle
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071564_72164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowDraw.s")

//parse_string_with_number
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071A0C_7260C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071B08_72708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontCreateDisplayList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontConvertString.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontGetLine.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontYSpacing.s")
