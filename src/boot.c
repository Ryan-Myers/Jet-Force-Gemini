#include "common.h"

void bootThread(UNUSED void *args);

void boot(void) {
    osInitialize();
    osCreateThread(&D_800F9B30, 1, bootThread, 0, &D_800F7B20, OS_PRIORITY_IDLE);
    osStartThread(&D_800F9B30);
}

void bootThread(void *args) {
    diCpuTraceInit();
    osCreateThread(&D_800F9D60, 3, mainThread, 0, &D_800F9B28, 10);
    D_800F7B28[0x400] = 0;
    D_800F7B28[0] = 0;
    osStartThread(&D_800F9D60);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);
    while (1) {}
}

void bootCheckStack(void) {
    D_800F7B28[0x400]++;
    D_800F7B28[0]++;
}
