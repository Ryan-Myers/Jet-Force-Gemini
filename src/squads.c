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
s32 D_800FE9A0_B91E0;
DisactivatedSquaddie **DisactivatedSquaddies;
s32 D_800FE9A8_B91E8;
RomDefHeader **D_800FE9AC_B91EC;
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

s32 BaddyTypeToIndex();
Object *objSetupObject(StaticInstanceSpawn *spawn, s32 arg1);

void CreateStaticInstance(s32 arg0) {
    u8 *slot;
    StaticInstanceSpawn spawn;

    slot = &D_800FE9B8_B91F8[BaddyTypeToIndex(arg0)];
    if (*slot == 0) {
        *slot = 1;
        D_800FEA14_B9254 = NULL;
        spawn.objectId = arg0;
        spawn.unk2 = 0x12;
        spawn.unk4 = 0;
        spawn.unk6 = 0;
        spawn.unk8 = 0;
        spawn.unkA = 0;
        spawn.unkC = 0;
        spawn.unkE = D_800FEA10_B9250->segment.trans.rotation.x;
        spawn.unk10 = 0;
        D_800FEA08_B9248 = objSetupObject(&spawn, 4);
        if (D_800FEA08_B9248 != NULL) {
            D_800FEA08_B9248->segment.unk3C = 0;
            D_800FEA0C_B924C = D_800FEA08_B9248->racer;
            D_800FEA08_B9248->segment.trans.flags |= 0x400;
            D_800FEA0C_B924C->flags = 0;
            D_800FEA0C_B924C->bit08 = 1;
            D_800FEA0C_B924C->unk24 = 0;
        }
    }
}


void squadsRememberRomdef(RomDefHeader *arg0) {
    D_800FE9AC_B91EC[D_800FE9A0_B91E0] = arg0;
    D_800FE9A0_B91E0 = D_800FE9A0_B91E0 + 1;
}

void AddToDisactivatedSquaddies(DisactivatedSquaddie *arg0) {
    DisactivatedSquaddies[D_800FE9A8_B91E8] = arg0;
    D_800FE9A8_B91E8 = D_800FE9A8_B91E8 + 1;
    if (D_800FE9A8_B91E8 >= D_800FE9A0_B91E0) {
        D_800FE9A8_B91E8 = 0;
    }
}

void *GetFirstDisactivatedSquaddie(s32 arg0) {
    s32 j;
    s32 i;
    DisactivatedSquaddie **slot;
    DisactivatedSquaddie *squaddie;

    i = D_800FE9A0_B91E0;
    slot = DisactivatedSquaddies;
    if (i >= 0x101) {
        return NULL;
    }
    if (DisactivatedSquaddies != NULL) {
        j = i--;
        if (j != 0) {
            do {
                squaddie = *slot;
                if ((squaddie != NULL) && (arg0 == squaddie->unk11)) {
                    *slot = NULL;
                    return squaddie;
                }
                slot++;
                j = i--;
            } while (j != 0);
        }
    }
    return NULL;
}

void squadsInitialiseBeforeObjects(void) {
    D_800FE9AC_B91EC = (RomDefHeader **) mmAlloc(0x400, 0xFFFF00FFU);
    SquadsModuleFlags = 1;
    DisactivatedSquaddies = 0;
    D_800FE9A0_B91E0 = 0;
    D_800FE9F0_B9230 = 0;
    SquaddieGrowlTimer = mathRnd(0xF0, 0x168);
}


void *GetSquadronFromIdentifier(s16 arg0) {
    s32 j;
    s32 i;
    Object **list;
    Object *node;

    i = D_800FE9F0_B9230;
    j = i--;
    if (j != 0) {
        list = (Object **) D_800A38E4_A44E4 + i;
        do {
            node = *list--;
            if ((node->unkA0 == 0) && (arg0 == ((Object_Squadron *) node->racer)->unk64)) {
                return node;
            }
            j = i--;
        } while (j != 0);
    }
    return NULL;
}

void *GetSquaddieFromIdentifiers(s16 arg0, s16 arg1) {
    s32 j;
    s32 i;
    Object **list;
    Object *node;
    Object_Squadron *squadron;
    Object_Racer *racer;
    Object *obj;

    i = D_800FE9F0_B9230;
    j = i--;
    if (j != 0) {
        list = (Object **) D_800A38E4_A44E4 + i;
        do {
            node = *list--;
            squadron = (Object_Squadron *) node->racer;
            if (arg0 == squadron->unk64) {
                obj = squadron->unk58;
                while (obj != NULL) {
                    racer = obj->racer;
                    if ((obj->unkA0 == 0) && (arg1 == ((Object_Grenade *) racer)->unk15)) {
                        return obj;
                    }
                    obj = ((Object_Grenade *) racer)->next;
                }
                return NULL;
            }
            j = i--;
        } while (j != 0);
    }
    return NULL;
}

void squadsCheckEnemyPointers(Object *arg0) {
    s32 j;
    s32 i;
    Object *node;
    Object_Racer *racer;
    Object *obj;

    i = D_800FE9F0_B9230;
    j = i--;
    if (j != 0) {
        do {
            node = ((Object **) D_800A38E4_A44E4)[i];
            racer = node->racer;
            obj = racer->unk58;
            while (obj != NULL) {
                node = (Object *) obj->racer;
                if (((Object_Grenade *) node)->ennemy == arg0) {
                    ((Object_Grenade *) node)->ennemy = NULL;
                }
                obj = ((Object_Grenade *) node)->next;
            }
            j = i--;
        } while (j != 0);
    }
}

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
    if ((*arg3 == 8) && (D_800FEA0C_B924C != NULL) && ((D_800FEA0C_B924C->flags << 9) >> 31)) {
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
