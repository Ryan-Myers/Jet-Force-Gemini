#include "common.h"

const char D_800AE670[] = "diRcpTrace: Buffer not allocated!\n";
const char D_800AE694[] = "diRcpTrace: Buffer overflow!\n";

#pragma GLOBAL_ASM("asm/nonmatchings/diRcpTrace/diRcpTraceInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcpTrace/diRcpTraceReset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcpTrace/diRcpTrace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcpTrace/diRcpTraceGetInfo.s")
