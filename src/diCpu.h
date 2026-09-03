#ifndef _DICPU_H_
#define _DICPU_H_

#include <PR/os_message.h>
#include <PR/os_thread.h>
#include <PR/ultratypes.h>

void _bcopy(const void *, void *, size_t);

extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801024E0_BCD20;
extern OSMesg D_801024F8_BCD38[8];
extern OSMesg D_80102518_BCD58[8];
extern OSMesgQueue D_80102538_BCD78;
extern s32 D_800A64C8_A70C8;
extern s32 D_800A64CC_A70CC;
extern s32 D_800A651C_A711C;

void func_80066E14_67A14(void);
void stop_all_threads_except_main(void);
void func_80066D28_67928(void *arg0);
void diCpuTraceInit(void);
void func_800681D0_68DD0(s32 x, s32 y, char *s);
void cpuXYPrintf(s32 x, s32 y, const char *format, ...);
void func_8006837C_68F7C(void);
void func_80066EB0_67AB0(OSThread *);

#endif
