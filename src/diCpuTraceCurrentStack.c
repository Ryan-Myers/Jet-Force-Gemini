#include "structs.h"

#ifdef __sgi
/**
 * Uses IDO specific -dollar compiler flag to get the current stack pointer.
 */
StackInfo *diCpuTraceCurrentStack(void) {
    return (StackInfo *) __$sp;
}
#else
/**
 * Uses GCC specific code to get the current stack pointer.
 */
StackInfo *diCpuTraceCurrentStack(void) {
    register StackInfo *sp;
    asm volatile ("move %0, $sp\n": "=r"(sp));
    return sp;
}
#endif
