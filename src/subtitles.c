#include "common.h"
#include "font.h"

s32 piRomGetFileSize(u32 assetIndex);
extern s8 D_800A6FB0_A7BB0;
extern s32 D_80104560_B1750;
extern s16 gTextTableEntries; //gTextTableEntries?
extern s16 D_80104568_B1758;
extern s16 D_80104574_B1764;
extern s16 D_80104578_B1768;
extern char *gGameTextTableEntries[2]; //gGameTextTableEntries?
extern s32 D_80104594_B1784;
extern s16 gDialogueAlpha; // gDialogueAlpha
extern s16 gDialogueXPos1; // gDialogueXPos1
extern s16 gDialogueYPos1; // gDialogueYPos1
extern s16 gDialogueXPos2; // gDialogueXPos2
extern s16 gDialogueYPos2; // gDialogueYPos2
extern s16 gSubtitleLineCount; // gSubtitleLineCount
extern char *gSubtitleProperties[1]; // gSubtitleProperties
extern s32 D_800A6FB8_A7BB8;

/**
 * Initializes the subtitle system.
 * Allocates memory for the subtitle buffers and sets up the default values.
 * Similar to load_game_text_table in DKR
 * Official Name: subtitlesInit
 */
void subtitlesInit(void) {
    gTextTableEntries = (piRomGetFileSize(5) >> 2) - 2;
    gGameTextTableEntries[0] = (char *) mmAlloc(0x790, COLOUR_TAG_GREEN);
    gGameTextTableEntries[1] = &gGameTextTableEntries[0][960];
    D_80104560_B1750 = (s32) &gGameTextTableEntries[1][960];
    D_80104594_B1784 = 0;
    D_80104574_B1764 = 0;
    gDialogueAlpha = 0;
    D_80104568_B1758 = 0x20;
    D_80104578_B1768 = 0;
    gDialogueXPos1 = 0x20;
    gDialogueXPos2 = 0x120;
    if (osTvType == OS_TV_TYPE_PAL) {
        gDialogueYPos1 = 224;
        gDialogueYPos2 = 248;
    } else {
        gDialogueYPos1 = 202;
        gDialogueYPos2 = 222;
    }
    D_800A6FB0_A7BB0 = 1;
}

void subtitlesFree(void) {
    if (D_800A6FB0_A7BB0 != 0) {
        mmFree(gGameTextTableEntries[0]);
        fontWindowDisable(6);
        fontWindowFlushStrings(6);
        D_800A6FB0_A7BB0 = 0;
        D_80104574_B1764 = 0;
    }
}

void subtitlesEnable(s32 arg0) {
    D_800A6FB8_A7BB8 = arg0;
}

enum TextProperties {
    TEXT_NONE,
    TEXT_COL_R,
    TEXT_COL_G,
    TEXT_COL_B,
    TEXT_ALPHA,
    TEXT_FONT,
    TEXT_FLAGS
};

//render_subtitles in DKR
void func_80073F38(void) {
    s32 textX;
    s32 textY;
    s32 i;
    s32 textFlags;
    char **textData;
    #define SUBTITLE_Y_OFFSET 14

    fontWindowFlushStrings(6);
    fontWindowSize(6, gDialogueXPos1, gDialogueYPos1, gDialogueXPos2, gDialogueYPos2);
    fontWindowColour(6, 0, 96, 0, (gDialogueAlpha * 100) >> 8);
    fontWindowFontBackground(6, 0, 0, 0, 0);
    textY = ((((gDialogueYPos2 - gDialogueYPos1) - (gSubtitleLineCount * 12)) - (gSubtitleLineCount * 2)) + 2) >> 1;
    for (i = 0; i < gSubtitleLineCount; i++) {
        textData = &gSubtitleProperties[0];
        fontWindowUseFont(6, (s32) textData[i][TEXT_FONT]);
        textFlags = textData[i][TEXT_FLAGS];
        if (textFlags == ALIGN_TOP_CENTER) {
            textX = (gDialogueXPos2 - gDialogueXPos1) >> 1;
        } else {
            if (textFlags == ALIGN_TOP_RIGHT) {
                textX = (gDialogueXPos2 - gDialogueXPos1) - 8;
            } else {
                textX = 8;
            }
        }
        fontWindowFontColour(6, textData[i][TEXT_COL_R], textData[i][TEXT_COL_G], textData[i][TEXT_COL_B], 255, (textData[i][TEXT_ALPHA] * gDialogueAlpha) >> 8);
        fontWindowAddStringXY(6, textX, textY, textData[i] + 8, 1, textFlags);
        fontWindowFontColour(6, 0, 0, 0, 255, (gDialogueAlpha * 255) >> 8);
        fontWindowAddStringXY(6, textX + 1, textY + 1, textData[i] + 8, 1, textFlags);
        textY += SUBTITLE_Y_OFFSET;
    }
    fontWindowEnable(6);
}

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/func_800741AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitlesTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitleStart.s")
