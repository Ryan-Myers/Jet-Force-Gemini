#include "common.h"

void boot(void) {
    osInitialize();
    osCreateThread(&D_800F9B30_FA730, 1, func_8003F088, 0, &D_800F7B20_F8720, OS_PRIORITY_IDLE);
    osStartThread(&D_800F9B30_FA730);
}

void func_8003F088(void *arg0) {
    diCpuTraceInit();
    osCreateThread(&D_800F9D60_FA960, 3, mainThread, 0, &D_800F9B28_FA728, 10);
    D_800F7B28_F8728[0x400] = 0;
    D_800F7B28_F8728[0] = 0;
    osStartThread(&D_800F9D60_FA960);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);
    while (1) {}
}

void bootCheckStack(void) {
    D_800F7B28_F8728[0x400]++;
    D_800F7B28_F8728[0]++;
}
