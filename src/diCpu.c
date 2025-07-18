#include "common.h"
#include "PR/rdb.h"
#include "PRinternal/rmonint.h"
#include "stdarg.h"

extern OSThread *__osGetActiveQueue(void);

/**
 * Start the exception program counter thread.
 */
#ifdef NON_MATCHING
// Need to migrate data to define diCpuThreadStack
void diCpuTraceInit(void) {
    osCreateThread(&diCpuOSThread, 0, diCpuThread, 0, &diCpuThreadStack, OS_PRIORITY_MAX);
    osStartThread(&diCpuOSThread);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuTraceInit.s")
#endif

void diCpuThread(UNUSED void *unused) {
    s32 sp2C;
    s32 s0 = 0;
    osCreateMesgQueue(&D_801031C0, D_801031D8, ARRAY_COUNT(D_801031D8));
    osSetEventMesg(OS_EVENT_FAULT, &D_801031C0, (OSMesg) RMON_MESG_FAULT);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &D_801031C0, (OSMesg) RMON_MESG_CPU_BREAK);
    osCreatePiManager(150, &D_80103218, D_801031F8, ARRAY_COUNT(D_801031F8));
    while (1) {
        osRecvMesg(&D_801031C0, (OSMesg) &sp2C, 1);
        s0 |= sp2C;
        if (!(s0 & RMON_MESG_FAULT) && !(s0 & RMON_MESG_CPU_BREAK)) {
            continue;
        }
        s0 &= ~RMON_MESG_FAULT;
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

#if 0
extern s32 D_800A6E90;
extern s32 D_800A6E94;
extern s32 D_800A6E98;
extern s8 D_800AF400;
extern s8 D_800AF408;
extern void *D_80103234;
extern ? D_80103238;
extern s32 D_80705014;
extern s32 D_80705018;
extern s32 D_8070501C;

void func_800676F8(void *arg0) {
    void *sp28;
    s32 var_v1;
    void *temp_a0;

    D_80705014 = D_800A6E98;
    D_80705018 = D_800A6E90;
    D_8070501C = D_800A6E94;
    _bcopy(arg0, (void *)0x80705094, 0x230);
    temp_a0 = arg0->unkF4;
    sp28 = temp_a0;
    _bcopy(temp_a0, (void *)0x80705294, 0x200);
    D_80103234 = temp_a0;
    _bcopy(temp_a0, &D_80103238, 0x200);
    var_v1 = 0x100005494;
    if (0x100005494 & 0x1F) {
        var_v1 = (0x100005494 & ~0x1F) + 0x20;
    }
    packWriteFile(0, -1, &D_800AF400, &D_800AF408, (u8 *)0x80700000, var_v1);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800676F8.s")
#endif

// write_epc_data_to_cpak
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
        while (1) {}
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
    while (1) {}
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067880.s")
#endif

UNUSED void diCpuReportWatchpoint(u32 address) {
    s32 moduleAddress;
    s32 moduleId;
    s32 i;
    for (i = 0; i < 100; i++) {
        func_8006869C();
    }
    cpuXYPrintf(30, 80, (char *) &D_800AF4AC /* "Watchpoint exception at %x" */, address);
    if (runlinkGetAddressInfo(address, &moduleId, &moduleAddress, NULL)) {
        cpuXYPrintf(30, 100, (char *) &D_800AF4C8 /* "Module %d at %08x" */, moduleId, moduleAddress);
    }
    while (1) {} // Infinite loop; waiting for the player to reset the console?
}

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
    while (1) {}
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

// Zeroes out `otherScreen`
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

// File split?

void __rmonSendFault(OSThread *thread) {
    volatile float f UNUSED;
    u8 *tPtr;
    u32 sent = 0;

    /* touch fpu to ensure registers are saved to the context structure */
    f = 0.0f;

    tPtr = (u8 *) thread;
    // sizeof(OSThread) in original, 0x230 in this.
    // TrapDanglingJump is __osRdbSend in the orginal
    while (sent < 0x230) {
        sent += TrapDanglingJump(tPtr + sent, 0x230 - sent, RDB_TYPE_GtoH_FAULT);
    }
}
