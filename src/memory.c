#include "common.h"

#define MAIN_POOL_SLOT_COUNT 1600
#define RAM_END 0x80400000
#define EXTENDED_RAM_END 0x80600000

#ifndef _ALIGN16
#define _ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)
//#define _ALIGN16(val) ((val)&0xFFFFFFF0) + 0x10
#endif

void mmInit(void) {
    D_800FE350_FEF50 = -1;
    if (D_800A3E50_A4A50) {
        D_800FE878_FF478 = EXTENDED_RAM_END;
    } else {
        D_800FE878_FF478 = RAM_END;
    }
    func_8004A7C4((MemoryPoolSlot *)&D_80106470, D_800FE878_FF478 - (s32)&D_80106470, MAIN_POOL_SLOT_COUNT);
    mmSetDelay(2);
    D_800FE858_FF458 = 0;
}

u8 mmExtended(void) {
	return D_800A3E50_A4A50;
}

/**
 * Creates a new memory pool that is sectioned off the main one.
 */
MemoryPoolSlot *mmAllocRegion(s32 poolDataSize, s32 numSlots) {
    s32 size;
    MemoryPoolSlot *slots;
    UNUSED s32 unused_2;
    s32 *flags = disableInterrupts();
    MemoryPoolSlot *newPool;

    size = poolDataSize + (numSlots * sizeof(MemoryPoolSlot));
    slots = (MemoryPoolSlot *) mmAlloc(size, COLOUR_TAG_WHITE);
    newPool = func_8004A7C4(slots, size, numSlots);
    enableInterrupts(flags);
    return newPool;
}

/**
 * Create and initialise a memory pool in RAM that will act as the place where arbitrary allocations can go.
 * Will return the location of the first free slot in that pool.
*/
MemoryPoolSlot *func_8004A7C4(MemoryPoolSlot *slots, s32 poolSize, s32 numSlots) {
    MemoryPoolSlot *firstSlot;
    s32 poolCount;
    s32 i;
	s32 firstSlotSize;
    
    poolCount = ++D_800FE350_FEF50;
	firstSlotSize = poolSize - (numSlots * sizeof(MemoryPoolSlot));
    D_800FE310_FEF10[poolCount].maxNumSlots = numSlots;
    D_800FE310_FEF10[poolCount].curNumSlots = 0;
    D_800FE310_FEF10[poolCount].slots = slots;
    D_800FE310_FEF10[poolCount].size = poolSize;
    firstSlot = slots;
    for (i = 0; i < D_800FE310_FEF10[poolCount].maxNumSlots; i++) {
        firstSlot->index = i;
        firstSlot++;
    }
    firstSlot = &D_800FE310_FEF10[poolCount].slots[0];
    slots += numSlots;
    if ((s32) slots & 0xF) {
        firstSlot->data = (u8 *) _ALIGN16(slots);
    } else {
        firstSlot->data = (u8 *) slots;
    }
    firstSlot->size = firstSlotSize;
    firstSlot->flags = 0;
    firstSlot->prevIndex = -1;
    firstSlot->nextIndex = -1;
    D_800FE310_FEF10[poolCount].curNumSlots++;
	if (poolCount == 0) {
		FreeRAM = firstSlotSize;
	}
	D_800FE868_FF468[poolCount] = firstSlotSize;
    return D_800FE310_FEF10[poolCount].slots;
}

void *mmAlloc(s32 size, u32 colourTag) {
    u32 pad;
    s32 sp28;
    s32 sp24;
    u32 newColourTag;
    volatile s32 sp1C = 0x666;
    newColourTag = colourTag;
    if (D_800A3E54_A4A54 != -1) {
        newColourTag = D_800A3E54_A4A54 | 0xFF000000;
    } else if (D_800A3E58_A4A58 != -1) {
        newColourTag = D_800A3E58_A4A58 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(sp1C - 8, &sp28, &sp24, NULL);
        newColourTag = (sp28 << 24) | sp24;
    }
    return func_8004A9BC_4B5BC(0, size, newColourTag);
}

//Only differs from above by not returning a value.
void mmAlloc2(s32 size, u32 colourTag) {
    u32 pad;
    s32 sp28;
    s32 sp24;
    u32 newColourTag;
    volatile s32 sp1C = 0x666;
    newColourTag = colourTag;
    if (D_800A3E54_A4A54 != -1) {
        newColourTag = D_800A3E54_A4A54 | 0xFF000000;
    } else if (D_800A3E58_A4A58 != -1) {
        newColourTag = D_800A3E58_A4A58 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(sp1C - 8, &sp28, &sp24, NULL);
        newColourTag = (sp28 << 24) | sp24;
    }
    func_8004A9BC_4B5BC(0, size, newColourTag);
}

//allocate_from_memory_pool
MemoryPoolSlot *func_8004A9BC_4B5BC(s32 poolIndex, s32 size, u32 colourTag) {
    s32 slotSize;
    MemoryPoolSlot *curSlot;
    UNUSED s32 pad;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    s32 *flags;
    s32 nextIndex;
    s32 currIndex;

    flags = disableInterrupts();
    pool = &D_800FE310_FEF10[poolIndex];
    if ((pool->curNumSlots + 1) == (*pool).maxNumSlots) {
        enableInterrupts(flags);
        return 0;
    }
    currIndex = -1;
    if (size & 0xF) {
        size = (size & ~0xF);
		size += 0x10;
    }
    slots = pool->slots;
    slotSize = 0x7FFFFFFF;
    nextIndex = 0;
    do {
        curSlot = &slots[nextIndex];
        if (curSlot->flags == 0) {
            if ((curSlot->size >= size) && (curSlot->size < slotSize)) {
                slotSize = curSlot->size;
                currIndex = nextIndex;
            }
        }
        nextIndex = curSlot->nextIndex;
    } while (nextIndex != -1);
    if (currIndex != -1) {
        func_8004B288_4BE88(poolIndex, (s32) currIndex, size, 1, 0, colourTag);
        enableInterrupts(flags);
        return (MemoryPoolSlot *) (slots + currIndex)->data;
    }
    enableInterrupts(flags);
    return 0;
}

void *mmAllocR(MemoryPoolSlot *slots, s32 size) {
    s32 i;
    for (i = D_800FE350_FEF50; i != 0; i--) {
        if (slots == D_800FE310_FEF10[i].slots) {
            return func_8004A9BC_4B5BC(i, size, COLOUR_TAG_NONE);
        }
    }
    return (void *)NULL;
}

void *mmAllocAtAddr(s32 size, u8 *address, u32 colorTag) {
    s32 i;
    MemoryPoolSlot *curSlot;
    MemoryPoolSlot *slots;
    s32 *flags;
    s32 sp4C;
    s32 sp48;
    u32 pad;
    volatile u32 sp40 = 0x666; //fakematch?

    flags = disableInterrupts();
    if (D_800A3E54_A4A54 != -1) {
        colorTag = D_800A3E54_A4A54 | 0xFF000000;
    } else if (D_800A3E58_A4A58 != -1) {
        colorTag = D_800A3E58_A4A58 | 0xFE000000;
    } else {        
        runlinkGetAddressInfo(sp40 - 8, &sp4C, &sp48, NULL);
        colorTag = (sp4C << 24) | sp48;
    }
    if ((D_800FE310_FEF10[0].curNumSlots + 1) == D_800FE310_FEF10[0].maxNumSlots) {
        enableInterrupts(flags);
    } else {
        if (size & 0xF) {
            size = _ALIGN16(size);
        }
        slots = D_800FE310_FEF10[0].slots;
        for (i = 0; i != -1; i = curSlot->nextIndex) {
            curSlot = &slots[i];
            if (curSlot->flags == 0) {
                if ((u32) address >= (u32) curSlot->data && (u32)address + size <= (u32) curSlot->data + curSlot->size)  {
                    if (address == (u8 *) curSlot->data) {
                        func_8004B288_4BE88(0, i, size, 1, 0, colorTag);
                        enableInterrupts(flags);
                        return curSlot->data;
                    } else {
                        i = func_8004B288_4BE88(0, i, (u32) address - (u32) curSlot->data, 0, 1, colorTag);
                        func_8004B288_4BE88(0, i, size, 1, 0, colorTag);
                        enableInterrupts(flags);
                        return (slots + i)->data;
                    }
                }
            }
        }
        enableInterrupts(flags);
    }
    return NULL;
}

void mmSetDelay(s32 state) {
    s32 *flags;
    s32 var_v0;

    flags = disableInterrupts();
    mmDelay = state;
    if (state == 0) {
        while (D_800FE858_FF458 > 0) {
            func_8004AFC0_4BBC0(D_800FE358_FEF58[--D_800FE858_FF458].dataAddress);
        }
    }
    enableInterrupts(flags);
}

void mmFree(void *data) {
    s32 *flags;
    volatile s32 sp18 = 0x666; //fakematch?
    flags = disableInterrupts();
    if (mmDelay == 0) {
        func_8004AFC0_4BBC0(data);
    } else {
        func_8004B05C_4BC5C(data);
    }
    enableInterrupts(flags);
}

void TrapDanglingJump(void);
void mmFreeTick(void) {
    s32 i;
    s32 *flags;

    flags = disableInterrupts();
    if (FreeRAM < 0x14000) {
        runlinkLowMemoryPanic();
        if ((FreeRAM < 0xC000) && (runlinkIsModuleLoaded(3) != 0)) {
            TrapDanglingJump();
        }
    }

    for (i = 0; i < D_800FE858_FF458;) {
        D_800FE758_FF358[i]--;
        if (D_800FE758_FF358[i] == 0) {
            func_8004AFC0_4BBC0(D_800FE358_FEF58[i].dataAddress);
            D_800FE358_FEF58[i].dataAddress = D_800FE358_FEF58[D_800FE858_FF458 - 1].dataAddress;
            D_800FE758_FF358[i] = D_800FE758_FF358[D_800FE858_FF458 - 1];
            D_800FE858_FF458--;
        } else {
            i++;
        }
    }

    enableInterrupts(flags);
}

//free_slot_containing_address
void func_8004AFC0_4BBC0(u8 *address) {
    s32 slotIndex;
    s32 poolIndex;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;

    poolIndex = func_8004B098_4BC98(address);
    pool = D_800FE310_FEF10;
    slots = pool[poolIndex].slots;
    for (slotIndex = 0; slotIndex != -1; slotIndex = slot->nextIndex) {
        slot = &slots[slotIndex];

        if (address == (u8 *) slot->data) {
            if (slot->flags == 1 || slot->flags == 4) {
                func_8004B0F8_4BCF8(poolIndex, slotIndex);
            }
            break;
        }
        slot = &slots[slotIndex];
    }
}

void func_8004B05C_4BC5C(void *dataAddress) {
    D_800FE358_FEF58[D_800FE858_FF458].dataAddress = dataAddress;
    D_800FE758_FF358[D_800FE858_FF458] = mmDelay;
    D_800FE858_FF458++;
}

/**
 * Returns the index of the memory pool containing the memory address.
 */
//get_memory_pool_index_containing_address
s32 func_8004B098_4BC98(u8 *address) {
    s32 i;
    MemoryPool *pool;

    for (i = D_800FE350_FEF50; i > 0; i--) {
        pool = &D_800FE310_FEF10[i];
        if ((u8 *)pool->slots >= address) {
            continue;
        }
        if (address < pool->size + (u8 *)pool->slots) {
            break;
        }
    }
    return i;
}

#ifdef NON_MATCHING
//single regalloc diff
//free_memory_pool_slot
void func_8004B0F8_4BCF8(s32 poolIndex, s32 slotIndex) {
    s32 nextIndex;
    s32 prevIndex;
    s32 tempNextIndex;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *nextSlot;
    MemoryPoolSlot *prevSlot;

    slots = D_800FE310_FEF10[poolIndex].slots;
    nextIndex = slots[slotIndex].nextIndex;
    prevIndex = slots[slotIndex].prevIndex;
    nextSlot = &slots[nextIndex];
    prevSlot = &slots[prevIndex];
    slots[slotIndex].flags = 0;
    if (poolIndex == 0) {
        FreeRAM += slots[slotIndex].size;
    }
    D_800FE868_FF468[poolIndex] += slots[slotIndex].size;
    if (nextIndex != -1) {
        if (nextSlot->flags == 0) {
            slots[slotIndex].size += nextSlot->size;
            tempNextIndex = nextSlot->nextIndex;
            slots[slotIndex].nextIndex = tempNextIndex;
            if (tempNextIndex != -1) {
                slots[tempNextIndex].prevIndex = slotIndex;
            }
            D_800FE310_FEF10[poolIndex].curNumSlots--;
            slots[D_800FE310_FEF10[poolIndex].curNumSlots].index = nextIndex;
        }
    }
    if (prevIndex != -1) {
        if (prevSlot->flags == 0) {
            prevSlot->size += slots[slotIndex].size;
            tempNextIndex = slots[slotIndex].nextIndex;
            prevSlot->nextIndex = tempNextIndex;
            if (tempNextIndex != -1) {
                slots[tempNextIndex].prevIndex = prevIndex;
            }
            D_800FE310_FEF10[poolIndex].curNumSlots--;
            slots[D_800FE310_FEF10[poolIndex].curNumSlots].index = slotIndex;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8004B0F8_4BCF8.s")
#endif

UNUSED MemoryPoolSlot *mmGetSlotPtr(s32 poolIndex) {
    return D_800FE310_FEF10[poolIndex].slots;
}

s32 mmGetDelay(void) {
	return mmDelay;
}

//allocate_memory_pool_slot
s32 func_8004B288_4BE88(s32 poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag) {
    MemoryPool *pool;
    MemoryPoolSlot *poolSlots;
    s32 index;
    s32 nextIndex;
    s32 poolSize;

    if (slotIsTaken == TRUE) {
        if (poolIndex == 0) {
            FreeRAM -= size;
        }
        D_800FE868_FF468[poolIndex] -= size;
    }

    pool = &D_800FE310_FEF10[poolIndex];
    poolSlots = pool->slots;
    pool = pool; // Fakematch
    poolSlots[slotIndex].flags = slotIsTaken;
    poolSize = poolSlots[slotIndex].size;
    poolSlots[slotIndex].size = size;
    poolSlots[slotIndex].colourTag = colourTag;
    index = poolSlots[pool->curNumSlots].index;
    if (size < poolSize) {
        index = (pool->curNumSlots + poolSlots)->index;
        pool->curNumSlots++;
        poolSlots[index].data = &poolSlots[slotIndex].data[size];
        poolSlots[index].size = poolSize;
        poolSlots[index].size -= size;
        poolSlots[index].flags = newSlotIsTaken;
        poolSize = poolSlots[slotIndex].nextIndex;
        nextIndex = poolSize;
        poolSlots[index].prevIndex = slotIndex;
        poolSlots[index].nextIndex = nextIndex;
        poolSlots[slotIndex].nextIndex = index;
        if (nextIndex != -1) {
            poolSlots[nextIndex].prevIndex = index;
        }
        return index;
    }
    return slotIndex;
}

/**
 * Returns the passed in address aligned to the next 16-byte boundary.
 */
u8 *mmAlign16(u8 *address) {
    s32 remainder = (s32)address & 0xF;
    if (remainder > 0) {
        address = (u8 *)(((s32)address - remainder) + 16);
    }
    return address;
}

/**
 * Returns the passed in address aligned to the next 4-byte boundary.
 */
u8 *mmAlign4(u8 *address) {
    s32 remainder = (s32)address & 0x3;
    if (remainder > 0) {
        address = (u8 *)(((s32)address - remainder) + 4);
    }
    return address;
}

void mmSlotPrint(void) {
    s32 i;
    s32 skip;
    s32 index;
    s32 index2;
    s32 nextIndex;
    MemoryPoolSlot *slot;

    for (i = 0; (D_800FE350_FEF50 ^ 0) >= i; i++) {
        if (i && i) {} // Fakematch
        slot = &D_800FE310_FEF10[i].slots[0];
        index = 1;
        index2 = -index;
        do {
            index = slot->flags;
            nextIndex = slot->nextIndex;
            skip = nextIndex == index2;
            if (index) {
                if (((((((((!slot->nextIndex) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) {
                    if (nextIndex && nextIndex) {} // Fakematch
                }
            }
            if (skip) {
                continue;
            }
            slot = &D_800FE310_FEF10[i].slots[slot->nextIndex];
        } while (nextIndex != (-1));
    }
}
