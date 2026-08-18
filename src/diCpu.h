#ifndef _DICPU_H_
#define _DICPU_H_

#include <PR/ultratypes.h>
#include <PR/os_thread.h>

void _bcopy(const void *, void *, size_t);

void func_80066E14_67A14(void);
void stop_all_threads_except_main(void);
void func_80066D28_67928(void *arg0);
void diCpuTraceInit(void);
void func_800681D0_68DD0(s32 x, s32 y, char *s);
void cpuXYPrintf(s32 x, s32 y, const char *format, ...);
void func_8006837C_68F7C(void);
void func_80066EB0_67AB0(OSThread *);

#endif
