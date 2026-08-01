#include "common.h"

s32 piRomGetFileSize(u32 assetIndex);
extern s8 D_800A6FB0_A7BB0;
extern s32 D_80104560_B1750;
extern s16 D_80104564_B1754; //gTextTableEntries?
extern s16 D_80104566_B1756;
extern s16 D_80104568_B1758;
extern s16 D_8010456C_B175C;
extern s16 D_8010456E_B175E;
extern s16 D_80104570_B1760;
extern s16 D_80104572_B1762;
extern s16 D_80104574_B1764;
extern s16 D_80104578_B1768;
extern char *D_80104588_B1778[2]; //gGameTextTableEntries?
extern s32 D_80104594_B1784;

#if 1
/**
 * Initializes the subtitle system.
 * Allocates memory for the subtitle buffers and sets up the default values.
 * Similar to load_game_text_table in DKR
 * Official Name: subtitlesInit
 */
void subtitlesInit(void) {
    D_80104564_B1754 = (piRomGetFileSize(5) >> 2) - 2;
    D_80104588_B1778[0] = (char *) mmAlloc(0x790, COLOUR_TAG_GREEN);
    D_80104588_B1778[1] = &D_80104588_B1778[0][960];
    D_80104560_B1750 = (s32) &D_80104588_B1778[1][960];
    D_80104594_B1784 = 0;
    D_80104574_B1764 = 0;
    D_80104566_B1756 = 0;
    D_80104568_B1758 = 0x20;
    D_80104578_B1768 = 0;
    D_8010456C_B175C = 0x20;
    D_80104570_B1760 = 0x120;
    if (osTvType == OS_TV_TYPE_PAL) {
        D_8010456E_B175E = 224;
        D_80104572_B1762 = 248;
    } else {
        D_8010456E_B175E = 202;
        D_80104572_B1762 = 222;
    }
    D_800A6FB0_A7BB0 = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitlesInit.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitlesFree.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitlesEnable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/func_80073F38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/func_800741AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitlesTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/subtitles/subtitleStart.s")
