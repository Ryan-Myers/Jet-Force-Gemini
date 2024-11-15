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

void squadsPreInit(RomDefHeader *list, s32 listSize) {
    RomDefHeader *header;
    s32 i;

    PlayersDisguise = 0;
    GlobalStartOfRomdefList = list;
    GlobalSizeOfRomdefList = listSize;
    header = list;
    for (i = 0; i < listSize; ) {
        if (header->id == 0x61 || header->id == 5 || header->id == 0x4F) {
            runlinkDownloadCode(3);
            break;
        }
        i += header->size;
        header = (RomDefHeader *) ((u32) (header) + header->size);
    }
}



#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsAddInterestingEvent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/CopyStaticsToKernel.s")
