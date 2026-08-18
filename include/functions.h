#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"
#include "memory.h"
#include "boot.h"
#include "libc/stdarg.h"
#include "enums.h"

f32 Cosf(s32);


// void diRcpPrintDL(Gfx *, Gfx *, s32);
// u32 *func_8004FC6C_5086C(OSSched *arg0, s32 arg1, Gfx *arg2, OSMesgQueue *arg3, u32 *arg4);
// void diRcpTraceGetInfo(u32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6);

//lights.c
s32 addObjectLight(Object *, ObjectHeader48 *);
void lightSetupLightSources(Object *obj);
f32 lightDistanceCalc(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
void freeLights();
void lightCreateLightTable(s32 arg0, s32 arg1, s32 arg2, s32* arg3);
void turnLightOff(UnkLight *light);
void turnLightOn(UnkLight *light);
void toggleLight(UnkLight *light);
void changeLightColour(UnkLight *light, u8 red, u8 green, u8 blue);
void changeLightColourCycle(s32 arg0, s32 arg1); //TODO: arg0 typing is incorrect
void changeLightIntensity(UnkLight *light, u8 intensity);
void lightUpdateLights(s32 arg0);
unk800DC950 **lightGetLights(s32 *arg0);
f32 lightDirectionCalc(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
s32 lightKillGlowingLight(void *arg0); //TODO: arg0 typing is incorrect
void freeLights(void);

//diCpu.c
void func_800681D0_68DD0(s32 x, s32 y, char *s);
void cpuXYPrintf(s32 x, s32 y, const char *format, ...);
void func_8006837C_68F7C(void);
void func_80066EB0_67AB0(OSThread *);

//gameVi.c
s32 viGetVideoMode(void);


// level
LevelHeader *levelGetLevel(void);

//camera 
s32 camGetMode(void);

// This function is unique in that it has no specific limit on arguments, 
// and they can change even within the same function call it.
// This empty signature seems to be the way to handle it.
s32 TrapDanglingJump();

s32 objTvTimes(s32 timer);

s32 piRomGetFileSize(u32 assetIndex);

#endif
