#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osCreateScheduler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osScAddClient.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osScRemoveClient.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osScGetCmdQ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osScGetInterruptQ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/osScGetAudioSPStats.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800504B8_510B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80050670_51270.s")

#ifdef NON_MATCHING
//Needs RODATA migrated for the jump table to match
char *func_80050718_51318(s32 arg0) {
    switch (arg0) {
    case 1:
        return &D_800AD510_AE110;
    case 2:
        return &D_800AD520_AE120;
    case 3:
        return &D_800AD52C_AE12C;
    case 4:
        return &D_800AD538_AE138;
    case 5:
        return &D_800AD550_AE150;
    case 6:
        return &D_800AD560_AE160;
    case 7:
        return &D_800AD570_AE170;
    default:
        return &D_800AD580_AE180;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80050718_51318.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800507A4_513A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800507AC_513AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80050AA4_516A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80050C94_51894.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80051374_51F74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800514A0_520A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/__scTaskReady.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80051584_52184.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_80051684_52284.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800516EC_522EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_800517B8_523B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/sched/func_8005180C_5240C.s")
