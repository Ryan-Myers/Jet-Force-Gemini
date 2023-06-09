#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsIsTribal.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsGetSquadronList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/BaddyTypeToIndex.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/CreateStaticInstance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsRememberRomdef.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/AddToDisactivatedSquaddies.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/GetFirstDisactivatedSquaddie.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsInitialiseBeforeObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/GetSquadronFromIdentifier.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/GetSquaddieFromIdentifiers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsCheckEnemyPointers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsCheckGrenades.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/GetFormationInfo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/ProcessNodeChange.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/func_80052900_53500.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsInitialiseAfterObjects.s")

void squadsInit(void) {
    BaddyDataArray = piRomLoad(0x3E);
}

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsPreInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/squadsAddInterestingEvent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sqauds/CopyStaticsToKernel.s")
