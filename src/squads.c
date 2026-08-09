#include "common.h"

const char D_800AD890[] = "CRITICAL ERROR: objSetupObject() returned NULL\n";
const char D_800AD8C0[] = "GFDS:%d ???\n";
const char D_800AD8D0[] = "**** WARNING: Duplicate squadron ID:%d ****\n";
const char D_800AD900[] = "WARNING:Can't lock to node, no nodes\n";

// .data
u32 *BaddyDataArray = NULL;   // Effectively unused
u32 *D_800A38E4_A44E4 = NULL; // Unknown type

// .bss
void *D_800FE9A0_B91E0;
void *DisactivatedSquaddies;
s32 D_800FE9A8_B91E8;
s32 D_800FE9AC_B91EC;
s32 SquadsModuleFlags;
#ifdef VERSION_kiosk
u8 D_800FE9B8_B91F8[5];
#else
u8 D_800FE9B8_B91F8[0x35];
#endif
u8 D_800FE9ED_B922D; // Linked with above?
u8 squadsBehaviour;
s32 D_800FE9F0_B9230;
s32 SquaddieGrowlTimer;
s32 iDeltaTime;
s32 PlayersDisguise;
RomDefHeader *GlobalStartOfRomdefList;
s32 GlobalSizeOfRomdefList;
Object *D_800FEA08_B9248;
Object_Racer *D_800FEA0C_B924C; // Could be any Object64 from DKR
Object *D_800FEA10_B9250;
Object_Racer *D_800FEA14_B9254; // Not sure if array, or not. Could be any Object64 from DKR
UNUSED u8 PathLists[0x3C];
s32 *D_800FEA54_B9294;
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

#pragma GLOBAL_ASM("asm/nonmatchings/squads/func_80051CCC_528CC.s")

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
    for (i = 0; i < listSize;) {
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
