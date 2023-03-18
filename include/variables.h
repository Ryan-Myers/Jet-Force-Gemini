#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "structs.h"
#include "libultra_internal.h"

//objects.c
extern s32 ObjListCount;
extern Object **ObjList; // Not sure about the number of elements
extern VertexPosition gEnvmapPos;
extern s32 *Ftables;
extern s32 *Findex;
extern s32 Fmax;

//boot.c

extern OSThread D_800F9B30; //bootOSThread
extern OSThread D_800F9D60; //mainOSThread
extern u64 D_800F7B20[1]; //bootThreadStack
extern u64 D_800F9B28[1]; //mainThreadStack
extern u64 D_800F7B28[1];
void func_8003F088(void *); //bootThread
void mainThread(void *);
void diCpuTraceInit(void);

//diCpu.c
extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801031C0;
extern void *D_801031D8;
extern void *D_801031F8;
extern OSMesgQueue D_80103218;


#endif
