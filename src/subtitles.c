#include "common.h"
#include "font.h"

s32 piRomGetFileSize(u32 assetIndex);
extern s8 D_800A6FB0_A7BB0;
extern s32 D_80104560_B1750;
extern s16 gTextTableEntries;
extern s16 gTextAlphaVelocity;
extern s16 gShowSubtitles;
extern char *gGameTextTableEntries[2];
extern s32 D_80104594_B1784;
extern s16 gDialogueAlpha;
extern s16 gDialogueXPos1;
extern s16 gDialogueYPos1;
extern s16 gDialogueXPos2;
extern s16 gDialogueYPos2;
extern s16 gSubtitleLineCount;
extern char *gSubtitleProperties[1];
extern s32 gSubtitleSetting;
extern char *gCurrentTextProperties;
extern s16 gShowSubtitles;
extern s16 gSubtitleTimer;
extern s16 gCurrentTextID;

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
    gShowSubtitles = FALSE;
    gDialogueAlpha = 0;
    gTextAlphaVelocity = 0x20;
    gCurrentTextID = 0;
    gDialogueXPos1 = 0x20;
    gDialogueXPos2 = 0x120;
    if (osTvType == OS_TV_TYPE_PAL) {
        gDialogueYPos1 = 224;
        gDialogueYPos2 = 248;
    } else {
        gDialogueYPos1 = 202;
        gDialogueYPos2 = 222;
    }
    D_800A6FB0_A7BB0 = TRUE;
}

void subtitlesFree(void) {
    if (D_800A6FB0_A7BB0) {
        mmFree(gGameTextTableEntries[0]);
        fontWindowDisable(6);
        fontWindowFlushStrings(6);
        D_800A6FB0_A7BB0 = FALSE;
        gShowSubtitles = FALSE;
    }
}

void subtitlesEnable(s32 setting) {
    gSubtitleSetting = setting;
}

enum TextProperties {
    TEXT_NONE,
    TEXT_COL_R,
    TEXT_COL_G,
    TEXT_COL_B,
    TEXT_ALPHA,
    TEXT_FONT,
    TEXT_FLAGS,
};

void render_subtitles(void) {
    s32 textX;
    s32 textY;
    s32 i;
    s32 textFlags;
    char **textData;

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
        fontWindowFontColour(6, textData[i][TEXT_COL_R], textData[i][TEXT_COL_G], textData[i][TEXT_COL_B], 255,
                             (textData[i][TEXT_ALPHA] * gDialogueAlpha) >> 8);
        fontWindowAddStringXY(6, textX, textY, textData[i] + 8, 1, textFlags);
        fontWindowFontColour(6, 0, 0, 0, 255, (gDialogueAlpha * 255) >> 8);
        fontWindowAddStringXY(6, textX + 1, textY + 1, textData[i] + 8, 1, textFlags);
        textY += 14;
    }
    fontWindowEnable(6);
}

/**
 * Get the line count and text timer from the next message of the subtitle.
 * Close the subtitles if none can be found.
 */
void find_next_subtitle(void) {
    u32 new_var3;
    u8 new_var;
    s32 new_var2;
    s32 done;

    gSubtitleLineCount = 0;
    gSubtitleTimer = 0;
    done = FALSE;
    while (gCurrentTextProperties[0] != NULL && done == FALSE) {
        gCurrentTextID = gCurrentTextProperties[0] - 1;
        gSubtitleProperties[gSubtitleLineCount] = gCurrentTextProperties;
        gSubtitleTimer = objTvTimes(gCurrentTextProperties[7] * 6);
        gCurrentTextProperties += 8;
        do {
            new_var = gCurrentTextProperties[0];
            if (new_var & 0x80) {
                gCurrentTextProperties += 2;
            } else {
                gCurrentTextProperties++;
            }
        } while (gCurrentTextProperties[0] != NULL);
        gSubtitleLineCount++;
        if (gSubtitleLineCount >= 2) {
            done = TRUE;
        }
        gCurrentTextProperties++;
        new_var2 = gCurrentTextProperties[0];
        if (gCurrentTextProperties[0] == 10) {
            gCurrentTextProperties++;
        } else if (new_var2 == 12) {
            gCurrentTextProperties++;
            done = TRUE;
        }
    }
    if (gSubtitleLineCount > 0) {
        gShowSubtitles = TRUE;
    }
}

/**
 * Handle the subtitle system from here.
 * Slowly show the text, tick down the timer, find the next message or close the box, then render.
 * Official Name: subtitlesTick
 */
void subtitlesTick(s32 updateRate) {
    if (D_800A6FB0_A7BB0) {
        if (gSubtitleSetting == FALSE) {
            gShowSubtitles = FALSE;
        }
        if (gShowSubtitles) {
            if (gSubtitleTimer <= 0) {
                gDialogueAlpha -= updateRate * gTextAlphaVelocity;
                if (gDialogueAlpha < 0) {
                    gDialogueAlpha = 0;
                    gShowSubtitles = FALSE;
                    fontWindowDisable(6);
                    fontWindowFlushStrings(6);
                }
            } else {
                gDialogueAlpha += updateRate * gTextAlphaVelocity;
                if (gDialogueAlpha > 256) {
                    gDialogueAlpha = 256;
                }
                gSubtitleTimer -= updateRate;
                if (gSubtitleTimer <= 0) {
                    find_next_subtitle();
                }
            }
        }
        if (gShowSubtitles) {
            render_subtitles();
        }
    }
}

void subtitleStart(s32 textID) {
    char **entries;
    Language language;
    s32 temp;
    s32 size;

    if (D_800A6FB0_A7BB0 && textID >= 0 && textID < gTextTableEntries) {
        language = frontGetLanguage();
        switch (language) {
            case LANGUAGE_2:
                textID += 48;
                break;
            case LANGUAGE_1:
                textID += 47;
                break;
            case LANGUAGE_3:
                textID += 49;
                break;
            case LANGUAGE_JAPANESE:
                textID += 50;
                break;
        }
        piRomLoadSection(5, (u32) D_80104560_B1750, (textID & ~1) << 2, 16);
        entries = (char **) D_80104560_B1750;
        temp = ((s32) entries[textID & 1]) & 0xFF000000;
        size = (((s32) entries[(textID & 1) + 1]) & 0xFFFFFF) - (((s32) entries[textID & 1]) & 0xFFFFFF);
        piRomLoadSection(4, (u32) gGameTextTableEntries[D_80104594_B1784], ((s32) entries[textID & 1]) ^ temp, size);
        gCurrentTextProperties = gGameTextTableEntries[D_80104594_B1784];
        find_next_subtitle();
        D_80104594_B1784 = (D_80104594_B1784 + 1) & 1;
    }
}
