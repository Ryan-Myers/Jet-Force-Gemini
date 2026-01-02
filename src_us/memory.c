#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmExtended.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAllocRegion.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_init.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAlloc.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAlloc2.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_slot_find.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAllocR.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAllocAtAddr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmSetDelay.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmFlushFreeStack.s")
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmFree.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmFreeTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_free_addr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_free_queue.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_get_pool.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_slot_clear.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmGetSlotPtr.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmGetDelay.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mempool_slot_assign.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAlign16.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmAlign4.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/mmSlotPrint.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/memory/D_800AC660_AD260.s")
