#include "common.h"
#include "math/math.h"
#include "src/memory.h"
#include "pi.h"
#include "runLink.h"

const char D_800AD890[] = "CRITICAL ERROR: objSetupObject() returned NULL\n";
const char D_800AD8C0[] = "GFDS:%d ???\n";
const char D_800AD8D0[] = "**** WARNING: Duplicate squadron ID:%d ****\n";
const char D_800AD900[] = "WARNING:Can't lock to node, no nodes\n";

// .data
u32 *BaddyDataArray = NULL;       // Effectively unused
Object **D_800A38E4_A44E4 = NULL; // Unknown type

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
PatrolNode *PatrolNodes;

Object **objGetObjList(s32 *first, s32 *last);
extern void GetRomlistInfo(RomDefHeader **list, s32 *size, s32 which);
extern s32 levelGetObjectID(s32 arg0);
extern void doorRegisterOpener(s32 arg0, s32 arg1);
extern s32 objGetControlNo(s16 arg0);
s32 ProcessNodeChange(s32 arg0);
s32 levelObjectFlagSet(s32 arg0);
void AIPointInit(s32 **arg0);
void BindRegionsToObjects(Object *arg0, s32 arg1);
void CopyStaticsToSquads(s32 arg0, Object *arg1);
s32 GetClosestPatrolNode(f32 x, f32 y, f32 z, s32 arg3);
s32 GetNextNodeNumber(s32 arg0, s32 arg1, s32 arg2, PatrolNode *node);
void SquaddieControl(Object *arg0, s32 arg1);
void doorUnlock(s32 arg0, s32 arg1);
void squadsAddToActiveSquaddies(DisactivatedSquaddie *arg0);
extern AnimPath **animpath;

int squadsIsTribal(s32 arg0) {
    return ((arg0 >= 0x11C) && (arg0 < 0x121)) || (arg0 == 0x66) || (arg0 == 0x70) || (arg0 == 0x90) ||
           (arg0 == 0x97) || (arg0 == 0x157) || (arg0 == 0xA5);
}

Object **squadsGetSquadronList(s32 *arg0) {
    *arg0 = D_800FE9F0_B9230;
    return D_800A38E4_A44E4;
}

s32 BaddyTypeToIndex(s32 arg0) {
    switch (arg0) { /* irregular */
        case 0x29:
            return 0;
        case 0x5F:
            return 1;
        case 0xBE:
            return 2;
        case 0xBD:
            return 3;
        case 0x67:
            return 4;
        case 0x78:
            return 5;
        case 0x8C:
            return 6;
        case 0x83:
            return 7;
        case 0x2A:
            return 9;
        case 0x2B:
            return 8;
        case 0x2D:
            return 10;
        case 0x54:
            return 11;
        case 0x68:
            return 12;
        case 0x8D:
            return 13;
        case 0x99:
            return 14;
        case 0x9C:
            return 15;
        case 0xDF:
            return 16;
        case 0xE0:
            return 17;
        case 0xE1:
            return 18;
        case 0xE6:
            return 19;
        case 0xEE:
            return 20;
        case 0xEF:
            return 21;
        case 0x12F:
            return 22;
        case 0xF2:
            return 23;
        case 0xF3:
            return 24;
        case 0x66:
            return 25;
        case 0x11C:
            return 26;
        case 0x11D:
            return 27;
        case 0x11E:
            return 28;
        case 0x11F:
            return 29;
        case 0x120:
            return 30;
        case 0x121:
            return 31;
        case 0xF1:
            return 32;
        case 0xF5:
            return 33;
        case 0xF6:
            return 34;
        case 0x122:
            return 35;
        case 0x143:
            return 36;
        case 0x144:
            return 37;
        case 0x149:
            return 38;
        case 0x150:
            return 39;
        case 0x151:
            return 40;
        case 0x152:
            return 41;
        case 0x153:
            return 42;
        case 0x166:
            return 43;
        case 0x70:
            return 44;
        case 0x90:
            return 45;
        case 0xA5:
            return 46;
        case 0x97:
            return 47;
        case 0x157:
            return 48;
        case 0x112:
            return 49;
        case 0x6D:
            return 50;
        case 0x1E5:
            return 51;
        case 0x269:
            return 52;
        case 0x2CE:
            return 53;
        default:
            return -1;
    }
}

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
    s32 i;
    DisactivatedSquaddie **slot;
    DisactivatedSquaddie *squaddie;

    i = D_800FE9A0_B91E0;
    slot = DisactivatedSquaddies;
    if (i > 0x100) {
        return NULL;
    }
    if (DisactivatedSquaddies != NULL) {
        while (i--) {
            squaddie = *slot;
            if ((squaddie != NULL) && (arg0 == squaddie->unk11)) {
                *slot = NULL;
                return squaddie;
            }
            slot++;
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
    s32 i;
    Object *node;

    i = D_800FE9F0_B9230;
    while (i--) {
        node = D_800A38E4_A44E4[i];
        if ((node->unkA0 == 0) && (arg0 == ((Object_Squadron *) node->racer)->unk64)) {
            return node;
        }
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

void func_80051CCC_528CC(void) {
    s32 i;
    Object **list;
    s32 first;
    s32 last;
    RomDefHeader **lp;
    s32 total;
    s32 *sp;
    s32 *endp;
    RomDefHeader *lists[2];
    RomDefHeader *entry;
    Object_Racer *racer;
    s32 sizes[2];
    s32 id;
    Object *obj;
    s32 door;

    GetRomlistInfo(&lists[1], &sizes[1], 1);
    GetRomlistInfo(&lists[0], &sizes[0], 0);
    list = objGetObjList(&first, &last);
    endp = &sizes[2];
    for (i = first; i < last; i++) {
        obj = list[i];
        if (obj->unkA0 != 0) {
            continue;
        }
        if (obj->behaviorId == 0x17) {
            racer = obj->racer;
            if (racer->unk2C == 0x2CE) {
                continue;
            }
            id = levelGetObjectID(racer->unk1);
            racer->unk88 = id;
            racer->unk8C = id;
            door = racer->unk75;
            if (door != 0) {
                doorRegisterOpener(door & 0xFFFFFFFFu, 0x20);
            }
            lp = &lists[0];
            sp = &sizes[0];
            do {
                entry = *lp;
                total = 0;
                while (total < *sp) {
                    if (objGetControlNo(entry->id) == 0x18) {
                        if (entry->unk11 == racer->unk64b) {
                            entry->unk18 = racer->unk8C;
                            racer->unk8C = racer->unk8C + 1;
                        }
                    }
                    total += entry->size;
                    entry = (RomDefHeader *) ((u32) entry + entry->size);
                }
                sp++;
                lp++;
                endp = &sizes[2];
            } while (sp != endp);
            if (racer->unk63 >= racer->unk1) {
                racer->unk30 = 7;
            }
        }
    }
}

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

extern void ReallyAddInterestingEvent(s32 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6);
void squadsAddInterestingEvent(s32 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6) {
    if (runlinkIsModuleLoaded(3) != 0) {
        ReallyAddInterestingEvent(arg0, arg1, arg2, arg3, (s32) arg4, (s32) arg5, (s32) arg6);
    }
}

void CopyStaticsToKernel(Object *arg0, Object *arg1) {
    D_800FEA08_B9248 = arg0;
    D_800FEA0C_B924C = D_800FEA08_B9248 != NULL ? arg0->racer : 0;
    D_800FEA10_B9250 = arg1;
    D_800FEA14_B9254 = D_800FEA10_B9250 != NULL ? arg1->racer : 0;
}
