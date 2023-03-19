#include "common.h"

void *mmAlloc(s32 size, u32 colourTag);

extern huft *D_800A7960;// = NULL;
extern s32 *D_800A7964;// = NULL;

void rzipInit(void) {
    D_800A7960 = mmAlloc(0x2800, COLOUR_TAG_BLACK);
    D_800A7964 = mmAlloc(0x10, COLOUR_TAG_BLACK);
}

#pragma GLOBAL_ASM("asm/nonmatchings/rzip/rzipUncompressSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rzip/rzipUncompressSizeROM.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rzip/rzipUncompress.s")

#pragma GLOBAL_ASM("asm/nonmatchings/rzip/huft_build.s")
