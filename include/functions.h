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

MemoryPoolSlot *new_memory_pool(MemoryPoolSlot *, s32, s32); // new_memory_pool
s32 allocate_memory_pool_slot(s32 poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag);
s32 get_memory_pool_index_containing_address(u8 *address);
void free_memory_pool_slot(s32 poolIndex, s32 slotIndex);
void mmSetDelay(s32 arg0);
s32 *disableInterrupts(void);
void enableInterrupts(s32*);
void *mmAlloc(s32 size, u32 colourTag);
void free_slot_containing_address(u8 *address);
void func_8004B05C_4BC5C(void *dataAddress);
MemoryPoolSlot *allocate_from_memory_pool(s32 poolIndex, s32 size, u32 colourTag);
s32 runlinkLowMemoryPanic(void);
s32 runlinkIsModuleLoaded(s32 module);
s32 runlinkGetAddressInfo(u32 arg0, s32 *arg1, s32 *arg2, u32 **arg3);
void func_80067880_68480(OSThread *);
void func_800676F8_682F8(void *arg0);
void func_8006768C(void);
void func_800677E4(void);

#endif
