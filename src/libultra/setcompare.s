.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

/* Handwritten function */
glabel __osSetCompare
    /* 9DFA0 8009D3A0 40845800 */  mtc0       $a0, $11 /* handwritten instruction */
    /* 9DFA4 8009D3A4 03E00008 */  jr         $ra
    /* 9DFA8 8009D3A8 00000000 */   nop
    /* 9DFAC 8009D3AC 00000000 */  nop
