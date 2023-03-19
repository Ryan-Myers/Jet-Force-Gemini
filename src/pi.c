#include "common.h"
void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);

extern OSMesg D_800FED28[16];//gPIMesgBuf[16];
extern OSMesgQueue D_800FED68;//gPIMesgQueue;
extern OSMesg D_800FED08;//gDmaMesg;
extern OSMesgQueue D_800FED10;//gDmaMesgQueue;
extern u32 *D_800FED80;//gAssetsLookupTable;
extern u8 D_B22B0[], D_B23E0[];
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

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomLoad.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomLoadCompressed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomLoadSection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomGetSectionPtr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/piRomGetFileSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/pi/romCopy.s")
