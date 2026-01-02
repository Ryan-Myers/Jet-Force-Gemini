#include "memory.h"
#include "common.h"

#ifndef _ALIGN16
#define _ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)
#endif

/************ .data ************/

u8 mmExtendedRam = FALSE;
s32 mmColourTagUnk1 = COLOUR_TAG_WHITE;
s32 mmColourTagUnk2 = COLOUR_TAG_WHITE;

/*******************************/

/************ .bss ************/

MemoryPool gMemoryPools[POOL_COUNT];
s32 gNumberOfMemoryPools;
FreeQueueSlot gFreeQueue[FREE_QUEUE_SIZE];
u8 gFreeQueueDelay[FREE_QUEUE_SIZE];
s32 gFreeQueueCount;
s32 gFreeQueueTimer;
s32 FreeRAM;                  // Official Name: FreeRAM
s32 gPoolRAMSize[POOL_COUNT]; // Stores the size of each pool. Used in Debug menu to show free memory
s32 mmEndRam;

/*******************************/

/**
 * Creates the main memory pool.
 * Starts at 0x80106470. Ends at 0x80400000. Contains 1600 allocation slots.
 * Official Name: mmInit
 */
void mmInit(void) {
    gNumberOfMemoryPools = -1;
    if (mmExtendedRam) {
        mmEndRam = EXTENDED_RAM;
    } else {
        mmEndRam = RAM_END;
    }
    mempool_init(&gMainMemoryPool, mmEndRam - (s32) (&gMainMemoryPool), MAIN_POOL_SLOT_COUNT);
    mmSetDelay(2);
    gFreeQueueCount = 0;
}
/**
 * Returns true if the RAM has been extended
 * Official name: mmExtended
 */
UNUSED u8 mmExtended(void) {
    return mmExtendedRam;
}

/**
 * Creates a new memory pool that's contained inside another one.
 * Official name: mmAllocRegion
 */
MemoryPoolSlot *mmAllocRegion(s32 poolDataSize, s32 numSlots) {
    s32 size;
    MemoryPoolSlot *slots;
    UNUSED s32 unused_2;
    u32 flags = disableInterrupts();
    MemoryPoolSlot *newPool;

    size = poolDataSize + (numSlots * sizeof(MemoryPoolSlot));
    slots = (MemoryPoolSlot *) mmAlloc(size, COLOUR_TAG_WHITE);
    newPool = mempool_init(slots, size, numSlots);
    enableInterrupts(flags);
    return newPool;
}

/**
 * Create and initialise a memory pool in RAM that will act as the place where arbitrary allocations can go.
 * Will return the location of the first free slot in that pool.
 */
MemoryPoolSlot *mempool_init(MemoryPoolSlot *slots, s32 poolSize, s32 numSlots) {
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
    firstSlot->flags = SLOT_FREE;
    firstSlot->prevIndex = MEMSLOT_NONE;
    firstSlot->nextIndex = MEMSLOT_NONE;
    gMemoryPools[poolCount].curNumSlots++;
#ifdef JFGDIFFS
    if (poolCount == POOL_MAIN) {
        FreeRAM = firstSlotSize;
    }
    gPoolRAMSize[poolCount] = firstSlotSize;
#endif
    return gMemoryPools[poolCount].slots;
}

MemoryPoolSlot *mmAlloc(s32 size, u32 colourTag) {
#ifdef JFGDIFFS
    UNUSED s32 pad;
    s32 moduleId;
    s32 moduleAddress;
    u32 newColourTag;
    volatile s32 address = 0x666;
    newColourTag = colourTag;
    if ((u32) mmColourTagUnk1 != COLOUR_TAG_WHITE) {
        newColourTag = mmColourTagUnk1 | 0xFF000000;
    } else if ((u32) mmColourTagUnk2 != COLOUR_TAG_WHITE) {
        newColourTag = mmColourTagUnk2 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(address - 8, &moduleId, &moduleAddress, NULL);
        newColourTag = (moduleId << 24) | moduleAddress;
    }
#endif
    return mempool_slot_find(POOL_MAIN, size, newColourTag);
}

// Only differs from above by not returning a value.
void mmAlloc2(s32 size, u32 colourTag) {
    UNUSED s32 pad;
    s32 moduleId;
    s32 moduleAddress;
    u32 newColourTag;
    volatile s32 address = 0x666;
    newColourTag = colourTag;
    if ((u32) mmColourTagUnk1 != COLOUR_TAG_WHITE) {
        newColourTag = mmColourTagUnk1 | 0xFF000000;
    } else if ((u32) mmColourTagUnk2 != COLOUR_TAG_WHITE) {
        newColourTag = mmColourTagUnk2 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(address - 8, &moduleId, &moduleAddress, NULL);
        newColourTag = (moduleId << 24) | moduleAddress;
    }
    mempool_slot_find(POOL_MAIN, size, newColourTag);
}

/**
 * Search the existing empty slots and try to find one that can meet the size requirement.
 * Afterwards, write the new allocation data to the slot in question and return the address.
 */
MemoryPoolSlot *mempool_slot_find(MemoryPools poolIndex, s32 size, u32 colourTag) {
    s32 slotSize;
    MemoryPoolSlot *curSlot;
    UNUSED s32 pad;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    u32 intFlags;
    s32 nextIndex;
    s32 currIndex;

    intFlags = disableInterrupts();
    pool = &gMemoryPools[poolIndex];
    if (pool->maxNumSlots == pool->curNumSlots + 1) {
        enableInterrupts(intFlags);
        return NULL;
    }
    currIndex = MEMSLOT_NONE;
    if (size & 0xF) {
        size = (size & ~0xF);
        size += 0x10;
    }
    slots = pool->slots;
    slotSize = 0x7FFFFFFF;
    nextIndex = 0;
    do {
        curSlot = &slots[nextIndex];
        if (curSlot->flags == SLOT_FREE) {
            if (curSlot->size >= size && curSlot->size < slotSize) {
                slotSize = curSlot->size;
                currIndex = nextIndex;
            }
        }
        nextIndex = curSlot->nextIndex;
    } while (nextIndex != MEMSLOT_NONE);
    if (currIndex != MEMSLOT_NONE) {
        mempool_slot_assign(poolIndex, (s32) currIndex, size, 1, 0, colourTag);
        enableInterrupts(intFlags);
        return (MemoryPoolSlot *) (slots + currIndex)->data;
    }
    enableInterrupts(intFlags);
    return NULL;
}

/**
 * Allocate memory from a specific pool.
 * Official name: mmAllocR
 */
void *mmAllocR(MemoryPoolSlot *slots, s32 size) {
    s32 i;
    for (i = gNumberOfMemoryPools; i != 0; i--) {
        if (slots == gMemoryPools[i].slots) {
            return mempool_slot_find(i, size, 0);
        }
    }
    return (void *) NULL;
}

/**
 * Allocates memory from the main pool at a fixed address.
 * Rearranges the memory slots to place one at that address if possible.
 * Official Name: mmAllocAtAddr
 */
void *mmAllocAtAddr(s32 size, u8 *address, u32 colorTag) {
    s32 i;
    MemoryPoolSlot *curSlot;
    MemoryPoolSlot *slots;
    u32 intFlags;
    s32 moduleId;
    s32 moduleAddress;
    UNUSED s32 pad;
    volatile s32 vaddress = 0x666;

    if (size == 0) {
        stubbed_printf("*** mmAllocAtAddr: size = 0 ***\n");
    }

    intFlags = disableInterrupts();
    if (mmColourTagUnk1 != -1) {
        colorTag = mmColourTagUnk1 | 0xFF000000;
    } else if (mmColourTagUnk2 != -1) {
        colorTag = mmColourTagUnk2 | 0xFE000000;
    } else {
        runlinkGetAddressInfo(vaddress - 8, &moduleId, &moduleAddress, NULL);
        colorTag = (moduleId << 24) | moduleAddress;
    }
    if ((gMemoryPools[POOL_MAIN].curNumSlots + 1) == gMemoryPools[POOL_MAIN].maxNumSlots) {
        enableInterrupts(intFlags);
        stubbed_printf("\n*** mm Error *** ---> No more slots available.\n");
    } else {
        if (size & 0xF) {
            size = _ALIGN16(size);
        }
        slots = gMemoryPools[POOL_MAIN].slots;
        for (i = 0; i != MEMSLOT_NONE; i = curSlot->nextIndex) {
            curSlot = &slots[i];
            if (curSlot->flags == SLOT_FREE) {
                if ((u32) address >= (u32) curSlot->data &&
                    (u32) address + size <= (u32) curSlot->data + curSlot->size) {
                    if (address == (u8 *) curSlot->data) {
                        mempool_slot_assign(POOL_MAIN, i, size, 1, 0, colorTag);
                        enableInterrupts(intFlags);
                        return curSlot->data;
                    } else {
                        i = mempool_slot_assign(POOL_MAIN, i, (u32) address - (u32) curSlot->data, 0, 1, colorTag);
                        mempool_slot_assign(POOL_MAIN, i, size, 1, 0, colorTag);
                        enableInterrupts(intFlags);
                        return (slots + i)->data;
                    }
                }
            }
        }
        enableInterrupts(intFlags);
    }
    stubbed_printf("\n*** mm Error *** ---> Can't allocate memory at desired address. (%x, size = %d bytes)\n", address,
                   size);
    return NULL;
}

/**
 * Sets the tick timer for the free queue.
 * If it's set to 0, then it clears the existing queue.
 * Nonzero amounts set any future frees to wait that many ticks
 * before clearing from memory.
 * Official Name: mmSetDelay
 */
void mmSetDelay(s32 state) {
    u32 intFlags = disableInterrupts();
    gFreeQueueTimer = state;
    if (state == 0) { // flush free queue if state is 0.
        while (gFreeQueueCount > 0) {
            mempool_free_addr(gFreeQueue[--gFreeQueueCount].dataAddress);
        }
    }
    enableInterrupts(intFlags);
}

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmFlushFreeStack.s")
#endif

/**
 * Unallocates data from the pool that contains the data. Will free immediately if the free queue
 * state is set to 0, otherwise the data will just be marked for deletion.
 * Official Name: mmFree
 */
void mmFree(void *data) {
    u32 intFlags;
    volatile s32 sp18 = 0x666;
    intFlags = disableInterrupts();
    if (gFreeQueueTimer == 0) {
        mempool_free_addr(data);
    } else {
        mempool_free_queue(data);
    }
    enableInterrupts(intFlags);
}

/**
 * Frees all the addresses in the free queue.
 * Official Name: mmFreeTick
 */
void mmFreeTick(void) {
    s32 i;
    u32 intFlags;

    intFlags = disableInterrupts();

#ifdef JFGDIFFS
    if (FreeRAM < 0x14000) {
        runlinkLowMemoryPanic();
        if (FreeRAM < 0xC000 && runlinkIsModuleLoaded(3) != 0) {
            TrapDanglingJump();
        }
    }
#endif

    for (i = 0; i < gFreeQueueCount;) {
        gFreeQueueDelay[i]--;
        if (gFreeQueueDelay[i] == 0) {
            mempool_free_addr(gFreeQueue[i].dataAddress);
            gFreeQueue[i].dataAddress = gFreeQueue[gFreeQueueCount - 1].dataAddress;
            gFreeQueueDelay[i] = gFreeQueueDelay[gFreeQueueCount - 1];
            gFreeQueueCount--;
        } else {
            stubbed_printf("\n*** mm Error *** ---> Can't free ram at this location: %x\n", gFreeQueue[i].dataAddress);
            i++;
        }
    }

    enableInterrupts(intFlags);
}

/**
 * Searches the memory pools for a slot matching the given address.
 * If a slot is found, free it.
 */
void mempool_free_addr(u8 *address) {
    s32 slotIndex;
    s32 poolIndex;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;

    poolIndex = mempool_get_pool(address);
    pool = gMemoryPools;
    slots = pool[poolIndex].slots;
    for (slotIndex = 0; slotIndex != MEMSLOT_NONE; slotIndex = slot->nextIndex) {
        slot = &slots[slotIndex];

        if (address == (u8 *) slot->data) {
            if (slot->flags == SLOT_USED || slot->flags == SLOT_SAFEGUARD) {
                mempool_slot_clear(poolIndex, slotIndex);
            }
            break;
        }
        slot = &slots[slotIndex];
    }
}

/**
 * Adds the current memory address to the back of the queue, so it can be freed.
 */
void mempool_free_queue(void *dataAddress) {
    gFreeQueue[gFreeQueueCount].dataAddress = dataAddress;
    gFreeQueueDelay[gFreeQueueCount] = gFreeQueueTimer;
    gFreeQueueCount++;

    if (gFreeQueueCount >= ARRAY_COUNT(gFreeQueue)) {
        stubbed_printf("\n*** mm Error *** ---> stbf stack too deep!\n");
    }
}

/**
 * Returns the index of the memory pool containing the memory address.
 */
s32 mempool_get_pool(u8 *address) {
    s32 i;
    MemoryPool *pool;

    for (i = gNumberOfMemoryPools; i > 0; i--) {
        pool = &gMemoryPools[i];
        if ((u8 *) pool->slots >= address) {
            continue;
        }
        if (address < pool->size + (u8 *) pool->slots) {
            break;
        }
    }
    return i;
}

// single regalloc diff
#ifdef NON_MATCHING
/**
 * Clears the current slot of all information, effectively freeing the allocated memory.
 * Unused slots before and after will be merged with this slot
 */
void mempool_slot_clear(MemoryPools poolIndex, s32 slotIndex) {
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
    slots[slotIndex].flags = SLOT_FREE;
    if (poolIndex == POOL_MAIN) {
        FreeRAM += slots[slotIndex].size;
    }
    gPoolRAMSize[poolIndex] += slots[slotIndex].size;
    if (nextIndex != MEMSLOT_NONE) {
        if (nextSlot->flags == SLOT_FREE) {
            slots[slotIndex].size += nextSlot->size;
            tempNextIndex = nextSlot->nextIndex;
            slots[slotIndex].nextIndex = tempNextIndex;
            if (tempNextIndex != MEMSLOT_NONE) {
                slots[tempNextIndex].prevIndex = slotIndex;
            }
            gMemoryPools[poolIndex].curNumSlots--;
            slots[gMemoryPools[poolIndex].curNumSlots].index = nextIndex;
        }
    }
    if (prevIndex != MEMSLOT_NONE) {
        if (prevSlot->flags == SLOT_FREE) {
            prevSlot->size += slots[slotIndex].size;
            tempNextIndex = slots[slotIndex].nextIndex;
            prevSlot->nextIndex = tempNextIndex;
            if (tempNextIndex != MEMSLOT_NONE) {
                slots[tempNextIndex].prevIndex = prevIndex;
            }
            gMemoryPools[poolIndex].curNumSlots--;
            slots[gMemoryPools[poolIndex].curNumSlots].index = slotIndex;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/memory/mempool_slot_clear.s")
#endif

/**
 * Return the address of the first slot of a given memory pool.
 * Official Name: mmGetSlotPtr
 */
UNUSED MemoryPoolSlot *mmGetSlotPtr(MemoryPools poolIndex) {
    return gMemoryPools[poolIndex].slots;
}

/**
 * Return the current delay value
 * Official Name: mmGetDelay
 */
s32 mmGetDelay(void) {
    return gFreeQueueTimer;
}

/**
 * Initialise and attempts to fit the new memory block in the slot given.
 * Updates the linked list with any entries before and after then returns the new slot index.
 * If the region cannot fit, return the old slot instead.
 */
s32 mempool_slot_assign(MemoryPools poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken,
                        u32 colourTag) {
    MemoryPool *pool;
    MemoryPoolSlot *poolSlots;
    s32 index;
    s32 nextIndex;
    s32 poolSize;

#ifdef JFGDIFFS
    if (slotIsTaken == TRUE) {
        if (poolIndex == POOL_MAIN) {
            FreeRAM -= size;
        }
        gPoolRAMSize[poolIndex] -= size;
    }
#endif

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
        if (nextIndex != MEMSLOT_NONE) {
            poolSlots[nextIndex].prevIndex = index;
        }
        return index;
    }
    return slotIndex;
}

/**
 * Returns the passed in address aligned to the next 16-byte boundary.
 * Official name: mmAlign16
 */
u8 *mmAlign16(u8 *address) {
    s32 remainder = (s32) address & 0xF;
    if (remainder > 0) {
        address = (u8 *) (((s32) address - remainder) + 16);
    }
    return address;
}

/**
 * Returns the passed in address aligned to the next 4-byte boundary.
 * Official name: mmAlign4
 */
u8 *mmAlign4(u8 *address) {
    s32 remainder = (s32) address & 0x3;
    if (remainder > 0) {
        address = (u8 *) (((s32) address - remainder) + 4);
    }
    return address;
}

/**
 * Prints out the status of each memory pool slot.
 * Will mark based on what flags the slot has.
 * Official name: mmSlotPrint
 */
void mmSlotPrint(void) {
    s32 i;
    s32 skip;
    s32 flags;
    s32 index;
    s32 nextIndex;
    MemoryPoolSlot *slot;

    for (i = 0; i <= gNumberOfMemoryPools; i++) {
        stubbed_printf("Region = %d\t loc = %x\t size = %x, col = %x\t", i, gMemoryPools[i].slots,
                       gMemoryPools[i].size);
        slot = &gMemoryPools[i].slots[0];
        do {
            flags = slot->flags;
            nextIndex = slot->nextIndex;

            switch (flags) {
                case 0:
                    stubbed_printf("FREE");
                    break;
                case 1:
                    stubbed_printf("ALLOCATED");
                    break;
                case 2:
                    stubbed_printf("ALLOCATED,FIXED");
                    break;
                default:
                    stubbed_printf("\n");
                    break;
            }
            stubbed_printf("\n");
            if (nextIndex == -1) {
                continue;
            } else {
                slot = &gMemoryPools[i].slots[slot->nextIndex];
            }
        } while (nextIndex != -1);
    }
}
