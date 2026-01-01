#ifdef MODERN_CC
.set gp=64
#endif

#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"
#include "asm_macros.h"

.text

/*
 * TrapDanglingJump - Runtime linker for dynamic code loading
 * 
 * This function is called when code attempts to jump to an address
 * in an overlay that isnt currently loaded. It:
 *   1. Saves all argument registers to preserve the original call
 *   2. Searches overlay tables to find which overlay contains the target
 *   3. Downloads/loads the required overlay via runlinkDownloadCode
 *   4. Jumps to the now-loaded target function with original arguments
 *
 * Returns 0 if the overlay could not be loaded.
 */
LEAF(TrapDanglingJump)
    /* Allocate stack frame */
    addiu      $sp, $sp, -0x60
    /* Save all argument registers to preserve the original function call */
    sw         a0, 0x14($sp)
    sw         a1, 0x18($sp)
    sw         a2, 0x1C($sp)
    sw         a3, 0x20($sp)
    swc1       fa0, 0x24($sp)
    swc1       fa0f, 0x28($sp)
    swc1       fa1, 0x2C($sp)
    swc1       fa1f, 0x30($sp)
    sw         ra, 0x34($sp)
    sw         v0, 0x58($sp)
    sw         v1, 0x5C($sp)
    
    /* Calculate callers jump instruction address (ra - 8 = the JAL that got us here) */
    addiu      t5, ra, -0x8                      /* t5 = callerJumpAddr */
     
    /* Load global overlay table pointers */    
    lw         t0, overlayCount                  /* t0 = number of overlays to search */
    lw         t1, overlayTable                  /* t1 = pointer to overlay header table */

search_overlay_loop:
    /* Check if weve exhausted all overlays */
    lw         v0, overlayCount
    subu       v0, v0, t0
    beqz       v0, use_main_module               /* If first iteration, try main module */
    
    /* Check if this overlay is loaded (base address != 0) */
    lw         t2, 0x0(t1)                       /* t2 = overlay VRAM base address */
    beqz       t2, next_overlay                  /* Skip if overlay not loaded */
    
    /* Calculate overlays relocation table address */
    /* overlayEnd = base + textSize + dataSize + rodataSize */
    lw         v0, 0x8(t1)                        /* text section size */
    lh         t4, 0x14(t1)                       /* relocation count (encoded) */
    addu       t3, t2, v0
    lw         v0, 0xC(t1)                        /* data section size */
    sra        t4, t4, 3                          /* t4 = actual relocation count */
    addu       t3, t3, v0
    lw         v0, 0x10(t1)                       /* rodata section size */
    addu       t3, t3, v0                         /* t3 = relocation table ptr */
    b          check_relocations

use_main_module:
    lui        t2, (0x80000450 >> 16)
    /* Fall back to searching main modules relocation table */
    ori        t2, t2, (0x80000450 & 0xFFFF)      /* t2 = main module base address */
    lw         t4, mainRelocCount                 /* t4 = number of relocations */
    lw         t3, mainRelocTable                 /* t3 = relocation table ptr */

check_relocations:
    /* Skip if no relocations to check */
    blez       t4, next_overlay

search_reloc_loop:
    /* Each relocation entry is 8 bytes: [overlayIndex:4][targetOffset:4] */
    /* Check if this relocations target matches our caller address */
    lw         v0, 0x4(t3)                        /* Get target offset (shifted) */
    srl        v0, v0, 8
    addu       v0, v0, t2                         /* v0 = absolute target address */
    bne        v0, t5, next_relocation            /* Not our caller? Skip */
    
    /* === FOUND MATCHING RELOCATION === */
    /* The caller jumped to this stub - now load the real target overlay */
    
    lw         t7, 0x0(t3)                        /* t7 = target overlay index */
    lui        v0, %hi(overlayRomTable)
    lw         v0, %lo(overlayRomTable)(v0)
    sll        t6, t7, 2                          /* t6 = index * 4 */
    
    /* Save search state to stack before calling download function */
    sw         t0, 0x38($sp)
    addu       t6, t6, v0                         /* t6 = &overlayRomTable[index] */
    sw         t1, 0x3C($sp)
    sw         t2, 0x40($sp)
    sw         t3, 0x44($sp)
    sw         t4, 0x48($sp)
    sw         t6, 0x4C($sp)
    sw         t5, 0x50($sp)
    sw         t7, 0x54($sp)
    
    /* Download the overlay - a0 = ROM offset >> 20 (overlay ID) */
    lw         a0, 0x0(t6)
    srl        a0, a0, 20
    jal        runlinkDownloadCode
    
    /* Check if download succeeded */
    beqz       v0, download_failed
    
    /* === DOWNLOAD SUCCEEDED - Calculate real target address === */
    lw         t6, 0x4C($sp)
    lw         v1, overlayTable
    lw         t9, 0x0(t6)                        /* ROM table entry */
    li         AT, 0xFFFFF                        /* AT = offset mask (20 bits) */
    srl        v0, t9, 20                         /* v0 = overlay index */
    sll        v0, v0, 5                          /* v0 = index * 32 (header size) */
    addu       v0, v0, v1                         /* v0 = &overlayTable[index] */
    lw         t8, 0x0(v0)                        /* t8 = overlay VRAM base */
    and        v0, t9, AT                         /* v0 = function offset within overlay */
    lw         t0, 0x38($sp)
    addu       t8, t8, v0                         /* t8 = real target function address */
    
    /* Restore search state (not really needed, but matches original) */
    lw         t1, 0x3C($sp)
    lw         t2, 0x40($sp)
    lw         t3, 0x44($sp)
    lw         t4, 0x48($sp)
    lw         t5, 0x50($sp)
    lw         t7, 0x54($sp)
    
    /* Restore all original arguments for the real function call */
    lw         a0, 0x14($sp)
    lw         a1, 0x18($sp)
    lw         a2, 0x1C($sp)
    lw         a3, 0x20($sp)
    lwc1       fa0, 0x24($sp)
    lwc1       fa0f, 0x28($sp)
    lwc1       fa1, 0x2C($sp)
    lwc1       fa1f, 0x30($sp)
    lw         ra, 0x34($sp)
    lw         v0, 0x58($sp)
    lw         v1, 0x5C($sp)
    
    /* Tail-call the real target function with all original arguments */
    addiu      $sp, $sp, 0x60
    jr         t8

download_failed:
    lw         ra, 0x34($sp)
    /* Overlay download failed - restore args and return 0 */
    lw         a0, 0x14($sp)
    lw         a1, 0x18($sp)
    lw         a2, 0x1C($sp)
    lw         a3, 0x20($sp)
    lwc1       fa0, 0x24($sp)
    lwc1       fa0f, 0x28($sp)
    lwc1       fa1, 0x2C($sp)
    lwc1       fa1f, 0x30($sp)
    or         v0, zero, zero                     /* return 0 */
    or         v1, zero, zero
    addiu      $sp, $sp, 0x60
    jr         ra

next_relocation:
    addiu     t4, t4, -0x1
    /* Move to next relocation entry (8 bytes each) */
    addiu      t3, t3, 0x8
    bnez       t4, search_reloc_loop
    /* fall through to next_overlay */

next_overlay:
    addiu     t0, t0, -0x1
    /* Move to next overlay header (32 bytes each) */
    addiu      t1, t1, 0x20
    bgtz       t0, search_overlay_loop
    
    /* No matching relocation found - return normally */
    lw         ra, 0x34($sp)
    addiu      $sp, $sp, 0x60
    jr         ra
END(TrapDanglingJump)
