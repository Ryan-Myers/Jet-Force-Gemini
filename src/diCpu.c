#include "common.h"
#include "stdarg.h"

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
    osCreateMesgQueue(&D_801031C0, &D_801031D8, 8);
    osSetEventMesg(OS_EVENT_FAULT, &D_801031C0, (void *)8);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &D_801031C0, (void *)2);
    osCreatePiManager(150, &D_80103218, &D_801031F8, 8);
    while (1) {
        osRecvMesg(&D_801031C0, (OSMesg) &sp2C, 1);
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

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800676F8.s")

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
        func_800676F8(thread);
    }
    func_80067880(thread);
}

#ifdef NON_EQUIVALENT
extern s32 osCicId;
extern s32 D_800A6E98;

void func_80067880(OSThread *thread) {
    s32 i;

    // Anti Piracy Check
    if ((D_800A3B74 == 0) || (osCicId != 6105)) {
        while(1){}
    }
    if (D_800A6E98 == 0) {
        if (thread && thread && thread) {}
    }
    if (viGetVideoMode() != 0) {
        D_800A6EDC = 1;
    } else {
        D_800A6EDC = 0;
    }
    for (i = 0; i < 100; i++) {
        func_8006869C();
    }
    while(1){}
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067880.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuReportWatchpoint.s")

#ifdef NON_EQUIVALENT
extern s32 D_800A6E90;
extern s32 D_800A6E94;
extern s32 D_800A6E98;

void diCpuLogMessage(const char *format, ...) {
    va_list args;
    char sp20[255];
    s32 i;
    char *var_v0;
    char *var_a1;
    char var_a0;

    va_start(args, format);
    vsprintf(&sp20, format, &args);
    va_end(args);

    var_a1 = (D_800A6E94 * 41) + 0x80700000;
    var_v0 = &sp20;
    i = 0;
    if (var_v0[i] != 0) {
        var_a0 = sp20[i];
loop_2:
        *var_a1 = var_a0;
        var_a0 = var_v0[1];
        var_a1 += 1;
        var_v0 += 1;
        i++;
        if (var_a0 != 0 && i < 40) {
            goto loop_2;
        }
    }
    *var_a1 = 0;
    D_800A6E94++;
    if (D_800A6E94 >= 500) {
        D_800A6E98 = 1;
        D_800A6E94 = 0;
    }
    if (D_800A6E98 != 0) {
        D_800A6E90 = D_800A6E94;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuLogMessage.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067AA0.s")

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
    D_800A6E8C += arg0;
    if (D_800A6E8C > 60) {
        D_800A6E8C = 0;
        D_800A6E88++;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/render_epc_lock_up_display.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800683D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800684F0.s")


void cpuXYPrintf(s32 x, s32 y, const char *format, ...) {
    va_list args;
    char s[255];

    va_start(args, format);
    vsprintf(s, format, args);
    va_end(args);

    if (D_800A6EDC != 0) {
        if (D_800A6EDC == 1) {
            y -= 8;
        } else {
            y -= 104;
        }
        if (y >= 0 && y < 116) {
            y *= 2;
            goto block_7;
        }
    } else {
block_7:
        func_800684F0(x, y, s);
    }
}

void func_8006869C(void) {
    UNUSED s32 pad;
    s32 height;
    s32 width;
    s32 screenSize;
    s16 *var_v1;

    viGetCurrentSize(&height, &width);
    screenSize = height * width;
    var_v1 = otherScreen;
    while (screenSize--) {
        *var_v1 = 0;
        var_v1++;
    }
}

//File split?
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
