.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.0 */

/* Handwritten function */
glabel __osGetSR
/* D5170 800D4570 40026000 */  mfc0       $v0, $12 /* handwritten instruction */
/* D5174 800D4574 03E00008 */  jr         $ra
/* D5178 800D4578 00000000 */   nop
/* D517C 800D457C 00000000 */  nop
