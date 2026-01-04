#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/diRcpTrace/diRcpTraceInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/diRcpTrace/diRcpTraceReset.s")

const char D_800AE670[] = "diRcpTrace: Buffer not allocated!\n";
const char D_800AE694[] = "diRcpTrace: Buffer overflow!\n";

#pragma GLOBAL_ASM("asm_us/nonmatchings/diRcpTrace/diRcpTrace.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/diRcpTrace/diRcpTraceGetInfo.s")
