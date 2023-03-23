#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"
#include "libultra_internal.h"


void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);
void mmFree(void *data);
u8 *rzipUncompress(u8 *compressedInput, u8 *decompressedOutput);
s32 rzipUncompressSize(u8 *arg0);
s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
s32 inflate_block(void);
void _blkclr(void *, size_t);

void func_8003F088(void *); //bootThread
void mainThread(void *);
void diCpuTraceInit(void);

MemoryPoolSlot *func_8004A7C4(MemoryPoolSlot *, s32, s32); // new_memory_pool
s32 func_8004B288_4BE88(s32 poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag);
s32 func_8004B098_4BC98(u8 *address);
void func_8004B0F8_4BCF8(s32 poolIndex, s32 slotIndex);
void mmSetDelay(s32 arg0);
s32 *disableInterrupts(void);
void enableInterrupts(s32*);
void *mmAlloc(s32 size, u32 colourTag);
void func_8004AFC0_4BBC0(u8 *address);
void func_8004B05C_4BC5C(void *dataAddress);
MemoryPoolSlot *func_8004A9BC_4B5BC(s32 poolIndex, s32 size, u32 colourTag);
s32 runlinkLowMemoryPanic(void);
s32 runlinkIsModuleLoaded(s32 module);
s32 runlinkGetAddressInfo(u32 arg0, s32 *arg1, s32 *arg2, u32 **arg3);

#endif
