#include "common.h"

/**
 * Start the exception program counter thread.
*/
#ifdef NON_MATCHING
//Need to migrate data to define diCpuThreadStack
void diCpuTraceInit(void) {
    osCreateThread(&diCpuOSThread, 0, diCpuThread, 0, &diCpuThreadStack, OS_PRIORITY_MAX);
    osStartThread(&diCpuOSThread);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuTraceInit.s")
#endif

void diCpuThread(void *unused) {
    s32 sp2C;
    s32 var_s0 = 0;
    osCreateMesgQueue(&D_801031C0_103DC0, &D_801031D8_103DD8, 8);
    osSetEventMesg(OS_EVENT_FAULT, &D_801031C0_103DC0, (void *)8);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &D_801031C0_103DC0, (void *)2);
    osCreatePiManager(150, &D_80103218_103E18, &D_801031F8_103DF8, 8);
    while (1) {
        osRecvMesg(&D_801031C0_103DC0, (OSMesg) &sp2C, 1);
        var_s0 |= (s32)sp2C;
        if ((var_s0 & 8) == 0 && (var_s0 & 2) == 0) {
            continue;
        }
        var_s0 &= ~8;
        stop_all_threads_except_main();
        func_800677E4();
    }
}

/**
 * Stop all threads except for the main thread 1
 */
void stop_all_threads_except_main(void) {
    OSThread *node = __osGetActiveQueue();
    while (node->priority != -1) {
        if (node->priority > OS_PRIORITY_IDLE && node->priority <= OS_PRIORITY_APPMAX) {
            osStopThread((OSThread *) &node->next);
        }
        node = node->tlnext;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800676F8_682F8.s")

void func_800677E4(void) {
    OSThread *thread;

    for (thread = __osGetActiveQueue(); thread->priority != -1; thread = thread->tlnext) {
        if (thread->priority > OS_PRIORITY_IDLE) {
            if (thread->flags & 2 || thread->flags & 1) {
                break;
            }
        }
    }
    if (thread->priority != -1) {
        func_800676F8_682F8(thread);
    }
    func_80067880_68480(thread);
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067880_68480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuReportWatchpoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuLogMessage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067AA0_686A0.s")

void render_epc_lock_up_display(epcInfo *arg0);

void diCpuTraceMallocFault(s32 epc, s32 size, u32 colourTag) {
    epcInfo epcinfo;
    _blkclr(&epcinfo, sizeof(epcInfo));
    epcinfo.epc = epc & 0xFFFFFFFFFFFFFFFF; 
    epcinfo.a0 = size;
    epcinfo.a1 = colourTag;
    epcinfo.cause = -1;
    render_epc_lock_up_display(&epcinfo);
    osWritebackDCacheAll();
    while(1) {}
}

s32 diCpuTraceGetFault(void) {
    return 0;
}

void diCpuTraceTick(s32 arg0) {
    D_800A6E8C_A7A8C += arg0;
    if (D_800A6E8C_A7A8C > 60) {
        D_800A6E8C_A7A8C = 0;
        D_800A6E88_A7A88++;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/render_epc_lock_up_display.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800683D0_68FD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800684F0_690F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/cpuXYPrintf.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_8006869C_6929C.s")


//TODO: TrapDanglingJump Seems to have a different definition here vs in mmFreeTick
s32 TrapDanglingJump(s32, s32, s32);
void __rmonSendFault(s32 arg0) {
    UNUSED volatile f32 sp2C; //Fakematch?
    u32 i;

    i = 0;
    sp2C = 0.0f;
    do {
        i += TrapDanglingJump(i + arg0, 560 - i, 2);
    } while (i < 560);
}
