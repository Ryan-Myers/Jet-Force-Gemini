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
extern s32 D_800A6E88;
extern s32 D_800A6E8C;
extern char D_800AF4EC[26];// = "Fault in thread %d   (%s)";
extern char D_800AF508[9];// = "epc NULL";
extern char D_800AF650[16];// = "free ram 		%08x";


//pi.c
extern OSMesg D_800FED28[16];//gPIMesgBuf[16];
extern OSMesgQueue D_800FED68;//gPIMesgQueue;
extern OSMesg D_800FED08;//gDmaMesg;
extern OSMesgQueue D_800FED10;//gDmaMesgQueue;
extern u32 *D_800FED80;//gAssetsLookupTable
extern u8 D_B22B0[], D_B23E0[];
void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);
void free_from_memory_pool(void *data);
u8 *rzipUncompress(u8 *compressedInput, u8 *decompressedOutput);
s32 rzipUncompressSize(u8 *arg0);

#endif
