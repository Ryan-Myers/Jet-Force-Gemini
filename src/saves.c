#include "common.h"

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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packClose.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/packWriteFile.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004DD08_4E908.s")

#pragma GLOBAL_ASM("asm/nonmatchings/saves/func_8004DDC4_4E9C4.s")
