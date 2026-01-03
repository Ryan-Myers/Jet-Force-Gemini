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

// The size of 1 sector is 128 pages (16K bytes), and each page of 0~0x7f, 0x80~0xff, 0x100~0x17f... is considered to be
// 1 sector. So basically, flash is written in sectors, and changing a single page needs to read a sector, modify the
// page, and write back the whole sector.
#define SECTOR_SIZE 128

typedef enum Language { LANGUAGE_0, LANGUAGE_1, LANGUAGE_2, LANGUAGE_3, LANGUAGE_JAPANESE } Language;

#ifdef VERSION_us
s32 mainGetPauseMode();
extern u8 D_800A3470_A4070;

s32 func_8004B070_4BC70(void) {
    s32 var_v0;

    var_v0 = D_800A3470_A4070 != 0;
    if (var_v0 != 0) {
        return mainGetPauseMode() == 0;
    }
    // Bug! Doesn't guarantee a return
}

void rumbleRumbles(s32 arg0) {
    D_800A3470_A4070 = arg0;
}
#endif

UNUSED void rumbleProcessing(s32 arg0) {
    if ((arg0 != 0) && (D_800A3ECC == 0)) {
        D_800A3EC4 = 1;
        D_800A3ECC = 1;
    } else {
        D_800A3ECC = 0;
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleStart.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleStop.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleAlter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleMax.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleKill.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleUpdate.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleTick.s")

UNUSED void rumbleGetRumble(s32 arg0, s32 *arg1, f32 *arg2) {
    *arg1 = 0;
    *arg2 = 0;
    if ((arg0 >= 0) && (arg0 < 3)) {
        *arg1 = D_800A3EAC[arg0].unk0;
        *arg2 = D_800A3EAC[arg0].unk4;
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004C2A8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packClearGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packEraseEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004BE44_4CA44.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadGlobalFlagsEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveGlobalFlagsEprom.s")

void flashROMInit(void) {
    osCreateMesgQueue(&cartEventQueue, (OSMesg *) &cartEventBuf, 1);
    osSetEventMesg(OS_EVENT_CART, &cartEventQueue, (OSMesg) 1);
    osCreateMesgQueue(&flashEventQueue, (OSMesg *) &flashEventBuf, 1);
    osCartRomInit();
    osFlashInit();
}

void flashROMWrite(u32 pageNum, u32 *dramAddr) {
    // Page 0x3fe and 0x3ff are reserved by Nintendo.  Please refrain from using the pages.
    if (pageNum < 0x3FE) {
        osWritebackDCache(dramAddr, SECTOR_SIZE);
        // Transfer data from RDRAM to the write buffer in 1M Flash
        osFlashWriteBuffer(&flashMesgReqBlock, 0, dramAddr, &flashEventQueue);
        osRecvMesg(&flashEventQueue, NULL, OS_MESG_BLOCK);
        // Transfer data from write buffer to each page of 1M Flash
        osFlashWriteArray(pageNum);
    }
}

void flashROMRead(u32 pageNum, u32 *dramAddr) {
    // Page 0x3fe and 0x3ff are reserved by Nintendo.  Please refrain from using the pages.
    if (pageNum < 0x3FE) {
        osInvalDCache(dramAddr, SECTOR_SIZE);
        osFlashReadArray(&flashMesgReqBlock, 0, pageNum, dramAddr, 1, &flashEventQueue);
        osRecvMesg(&flashEventQueue, NULL, OS_MESG_BLOCK);
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packOpen.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packClose.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packIsPresent.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packRepair.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packFormat.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packDirectory.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packDirectoryFree.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packFreeSpace.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packDeleteFile.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packCopyFile.s")

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
    } else if (osTvType == OS_TV_TYPE_PAL) {
        gameCode = PAL_GAME_CODE;
    } else {
        gameCode = NTSC_GAME_CODE;
    }

    ret = osPfsFindFile(&pfs[controllerIndex], COMPANY_CODE, gameCode, (u8 *) fileNameAsFontCodes,
                        (u8 *) fileExtAsFontCodes, fileNumber);
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

SIDeviceStatus packWriteFile(s32 controllerIndex, s32 fileNumber, char *fileName, char *fileExt, u8 *dataToWrite,
                             s32 fileSize) {
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

    string_to_font_codes(fileName, (char *) fileNameAsFontCodes, PFS_FILE_NAME_LEN);
    string_to_font_codes(fileExt, (char *) fileExtAsFontCodes, PFS_FILE_EXT_LEN);

    if (frontGetLanguage() == LANGUAGE_JAPANESE) {
        game_code = JPN_GAME_CODE;
    } else if (osTvType == OS_TV_TYPE_PAL) {
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
            temp = osPfsAllocateFile(&pfs[controllerIndex], COMPANY_CODE, game_code, fileNameAsFontCodes,
                                     fileExtAsFontCodes, bytesToSave, &file_number);
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
        } else {
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

// Converts N64 Font codes used in controller pak file names, into C ASCII a coded string
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
            // Replace invalid characters with a hyphen
            *outString = '-';
            outString++;
        }

        inString++;
        stringLength--;
        index = *inString;
    }

    // Pad any extra characters with NULL font code
    while (stringLength != 0) {
        *outString = 0;
        stringLength--;
        outString++;
    }

    *outString = 0;
    return ret;
}

// Converts a C ASCII string into N64 Font codes for controller pak file names
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

// Essentially the same as get_file_type in DKR
s32 func_8004DDC4(s32 controllerIndex, s32 fileNum) {
    s32 *data;
    UNUSED s32 pad;
    s32 ret;

    ret = 1;
    data = mmAlloc(0x100, COLOUR_TAG_BLACK);
    if (packReadFile(controllerIndex, fileNum, (u8 *) data, 0x100) == CONTROLLER_PAK_GOOD) {
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

#ifdef VERSION_us
const char D_800AC7E0[] = "*** SAVING GAME ***\n";
const char D_800AC7F8[] = "FlashSectorErase failed\n";
const char D_800AC814[] = "*** ERASING GAME %d***\n";
const char D_800AC82C[] = "FlashSectorErase failed\n";
const char D_800AC848[] = "*** ERASING FLASH ROM ***\n";
const char D_800AC864[] = "FlashSectorErase failed\n";
const char D_800AC880[] = "Global Flags size = %d\n";
const char D_800AC898[] = "Loaded Globals :: (%04x == %04x) ?\n";
const char D_800AC8BC[] = "RESET GLOBAL FLAGS\n";
const char D_800AC8D0[] = "FlashSectorErase failed\n";
const char D_800AC8EC[] = "Saved Globals :: %04x\n";
const char D_800AC904[] = "osFlashWriteArray failed (%d)\n";
const char D_800AC924[] = "osFlashReadArray failed!\n";
#endif
