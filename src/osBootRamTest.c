#include "osBootRamTest.h"
#include "common.h"

#define RAMTEST1_ADDRESS 0x2FB1F4 // 0xA02FB1F4
#define RAMTEST1_EXPECTED 0xD090010 // 0xAD090010
#define RAMTEST2_ADDRESS 0x2FE1C0 // 0xA02FE1C0
#define RAMTEST2_EXPECTED 0xD170014 // 0xAD170014

#ifdef VERSION_kiosk
// First value is "TRUE" if the test has not been run yet, and "FALSE" if it has. Second value is the result of the test.
static s32 sRamTest1[2] = { TRUE, FALSE };
static s32 sRamTest2[2] = { TRUE, FALSE };

s32 osBootRamTest1_6105(void) {
    s32 *ramTest = sRamTest1;
    if (ramTest[0] != FALSE) {
        ramTest[0] = FALSE;
        if (IO_READ(RAMTEST1_ADDRESS) == PHYS_TO_K1(RAMTEST1_EXPECTED)) {
            ramTest[1] = TRUE;
        }
    }
    return ramTest[1];
}
#else
s32 osBootRamTest1_6105(void) {
    s32 ret;

    ret = FALSE;
    if (IO_READ(RAMTEST1_ADDRESS) == PHYS_TO_K1(RAMTEST1_EXPECTED)) {
        ret = TRUE;
    }
    IO_WRITE(RAMTEST1_ADDRESS, FALSE);
    return ret;
}
#endif

#ifdef VERSION_kiosk
s32 osBootRamTest2_6105(void) {
    s32 *ramTest = sRamTest2;
    if (ramTest[0] != FALSE) {
        ramTest[0] = FALSE;
        if (IO_READ(RAMTEST2_ADDRESS) == PHYS_TO_K1(RAMTEST2_EXPECTED)) {
            ramTest[1] = TRUE;
        }
    }
    return ramTest[1];
}
#else
s32 osBootRamTest2_6105(void) {
    s32 ret;

    ret = FALSE;
    if (IO_READ(RAMTEST2_ADDRESS) == PHYS_TO_K1(RAMTEST2_EXPECTED)) {
        ret = TRUE;
    }
    IO_WRITE(RAMTEST2_ADDRESS, FALSE);
    return ret;
}
#endif
