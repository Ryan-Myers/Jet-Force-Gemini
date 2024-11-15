#include "common.h"

// .data
u32 *BaddyDataArray = NULL; // Effectively unused
u32 *D_800A4334 = NULL; // Unknown type

// .bss
void *D_800FF6B0;
void *DisactivatedSquaddies;
s32 D_800FF6B8;
s32 D_800FF6BC;
s32 SquadsModuleFlags;
u8 D_800FF6C4[5];
u8 D_800FF6C9; // Linked with above?
u8 squadsBehaviour;
s32 D_800FF6CC;
s32 SquaddieGrowlTimer;
s32 iDeltaTime;
s32 PlayersDisguise;
RomDefHeader *GlobalStartOfRomdefList;
s32 GlobalSizeOfRomdefList;
Object *D_800FF6E4;
Object_Racer *D_800FF6E8; // Could be any Object64 from DKR
Object *D_800FF6EC;
Object_Racer *D_800FF6F0; // Not sure if array, or not. Could be any Object64 from DKR
UNUSED s32 D_800FF6F4;
UNUSED u8 PathLists[0x3C];
s32 *D_800FF734;
UNUSED u8 AnimPathNumbers[0x40];
s32 MaxPatrolNodes;
void *PatrolNodes;

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
