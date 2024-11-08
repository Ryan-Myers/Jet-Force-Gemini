#include "common.h"

#define READ_ADDRESS_DIRECT(addr) (*(vu32 *)(addr))

#ifdef NON_MATCHING
extern s32 D_800A9F60_AAB60;
extern s32 D_800A9F64_AAB64;
extern s32 D_A02FB1F4;

s32 osBootRamTest1_6105(void) {
    if (READ_ADDRESS_DIRECT(0x800A9F60) != 0) {
    //if (D_800A9F60_AAB60 != 0) {
        D_800A9F60_AAB60 = 0;
        if (IO_READ(0x2FB1F4) == 0xAD090010) {
            D_800A9F64_AAB64 = 1;
        }
    }
    return D_800A9F64_AAB64;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/osBootRamTest/osBootRamTest1_6105.s")
#endif

#ifdef NON_MATCHING
extern s32 D_800A9F68_AAB68;
extern s32 D_800A9F6C_AAB6C;
extern s32 D_A02FE1C0;

s32 osBootRamTest2_6105(void) {
    if (READ_ADDRESS_DIRECT(0x800A9F68) != 0) {
    //if (D_800A9F68_AAB68 != 0) {
        D_800A9F68_AAB68 = 0;
        if (IO_READ(0x2FE1C0) == 0xAD170014) {
            D_800A9F6C_AAB6C = 1;
        }
    }
    return D_800A9F6C_AAB6C;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/osBootRamTest/osBootRamTest2_6105.s")
#endif
