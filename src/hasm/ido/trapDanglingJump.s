#ifdef MODERN_CC
.set gp=64
#endif

#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"
#include "asm_macros.h"

.text

/*
 * TrapDanglingJump - Runtime dynamic linker for overlay code loading
 * 
 * When game code calls a function in an unloaded overlay, those calls are
 * redirected to stub functions that call TrapDanglingJump. This function:
 *
 *   1. Saves all argument registers (a0-a3, f12-f15) to preserve the original call
 *   2. Uses the return address to identify which stub was called
 *   3. Searches relocation tables to find which overlay contains the target function
 *   4. Downloads/loads the required overlay via runlinkDownloadCode
 *   5. Calculates the real function address within the loaded overlay
 *   6. Restores arguments and tail-calls the real function
 *
 * Register usage:
 *   t0 - remaining overlay count (counts down)
 *   t1 - current overlay header pointer (iterates through overlayTable)
 *   t2 - overlay VRAM base address (or 0x80000450 for main module)
 *   t3 - relocation table pointer
 *   t4 - remaining relocation count (counts down)
 *   t5 - caller`s jump address (ra - 8, points to the JAL instruction)
 *   t6 - pointer into overlayRomTable
 *   t7 - target overlay index from relocation entry
 *   t8 - final computed target function address
 *   t9 - ROM table entry (overlay_index << 20 | function_offset)
 *
 * Overlay header structure (32 bytes):
 *   0x00: VRAM base address (0 if not loaded)
 *   0x08: .text section size
 *   0x0C: .data section size  
 *   0x10: .rodata section size
 *   0x14: relocation count (encoded, divide by 8)
 *
 * Relocation entry structure (8 bytes):
 *   0x00: target overlay index
 *   0x04: stub address offset (shifted left 8 bits)
 *
 * ROM table entry format:
 *   bits 31-20: overlay index
 *   bits 19-0:  function offset within overlay
 *
 * Returns: 0 if overlay could not be loaded, otherwise does not return
 *          (tail-calls the target function)
 */
LEAF(TrapDanglingJump)
    /* === PROLOGUE: Allocate stack and save all argument registers === */
    addiu      $sp, $sp, -0x60
    sw         a0, 0x14($sp)              /* Save integer arguments */
    sw         a1, 0x18($sp)
    sw         a2, 0x1C($sp)
    sw         a3, 0x20($sp)
    swc1       fa0, 0x24($sp)             /* Save float arguments */
    swc1       fa0f, 0x28($sp)
    swc1       fa1, 0x2C($sp)
    swc1       fa1f, 0x30($sp)
    sw         ra, 0x34($sp)              /* Save return address */
    sw         v0, 0x58($sp)              /* Save return value registers (may be set by caller) */
    sw         v1, 0x5C($sp)    
    subu       t5, ra, 8                  /* t5 = address of JAL that called the stub */
    lw         t0, overlayCount           /* t0 = number of overlays to search */
    lw         t1, overlayTable           /* t1 = pointer to first overlay header */

    /* === OUTER LOOP: Iterate through all loaded overlays === */
search_next_overlay:
    lw         v0, overlayCount
    subu       v0, v0, t0                 /* v0 = how many overlays we`ve checked */
    beqz       v0, search_main_module     /* First iteration: check main module`s relocations */
    
    /* Check if this overlay is currently loaded */
    lw         t2, 0x0(t1)                /* t2 = overlay VRAM base (0 if not loaded) */
    beqz       t2, advance_to_next_overlay
    
    /* Calculate address of overlay`s relocation table */
    /* relocTable = vramBase + textSize + dataSize + rodataSize */
    lw         v0, 0x8(t1)                /* .text size */
    addu       t3, t2, v0
    lw         v0, 0xC(t1)                /* .data size */
    addu       t3, t3, v0
    lh         t4, 0x14(t1)               /* relocation count (encoded) */
    sra        t4, t4, 3                  /* t4 = actual relocation count */
    lw         v0, 0x10(t1)               /* .rodata size */
    addu       t3, t3, v0                 /* t3 = relocation table address */
    b          search_relocation_table

    /* Fall back to main module`s relocation table */
search_main_module:
    li         t2, 0x80000450             /* Main module VRAM base */
    lw         t4, mainRelocCount         /* Number of main module relocations */
    lw         t3, mainRelocTable         /* Main module relocation table */

    /* === INNER LOOP: Search relocation table for matching stub address === */
search_relocation_table:
    blez       t4, advance_to_next_overlay  /* No relocations? Try next overlay */

search_next_relocation:
    /* Check if this relocation`s stub address matches our caller */
    lw         v0, 0x4(t3)                /* Get encoded stub offset */
    srl        v0, v0, 8                  /* Decode offset */
    addu       v0, v0, t2                 /* v0 = absolute stub address */
    bne        v0, t5, advance_to_next_relocation  /* No match? Keep searching */
    
    /* === FOUND MATCHING RELOCATION === */
    /* This relocation entry tells us which overlay contains the real function */
    
    lw         v0, overlayRomTable        /* Get ROM table base */
    lw         t7, 0x0(t3)                /* t7 = target overlay index */
    sll        t6, t7, 2                  /* t6 = index * 4 (pointer offset) */
    addu       t6, t6, v0                 /* t6 = &overlayRomTable[overlayIndex] */
    
    /* Save search state before calling download function */
    sw         t0, 0x38($sp)
    sw         t1, 0x3C($sp)
    sw         t2, 0x40($sp)
    sw         t3, 0x44($sp)
    sw         t4, 0x48($sp)
    sw         t6, 0x4C($sp)
    sw         t5, 0x50($sp)
    sw         t7, 0x54($sp)
    
    /* Download/load the target overlay */
    lw         a0, 0x0(t6)                /* ROM table entry */
    srl        a0, a0, 20                 /* Extract overlay index for download */
    jal        runlinkDownloadCode
    
    beqz       v0, download_failed        /* Download failed? Return 0 */
    
    /* === DOWNLOAD SUCCEEDED: Calculate real function address === */
    lw         v1, overlayTable
    lw         t6, 0x4C($sp)              /* Restore ROM table pointer */
    lw         t9, 0x0(t6)                /* t9 = ROM entry (index << 20 | offset) */
    srl        v0, t9, 20                 /* v0 = overlay index */
    sll        v0, v0, 5                  /* v0 = index * 32 (overlay header size) */
    addu       v0, v0, v1                 /* v0 = &overlayTable[index] */
    lw         t8, 0x0(v0)                /* t8 = overlay VRAM base (now loaded) */
    and        v0, t9, 0xFFFFF            /* v0 = function offset (low 20 bits) */
    addu       t8, t8, v0                 /* t8 = real function address! */
    
    /* Restore search state (for consistency, though not strictly needed) */
    lw         t0, 0x38($sp)
    lw         t1, 0x3C($sp)
    lw         t2, 0x40($sp)
    lw         t3, 0x44($sp)
    lw         t4, 0x48($sp)
    lw         t5, 0x50($sp)
    lw         t7, 0x54($sp)
    
    /* === EPILOGUE: Restore arguments and tail-call real function === */
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
    addiu      $sp, $sp, 0x60
    jr         t8                         /* Jump to real function (tail call) */

    /* === ERROR PATH: Overlay download failed === */
download_failed:
    lw         a0, 0x14($sp)              /* Restore arguments (for debugging) */
    lw         a1, 0x18($sp)
    lw         a2, 0x1C($sp)
    lw         a3, 0x20($sp)
    lwc1       fa0, 0x24($sp)
    lwc1       fa0f, 0x28($sp)
    lwc1       fa1, 0x2C($sp)
    lwc1       fa1f, 0x30($sp)
    lw         ra, 0x34($sp)
    or         v0, zero, zero             /* Return 0 (failure) */
    or         v1, zero, zero
    addiu      $sp, $sp, 0x60
    jr         ra

    /* === LOOP CONTROL: Advance to next relocation entry === */
advance_to_next_relocation:
    subu       t4, t4, 1                  /* Decrement relocation count */
    addiu      t3, t3, 0x8                /* Advance to next entry (8 bytes each) */
    bnez       t4, search_next_relocation

    /* === LOOP CONTROL: Advance to next overlay === */
advance_to_next_overlay:
    addiu      t1, t1, 0x20               /* Advance to next header (32 bytes each) */
    subu       t0, t0, 1                  /* Decrement overlay count */
    bgtz       t0, search_next_overlay
    
    /* === FALLTHROUGH: No matching relocation found === */
    lw         ra, 0x34($sp)
    addiu      $sp, $sp, 0x60
    jr         ra                         /* Return (v0/v1 undefined) */
END(TrapDanglingJump)
