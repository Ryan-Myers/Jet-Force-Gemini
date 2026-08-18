#ifndef _DICPU_H_
#define _DICPU_H_

#include <PR/ultratypes.h>

void _bcopy(const void *, void *, size_t);

void func_80066E14_67A14(void);
void stop_all_threads_except_main(void);
void func_80066D28_67928(void *arg0);
void diCpuTraceInit(void);

#endif
