#include "common.h"
#include "gameVi.h"
#include "memory.h"

#define SCREEN_HEIGHT_PART (SCREEN_HEIGHT / 40)

extern Gfx D_800A4F20_A5B20[];
extern Gfx D_800A4F48_A5B48[];
extern u32 D_800A4F70_A5B70;
extern Gfx D_A4F78[];

const char D_800ADF00[] = "SCREEN: No out of range!!\n";

void *screenLoad(s32 screenIndex) {
    s32 *screenTable;
    s32 start;
    s32 size;
    s32 count;
    s32 uncompressedSize;
    u8 *decompressedAddr;
    u8 *sp1C;
    u32 compressedAddr;

    screenTable = (s32 *) piRomLoad(0x14);
    for (count = 0; screenTable[count] != -1; count++) {}
    count--;
    if (count == 0) {
        mmFree(screenTable);
        return OS_PHYSICAL_TO_K0(0x100000);
    }

    if ((screenIndex < 0) || (screenIndex >= count)) {
        screenIndex = 0;
    }
    size = screenTable[screenIndex + 1] - screenTable[screenIndex];
    start = screenTable[screenIndex];
    decompressedAddr = NULL;
    sp1C = (u8 *) mmAlloc(0x10, COLOUR_TAG_BLUE);
    if (sp1C != NULL) {
        piRomLoadSection(0x13, (u32) sp1C, start, 0x10);
        uncompressedSize = rzipUncompressSize(sp1C) + 0x80;
        mmFree(sp1C);
        decompressedAddr = (u8 *) mmAlloc(uncompressedSize, COLOUR_TAG_BLUE);
        if (decompressedAddr != NULL) {
            compressedAddr = (u32) ((decompressedAddr + uncompressedSize) - size);
            compressedAddr -= (compressedAddr & 0xF);
            piRomLoadSection(0x13, compressedAddr, start, size);
            rzipUncompress((u8 *) (compressedAddr & 0xFFFFFFFF), decompressedAddr); // fakematch
        }
    }
    mmFree(screenTable);
    return decompressedAddr;
}

void screenDraw(Gfx **dList, u8 *screenAddress, s32 arg2) {
    s32 yl;
    s32 yPos;
    s32 xh;
    s32 xl;
    u32 dsdx;
    u32 dtdy;
    u32 dy;
    u32 width;
    u32 height;

    screenAddress += 0x10;
    viGetCurrentSize((s32 *) &width, (s32 *) &height);
    if (((width == SCREEN_WIDTH) && (height == SCREEN_HEIGHT)) || arg2 == 0) {
        yl = (height - SCREEN_HEIGHT) << 0xF;
        xl = (width - SCREEN_WIDTH) << 1;
        dy = (SCREEN_HEIGHT << 0x10) / 40;
        xh = xl + (SCREEN_WIDTH << 2);
        dsdx = (SCREEN_WIDTH << 10) / SCREEN_WIDTH;
        dtdy = (SCREEN_HEIGHT << 10) / SCREEN_HEIGHT;
        gSPDisplayList((*dList)++, D_800A4F20_A5B20);
    } else {
        yl = 0;
        xl = 0;
        dy = (height << 0x10) / 40;
        xh = width << 2;
        dsdx = (SCREEN_WIDTH << 10) / width;
        dtdy = (SCREEN_HEIGHT << 10) / height;
        gSPDisplayList((*dList)++, D_800A4F48_A5B48);
    }

    for (yPos = 0; yPos != SCREEN_HEIGHT; yPos += SCREEN_HEIGHT_PART) {
        (*dList)->words.w0 = D_800A4F70_A5B70;
        (*dList)->words.w1 = (u32) screenAddress;
        (*dList)++;
        gDkrDmaDisplayList((*dList)++, (u32) D_A4F78, 6);
        gSPTextureRectangle((*dList)++, xl, yl >> 0xE, xh, (s32) (yl + dy) >> 0xE, 0, 0, 0, dsdx, dtdy);
        screenAddress += SCREEN_WIDTH * SCREEN_HEIGHT_PART * 2;
        yl += dy;
    }
    texDPInit(dList);
}
