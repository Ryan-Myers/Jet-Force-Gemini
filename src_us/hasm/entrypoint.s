.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .bss

.equ entrypointThreadStackSize, 0x1000
dlabel entrypointThreadStack
.space entrypointThreadStackSize
.size entrypointThreadStack, entrypointThreadStackSize

.section .text, "ax"

glabel entrypoint
lui        $t0, %hi(__BSS_SECTION_START)
lui        $t1, (0x54E50 >> 16)
addiu      $t0, $t0, %lo(__BSS_SECTION_START)
ori        $t1, $t1, (0x54E50 & 0xFFFF)
.clear_bytes:
addi       $t1, $t1, -0x8
sw         $zero, 0x0($t0)
sw         $zero, 0x4($t0)
bnez       $t1, .clear_bytes
 addi      $t0, $t0, 0x8
lui        $t2, %hi(boot)
lui        $sp, %hi(entrypointThreadStack + entrypointThreadStackSize)
addiu      $t2, $t2, %lo(boot)
jr         $t2
 addiu     $sp, $sp, %lo(entrypointThreadStack + entrypointThreadStackSize)
nop
nop
nop
nop
nop
nop
endlabel entrypoint
