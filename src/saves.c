#include "common.h"

// These are the unique game codes assigned to DKR
#define NDYJ 0x4E44594A // 'NDYJ' ASCII
#define NDYP 0x4E445950 // 'NDYP' ASCII
#define NDYE 0x4E445945 // 'NDYE' ASCII
#define JPN_GAME_CODE NDYJ
#define PAL_GAME_CODE NDYP
#define NTSC_GAME_CODE NDYE
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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packOpen.s")

s32 packClose(UNUSED s32 controllerIndex) {
    osContStartReadData(sControllerMesgQueue);
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packIsPresent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packRepair.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packFormat.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packDirectory.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packDirectoryFree.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packFreeSpace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packDeleteFile.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packCopyFile.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packOpenFile.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packReadFile.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004DC5C_4E85C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/string_to_font_codes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004DDC4_4E9C4.s")
