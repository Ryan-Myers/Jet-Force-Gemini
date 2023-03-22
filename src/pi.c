#include "common.h"

OSIoMesg D_800FECF0_FF8F0; //gAssetsDmaIoMesg;
OSMesg D_800FED08_FF908; //gDmaMesg;
OSMesgQueue D_800FED10_FF910; //gDmaMesgQueue;
OSMesg D_800FED28_FF928[16]; //gPIMesgBuf[16];
OSMesgQueue D_800FED68_FF968; //gPIMesgQueue
u32 *D_800FED80_FF980; //gAssetsLookupTable

extern u8 __ASSETS_LUT_START[], __ASSETS_LUT_END[];

/**
 * Set up the peripheral interface message queues and scheduling.
 * This will send messages when DMA reads are finished.
 * After, allocate space and load the asset table into RAM.
*/
void piInit(void) {
    u32 assetTableSize;
    osCreateMesgQueue(&D_800FED68_FF968, D_800FED28_FF928, ARRAY_COUNT(D_800FED28_FF928));
    osCreateMesgQueue(&D_800FED10_FF910, &D_800FED08_FF908, 1);
    osCreatePiManager((OSPri) 150, &D_800FED68_FF968, D_800FED28_FF928, ARRAY_COUNT(D_800FED28_FF928));
    assetTableSize = __ASSETS_LUT_END - __ASSETS_LUT_START;
    D_800FED80_FF980 = (u32 *) mmAlloc(assetTableSize, COLOUR_TAG_GREY);
    romCopy((u32) __ASSETS_LUT_START, (u32) D_800FED80_FF980, (s32) assetTableSize);
}

/**
 * Returns the memory address containing an asset section loaded from ROM.
 */
u32 *piRomLoad(u32 assetIndex) {
    u32 *index;
    u32 *out;
    s32 size;
    u32 start;
    if (D_800FED80_FF980[0] < assetIndex) {
        return 0;
    }
    assetIndex++;
    index = assetIndex + D_800FED80_FF980;
    start = *index;
    size = *(index + 1) - start;
	if (size == 0) {
		return 0;
	}
    out = (u32 *) mmAlloc(size, COLOUR_TAG_GREY);
    if (out == 0) {
        return 0;
    }
    romCopy((u32) (start + __ASSETS_LUT_END), (u32)out, size);
    return out;
}

/**
 * Loads a gzip compressed asset from the ROM file.
 * Returns a pointer to the decompressed data.
 */
UNUSED u8 *piRomLoadCompressed(u32 assetIndex, s32 extraMemory) {
    s32 size;
    s32 start;
    s32 totalSpace;
    u8 *gzipHeaderRamPos;
    u8 *out;
    if (D_800FED80_FF980[0] < assetIndex) {
        return NULL;
    }
    assetIndex++;
    out = (u8 *) (assetIndex + D_800FED80_FF980);
    start = ((s32 *) out)[0];
    size = ((s32 *) out)[1] - start;
    gzipHeaderRamPos = (u8 *) mmAlloc(8, COLOUR_TAG_WHITE);
    romCopy((u32) (start + __ASSETS_LUT_END), (u32) gzipHeaderRamPos, 8);
    totalSpace = rzipUncompressSize(gzipHeaderRamPos) + extraMemory;
    mmFree(gzipHeaderRamPos);
    out = (u8 *) mmAlloc(totalSpace + extraMemory, COLOUR_TAG_GREY);
    if (out == NULL){
        return NULL;
    }
    gzipHeaderRamPos = (out + totalSpace) - size;
    if (1) {} // Fakematch
    romCopy((u32) (start + __ASSETS_LUT_END), (u32) gzipHeaderRamPos, size);
    rzipUncompress(gzipHeaderRamPos, out);
    return out;
}

/**
 * Loads part of an asset section to a specific memory address.
 * Returns the size argument.
 */
s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size) {
    u32 *index;
    s32 start;

    if (size == 0 || D_800FED80_FF980[0] < assetIndex) {
        return 0;
    }

    assetIndex++;
    index = assetIndex + D_800FED80_FF980;
    start = *index + assetOffset;
    romCopy((u32) (start + __ASSETS_LUT_END), address, size);
    return size;
}

/**
 * Returns a rom offset of an asset given its asset section and a local offset.
 */
u8 *piRomGetSectionPtr(u32 assetIndex, u32 assetOffset) {
    u32 *index;
    u32 start;

    if (D_800FED80_FF980[0] < assetIndex) {
        return NULL;
    }

    assetIndex++;
    index = assetIndex + D_800FED80_FF980;
    start = *index + assetOffset;
    return start + __ASSETS_LUT_END;
}

/**
 * Returns the size of an asset section.
 */
s32 piRomGetFileSize(u32 assetIndex) {
    u32 *index;

    if (D_800FED80_FF980[0] < assetIndex) {
        return 0;
    }

    assetIndex++;
    index = assetIndex + D_800FED80_FF980;
    return *(index + 1) - *index;
}

#define MAX_TRANSFER_SIZE 0x5000

/**
 * Copies data from the game cartridge to a ram address.
 */
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes) {
    OSMesg dmaMesg;
    s32 numBytesToDMA;

    osInvalDCache((u32 *) ramAddress, numBytes);
    numBytesToDMA = MAX_TRANSFER_SIZE;
    while (numBytes > 0) {
        if (numBytes < numBytesToDMA) {
            numBytesToDMA = numBytes;
        }
        osPiStartDma(&D_800FECF0_FF8F0, OS_MESG_PRI_NORMAL, OS_READ, romOffset, (u32 *) ramAddress, numBytesToDMA, &D_800FED10_FF910);
        osRecvMesg(&D_800FED10_FF910, &dmaMesg, OS_MESG_BLOCK);
        numBytes -= numBytesToDMA;
        romOffset += numBytesToDMA;
        ramAddress += numBytesToDMA;
    }
}
