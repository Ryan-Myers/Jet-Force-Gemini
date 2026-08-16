#include "common.h"
#include "gameVi.h"
#include "PR/rdb.h"
#include "PRinternal/rmonint.h"
#include "runLink.h"
#include "stdarg.h"

extern OSThread *__osGetActiveQueue(void);

#ifdef VERSION_kiosk
const char D_800AE750_AF350[] = "7.2";
#else
const char D_800AE750_AF350[] = "9.6";
#endif
const char D_800AE754_AF354[] = "interrupt";
const char D_800AE760_AF360[] = "TLB modify";
const char D_800AE76C_AF36C[] = "TLB load or fetch";
const char D_800AE780_AF380[] = "TLB store";
const char D_800AE78C_AF38C[] = "addr load or fetch";
const char D_800AE7A0_AF3A0[] = "addr store";
const char D_800AE7AC_AF3AC[] = "bus error fetch";
const char D_800AE7BC_AF3BC[] = "bus error reference";
const char D_800AE7D0_AF3D0[] = "syscall";
const char D_800AE7D8_AF3D8[] = "breakpoint";
const char D_800AE7E4_AF3E4[] = "reserved instruction";
const char D_800AE7FC_AF3FC[] = "co-pro unusable";
const char D_800AE80C_AF40C[] = "overflow";
const char D_800AE818_AF418[] = "trap";
const char D_800AE820_AF420[] = "virtual coherency";
const char D_800AE834_AF434[] = "floating point";
const char D_800AE844_AF444[] = ">fault< ";
const char D_800AE850_AF450[] = "CORE";
const char D_800AE858_AF458[] = "";
#ifdef VERSION_kiosk
// These rodata symbols are contained within the relevant functions, which I didn't do for kiosk.
const char D_800AF40C[] = "%08x:%08x";
const char D_800AF418[] = "MEMORY REGION %d";
const char D_800AF42C[] = "MOD";
const char D_800AF430[] = "OFFSET";
const char D_800AF438[] = "SIZE";
const char D_800AF440[] = "ADDRESS";
const char D_800AF448[] = "Texture %d";
const char D_800AF454[] = "Module %d";
const char D_800AF460[] = "%d";
const char D_800AF464[] = "%08x";
const char D_800AF46C[] = "%d";
const char D_800AF470[] = "%08x";
const char D_800AF478[] = "Module %d at %08x";
const char D_800AF48C[] = "%1x";
const char D_800AF490[] = "%d lines logged";
const char D_800AF4A0[] = "Page %d/%d";
#endif

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
    osCreateMesgQueue(&D_801024E0_BCD20, D_801024F8_BCD38, ARRAY_COUNT(D_801024F8_BCD38));
    osSetEventMesg(OS_EVENT_FAULT, &D_801024E0_BCD20, (OSMesg) RMON_MESG_FAULT);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &D_801024E0_BCD20, (OSMesg) RMON_MESG_CPU_BREAK);
    osCreatePiManager(150, &D_80102538_BCD78, D_80102518_BCD58, ARRAY_COUNT(D_80102518_BCD58));
    while (1) {
        osRecvMesg(&D_801024E0_BCD20, (OSMesg) &sp2C, 1);
        s0 |= sp2C;
        if (!(s0 & RMON_MESG_FAULT) && !(s0 & RMON_MESG_CPU_BREAK)) {
            continue;
        }
        s0 &= ~RMON_MESG_FAULT;
        stop_all_threads_except_main();
        func_80066E14_67A14();
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
extern s32 D_800A64D0_A70D0;
extern s32 D_800A64D4_A70D4;
extern s32 D_800A64D8_A70D8;
extern s8 D_800AE850_AF450;
extern s8 D_800AE858_AF458;
extern void *D_80102554_BCD94;
extern ? D_80102558_BCD98;
extern s32 D_80705014;
extern s32 D_80705018;
extern s32 D_8070501C;

void func_80066D28_67928(void *arg0) {
    void *sp28;
    s32 var_v1;
    void *temp_a0;

    D_80705014 = D_800A64D8_A70D8;
    D_80705018 = D_800A64D0_A70D0;
    D_8070501C = D_800A64D4_A70D4;
    _bcopy(arg0, (void *)0x80705094, 0x230);
    temp_a0 = arg0->unkF4;
    sp28 = temp_a0;
    _bcopy(temp_a0, (void *)0x80705294, 0x200);
    D_80102554_BCD94 = temp_a0;
    _bcopy(temp_a0, &D_80102558_BCD98, 0x200);
    var_v1 = 0x100005494;
    if (0x100005494 & 0x1F) {
        var_v1 = (0x100005494 & ~0x1F) + 0x20;
    }
    packWriteFile(0, -1, &D_800AE850_AF450, &D_800AE858_AF458, (u8 *)0x80700000, var_v1);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80066D28_67928.s")
#endif

// write_epc_data_to_cpak
void func_80066E14_67A14(void) {
    OSThread *thread;

    for (thread = __osGetActiveQueue(); thread->priority != -1; thread = thread->tlnext) {
        if (thread->priority > OS_PRIORITY_IDLE) {
            if (thread->flags & 2 || thread->flags & 1) {
                break;
            }
        }
    }
    if (thread->priority != -1) {
        func_80066D28_67928(thread);
    }
    func_80066EB0_67AB0(thread);
}

// interactive_crash_screens
#ifdef NON_EQUIVALENT
extern s32 osCicId;
extern s32 D_800A64D8_A70D8;

void func_80066EB0_67AB0(OSThread *thread) {
    s32 i;

    // Anti Piracy Check
    if ((D_800A3320_A3F20 == 0) || (osCicId != 6105)) {
        while (1) {}
    }
    if (D_800A64D8_A70D8 == 0) {
        if (thread && thread && thread) {}
    }
    if (viGetVideoMode() != 0) {
        D_800A651C_A711C = 1;
    } else {
        D_800A651C_A711C = 0;
    }
    for (i = 0; i < 100; i++) {
        func_8006837C_68F7C();
    }
    while (1) {}
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80066EB0_67AB0.s")
#endif

UNUSED void diCpuReportWatchpoint(u32 address) {
    s32 moduleAddress;
    s32 moduleId;
    s32 i;
    for (i = 0; i < 100; i++) {
        func_8006837C_68F7C();
    }
    cpuXYPrintf(30, 80, "Watchpoint exception at %x\n", address);
    if (runlinkGetAddressInfo(address, &moduleId, &moduleAddress, NULL)) {
        cpuXYPrintf(30, 100, "Module %d at %08x", moduleId, moduleAddress);
    }
    while (1) {} // Infinite loop; waiting for the player to reset the console?
}

#ifdef VERSION_kiosk
const char D_800AF4DC[] = "%04d: %04d: %s";
const char D_800AF4EC[] = "Fault in thread %d   (%s)";
const char D_800AF508[] = "epc NULL";
const char D_800AF514[] = "epc %d,%x,%s";
const char D_800AF524[] = "epc %08x,?,?,?";
const char D_800AF534[] = "ra   NULL";
const char D_800AF540[] = "ra   %d,%x,%s";
const char D_800AF550[] = "ra   %08x,?,?,?";
const char D_800AF560[] = " cause\t\tmmAlloc(%d,%8x)\n";
const char D_800AF57C[] = "%s exception";
const char D_800AF58C[] = "cause\t\t\t%08x";
const char D_800AF59C[] = "crashed %s %s";
const char D_800AF5AC[] = "crashed %s type %d";
const char D_800AF5C0[] = "crashed %s unknown";
const char D_800AF5D4[] = "error %s %s";
const char D_800AF5E0[] = "error %s type %d";
const char D_800AF5F4[] = "error %s unknown";
const char D_800AF608[] = "aborted %s %s";
const char D_800AF618[] = "aborted %s type %d";
const char D_800AF62C[] = "aborted %s unknown";
const char D_800AF640[] = "badvaddr\t%08x";
const char D_800AF650[] = "free ram\t\t%08x";
const char D_800AF660[] = "at 0x%08x v0 0x%08x";
const char D_800AF674[] = "v1 0x%08x a0 0x%08x";
const char D_800AF688[] = "a1 0x%08x a2 0x%08x";
const char D_800AF69C[] = "a3 0x%08x t0 0x%08x";
const char D_800AF6B0[] = "t1 0x%08x t2 0x%08x";
const char D_800AF6C4[] = "t3 0x%08x t4 0x%08x";
const char D_800AF6D8[] = "t5 0x%08x t6 0x%08x";
const char D_800AF6EC[] = "t7 0x%08x s0 0x%08x";
const char D_800AF700[] = "s1 0x%08x s2 0x%08x";
const char D_800AF714[] = "s3 0x%08x s4 0x%08x";
const char D_800AF728[] = "s5 0x%08x s6 0x%08x";
const char D_800AF73C[] = "s7 0x%08x t8 0x%08x";
const char D_800AF750[] = "t9 0x%08x gp 0x%08x";
const char D_800AF764[] = "sp 0x%08x s8 0x%08x";
const char D_800AF778[] = "sr 0x%08x";
const char D_800AF784[] = "Level is\t\t\t%s";
const char D_800AF794[] = "Position is\t%.1f, %.1f, %.1f";
const char D_800AF7B4[] = "Track polygon buffer overflow";
const char D_800AF7D4[] = "Track edge buffer overflow";
const char D_800AF7F0[] = "Object polygon buffer overflow";
const char D_800AF810[] = "Object edge buffer overflow";
#endif

#ifdef NON_EQUIVALENT
extern s32 D_800A64D0_A70D0;
extern s32 D_800A64D4_A70D4;
extern s32 D_800A64D8_A70D8;

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

    var_a1 = (D_800A64D4_A70D4 * 41) + 0x80700000;
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
    D_800A64D4_A70D4++;
    if (D_800A64D4_A70D4 >= 500) {
        D_800A64D8_A70D8 = 1;
        D_800A64D4_A70D4 = 0;
    }
    if (D_800A64D8_A70D8 != 0) {
        D_800A64D0_A70D0 = D_800A64D4_A70D4;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/diCpuLogMessage.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_80067790_68390.s")

void render_epc_lock_up_display(epcInfo *arg0);

void diCpuTraceMallocFault(s32 epc, s32 size, u32 colourTag) {
    epcInfo epcinfo;
    bzero(&epcinfo, sizeof(epcInfo));
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
    D_800A64CC_A70CC += arg0;
    if (D_800A64CC_A70CC > 60) {
        D_800A64CC_A70CC = 0;
        D_800A64C8_A70C8++;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/render_epc_lock_up_display.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800680B0_68CB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diCpu/func_800681D0_68DD0.s")

void cpuXYPrintf(s32 x, s32 y, const char *format, ...) {
    va_list args;
    char s[255];

    va_start(args, format);
    vsprintf(s, format, args);
    va_end(args);

    if (D_800A651C_A711C != 0) {
        if (D_800A651C_A711C == 1) {
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
        func_800681D0_68DD0(x, y, s);
    }
}

// Zeroes out `otherScreen`
void func_8006837C_68F7C(void) {
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

extern u32 __osRdbSend_Trap(u8 *buf, u32 size, u32 type);
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
        sent += __osRdbSend_Trap(tPtr + sent, 0x230 - sent, RDB_TYPE_GtoH_FAULT);
    }
}
