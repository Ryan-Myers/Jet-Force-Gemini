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

extern OSThread D_800F9B30_FA730; //bootOSThread
extern OSThread D_800F9D60_FA960; //mainOSThread
extern u64 D_800F7B20_F8720[1]; //bootThreadStack
extern u64 D_800F9B28_FA728[1]; //mainThreadStack
extern u64 D_800F7B28_F8728[1];
void func_8003F088(void *); //bootThread
void mainThread(void *);
void diCpuTraceInit(void);

//diCpu.c
extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801031C0_103DC0;
extern void *D_801031D8_103DD8;
extern void *D_801031F8_103DF8;
extern OSMesgQueue D_80103218_103E18;
extern s32 D_800A6E88_A7A88;
extern s32 D_800A6E8C_A7A8C;
extern char D_800AF4EC_B00EC[26];// = "Fault in thread %d   (%s)";
extern char D_800AF508_B0108[9];// = "epc NULL";
extern char D_800AF650_B0250[16];// = "free ram 		%08x";

//prnBorders.c
extern Gfx D_800A3F90_A4B90[11][2]; //unknown number of graphics commands. This is a best guess.

#endif
