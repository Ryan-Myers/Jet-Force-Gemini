#include "common.h"
void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);

// These are both defined in the generated dkr.ld file.
// extern u8 __ASSETS_LUT_START[], __ASSETS_LUT_END[]; // __ASSETS_LUT_START = 0xECB60, _END = 0xECC30

/**
 * Set up the peripheral interface message queues and scheduling.
 * This will send messages when DMA reads are finished.
 * After, allocate space and load the asset table into RAM.
*/
void piInit(void) {
    u32 assetTableSize;
    osCreateMesgQueue(&D_800FED68, D_800FED28, ARRAY_COUNT(D_800FED28));
    osCreateMesgQueue(&D_800FED10, &D_800FED08, 1);
    osCreatePiManager((OSPri) 150, &D_800FED68, D_800FED28, ARRAY_COUNT(D_800FED28));
    assetTableSize = D_B23E0 - D_B22B0;
    D_800FED80 = (u32 *) mmAlloc(assetTableSize, COLOUR_TAG_GREY);
    romCopy((u32) D_B22B0, (u32) D_800FED80, (s32) assetTableSize);
}

/**
 * Returns the memory address containing an asset section loaded from ROM.
 */
u32 *piRomLoad(u32 assetIndex) {
    u32 *index;
    u32 *out;
    s32 size;
    u32 start;
    if (D_800FED80[0] < assetIndex) {
        return 0;
    }
    assetIndex++;
    index = assetIndex + D_800FED80;
    start = *index;
    size = *(index + 1) - start;
	if (size == 0) {
		return 0;
	}
    out = (u32 *) mmAlloc(size, COLOUR_TAG_GREY);
    if (out == 0) {
        return 0;
    }
    romCopy((u32) (start + D_B23E0), (u32)out, size);
    return out;
}

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomLoadCompressed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomLoadSection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomGetSectionPtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomGetFileSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/romCopy.s")
