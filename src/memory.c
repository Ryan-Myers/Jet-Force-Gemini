#include "common.h"

#define MAIN_POOL_SLOT_COUNT 1600
#define RAM_END 0x80400000
#define EXTENDED_RAM 0x80600000
#define _ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

u8 mmExtendedRam = FALSE;
s32 mmColourTagUnk1 = 0xFFFFFFFF;
s32 mmColourTagUnk2 = 0xFFFFFFFF;
MemoryPool gMemoryPools[4];
s32 gNumberOfMemoryPools;
FreeQueueSlot gFreeQueue[255];
u8 gFreeQueueDelay[255];
s32 gFreeQueueCount;
s32 mmDelay;
s32 FreeRAM;
s32 D_800FE868[4]; //Same count of gMemoryPools. Possibly stores the size of each pool. Used in Debug meny to show free memory
s32 mmEndRam;

const char D_800AD1C0[] = "*** mmAllocAtAddr: size = 0 ***\n";
const char D_800AD1E4[] = "\n*** mm Error *** ---> No more slots available.\n";
const char D_800AD218[] = "\n*** mm Error *** ---> Can't allocate memory at desired address. (%x, size = %d bytes)\n";
const char D_800AD270[] = "\n*** mm Error *** ---> Can't free ram at this location: %x\n";
const char D_800AD2AC[] = "\n*** mm Error *** ---> stbf stack too deep!\n";
const char D_800AD2DC[] = "Region = %d\t loc = %x\t size = %x, col = %x\t";
const char D_800AD308[] = "FREE";
const char D_800AD310[] = "ALLOCATED";
const char D_800AD31C[] = "ALLOCATED,FIXED";
const char D_800AD32C[] = "\n";
const char D_800AD330[] = "\n";

void mmInit(void) {
    gNumberOfMemoryPools = -1;
    if (mmExtendedRam) {
        mmEndRam = EXTENDED_RAM;
    } else {
        mmEndRam = RAM_END;
    }
    new_memory_pool((MemoryPoolSlot *)&gMainMemoryPool, mmEndRam - (s32)&gMainMemoryPool, MAIN_POOL_SLOT_COUNT);
    mmSetDelay(2);
    gFreeQueueCount = 0;
}

u8 mmExtended(void) {
    return mmExtendedRam;
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
    newPool = new_memory_pool(slots, size, numSlots);
    enableInterrupts(flags);
    return newPool;
}

/**
 * Create and initialise a memory pool in RAM that will act as the place where arbitrary allocations can go.
 * Will return the location of the first free slot in that pool.
*/
MemoryPoolSlot *new_memory_pool(MemoryPoolSlot *slots, s32 poolSize, s32 numSlots) {
    MemoryPoolSlot *firstSlot;
    s32 poolCount;
    s32 i;
    s32 firstSlotSize;
    
    poolCount = ++gNumberOfMemoryPools;
    firstSlotSize = poolSize - (numSlots * sizeof(MemoryPoolSlot));
    gMemoryPools[poolCount].maxNumSlots = numSlots;
    gMemoryPools[poolCount].curNumSlots = 0;
    gMemoryPools[poolCount].slots = slots;
    gMemoryPools[poolCount].size = poolSize;
    firstSlot = slots;
    for (i = 0; i < gMemoryPools[poolCount].maxNumSlots; i++) {
        firstSlot->index = i;
        firstSlot++;
    }
    firstSlot = &gMemoryPools[poolCount].slots[0];
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
    gMemoryPools[poolCount].curNumSlots++;
    if (poolCount == 0) {
        FreeRAM = firstSlotSize;
    }
    D_800FE868[poolCount] = firstSlotSize;
    return gMemoryPools[poolCount].slots;
}

void *mmAlloc(s32 size, u32 colourTag) {
    UNUSED s32 pad;
    s32 sp28;
    s32 sp24;
    u32 newColourTag;
    volatile s32 sp1C = 0x666;
    newColourTag = colourTag;
    if (mmColourTagUnk1 != -1) {
        newColourTag = mmColourTagUnk1 | 0xFF000000;
    } else if (mmColourTagUnk2 != -1) {
        newColourTag = mmColourTagUnk2 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(sp1C - 8, &sp28, &sp24, NULL);
        newColourTag = (sp28 << 24) | sp24;
    }
    return allocate_from_memory_pool(0, size, newColourTag);
}

//Only differs from above by not returning a value.
void mmAlloc2(s32 size, u32 colourTag) {
    UNUSED s32 pad;
    s32 sp28;
    s32 sp24;
    u32 newColourTag;
    volatile s32 sp1C = 0x666; //fakematch?
    newColourTag = colourTag;
    if (mmColourTagUnk1 != -1) {
        newColourTag = mmColourTagUnk1 | 0xFF000000;
    } else if (mmColourTagUnk2 != -1) {
        newColourTag = mmColourTagUnk2 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(sp1C - 8, &sp28, &sp24, NULL);
        newColourTag = (sp28 << 24) | sp24;
    }
    allocate_from_memory_pool(0, size, newColourTag);
}

MemoryPoolSlot *allocate_from_memory_pool(s32 poolIndex, s32 size, u32 colourTag) {
    s32 slotSize;
    MemoryPoolSlot *curSlot;
    UNUSED s32 pad;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    s32 *flags;
    s32 nextIndex;
    s32 currIndex;

    flags = disableInterrupts();
    pool = &gMemoryPools[poolIndex];
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
        allocate_memory_pool_slot(poolIndex, (s32) currIndex, size, 1, 0, colourTag);
        enableInterrupts(flags);
        return (MemoryPoolSlot *) (slots + currIndex)->data;
    }
    enableInterrupts(flags);
    return 0;
}

void *mmAllocR(MemoryPoolSlot *slots, s32 size) {
    s32 i;
    for (i = gNumberOfMemoryPools; i != 0; i--) {
        if (slots == gMemoryPools[i].slots) {
            return allocate_from_memory_pool(i, size, COLOUR_TAG_NONE);
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
    UNUSED s32 pad;
    volatile s32 sp40 = 0x666; //fakematch?

    flags = disableInterrupts();
    if (mmColourTagUnk1 != -1) {
        colorTag = mmColourTagUnk1 | 0xFF000000;
    } else if (mmColourTagUnk2 != -1) {
        colorTag = mmColourTagUnk2 | 0xFE000000;
    } else {        
        runlinkGetAddressInfo(sp40 - 8, &sp4C, &sp48, NULL);
        colorTag = (sp4C << 24) | sp48;
    }
    if ((gMemoryPools[0].curNumSlots + 1) == gMemoryPools[0].maxNumSlots) {
        enableInterrupts(flags);
    } else {
        if (size & 0xF) {
            size = _ALIGN16(size);
        }
        slots = gMemoryPools[0].slots;
        for (i = 0; i != -1; i = curSlot->nextIndex) {
            curSlot = &slots[i];
            if (curSlot->flags == 0) {
                if ((u32) address >= (u32) curSlot->data && (u32)address + size <= (u32) curSlot->data + curSlot->size)  {
                    if (address == (u8 *) curSlot->data) {
                        allocate_memory_pool_slot(0, i, size, 1, 0, colorTag);
                        enableInterrupts(flags);
                        return curSlot->data;
                    } else {
                        i = allocate_memory_pool_slot(0, i, (u32) address - (u32) curSlot->data, 0, 1, colorTag);
                        allocate_memory_pool_slot(0, i, size, 1, 0, colorTag);
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
        while (gFreeQueueCount > 0) {
            free_slot_containing_address(gFreeQueue[--gFreeQueueCount].dataAddress);
        }
    }
    enableInterrupts(flags);
}

void mmFree(void *data) {
    s32 *flags;
    volatile s32 sp18 = 0x666; //fakematch?
    flags = disableInterrupts();
    if (mmDelay == 0) {
        free_slot_containing_address(data);
    } else {
        func_8004B05C(data);
    }
    enableInterrupts(flags);
}

//TODO: TrapDanglingJump Seems to have a different definition here vs in __rmonSendFault
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

    for (i = 0; i < gFreeQueueCount;) {
        gFreeQueueDelay[i]--;
        if (gFreeQueueDelay[i] == 0) {
            free_slot_containing_address(gFreeQueue[i].dataAddress);
            gFreeQueue[i].dataAddress = gFreeQueue[gFreeQueueCount - 1].dataAddress;
            gFreeQueueDelay[i] = gFreeQueueDelay[gFreeQueueCount - 1];
            gFreeQueueCount--;
        } else {
            i++;
        }
    }

    enableInterrupts(flags);
}

void free_slot_containing_address(u8 *address) {
    s32 slotIndex;
    s32 poolIndex;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;

    poolIndex = get_memory_pool_index_containing_address(address);
    pool = gMemoryPools;
    slots = pool[poolIndex].slots;
    for (slotIndex = 0; slotIndex != -1; slotIndex = slot->nextIndex) {
        slot = &slots[slotIndex];

        if (address == (u8 *) slot->data) {
            if (slot->flags == 1 || slot->flags == 4) {
                free_memory_pool_slot(poolIndex, slotIndex);
            }
            break;
        }
        slot = &slots[slotIndex];
    }
}

void func_8004B05C(void *dataAddress) {
    gFreeQueue[gFreeQueueCount].dataAddress = dataAddress;
    gFreeQueueDelay[gFreeQueueCount] = mmDelay;
    gFreeQueueCount++;
}

/**
 * Returns the index of the memory pool containing the memory address.
 */
s32 get_memory_pool_index_containing_address(u8 *address) {
    s32 i;
    MemoryPool *pool;

    for (i = gNumberOfMemoryPools; i > 0; i--) {
        pool = &gMemoryPools[i];
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
void free_memory_pool_slot(s32 poolIndex, s32 slotIndex) {
    s32 nextIndex;
    s32 prevIndex;
    s32 tempNextIndex;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *nextSlot;
    MemoryPoolSlot *prevSlot;

    slots = gMemoryPools[poolIndex].slots;
    nextIndex = slots[slotIndex].nextIndex;
    prevIndex = slots[slotIndex].prevIndex;
    nextSlot = &slots[nextIndex];
    prevSlot = &slots[prevIndex];
    slots[slotIndex].flags = 0;
    if (poolIndex == 0) {
        FreeRAM += slots[slotIndex].size;
    }
    D_800FE868[poolIndex] += slots[slotIndex].size;
    if (nextIndex != -1) {
        if (nextSlot->flags == 0) {
            slots[slotIndex].size += nextSlot->size;
            tempNextIndex = nextSlot->nextIndex;
            slots[slotIndex].nextIndex = tempNextIndex;
            if (tempNextIndex != -1) {
                slots[tempNextIndex].prevIndex = slotIndex;
            }
            gMemoryPools[poolIndex].curNumSlots--;
            slots[gMemoryPools[poolIndex].curNumSlots].index = nextIndex;
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
            gMemoryPools[poolIndex].curNumSlots--;
            slots[gMemoryPools[poolIndex].curNumSlots].index = slotIndex;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/memory/free_memory_pool_slot.s")
#endif

UNUSED MemoryPoolSlot *mmGetSlotPtr(s32 poolIndex) {
    return gMemoryPools[poolIndex].slots;
}

s32 mmGetDelay(void) {
    return mmDelay;
}

s32 allocate_memory_pool_slot(s32 poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag) {
    MemoryPool *pool;
    MemoryPoolSlot *poolSlots;
    s32 index;
    s32 nextIndex;
    s32 poolSize;

    if (slotIsTaken == TRUE) {
        if (poolIndex == 0) {
            FreeRAM -= size;
        }
        D_800FE868[poolIndex] -= size;
    }

    pool = &gMemoryPools[poolIndex];
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

    for (i = 0; (gNumberOfMemoryPools ^ 0) >= i; i++) {
        if (i && i) {} // Fakematch
        slot = &gMemoryPools[i].slots[0];
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
            slot = &gMemoryPools[i].slots[slot->nextIndex];
        } while (nextIndex != (-1));
    }
}
