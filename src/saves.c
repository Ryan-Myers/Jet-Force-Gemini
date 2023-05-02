#include "common.h"

// These are the unique game codes assigned to DKR
#define NDYJ 0x4E44594A // 'NDYJ' ASCII
#define NDYP 0x4E445950 // 'NDYP' ASCII
#define NDYE 0x4E445945 // 'NDYE' ASCII
#define JPN_GAME_CODE NDYJ
#define PAL_GAME_CODE NDYP
#define NTSC_GAME_CODE NDYE
#define CHARFILETYPE 0x43484152
// This is RARE's unique code
#define COMPANY_CODE 0x3459

//The size of 1 sector is 128 pages (16K bytes), and each page of 0~0x7f, 0x80~0xff, 0x100~0x17f... is considered to be 1 sector.
//So basically, flash is written in sectors, and changing a single page needs to read a sector, modify the page, and write back the whole sector.
#define SECTOR_SIZE 128

typedef enum Language {
    LANGUAGE_0,
    LANGUAGE_1,
    LANGUAGE_2,
    LANGUAGE_3,
    LANGUAGE_JAPANESE
} Language;

UNUSED void rumbleProcessing(s32 arg0) {
    if ((arg0 != 0) && (D_800A3ECC_A4ACC == 0)) {
        D_800A3EC4_A4AC4 = 1;
        D_800A3ECC_A4ACC = 1;
    } else {
        D_800A3ECC_A4ACC = 0;
    }
}

UNUSED void rumbleStart(s32 controllerIndex, s32 arg1, f32 arg2) {
    RumbleStruct *rumblePak;
    u8 controllerNum;

    if (controllerIndex >= 0 && controllerIndex < MAXCONTROLLERS) {
        controllerNum = joyGetController(controllerIndex);
        rumblePak = &D_800FEC68_FF868[controllerNum];
        if (rumblePak->state.upper != 2) {
            rumblePak->state.state = (rumblePak->state.state & ~0xF0) | 0x10;
            rumblePak->unk2 = ((arg1 * arg1) * D_800AD4FC_AE0FC);
            rumblePak->unk4 = rumblePak->unk2;
            rumblePak->rumbleTime = (arg2 * 60.0f);
        }
    }
}

void rumbleStop(s32 controllerIndex) {
    RumbleStruct *rumblePak;
    u8 controllerNum;
    u32 temp;
    s32 flag = 3;

    if (controllerIndex >= 0 && controllerIndex < MAXCONTROLLERS) {
        controllerNum = joyGetController(controllerIndex);
        rumblePak = &D_800FEC68_FF868[controllerNum];
        temp = rumblePak->state.upper;
        if ((temp != 0) && (temp != flag)) {
            rumblePak->state.flag = flag;
            rumblePak->state.state = (rumblePak->state.state & ~0xF0) | 0x30;
        }
    }
}

void rumbleAlter(s32 controllerIndex, s32 arg1, f32 arg2) {
    s32 controllerNum;
    RumbleStruct *rumblePak;

    if (controllerIndex >= 0 && controllerIndex < MAXCONTROLLERS) {
        controllerNum = joyGetController(controllerIndex);
        rumblePak = &D_800FEC6A_FF86A[controllerNum];
        if (arg1 != 0) {
            rumblePak->state.half = ((arg1 * arg1) * D_800AD500_AE100);
        }
        rumblePak = &D_800FEC68_FF868[controllerNum];
        if (rumblePak->state.upper != 2 && arg2 != 0.0f) {
            rumblePak->state.state = (rumblePak->state.state & ~0xF0) | 0x10;
            rumblePak->rumbleTime = (arg2 * 60.0f);
        }
    }
}

void rumbleMax(s32 controllerIndex, s32 arg1, f32 arg2) {
    RumbleStruct *rumblePak;
    s32 temp_f16;
    s32 controllerNum;

    if (controllerIndex >= 0 && controllerIndex < MAXCONTROLLERS) {
        controllerNum = joyGetController(controllerIndex);
        rumblePak = &D_800FEC68_FF868[controllerNum];
        if (arg1 != 0) {
            arg1 = ((arg1 * arg1) * D_800AD504_AE104);
            if (rumblePak->unk2 < arg1) {
                rumblePak->unk2 = arg1;
            }
        }
        if (rumblePak->state.upper != 2) {
            rumblePak->state.state = (rumblePak->state.state & ~0xF0) | 0x10;
            temp_f16 = (arg2 * 60.0f);
            if (rumblePak->rumbleTime < temp_f16) {
                rumblePak->rumbleTime = temp_f16;
            }
        }
    }
}

void rumbleKill(void) {
    s32 i = MAXCONTROLLERS;
    while (i--) {
        rumbleStop(i);
    }
}

void rumbleUpdate(void) {
    D_800A3EC4_A4AC4 = 1;
}

#ifdef NON_EQUIVALENT
s32 nosMotorStart(OSPfs *);
s32 nosMotorStop(OSPfs *);
extern f32 D_800AD508_AE108;
extern RumbleStruct D_800FEC68_FF868[];

void rumbleTick(s32 updateRate) {
    RumbleStruct *rumble;
    s32 pfsStatus;
    s32 i;
    s32 controllerToCheck;
    u8 pfsBitPattern;

    if (D_800A3ECC_A4ACC != 0) {
        if (D_800A3EC4_A4AC4 != 0) {
            osPfsIsPlug(sControllerMesgQueue, &pfsBitPattern);
            for (i = 0, controllerToCheck = 1, rumble = D_800FEC68_FF868; i < MAXCONTROLLERS; i++, controllerToCheck <<= 1, rumble++) {
                if (pfsBitPattern & controllerToCheck) {
                    if (nosMotorInit(sControllerMesgQueue, &pfs[i], i) != 0) {
                        rumble->state.state &= ~4;
                        sRumblePaksPresent &= ~controllerToCheck;
                    } else {
                        rumble->state.state |= 4;
                        sRumblePaksPresent |= controllerToCheck;
                    }
                }
            }
            D_800A3EC4_A4AC4 = 0;
        }
        for (i = 0, controllerToCheck = 1, rumble = D_800FEC68_FF868; i < MAXCONTROLLERS; i++, controllerToCheck <<= 1, rumble++) {
            if (rumble->state.upper & 0x400) {
                pfsStatus = 0;
                switch (rumble->state.upper) {
                case 1:
                    rumble->rumbleTime -= updateRate;
                    rumble->timer += updateRate;
                    if (rumble->rumbleTime <= 0) {
                        rumble->state.flag = 3;
                        rumble->state.state = (rumble->state.state & ~0xF0) | 0x30;
                    } else if (rumble->timer >= 0xF0) {
                        rumble->timer = 60;
                        rumble->state.state = (rumble->state.state & ~0xF0) | 0x20;
                        rumble->state.flag = 3;
                    } else {
                        if (rumble->unk2 > 490.0f) {
                            if (!(rumble->state.half_unsigned & 0x800)) {
                                pfsStatus = nosMotorStart(&pfs[i]);
                                rumble->state.state |= 8;
                            }
                        } else if (rumble->unk2 < D_800AD508_AE108) {
                            if (rumble->state.half_unsigned & 0x800) {
                                pfsStatus = nosMotorStop(&pfs[i]);
                                rumble->state.state &= ~8;
                            }
                        } else {
                            if (rumble->unk4 >= 0x100) {
                                rumble->unk4 -= 0x100;
                                if (!(rumble->state.half_unsigned & 0x800)) {
                                    pfsStatus = nosMotorStart(&pfs[i]);
                                    rumble->state.state |= 8;
                                    rumble->unk4 -= 0x100;
                                }
                            } else {
                                s32 var_t6;
                                var_t6 = rumble->unk4 + rumble->unk2;
                                if (rumble->state.half_unsigned & 0x800) {
                                    pfsStatus = nosMotorStop(&pfs[i]);
                                    rumble->state.state &= ~8;
                                    var_t6 = rumble->unk4 + rumble->unk2;
                                }
                                rumble->unk4 = var_t6 + 4;
                            }
                        }
                    }
                    break;
                case 2:
                    if (rumble->state.lower != 0) {
                        if (nosMotorInit(sControllerMesgQueue, &pfs[i], i) == 0) {
                            nosMotorStop(&pfs[i]);
                        }
                        rumble->rumbleTime = 0;
                        rumble->state.state &= ~8;
                        rumble->state.flag = (s8) rumble->state.flag - 1;
                    }
                    rumble->timer -= updateRate;
                    if (rumble->timer <= 0) {
                        rumble->timer = 0;
                        rumble->state.state &= ~0xF0;
                    }
                    break;
                case 3: {
                    u8 temp_t2;
                    if (nosMotorInit(sControllerMesgQueue, &pfs[i], i) == 0) {
                        nosMotorStop(&pfs[i]);
                    }
                    rumble->state.lower--;
                    temp_t2 = rumble->state.state & ~8;
                    rumble->rumbleTime = 0;
                    rumble->timer = 0;
                    rumble->state.state = temp_t2;
                    if (rumble->state.lower == 0) {
                        rumble->state.state = temp_t2 & 0xF;
                    }
                    break;
                }
                }
                if (pfsStatus != 0) {
                    rumble->state.state &= ~4;
                    sRumblePaksPresent &= ~controllerToCheck;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleTick.s")
#endif

UNUSED void rumbleGetRumble(s32 arg0, s32 *arg1, f32 *arg2) {
    *arg1 = 0;
    *arg2 = 0;
    if ((arg0 >= 0) && (arg0 < 3)) {
        *arg1 = D_800A3EAC_A4AAC[arg0].unk0;
        *arg2 = D_800A3EAC_A4AAC[arg0].unk4;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004C2A8_4CEA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packLoadCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packSaveCharacter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packLoadGameEprom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packSaveGameEprom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packClearGameEprom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packEraseEprom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004C9B8_4D5B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packLoadGlobalFlagsEprom.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packSaveGlobalFlagsEprom.s")

void flashROMInit(void) {
    osCreateMesgQueue(&cartEventQueue, (OSMesg*)&cartEventBuf, 1);
    osSetEventMesg(OS_EVENT_CART, &cartEventQueue, (OSMesg)1);
    osCreateMesgQueue(&flashEventQueue, (OSMesg*)&flashEventBuf, 1);
    osCartRomInit();
    osFlashInit();
}

void flashROMWrite(u32 pageNum, u32 *dramAddr) {
    //Page 0x3fe and 0x3ff are reserved by Nintendo.  Please refrain from using the pages. 
    if (pageNum < 0x3FE) {
        osWritebackDCache(dramAddr, SECTOR_SIZE);
        //Transfer data from RDRAM to the write buffer in 1M Flash 
        osFlashWriteBuffer(&flashMesgReqBlock, 0, dramAddr, &flashEventQueue);
        osRecvMesg(&flashEventQueue, NULL, OS_MESG_BLOCK);
        //Transfer data from write buffer to each page of 1M Flash 
        osFlashWriteArray(pageNum);
    }
}

void flashROMRead(u32 pageNum, u32 *dramAddr) {
    //Page 0x3fe and 0x3ff are reserved by Nintendo.  Please refrain from using the pages. 
    if (pageNum < 0x3FE) {
        osInvalDCache(dramAddr, SECTOR_SIZE);
        osFlashReadArray(&flashMesgReqBlock, 0, pageNum, dramAddr, 1, &flashEventQueue);
        osRecvMesg(&flashEventQueue, NULL, OS_MESG_BLOCK);
    }
}

SIDeviceStatus packOpen(s32 controllerIndex) {
    OSMesg unusedMsg;
    s32 ret;
    s32 bytes_not_used;
    s32 i;

    if (sControllerMesgQueue->validCount == 0) {
        if (nosMotorInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == 0) {
            return RUMBLE_PAK;
        }
    }

    //Couldn't get a for loop to match this
    i = 0;
    while (sControllerMesgQueue->validCount != 0 && i < 10) {
        osRecvMesg(sControllerMesgQueue, &unusedMsg, OS_MESG_NOBLOCK);
        i++;
    }

    for (i = 0; i <= 4; i++) {
        ret = osPfsFreeBlocks( &pfs[controllerIndex], &bytes_not_used);
        if (ret == PFS_ERR_INVALID) {
            ret = osPfsInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex);
        }
        if (ret == PFS_ERR_ID_FATAL) {
            if (nosMotorInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == 0) {
                return RUMBLE_PAK;
            }
        }
        if (ret == PFS_ERR_NEW_PACK) {
            if ((osPfsInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == PFS_ERR_ID_FATAL)
                && (nosMotorInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == 0)) {
                return RUMBLE_PAK;
            }
            return CONTROLLER_PAK_CHANGED;
        }
        if (ret == PFS_ERR_NOPACK || ret == PFS_ERR_DEVICE) {
            return NO_CONTROLLER_PAK;
        }
        if (ret == PFS_ERR_BAD_DATA) {
            return CONTROLLER_PAK_BAD_DATA;
        }
        if (ret == PFS_ERR_ID_FATAL) {
            return CONTROLLER_PAK_WITH_BAD_ID;
        }
        if (ret == PFS_ERR_INCONSISTENT) {
            return CONTROLLER_PAK_INCONSISTENT;
        }
        if (ret == 0) {
            return CONTROLLER_PAK_GOOD;
        }
    }

    return NO_CONTROLLER_PAK;
}

s32 packClose(UNUSED s32 controllerIndex) {
    osContStartReadData(sControllerMesgQueue);
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packInit.s")

SIDeviceStatus packIsPresent(s32 controllerIndex) {
    s32 ret;

    ret = packOpen(controllerIndex);
    packClose(controllerIndex);

    if (ret == RUMBLE_PAK) {
        sRumblePaksPresent |= 1 << controllerIndex;
    }

    return ret;
}


SIDeviceStatus packRepair(s32 controllerIndex) {
    s32 ret;
    s32 status = packOpen(controllerIndex);
    if (status == CONTROLLER_PAK_GOOD || status == CONTROLLER_PAK_INCONSISTENT) {
        status = osPfsChecker(&pfs[controllerIndex]);
        if (status == 0) {
            ret = CONTROLLER_PAK_GOOD;
        } else if (status == PFS_ERR_NEW_PACK) {
            ret = CONTROLLER_PAK_CHANGED;
        } else {
            ret = CONTROLLER_PAK_INCONSISTENT;
        }
    } else {
        ret = CONTROLLER_PAK_GOOD;
    }
    packClose(controllerIndex);
    return ret;
}

SIDeviceStatus packFormat(s32 controllerIndex) {
    s32 ret;
    s32 status = packOpen(controllerIndex);
    if (status == CONTROLLER_PAK_GOOD || status == CONTROLLER_PAK_INCONSISTENT || status == CONTROLLER_PAK_WITH_BAD_ID) {
        status = osPfsReFormat(&pfs[controllerIndex], sControllerMesgQueue, controllerIndex);
        if (status == 0) {
            ret = CONTROLLER_PAK_GOOD;
        } else if (status == PFS_ERR_NEW_PACK) {
            ret = CONTROLLER_PAK_CHANGED;
        } else {
            ret = CONTROLLER_PAK_INCONSISTENT;
        }
    } else {
        ret = CONTROLLER_PAK_GOOD;
    }
    packClose(controllerIndex);
    return ret;
}

SIDeviceStatus packDirectory(s32 controllerIndex, s32 maxNumOfFilesToGet, char **fileNames, char **fileExtensions, u32 *fileSizes, u8 *fileTypes) {
    OSPfsState state;
    s32 ret;
    s32 maxNumOfFilesOnCpak;
    s32 files_used;
    s8 *temp_D_800DE440;
    s32 i;
    u32 gameCode;

    ret = packOpen(controllerIndex);
    if (ret != CONTROLLER_PAK_GOOD) {
        packClose(controllerIndex);
        return ret;
    }

    if (osPfsNumFiles(&pfs[controllerIndex], &maxNumOfFilesOnCpak, &files_used) != 0) {
        packClose(controllerIndex);
        return CONTROLLER_PAK_BAD_DATA;
    }

    if (frontGetLanguage() == LANGUAGE_JAPANESE) {
        gameCode = JPN_GAME_CODE;
    } else if (osTvType == TV_TYPE_PAL) {
        gameCode = PAL_GAME_CODE;
    } else {
        gameCode = NTSC_GAME_CODE;
    }
    
    if (maxNumOfFilesToGet < maxNumOfFilesOnCpak) {
        maxNumOfFilesOnCpak = maxNumOfFilesToGet;
    }
    
    if (sPackDirectory != NULL) {
        mmFree(sPackDirectory);
    }
    
    files_used = maxNumOfFilesOnCpak * 24;
    sPackDirectory = mmAlloc(files_used, COLOUR_TAG_BLACK);
    _blkclr(sPackDirectory, files_used);
    temp_D_800DE440 = sPackDirectory;
    
    //TODO: There's probably an unidentified struct here
    for (i = 0; i < maxNumOfFilesOnCpak; i++) {
        fileNames[i] = (char *) temp_D_800DE440;
        temp_D_800DE440 += 0x12;
        fileExtensions[i] = (char *) temp_D_800DE440;
        fileSizes[i] = 0;
        fileTypes[i] = -1;
        temp_D_800DE440 += 6;
    }
    
    while (i < maxNumOfFilesToGet) {
        fileExtensions[i] = 0;
        fileNames[i] = 0;
        fileSizes[i] = 0;
        fileTypes[i] = -1;
        i++;
    }
    
    for (i = 0; i < maxNumOfFilesOnCpak; i++) {
        ret = osPfsFileState(&pfs[controllerIndex], i, &state);
        if (ret == PFS_ERR_INVALID) {
            fileNames[i] = 0;
            continue;
        }
        
        if (ret != 0) {
            packClose(controllerIndex);
            return CONTROLLER_PAK_BAD_DATA;
        }
        
        font_codes_to_string((char *)&state.game_name, (char *)fileNames[i], PFS_FILE_NAME_LEN);
        font_codes_to_string((char *)&state.ext_name, (char *)fileExtensions[i], PFS_FILE_EXT_LEN);
        fileSizes[i] = state.file_size;
        fileTypes[i] = 1; // Unknown file type? Possibly from another game?
        
        if ((state.game_code == gameCode) && (state.company_code == COMPANY_CODE)) {
            fileTypes[i] = func_8004DDC4_4E9C4(controllerIndex, i);
        }
    }
    
    packClose(controllerIndex);
    return CONTROLLER_PAK_GOOD;
}

void packDirectoryFree(void) {
    if (sPackDirectory != 0) {
        mmFree(sPackDirectory);
    }
    sPackDirectory = 0;
}

SIDeviceStatus packFreeSpace(s32 controllerIndex, u32 *bytesFree, s32 *notesFree) {
    s32 ret;
    s32 bytesNotUsed;
    s32 maxNotes;
    s32 notesUsed;

    ret = packOpen(controllerIndex);
    if (ret == CONTROLLER_PAK_GOOD) {
        if (bytesFree != 0) {
            ret = osPfsFreeBlocks(&pfs[controllerIndex], &bytesNotUsed);
            if (ret != 0) {
                packClose(controllerIndex);
                return CONTROLLER_PAK_BAD_DATA;
            }
            *bytesFree = bytesNotUsed;
        }
        if (notesFree != 0) {
            ret = osPfsNumFiles(&pfs[controllerIndex], &maxNotes, &notesUsed);
            if (ret != 0) {
                packClose(controllerIndex);
                return CONTROLLER_PAK_BAD_DATA;
            }
            if (notesUsed >= 16) {
                *notesFree = 0;
            } else {
                *notesFree = 16 - notesUsed;
            }
        }
    }

    packClose(controllerIndex);
    return ret;
}

SIDeviceStatus packDeleteFile(s32 controllerIndex, s32 fileNum) {
    OSPfsState state;
    s32 ret;

    ret = packOpen(controllerIndex);
    if (ret != CONTROLLER_PAK_GOOD) {
        packClose(controllerIndex);
        return ret;
    }

    ret = CONTROLLER_PAK_BAD_DATA;

    if (osPfsFileState(&pfs[controllerIndex], fileNum, &state) == 0) {
        if (osPfsDeleteFile(&pfs[controllerIndex], state.company_code, state.game_code, (u8 *)&state.game_name, (u8 *)&state.ext_name) == 0) {
            ret = CONTROLLER_PAK_GOOD;
        }
    }

    packClose(controllerIndex);

    return ret;
}

SIDeviceStatus packCopyFile(s32 controllerIndex, s32 fileNumber, s32 secondControllerIndex) {
    UNUSED s32 pad;
    char fileName[PFS_FILE_NAME_LEN];
    UNUSED s32 pad2;
    char fileExt[PFS_FILE_EXT_LEN];
    OSPfsState state;
    s32 status;
    u8 *alloc;

    status = packOpen(controllerIndex);
    if (status != CONTROLLER_PAK_GOOD) {
        packClose(controllerIndex);
        return status;
    }

    if (osPfsFileState(&pfs[controllerIndex], fileNumber, &state) != 0) {
        packClose(controllerIndex);
        return CONTROLLER_PAK_BAD_DATA;
    }

    alloc = mmAlloc(state.file_size, COLOUR_TAG_BLACK);

    status = packReadFile(controllerIndex, fileNumber, alloc, state.file_size);
    packClose(controllerIndex);
    if (status != CONTROLLER_PAK_GOOD) {
        mmFree(alloc);
        return status;
    }

    font_codes_to_string(state.game_name, fileName, PFS_FILE_NAME_LEN);
    font_codes_to_string(state.ext_name, fileExt, PFS_FILE_EXT_LEN);

    status = packWriteFile(secondControllerIndex, -1, fileName, fileExt, alloc, state.file_size);

    mmFree(alloc);
    return status;
}

SIDeviceStatus packOpenFile(s32 controllerIndex, char *fileName, char *fileExt, s32 *fileNumber) {
    u32 gameCode;
    char fileNameAsFontCodes[PFS_FILE_NAME_LEN];
    UNUSED s32 pad;
    char fileExtAsFontCodes[PFS_FILE_EXT_LEN];
    UNUSED s32 pad2;
    s32 ret;

    string_to_font_codes(fileName, fileNameAsFontCodes, PFS_FILE_NAME_LEN);
    string_to_font_codes(fileExt, fileExtAsFontCodes, PFS_FILE_EXT_LEN);

    if (frontGetLanguage() == LANGUAGE_JAPANESE) {
        gameCode = JPN_GAME_CODE;
    } else if (osTvType == TV_TYPE_PAL) {
        gameCode = PAL_GAME_CODE;
    } else {
        gameCode = NTSC_GAME_CODE;
    }


    ret = osPfsFindFile(&pfs[controllerIndex], COMPANY_CODE, gameCode, (u8 *)fileNameAsFontCodes, (u8 *)fileExtAsFontCodes, fileNumber);
    if (ret == 0) {
        return CONTROLLER_PAK_GOOD;
    }
    if ((ret == PFS_ERR_NOPACK) || (ret == PFS_ERR_DEVICE)) {
        return NO_CONTROLLER_PAK;
    }
    if (ret == PFS_ERR_INCONSISTENT) {
        return CONTROLLER_PAK_INCONSISTENT;
    }
    if (ret == PFS_ERR_ID_FATAL) {
        return CONTROLLER_PAK_WITH_BAD_ID;
    }
    if (ret == PFS_ERR_INVALID) {
        return CONTROLLER_PAK_CHANGED;
    }
    
    return CONTROLLER_PAK_BAD_DATA;
}

SIDeviceStatus packReadFile(s32 controllerIndex, s32 fileNum, u8 *data, s32 dataLength) {
    s32 readResult = osPfsReadWriteFile(&pfs[controllerIndex], fileNum, PFS_READ, 0, dataLength, data);

    if (readResult == 0) {
        return CONTROLLER_PAK_GOOD;
    }
    if ((readResult == PFS_ERR_NOPACK) || (readResult == PFS_ERR_DEVICE)) {
        return NO_CONTROLLER_PAK;
    }
    if (readResult == PFS_ERR_INCONSISTENT) {
        return CONTROLLER_PAK_INCONSISTENT;
    }
    if (readResult == PFS_ERR_ID_FATAL) {
        return CONTROLLER_PAK_WITH_BAD_ID;
    }
    if (readResult == PFS_ERR_INVALID) {
        return CONTROLLER_PAK_CHANGED;
    }

    return CONTROLLER_PAK_BAD_DATA;
}

SIDeviceStatus packWriteFile(s32 controllerIndex, s32 fileNumber, char *fileName, char *fileExt, u8 *dataToWrite, s32 fileSize) {
    s32 temp;
    u8 fileNameAsFontCodes[PFS_FILE_NAME_LEN];
    UNUSED s32 temp2;
    u8 fileExtAsFontCodes[PFS_FILE_EXT_LEN];
    s32 ret;
    s32 file_number;
    s32 bytesToSave;
    u32 game_code;

    ret = packOpen(controllerIndex);
    if (ret != CONTROLLER_PAK_GOOD) {
        packClose(controllerIndex);
        return ret;
    }

    bytesToSave = fileSize;
    temp = fileSize & 0xFF;
    if (temp != 0) {
        bytesToSave = (fileSize - temp) + 0x100;
    }

    string_to_font_codes(fileName, (char *)fileNameAsFontCodes, PFS_FILE_NAME_LEN);
    string_to_font_codes(fileExt, (char *)fileExtAsFontCodes, PFS_FILE_EXT_LEN);

    if (frontGetLanguage() == LANGUAGE_JAPANESE) {
        game_code = JPN_GAME_CODE;
    } else if (osTvType == TV_TYPE_PAL) {
        game_code = PAL_GAME_CODE;
    } else {
        game_code = NTSC_GAME_CODE;
    }

    ret = packOpenFile(controllerIndex, fileName, fileExt, &file_number);
    if (ret == CONTROLLER_PAK_GOOD) {
        if (fileNumber != -1 && fileNumber != file_number) {
            ret = CONTROLLER_PAK_BAD_DATA;
        }
    } else if (ret == CONTROLLER_PAK_CHANGED) {
        if (fileNumber != -1) {
            ret = CONTROLLER_PAK_BAD_DATA;
        } else {
            temp = osPfsAllocateFile(&pfs[controllerIndex], COMPANY_CODE, game_code, fileNameAsFontCodes, fileExtAsFontCodes, bytesToSave, &file_number);
            if (temp == 0) {
                ret = CONTROLLER_PAK_GOOD;
            } else if (temp == PFS_DATA_FULL || temp == PFS_DIR_FULL) {
                ret = CONTROLLER_PAK_FULL;
            } else {
                ret = CONTROLLER_PAK_BAD_DATA;
            }
        }
    }

    if (ret == CONTROLLER_PAK_GOOD) {
        temp = osPfsReadWriteFile(&pfs[controllerIndex], file_number, PFS_WRITE, 0, bytesToSave, dataToWrite);
        if (temp == 0) {
            ret = CONTROLLER_PAK_GOOD;
        } else if ((temp == PFS_ERR_NOPACK) || (temp == PFS_ERR_DEVICE)) {
            ret = NO_CONTROLLER_PAK;
        } else if (temp == PFS_ERR_INCONSISTENT) {
            ret = CONTROLLER_PAK_INCONSISTENT;
        } else if (temp == PFS_ERR_ID_FATAL) {
            ret = CONTROLLER_PAK_WITH_BAD_ID;
        }
        else {
            ret = CONTROLLER_PAK_BAD_DATA;
        }
    }

    packClose(controllerIndex);
    return ret;
}

SIDeviceStatus packFileSize(s32 controllerIndex, s32 fileNum, s32 *fileSize) {
    OSPfsState state;

    *fileSize = 0;
    if (osPfsFileState(&pfs[controllerIndex], fileNum, &state) == 0) {
        *fileSize = state.file_size;
        return CONTROLLER_PAK_GOOD;
    }
    return CONTROLLER_PAK_BAD_DATA;
}

//Converts N64 Font codes used in controller pak file names, into C ASCII a coded string
char *font_codes_to_string(char *inString, char *outString, s32 stringLength) {
    s32 index = *inString;
    char *ret = outString;

    while (index != 0 && stringLength != 0) {
        // Less than sizeof(gN64FontCodes) - 3.
        // So basically make sure it's a valid font code value
        if (index <= 65) {
            *outString = gN64FontCodes[index];
            outString++;
        } else {
            //Replace invalid characters with a hyphen
            *outString = '-';
            outString++;
        }

        inString++;
        stringLength--;
        index = *inString;
    }

    //Pad any extra characters with NULL font code
    while (stringLength != 0) {
        *outString = 0;
        stringLength--;
        outString++;
    }
    
    *outString = 0;
    return ret;
}

//Converts a C ASCII string into N64 Font codes for controller pak file names
char *string_to_font_codes(char *inString, char *outString, s32 stringLength) {
    s32 i;
    char currentChar;
    char *ret = outString;

    while (*inString != 0 && stringLength != 0) {
        *outString = 0;
        for (i = 0; i < 65; i++) {
            currentChar = *inString;
            if (currentChar == gN64FontCodes[i]) {
                *outString = i;
                outString++;
                break;
            }
        }

        inString++;
        stringLength--;
    }

    while (stringLength != 0) {
        *outString = 0;
        stringLength--;
        outString++;
    }
    
    *outString = 0;
    return ret;
}

//Essentially the same as get_file_type in DKR
s32 func_8004DDC4_4E9C4(s32 controllerIndex, s32 fileNum) {
    s32 *data;
    UNUSED s32 pad;
    s32 ret;

    ret = 1;
    data = mmAlloc(0x100, COLOUR_TAG_BLACK);
    if (packReadFile(controllerIndex, fileNum, (u8 *)data, 0x100) == CONTROLLER_PAK_GOOD) {
        switch (*data) {
            case CHARFILETYPE:
                ret = 0;
                break;
            default:
                ret = 1;
                break;
        }
    } else {
        ret = 1;
    }

    mmFree(data);
    return ret;
}
