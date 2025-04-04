.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

/* Handwritten function */
glabel osGetCount
    /* 94100 80093500 40024800 */  mfc0       $v0, $9 /* handwritten instruction */
    /* 94104 80093504 03E00008 */  jr         $ra
    /* 94108 80093508 00000000 */   nop
    /* 9410C 8009350C 00000000 */  nop
    /* 94110 80093510 00000000 */  nop
    /* 94114 80093514 00000000 */  nop
    /* 94118 80093518 00000000 */  nop
    /* 9411C 8009351C 00000000 */  nop
    /* 94120 80093520 00000000 */  nop
    /* 94124 80093524 00000000 */  nop
    /* 94128 80093528 00000000 */  nop
    /* 9412C 8009352C 00000000 */  nop
    /* 94130 80093530 00000000 */  nop
    /* 94134 80093534 00000000 */  nop
    /* 94138 80093538 00000000 */  nop
    /* 9413C 8009353C 00000000 */  nop
    /* 94140 80093540 00000000 */  nop
    /* 94144 80093544 00000000 */  nop
    /* 94148 80093548 00000000 */  nop
    /* 9414C 8009354C 00000000 */  nop
