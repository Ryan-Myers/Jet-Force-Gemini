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
typedef enum Language {
    LANGUAGE_0,
    LANGUAGE_1,
    LANGUAGE_2,
    LANGUAGE_3,
    LANGUAGE_JAPANESE
} Language;

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleProcessing.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleStart.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleAlter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleMax.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleKill.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/rumbleGetRumble.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/flashROMInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004CB0C_4D70C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004CB7C_4D77C.s")

SIDeviceStatus packOpen(s32 controllerIndex) {
    OSMesg unusedMsg;
    s32 ret;
    s32 bytes_not_used;
    s32 i;

    if (sControllerMesgQueue->validCount == 0) {
        if (nosMotorInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == 0) {
            return CONTROLLER_PAK_UNK8;
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
                return CONTROLLER_PAK_UNK8;
            }
        }
        if (ret == PFS_ERR_NEW_PACK) {
            if ((osPfsInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == PFS_ERR_ID_FATAL)
                && (nosMotorInit(sControllerMesgQueue,  &pfs[controllerIndex], controllerIndex) == 0)) {
                return CONTROLLER_PAK_UNK8;
            }
            return CONTROLLER_PAK_CHANGED;
        }
        if (ret == PFS_ERR_NOPACK || ret == PFS_ERR_DEVICE) {
            return NO_CONTROLLER_PAK;
        }
        if (ret == PFS_ERR_BAD_DATA) {
            return CONTROLLER_PAK_UNK6;
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

    if (ret == CONTROLLER_PAK_UNK8) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packDirectory.s")

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
                return CONTROLLER_PAK_UNK6;
            }
            *bytesFree = bytesNotUsed;
        }
        if (notesFree != 0) {
            ret = osPfsNumFiles(&pfs[controllerIndex], &maxNotes, &notesUsed);
            if (ret != 0) {
                packClose(controllerIndex);
                return CONTROLLER_PAK_UNK6;
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

    ret = CONTROLLER_PAK_UNK6;

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
        return CONTROLLER_PAK_UNK6;
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
    
    return CONTROLLER_PAK_UNK6;
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

    return CONTROLLER_PAK_UNK6;
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
            ret = CONTROLLER_PAK_UNK6;
        }
    } else if (ret == CONTROLLER_PAK_CHANGED) {
        if (fileNumber != -1) {
            ret = CONTROLLER_PAK_UNK6;
        } else {
            temp = osPfsAllocateFile(&pfs[controllerIndex], COMPANY_CODE, game_code, fileNameAsFontCodes, fileExtAsFontCodes, bytesToSave, &file_number);
            if (temp == 0) {
                ret = CONTROLLER_PAK_GOOD;
            } else if (temp == PFS_DATA_FULL || temp == PFS_DIR_FULL) {
                ret = CONTROLLER_PAK_FULL;
            } else {
                ret = CONTROLLER_PAK_UNK6;
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
            ret = CONTROLLER_PAK_UNK6;
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
    return CONTROLLER_PAK_UNK6;
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
