#include "boot.h"
#include "common.h"
#include "stacks.h"

/************ .bss ************/

u64 gThread1Stack[STACKSIZE(STACK_IDLE) + 1];
u64 gThread3Stack[STACKSIZE(STACK_GAME) + 1];
OSThread gThread1;    // OSThread for thread 1
u64 gPadStack1[0x10]; // Not sure what this padding is
OSThread gThread3;    // OSThread for thread 3
u64 gPadStack2[0x10]; // Not sure what this padding is

/******************************/

/**
 * Where it all begins.
 * Once the boot procedure is finished in IPL3, this function is run.
 * It kicks things off by initialising thread1, which serves as the top level
 * Official name: boot
 */
void boot(void) {
    osInitialize();
    osCreateThread(&gThread1, 1, &thread1_main, 0, &gThread1Stack[STACKSIZE(STACK_IDLE)], OS_PRIORITY_IDLE);
    osStartThread(&gThread1);
}

/**
 * Initialise the crash handler thread, then initialise the main game thread.
 * Reset the start and endpoint of the game thread stack, then set thread priority to zero, effectively
 * stopping this thread, as it's no longer needed.
 */
void thread1_main(UNUSED void *unused) {
    diCpuTraceInit();
    osCreateThread(&gThread3, 3, &mainThread, 0, &gThread3Stack[STACKSIZE(STACK_GAME)], 10);
    gThread3Stack[STACKSIZE(STACK_GAME)] = 0;
    gThread3Stack[0] = 0;
    osStartThread(&gThread3);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);
    while (1) {}
}

/**
 * Increments the start and endpoint of the stack.
 * They should have an equal value, so if they don't, that triggers a printout saying a stack wraparound has occured.
 * Official Name: bootCheckStack
 */
void bootCheckStack(void) {
    gThread3Stack[STACKSIZE(STACK_GAME)]++;
    gThread3Stack[0]++;
    if ((gThread3Stack[STACKSIZE(STACK_GAME)] != gThread3Stack[0])) {
        stubbed_printf("WARNING: Stack overflow/underflow!!!\n");
    }
}
