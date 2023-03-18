#include "common.h"




/* Size: 0x1B0 bytes */
typedef struct epcInfo {
/*  0x00 */ s32 unk0;
/*  0x04 */ s32 unk4;
/*  0x08 */ s32 unk8;
/*  0x0C */ s32 unkC;
/*  0x10 */ s32 unk10;
/*  0x14 */ s32 thread[1];
/*  0x18 */ s32 unk18;
/*  0x1C */ s32 unk1C;
/*  0x20 */ s64 at;
/*  0x28 */ s64 v0;
/*  0x30 */ s64 v1;
/*  0x38 */ s64 a0;
/*  0x40 */ s64 a1;
/*  0x48 */ s64 a2;
/*  0x50 */ s64 a3;
/*  0x58 */ s64 t0;
/*  0x60 */ s64 t1;
/*  0x68 */ s64 t2;
/*  0x70 */ s64 t3;
/*  0x78 */ s64 t4;
/*  0x80 */ s64 t5;
/*  0x88 */ s64 t6;
/*  0x90 */ s64 t7;
/*  0x98 */ s64 s0;
/*  0xA0 */ s64 s1;
/*  0xA8 */ s64 s2;
/*  0xB0 */ s64 s3;
/*  0xB8 */ s64 s4;
/*  0xC0 */ s64 s5;
/*  0xC8 */ s64 s6;
/*  0xD0 */ s64 s7;
/*  0xD8 */ s64 t8;
/*  0xE0 */ s64 t9;
/*  0xE8 */ s64 gp;
/*  0xF0 */ s64 sp;
/*  0xF8 */ s64 s8;
/* 0x100 */ s64 ra;
/* 0x108 */ s32 unk108;
/* 0x10C */ s32 unk10C;
/* 0x110 */ s32 unk110;
/* 0x114 */ s32 unk114;
/* 0x118 */ s32 sr;
/* 0x11C */ s32 epc;
/* 0x120 */ s32 cause;
/* 0x124 */ s32 badvaddr;
/* 0x128 */ s32 unk128;
/* 0x12C */ s32 unk12C;
/* 0x130 */ f32 unk130[3];
/* 0x13C */ u8 pad13C[0x74];
} epcInfo;

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

void func_8006768C(void);
void func_800677E4(void);

#ifdef NON_MATCHING
//Regalloc diff that's the same in DKR
void diCpuThread(UNUSED void *unused) {
    s32 sp2C;
    s32 var_s0 = 0;
    osCreateMesgQueue(&D_801031C0, &D_801031D8, 8);
    osSetEventMesg(OS_EVENT_FAULT, &D_801031C0, (void *) 8);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &D_801031C0, (void *) 2);
    osCreatePiManager(150, &D_80103218, &D_801031F8, 8);
	while (1) {
		do {
			osRecvMesg(&D_801031C0, (OSMesg) &sp2C, OS_MESG_BLOCK);
			var_s0 |= sp2C;
		} while ((!(var_s0 & 8) && !(var_s0 & 2)));
		var_s0 &= ~8;
		func_8006768C();
		func_800677E4();
	}
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuThread.s")
#endif

void func_8006768C(void) {
    OSThread *node = __osGetActiveQueue();
    while (node->priority != -1) {
        if ((node->priority > 0) && (node->priority < 128)) {
            osStopThread((OSThread *)&node->next);
        }
        node = node->tlnext;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800676F8.s")

void func_80067880(OSThread *);
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
        func_800676F8(node);
    }
    func_80067880(node);
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuReportWatchpoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuLogMessage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuTraceMallocFault.s")

s32 diCpuTraceGetFault(void) {
    return 0;
}

void diCpuTraceTick(s32 arg0) {
    D_800A6E8C += arg0;
    if (D_800A6E8C > 60) {
        D_800A6E8C = 0;
        D_800A6E88++;
    }
}

//render_epc_lock_up_display in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067CA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800683D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800684F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/cpuXYPrintf.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_8006869C.s")

s32 TrapDanglingJump(s32, s32, s32);

void __rmonSendFault(s32 arg0) {
    volatile f32 sp2C; //Fakematch?
    u32 i;

	i = 0;
    sp2C = 0.0f;
	do {
	    i += TrapDanglingJump(i + arg0, 560 - i, 2);
    } while (i < 560);
}
