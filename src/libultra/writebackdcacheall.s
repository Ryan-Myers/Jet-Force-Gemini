.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

/* Handwritten function */
glabel osWritebackDCacheAll
    /* 995D0 800989D0 3C088000 */  lui        $t0, 0x8000
    /* 995D4 800989D4 240A2000 */  addiu      $t2, $zero, 0x2000
    /* 995D8 800989D8 010A4821 */  addu       $t1, $t0, $t2
    /* 995DC 800989DC 2529FFF0 */  addiu      $t1, $t1, -0x10
  .L800989E0_995E0:
    /* 995E0 800989E0 BD010000 */  cache      0x01, 0x0($t0) /* handwritten instruction */
    /* 995E4 800989E4 0109082B */  sltu       $at, $t0, $t1
    /* 995E8 800989E8 1420FFFD */  bnez       $at, .L800989E0_995E0
    /* 995EC 800989EC 25080010 */   addiu     $t0, $t0, (0x80000010 & 0xFFFF)
    /* 995F0 800989F0 03E00008 */  jr         $ra
    /* 995F4 800989F4 00000000 */   nop
    /* 995F8 800989F8 00000000 */  nop
    /* 995FC 800989FC 00000000 */  nop