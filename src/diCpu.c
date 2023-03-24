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
		func_8006768C();
		func_800677E4();
	}
}

void func_8006768C(void) {
    OSThread *node = __osGetActiveQueue();
    while (node->priority != -1) {
        if ((node->priority > 0) && (node->priority < 128)) {
            osStopThread((OSThread *)&node->next);
        }
        node = node->tlnext;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800676F8_682F8.s")

void func_800677E4(void) {
    OSThread *node = __osGetActiveQueue();
    while (node->priority != -1) {
		if (node->priority <= 0 || (!(node->flags & 2) && !(node->flags & 1))) {
        	node = node->tlnext;
		}
		else {
			break;
		}
    }
    if (node->priority != -1) {
        func_800676F8_682F8(node);
    }
    func_80067880_68480(node);
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067880_68480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuReportWatchpoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuLogMessage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067AA0_686A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuTraceMallocFault.s")

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

//render_epc_lock_up_display in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067CA4_688A4.s")

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
