#include "common.h"

enum WindowFlags {
    DIALOGUE_BOX_UNUSED_01 = 0x0001,
    DIALOGUE_BOX_VERTS = 0x4000,
    WINDOW_CLOSED = 0x7FFF,
    WINDOW_OPEN = 0x8000,
    DIALOGUE_BOX_UNK_02 = 0xBFFF,
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
#define DIALOGUE_NUM_NULL 0xFF

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
    D_800A7868 = arg0;
    D_800A786C = arg1;
    D_800A7870 = arg2;
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
        func_80070518(displayList, temp, text, alignmentFlags);
    }
}

// render_text_string
#ifdef NON_EQUIVALENT
/**
 * Loops through a string, then draws each character onscreen.
 * Will also draw a fillrect if text backgrounds are enabled.
 */
extern u8 fontInUse;
void func_80070518(Gfx **dList, DialogueBoxBackground *box, char *text, AlignmentFlags alignmentFlags) {
    s32 fontInUse_saved;
    s32 scisOffset;
    s32 scisPos;
    s32 ypos;
    s32 xpos;
    TextureHeader *texture;
    s32 textureLrx;
    s32 textureLry;
    s32 textureS;
    s32 textureT;
    s32 textureUlx;
    s32 textureUly;
    s32 textureWidth;
    s32 textureHeight;
    s32 xAlignmentDiff;
    s32 yAlignmentDiff;
    s32 lastTextureIndex;
    s32 charIndex;
    s32 charSpace;
    s32 newData;
    s32 textureIndex;
    FontData *fontData;
    s32 newTempX;
    s32 newTempY;
    u8 curChar;

    xAlignmentDiff = -1;
    lastTextureIndex = -1;
    if (text == NULL) {
        return;
    }

    fontInUse_saved = fontInUse;
    fontInUse = box->font;
    xpos = box->xpos;
    ypos = box->ypos;
    fontData = &Font[fontInUse];
    gSPDisplayList((*dList)++, D_800A77F0);
    if (box != Window) {
        scisOffset = (((box->y2 - box->y1) + 1) / (f32) 2) * 4.0f;
        scisPos = (box->y1 + box->y2) >> 1;
        gDPSetScissor((*dList)++, 0, box->x1, scisPos - scisOffset, box->x2, scisPos + scisOffset);
    }
    if (alignmentFlags & (HORZ_ALIGN_RIGHT | HORZ_ALIGN_CENTER)) {
        xAlignmentDiff = fontStringWidth(text, xpos, box->font);
        if (alignmentFlags & HORZ_ALIGN_RIGHT) {
            xpos = (xpos - xAlignmentDiff) + 1;
        } else {
            xpos -= xAlignmentDiff >> 1;
        }
    }
    if (alignmentFlags & VERT_ALIGN_BOTTOM) {
        ypos = (ypos - fontData->y) + 1;
    }
    if (alignmentFlags & VERT_ALIGN_MIDDLE) {
        ypos -= fontData->y >> 1;
    }
    if (box->textBGColourA != 0) {
        gDPSetEnvColor((*dList)++, box->textBGColourR, box->textBGColourG, box->textBGColourB, box->textBGColourA);
        if (xAlignmentDiff == -1) {
            xAlignmentDiff = fontStringWidth(text, xpos, box->font);
        }
        gDPSetPrimColor((*dList)++, 0, 0, D_800A7868, D_800A786C, D_800A7870, 0);
        newTempX = xpos + xAlignmentDiff - 1;
        newTempY = ypos + fontData->y - 1;
        gDkrDmaDisplayList((*dList)++, &D_A7858, 2);
        gDPFillRectangle((*dList)++, xpos + box->x1, ypos + box->y1, newTempX + box->x1, newTempY + box->y1);
        gDPPipeSync((*dList)++);
    }
    gDPSetPrimColor((*dList)++, textureLry, 0, 255, 255, 255, box->opacity);
    gDPSetEnvColor((*dList)++, box->textColourR, box->textColourG, box->textColourB, box->textColourA);
    // gDkrDmaDisplayList((*dList)++, OS_K0_TO_PHYSICAL(dDialogueBoxDrawModes[0]), 2);
    // gDPPipeSync((*dList)++);
    xpos += box->textOffsetX;
    ypos += box->textOffsetY;
    for (charIndex = 0; (text[charIndex] != '\0') && (box->y2 >= ypos); xpos += charSpace, charIndex++) {
        curChar = text[charIndex];
        newData = FALSE;
        charSpace = 0;
        if (curChar <= 0x20 || curChar >= 0x80) {
            switch (curChar) {
                case ' ': // Space
                    xpos += fontData->charWidth;
                    break;
                case '\n': // Line Feed
                    xpos = box->textOffsetX;
                    ypos += fontData->y;
                    break;
                case '\t': // Tab
                    xpos += fontData->charHeight - ((xpos - box->textOffsetX) % fontData->charHeight);
                    break;
                case '\v': // VT - Vertical Tab
                    ypos += fontData->y;
                    break;
                case '\r': // Carriage Return
                    xpos = box->textOffsetX;
                    break;
                default:
                    xpos += fontData->charWidth;
                    break;
            }
        } else {
            curChar -= 0x20; // Convert lower case to upper case ASCII
            textureIndex = fontData->letter[curChar].textureID;
            if (textureIndex != 0xFF) {
                newData = TRUE;
                if (lastTextureIndex != textureIndex) {
                    lastTextureIndex = textureIndex;
                    texture = fontData->texturePointers[textureIndex];
                    gDkrDmaDisplayList((*dList)++, OS_PHYSICAL_TO_K0(texture->cmd), texture->numberOfCommands);
                }
                textureWidth = fontData->letter[curChar].width;
                textureHeight = fontData->letter[curChar].height;
                textureS = fontData->letter[curChar].s;
                textureT = fontData->letter[curChar].t;
                xAlignmentDiff = fontData->letter[curChar].lrx;
                yAlignmentDiff = fontData->letter[curChar].lry;
                charSpace = (fontData->x == 0) ? (fontData->letter[curChar].ulx) : (fontData->x);
                newData = TRUE;
            }
        }
        if (newData) {
            textureUlx = ((box->x1 + xpos) + textureWidth) * 4;
            textureUly = ((box->y1 + ypos) + textureHeight) * 4;
            textureLrx = (xAlignmentDiff * 4) + textureUlx;
            newTempY = (yAlignmentDiff * 4) + textureUly;
            textureS *= 32;
            textureT *= 32;
            if ((textureUlx < 0) && (textureLrx > 0)) {
                textureS += (-textureUlx) * 8;
                textureUlx = 0;
            }
            if ((textureUly <= (0 - 1)) && (newTempY > 0)) {
                textureT += -textureUly * 8;
                textureUly = 0;
            }
            gSPTextureRectangle((*dList)++, textureUlx, textureUly, textureLrx, newTempY, 0, textureS, textureT,
                                1 << 10, 1 << 10);
        }
        if (squash && charSpace) {
            charSpace--;
        }
    }

    box->xpos = xpos - box->textOffsetX;
    box->ypos = ypos - box->textOffsetY;
    fontInUse = fontInUse_saved;
    texDPInit(dList);
    if (box != Window) {
        camSetScissor(dList);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80070518.s")
#endif

#ifdef NON_EQUIVALENT
typedef struct FontJpSpacing {
    u8 spacing[256]; // 256 characters in the japanese font
} FontJpSpacing;
// Japanese Font Header, 0x10 bytes
// Asset45 contains 4 font headers.
typedef struct FontData_JP {
    u8 unk0;
    u8 x;
    u8 y;
    u8 charWidth;
    u8 height;
    u8 pad[3];
    s32 offsetToData;
    s32 bytesPerCharacter;
} FontData_JP;

FontData_JP *dxTable;
s32 fontStringWidth(char *text, s32 font, s32 convertString) {
    FontCharDataAlt *currentFontSpacing;
    FontData_JP *currentFont;
    s32 glyphIndex;
    s32 length;
    s32 retLength;
    char currentChar;

    // do {
    //     currentChar = *input++;

    currentFont = (&dxTable)[font];
    currentFontSpacing = &Font[font];
    if (convertString) {
        fontConvertString(text, convertBuffer);
        text = convertBuffer;
    }
    retLength = 0;
    do {
        length += currentFont->charWidth;
        if (*text & 0x80) {
            glyphIndex = (text[1] & 0xFF) | ((text[0] & 0x7F) << 8);
            text += 2;
            if ((glyphIndex == 0) || (glyphIndex == 15)) {
                length += currentFont->charWidth;
            }
        }
        text++;
        retLength += length;
    } while (*text != '\0');
    return retLength;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/font/fontStringWidth.s")
#endif

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

void *fontWindowAddStringXY(s32 windowId, s32 posX, s32 posY, char *text, s32 number, s32 flags) {
    s32 i;
    s32 width;
    DialogueTextElement *ret;
    s32 pad;
    DialogueBox *textBox;
    DialogueBox **textBoxPtr;
    FontCharDataAlt *fontData;

    if (text == NULL) {
        return NULL;
    }

    for (i = 0, ret = NULL; i < 32 && ret == NULL; i++) {
        if (String[i].number == 255) {
            ret = &String[i];
        }
    }

    if (ret != NULL) {
        if (posX == POS_CENTRED) {
            posX = Window[windowId].width >> 1;
        }
        if (posY == POS_CENTRED) {
            posY = Window[windowId].height >> 1;
        }
        if (Window[windowId].font != FONT_UNK_FF) {
            fontData = &Font[Window[windowId].font];
            if (flags & (HORZ_ALIGN_CENTER | HORZ_ALIGN_RIGHT)) {
                func_80071A0C(text, combineBuffer, number);
                width = fontStringWidth(combineBuffer, Window[windowId].font, 1);
                if (flags & HORZ_ALIGN_RIGHT) {
                    posX = (posX - width) + 1;
                } else {
                    posX = posX - (width >> 1);
                }
            }
            if (flags & VERT_ALIGN_BOTTOM) {
                posY = (posY - fontData->ulx) + 1;
            }
            if (flags & VERT_ALIGN_MIDDLE) {
                posY -= fontData->ulx >> 1;
            }
        }
        if (Window[windowId].textBox == NULL) {
            Window[windowId].textBox = (DialogueBox *) ret;
            ret->nextBox = NULL;
        } else {
            textBoxPtr = &Window[windowId].textBox;
            textBox = *textBoxPtr;
            while (textBox != NULL && number < textBox->textNum) {
                textBoxPtr = &textBox->nextBox;
                textBox = textBox->nextBox;
            }
            *textBoxPtr = (DialogueBox *) ret;
            ret->nextBox = textBox;
        }
        ret->number = number;
        ret->text = text;
        ret->posX = posX;
        ret->posY = posY;
        ret->offsetX = 0;
        ret->offsetY = 0;
        ret->textColourR = Window[windowId].textColourR;
        ret->textColourG = Window[windowId].textColourG;
        ret->textColourB = Window[windowId].textColourB;
        ret->textColourA = Window[windowId].textColourA;
        ret->textBGColourR = Window[windowId].textBGColourR;
        ret->textBGColourG = Window[windowId].textBGColourG;
        ret->textBGColourB = Window[windowId].textBGColourB;
        ret->textBGColourA = Window[windowId].textBGColourA;
        ret->opacity = Window[windowId].opacity;
        ret->font = Window[windowId].font;
        ret->flags = Window[windowId].flags;
    }

    return ret;
}

void fontWindowFlushStrings(s32 windowId) {
    DialogueBoxBackground *dialogueBox;
    DialogueBox *dialogueTextBox, *dialogueTextBoxTemp;

    dialogueBox = &Window[windowId];
    dialogueTextBox = dialogueBox->textBox;
    if (dialogueTextBox != NULL) {
        dialogueTextBoxTemp = dialogueTextBox; // This seems redundant.
        while (dialogueTextBoxTemp != NULL) {
            dialogueTextBoxTemp->textNum = DIALOGUE_NUM_NULL;
            dialogueTextBoxTemp = dialogueTextBoxTemp->nextBox;
        }
        dialogueBox->textBox = NULL;
    }
}

void fontWindowEnable(s32 windowId) {
    Window[windowId].flags |= WINDOW_OPEN;
}

void fontWindowDisable(s32 windowId) {
    Window[windowId].flags &= WINDOW_CLOSED;
}

/**
 * Convert an integer to a string.
 */
void fontStringAddNumber(char **outString, s32 number) {
    u8 digit;
    s32 i;
    s32 hasDigit; // boolean
    s32 div;
    s32 *pow;
    char *ret = *outString;

    // Check for a negative number
    if (number < 0) {
        *ret = '-';
        ret++;
        number = -number;
    }

    // Loop through digit places.
    hasDigit = FALSE;
    for (i = 0; D_800A7874[i] != 0; i++) {
        digit = '0';
        pow = &D_800A7874[i];
        if (number >= *pow) {
            div = number / *pow;
            number -= div * *pow;
            digit += div;
            hasDigit = TRUE;
        }
        if (hasDigit) {
            *ret = digit;
            ret++;
        }
    }

    // One's digit place
    *ret++ = '0' + number;
    *outString = ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowsDraw.s")

// render_fill_rectangle
#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071564.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontWindowDraw.s")

void fontStringAddNumber(char **outString, s32 number);
// parse_string_with_number
void func_80071A0C(char *input, char *output, s32 number) {
    char currentChar;

    do {
        currentChar = *input++;
        if (currentChar & 0x80) {
            char nextChar = *input++;
            if (nextChar == 0xE) {
                fontStringAddNumber(&output, number);
            } else {
                *output++ = currentChar;
                *output++ = nextChar;
            }
        } else if (currentChar == '~') { // ~ is equivalent to a %d.
            fontStringAddNumber(&output, number);
        } else {
            *output++ = currentChar;
        }
    } while (currentChar);
}

#pragma GLOBAL_ASM("asm/nonmatchings/font/func_80071B08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontCreateDisplayList.s")

// char D_800A78C4[] = {
//     0x0F, 0x34, 0x0A, 0x36, 0x02, 0x06, 0x0D, 0x37, 0x03, 0x04, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
//     0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x3E, 0x05, 0x0B, 0x3F, 0x0C, 0x40,
//     0x41, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
//     0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x07, 0x0F, 0x08, 0x0F, 0x0F,
//     0x09, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3,
//     0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0x03, 0x0F, 0x04, 0x0E, 0x0F,
// };
void fontConvertString(char *inString, char *outString) {
    char currentChar;
    char *conversionTable = D_800A78C4;

    do {
        currentChar = *inString++;
        if (currentChar & 0x80) {
            *outString++ = currentChar;
            *outString++ = *inString++;
        } else if (currentChar < 32) {
            *outString++ = currentChar;
        } else {
            *outString++ = 0x80;
            *outString++ = conversionTable[currentChar - 32];
        }
    } while (currentChar);
}

#pragma GLOBAL_ASM("asm/nonmatchings/font/fontGetLine.s")

u8 fontYSpacing(s32 arg0) {
    return Font[arg0].height;
}
