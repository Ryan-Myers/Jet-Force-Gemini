#include "common.h"

// TODO: Get a better size for the number of files
#define NUM_FILES 1

typedef struct {
    u32 fileCount;
    u32 offsets[NUM_FILES];
} Fs;

OSIoMesg gAssetsDmaIoMesg;
OSMesg gDmaMesg;
OSMesgQueue gDmaMesgQueue;
OSMesg gPIMesgBuf[16];
OSMesgQueue gPIMesgQueue;
Fs *gAssetsLookupTable;

extern u8 __ASSETS_LUT_START[], __ASSETS_LUT_END[];

/**
 * Set up the peripheral interface message queues and scheduling.
 * This will send messages when DMA reads are finished.
 * After, allocate space and load the asset table into RAM.
 */
void piInit(void) {
    u32 assetTableSize;
    osCreateMesgQueue(&gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    osCreateMesgQueue(&gDmaMesgQueue, &gDmaMesg, 1);
    osCreatePiManager((OSPri) 150, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    assetTableSize = __ASSETS_LUT_END - __ASSETS_LUT_START;
    gAssetsLookupTable = (Fs *) mmAlloc(assetTableSize, COLOUR_TAG_GREY);
    romCopy((u32) __ASSETS_LUT_START, (u32) gAssetsLookupTable, (s32) assetTableSize);
}

/**
 * Returns the memory address containing an asset section loaded from ROM.
 */
u32 *piRomLoad(u32 assetIndex) {
    u32 *index;
    u32 *out;
    s32 size;
    u32 start;
    if (assetIndex > gAssetsLookupTable->fileCount) {
        return NULL;
    }
    assetIndex++;
    index = assetIndex + gAssetsLookupTable->offsets - 1;
    start = index[0];
    size = index[1] - start;
    if (size == 0) {
        return NULL;
    }
    out = (u32 *) mmAlloc(size, COLOUR_TAG_GREY);
    if (out == 0) {
        return NULL;
    }
    romCopy((u32) (start + __ASSETS_LUT_END), (u32) out, size);
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
    if (assetIndex > gAssetsLookupTable->fileCount) {
        return NULL;
    }
    assetIndex++;
    out = (u8 *) (assetIndex + gAssetsLookupTable->offsets - 1);
    start = ((s32 *) out)[0];
    size = ((s32 *) out)[1] - start;
    gzipHeaderRamPos = (u8 *) mmAlloc(8, COLOUR_TAG_WHITE);
    romCopy((u32) (start + __ASSETS_LUT_END), (u32) gzipHeaderRamPos, 8);
    totalSpace = rzipUncompressSize(gzipHeaderRamPos) + extraMemory;
    mmFree(gzipHeaderRamPos);
    out = (u8 *) mmAlloc(totalSpace + extraMemory, COLOUR_TAG_GREY);
    if (out == NULL) {
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

    if (size == 0 || gAssetsLookupTable->fileCount < assetIndex) {
        return 0;
    }

    assetIndex++;
    index = assetIndex + gAssetsLookupTable->offsets - 1;
    start = index[0] + assetOffset;
    romCopy((u32) (start + __ASSETS_LUT_END), address, size);
    return size;
}

/**
 * Returns a rom offset of an asset given its asset section and a local offset.
 */
u8 *piRomGetSectionPtr(u32 assetIndex, u32 assetOffset) {
    u32 *index;
    u32 start;

    if (assetIndex > gAssetsLookupTable->fileCount) {
        return NULL;
    }

    assetIndex++;
    index = assetIndex + gAssetsLookupTable->offsets - 1;
    start = index[0] + assetOffset;
    return start + __ASSETS_LUT_END;
}

/**
 * Returns the size of an asset section.
 */
s32 piRomGetFileSize(u32 assetIndex) {
    u32 *index;
    s32 size;

    if (assetIndex > gAssetsLookupTable->fileCount) {
        return 0;
    }

    assetIndex++;
    index = assetIndex + gAssetsLookupTable->offsets - 1;
    size = index[1] - index[0];
    return size;
}

#define MAX_TRANSFER_SIZE 0x5000

#ifdef VERSION_us
s32 D_800A3530_A4130 = 0;
#endif

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
        osPiStartDma(&gAssetsDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, romOffset, (u32 *) ramAddress, numBytesToDMA,
                     &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &dmaMesg, OS_MESG_BLOCK);
#ifdef VERSION_us
        if (D_800A3530_A4130 != 0) {
            mainPreNMI();
        }
#endif
        numBytes -= numBytesToDMA;
        romOffset += numBytesToDMA;
        ramAddress += numBytesToDMA;
    }
}
