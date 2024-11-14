#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsIsTribal.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsGetSquadronList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/BaddyTypeToIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/CreateStaticInstance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsRememberRomdef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/AddToDisactivatedSquaddies.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetFirstDisactivatedSquaddie.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsInitialiseBeforeObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetSquadronFromIdentifier.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetSquaddieFromIdentifiers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsCheckEnemyPointers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsCheckGrenades.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetFormationInfo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/ProcessNodeChange.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/func_80052900.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsInitialiseAfterObjects.s")

void squadsInit(void) {
    BaddyDataArray = piRomLoad(0x3E);
}

void squadsPreInit(u8 *list, s32 listSize) {
    u8 *listBytes;
    s32 i;

    PlayersDisguise = 0;
    GlobalStartOfRomdefList = list;
    GlobalSizeOfRomdefList = listSize;
    listBytes = list;
    for (i = 0; i < listSize; ) {
        // TODO: Figure out a better match...
        if ((CAST_TO_S16(listBytes[0]) == 0x61) || (CAST_TO_S16(listBytes[0]) == 5) || (CAST_TO_S16(listBytes[0]) == 0x4F)) {
            runlinkDownloadCode(3);
            break;
        }
        i += CAST_TO_U8(listBytes[2]);
        listBytes += CAST_TO_U8(listBytes[2]);
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsAddInterestingEvent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/CopyStaticsToKernel.s")
