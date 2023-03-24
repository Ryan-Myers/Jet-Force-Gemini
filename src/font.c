#include "common.h"

enum WindowFlags {
    DIALOGUE_BOX_UNUSED_01 = 0x0001,
    DIALOGUE_BOX_VERTS     = 0x4000,
    WINDOW_CLOSED    = 0x7FFF,
    WINDOW_OPEN      = 0x8000,
    DIALOGUE_BOX_UNK_02    = 0xBFFF,
    DIALOGUE_BOX_UNUSED_02 = 0xFFFE
};

// typedef enum HorizontalAlignmentFlags {
//     HORZ_ALIGN_LEFT   = 0,
//     HORZ_ALIGN_RIGHT  = 1,
//     HORZ_ALIGN_CENTER = 4
// } HorizontalAlignmentFlags;

// typedef enum VerticalAlignmentFlags {
//     VERT_ALIGN_TOP    = 0,
//     VERT_ALIGN_BOTTOM = 2,
//     VERT_ALIGN_MIDDLE = 8
// } VerticalAlignmentFlags;

// typedef enum AlignmentFlags {
//     ALIGN_TOP_LEFT      = VERT_ALIGN_TOP    | HORZ_ALIGN_LEFT,
//     ALIGN_TOP_CENTER    = VERT_ALIGN_TOP    | HORZ_ALIGN_CENTER,
//     ALIGN_TOP_RIGHT     = VERT_ALIGN_TOP    | HORZ_ALIGN_RIGHT,
//     ALIGN_MIDDLE_LEFT   = VERT_ALIGN_MIDDLE | HORZ_ALIGN_LEFT,
//     ALIGN_MIDDLE_CENTER = VERT_ALIGN_MIDDLE | HORZ_ALIGN_CENTER,
//     ALIGN_MIDDLE_RIGHT  = VERT_ALIGN_MIDDLE | HORZ_ALIGN_RIGHT,
//     ALIGN_BOTTOM_LEFT   = VERT_ALIGN_BOTTOM | HORZ_ALIGN_LEFT,
//     ALIGN_BOTTOM_CENTER = VERT_ALIGN_BOTTOM | HORZ_ALIGN_CENTER,
//     ALIGN_BOTTOM_RIGHT  = VERT_ALIGN_BOTTOM | HORZ_ALIGN_RIGHT
// } AlignmentFlags;

#define WINDOW_COUNT 8
#define POS_CENTRED -0x8000

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

void fontUseFont(s32 font) {
    fontInUse = font;
    Window->font = font;
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

void fontPrintXY(Gfx **displayList, s32 xpos, s32 ypos, char *text, AlignmentFlags alignmentFlags) {
    fontPrintWindowXY(displayList, 0, xpos, ypos, text, alignmentFlags);
}

/**
 * Text draw function that draws text in the current window while overriding position.
 */
void fontPrintWindowXY(Gfx **displayList, s32 windowId, s32 xpos, s32 ypos, char *text, AlignmentFlags alignmentFlags) {
    if (windowId >= 0 && windowId < WINDOW_COUNT) {
        DialogueBoxBackground *temp = &Window[windowId];
        temp->xpos = (xpos == POS_CENTRED) ? temp->width >> 1 : xpos;
        temp->ypos = (ypos == POS_CENTRED) ? temp->height >> 1 : ypos;
        func_80070518_71118(displayList, temp, text, alignmentFlags);
    }
}

//render_text_string
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80070518_71118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringWidth.s")

/**
 * Sets the position and size of the current Window.
 */
void fontWindowSize(s32 windowId, s32 x1, s32 y1, s32 x2, s32 y2) {
    if (windowId > 0 && windowId < WINDOW_COUNT) {
        DialogueBoxBackground *temp = &Window[windowId];
        temp->xpos = 0;
        temp->ypos = 0;
        if (x1 < x2) {
            temp->x1 = (s16) x1;
            temp->x2 = (s16) x2;
        } else {
            temp->x2 = (s16) x1;
            temp->x1 = (s16) x2;
        }
        if (y1 < y2) {
            temp->y1 = (s16) y1;
            temp->y2 = (s16) y2;
        } else {
            temp->y2 = (s16) y1;
            temp->y1 = (s16) y2;
        }
        temp->width = (temp->x2 - temp->x1) + 1;
        temp->height = (temp->y2 - temp->y1) + 1;
    }
}

void fontWindowUseFont(s32 windowId, s32 font) {
    Window[windowId].font = font;
}

/**
 * Sets the background colour of the current dialogue box.
 */
void fontWindowColour(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha) {
    if (windowId > 0 && windowId < WINDOW_COUNT) {
        DialogueBoxBackground *temp = &Window[windowId];
        temp->backgroundColourR = red;
        temp->backgroundColourG = green;
        temp->backgroundColourB = blue;
        temp->backgroundColourA = alpha;
    }
}

void fontWindowFontColour(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity) {
    if ((windowId > 0) && (windowId < WINDOW_COUNT)) {
        DialogueBoxBackground *temp;
        temp = &Window[windowId];
        temp->textColourR = red;
        temp->textColourG = green;
        temp->textColourB = blue;
        temp->textColourA = alpha;
        temp->opacity = opacity;
    }
}

void fontWindowFontBackground(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha) {
    if ((windowId > 0) && (windowId < WINDOW_COUNT)) {
        DialogueBoxBackground *temp;
        temp = &Window[windowId];
        temp->textBGColourR = red;
        temp->textBGColourG = green;
        temp->textBGColourB = blue;
        temp->textBGColourA = alpha;
    }
}

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

/* Size: 10 bytes */
typedef struct FontCharDataAlt {
    u8 textureID; // Texture Pointer Index?
    u8 ulx; // Upper Left Corner? Maybe only used when FontData->unk20 is 0 for some reason.
    u8 width; // Font Char Width?
    u8 height; // Font Char Height?
    u8 s; // Upper left coordinate for the texture derived from X?
    u8 t; // Upper left coordinate for the texture derived from Y?
    u8 lrx; // Related to the lower right X Coordinate.
    u8 lry; // Related to the lower right Y Coordinate.
    u8 pad[8];
} FontCharDataAlt;
extern FontCharDataAlt *Font;

u8 fontYSpacing(s32 arg0) {
    return Font[arg0].height;
}
