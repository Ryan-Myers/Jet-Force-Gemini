#include "common.h"
#include "pi.h"
#include "runLink.h"

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

int squadsIsTribal(s32 arg0) {
    return ((arg0 >= 0x11C) && (arg0 < 0x121)) || (arg0 == 0x66) || (arg0 == 0x70) || (arg0 == 0x90) ||
           (arg0 == 0x97) || (arg0 == 0x157) || (arg0 == 0xA5);
}

s32 squadsGetSquadronList(s32 *arg0) {
    *arg0 = D_800FE9F0_B9230;
    return D_800A38E4_A44E4;
}

#pragma GLOBAL_ASM("asm/nonmatchings/squads/BaddyTypeToIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/CreateStaticInstance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsRememberRomdef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/AddToDisactivatedSquaddies.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetFirstDisactivatedSquaddie.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsInitialiseBeforeObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetSquadronFromIdentifier.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/GetSquaddieFromIdentifiers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsCheckEnemyPointers.s")

void squadsCheckGrenades(Object *arg0) {
    s32 i;
    s32 j;
    Object **list;
    Object *node;
    Object_Racer *racer;
    Object *obj;

    i = D_800FE9F0_B9230;
    j = i--;
    if (j != 0) {
        list = (Object **) D_800A38E4_A44E4 + i;
        do {
            node = *list--;
            racer = node->racer;
            obj = racer->unk58;
            while (obj != NULL) {
                node = (Object *) obj->racer;
                if (((Object_Grenade *) node)->owner == arg0) {
                    ((Object_Grenade *) node)->owner = NULL;
                    return;
                }
                obj = ((Object_Grenade *) node)->next;
            }
            j = i--;
        } while (j != 0);
    }
}

void GetFormationInfo(Object *arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    Object_Racer *temp_v0;
    s16 temp_a0;
    f32 var_f0;

    temp_v0 = arg0->racer;
    if ((D_800FEA0C_B924C != NULL) && (temp_v0 != NULL) && (temp_a0 = temp_v0->unk2A, (temp_a0 != 0))) {
        var_f0 = (f32) D_800FEA0C_B924C->unkA6 / (f32) temp_a0;
    } else {
        var_f0 = 1.0f;
    }
    if (temp_v0->unk30 == 3) {
        *arg1 = temp_v0->unk6;
        *arg2 = temp_v0->unk8;
        *arg3 = temp_v0->unk7;
    } else if ((temp_v0->unk30 == 4) || ((var_f0 * 100.0f) < (f32) (0x64 - temp_v0->unkD))) {
        *arg1 = temp_v0->unk9;
        *arg2 = temp_v0->unkB;
        *arg3 = temp_v0->unkA;
    } else {
        *arg1 = temp_v0->unk3;
        *arg2 = temp_v0->unk5;
        *arg3 = temp_v0->unk4;
    }
    temp_v0->unk32 = *arg3;
    temp_v0->unk31 = *arg1;
    if ((*arg3 == 8) && (D_800FEA0C_B924C != NULL) && ((*(u32 *) D_800FEA0C_B924C << 9) >> 31)) {
        *arg3 = 9;
        temp_v0->unk2E = 0;
    }
    temp_a0 = temp_v0->unk33;
    if (temp_a0 != 0) {
        if (temp_a0 != 0x1A) {
            *arg3 = temp_a0;
            return;
        }
        *arg1 = 0xA;
        *arg3 = temp_v0->unk33;
    }
}

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

extern void ReallyAddInterestingEvent_Trap(s32 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6);
void squadsAddInterestingEvent(s32 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6) {
    if (runlinkIsModuleLoaded(3) != 0) {
        ReallyAddInterestingEvent_Trap(arg0, arg1, arg2, arg3, (s32) arg4, (s32) arg5, (s32) arg6);
    }
}

void CopyStaticsToKernel(Object *arg0, Object *arg1) {
    D_800FEA08_B9248 = arg0;
    D_800FEA0C_B924C = D_800FEA08_B9248 != NULL ? arg0->racer : 0;
    D_800FEA10_B9250 = arg1;
    D_800FEA14_B9254 = D_800FEA10_B9250 != NULL ? arg1->racer : 0;
}
