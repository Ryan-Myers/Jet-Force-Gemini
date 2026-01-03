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

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004B070_4BC70.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleRumbles.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleProcessing.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleStart.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleStop.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleAlter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleMax.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleKill.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleUpdate.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/rumbleGetRumble.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004BA98_4C698.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveCharacter.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packClearGameEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packEraseEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004BE44_4CA44.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packLoadGlobalFlagsEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packSaveGlobalFlagsEprom.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/flashROMInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004BF98_4CB98.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/func_8004C008_4CC08.s")

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

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packOpenFile.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packReadFile.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packWriteFile.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/saves/packFileSize.s")

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
