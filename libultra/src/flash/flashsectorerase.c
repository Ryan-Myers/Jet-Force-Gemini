#include "ultra64.h"
#include "PR/os_internal_flash.h"

s32 osFlashSectorErase(u32 page_num) {
    u32 status;
#ifndef JFGDIFFS
    OSTimer mytimer;
    OSMesgQueue timerMesgQueue;
    OSMesg dummy;
#endif

    // start sector erase operation
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_SECTOR_ERASE | page_num);
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_EXECUTE_ERASE);

#ifndef JFGDIFFS
    // wait for completion by polling erase-busy flag
    osCreateMesgQueue(&timerMesgQueue, &dummy, 1);
#endif

    do {
#ifndef JFGDIFFS
        osSetTimer(&mytimer, OS_USEC_TO_CYCLES(12500), 0, &timerMesgQueue, &dummy);
        osRecvMesg(&timerMesgQueue, &dummy, OS_MESG_BLOCK);
#endif
        osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);
    } while ((status & FLASH_STATUS_ERASE_BUSY) == FLASH_STATUS_ERASE_BUSY);

    // check erase operation status, clear status
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);
    osFlashClearStatus();

    if (((status & 0xFF) == 8) || ((status & 0xFF) == 0x48) || ((status & 8) == 8)) {
        return FLASH_STATUS_ERASE_OK;
    } else {
        return FLASH_STATUS_ERASE_ERROR;
    }
}

#ifdef JFGDIFFS
s32 osFlashWriteBuffer(OSIoMesg* mb, s32 priority, void* dramAddr, OSMesgQueue* mq) {
    s32 ret;

    // select page program mode
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);

    // DMA 128-byte page
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = 0;
    mb->size = FLASH_BLOCK_SIZE;

    ret = osEPiStartDma(&__osFlashHandler, mb, OS_WRITE);

    return ret;
}

s32 osFlashWriteArray(u32 page_num) {
    u32 status;
#ifndef JFGDIFFS
    OSTimer mytimer;
    OSMesgQueue timerMesgQueue;
    OSMesg dummy;
#endif

#ifndef JFGDIFFS
    if ((u32)__osFlashVersion == NEW_FLASH) {
        osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);
    }
#endif

    // start program page operation
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PROGRAM_PAGE | page_num);

#ifndef JFGDIFFS
    // wait for completion by polling write-busy flag
    osCreateMesgQueue(&timerMesgQueue, &dummy, 1);
#endif
    do {
#ifndef JFGDIFFS
        osSetTimer(&mytimer, OS_USEC_TO_CYCLES(200), 0, &timerMesgQueue, &dummy);
        osRecvMesg(&timerMesgQueue, &dummy, OS_MESG_BLOCK);
#endif
        osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);
    } while ((status & FLASH_STATUS_WRITE_BUSY) == FLASH_STATUS_WRITE_BUSY);

#ifdef JFGDIFFS
    status = 0xFFFFFF;
#endif

    // check program operation status, clear status
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);
    osFlashClearStatus();

    if (((status & 0xFF) == 4) || ((status & 0xFF) == 0x44) || ((status & 4) == 4)) {
        return FLASH_STATUS_WRITE_OK;
    } else {
        return FLASH_STATUS_WRITE_ERROR;
    }
}
#endif
