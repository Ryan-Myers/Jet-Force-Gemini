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

#pragma GLOBAL_ASM("asm/nonmatchings/squads/func_80052900_53500.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsInitialiseAfterObjects.s")

void squadsInit(void) {
    BaddyDataArray = piRomLoad(0x3E);
}

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsPreInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/squadsAddInterestingEvent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/squads/CopyStaticsToKernel.s")
