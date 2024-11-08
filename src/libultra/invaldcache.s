.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

/* Handwritten function */
glabel osInvalDCache
    /* 96A50 80095E50 18A0001F */  blez       $a1, .L80095ED0_96AD0
    /* 96A54 80095E54 00000000 */   nop
    /* 96A58 80095E58 240B2000 */  addiu      $t3, $zero, 0x2000
    /* 96A5C 80095E5C 00AB082B */  sltu       $at, $a1, $t3
    /* 96A60 80095E60 1020001D */  beqz       $at, .L80095ED8_96AD8
    /* 96A64 80095E64 00000000 */   nop
    /* 96A68 80095E68 00804025 */  or         $t0, $a0, $zero
    /* 96A6C 80095E6C 00854821 */  addu       $t1, $a0, $a1
    /* 96A70 80095E70 0109082B */  sltu       $at, $t0, $t1
    /* 96A74 80095E74 10200016 */  beqz       $at, .L80095ED0_96AD0
    /* 96A78 80095E78 00000000 */   nop
    /* 96A7C 80095E7C 310A000F */  andi       $t2, $t0, 0xF
    /* 96A80 80095E80 11400007 */  beqz       $t2, .L80095EA0_96AA0
    /* 96A84 80095E84 2529FFF0 */   addiu     $t1, $t1, -0x10
    /* 96A88 80095E88 010A4023 */  subu       $t0, $t0, $t2
    /* 96A8C 80095E8C BD150000 */  cache      0x15, 0x0($t0) /* handwritten instruction */
    /* 96A90 80095E90 0109082B */  sltu       $at, $t0, $t1
    /* 96A94 80095E94 1020000E */  beqz       $at, .L80095ED0_96AD0
    /* 96A98 80095E98 00000000 */   nop
    /* 96A9C 80095E9C 25080010 */  addiu      $t0, $t0, 0x10
  .L80095EA0_96AA0:
    /* 96AA0 80095EA0 312A000F */  andi       $t2, $t1, 0xF
    /* 96AA4 80095EA4 11400006 */  beqz       $t2, .L80095EC0_96AC0
    /* 96AA8 80095EA8 00000000 */   nop
    /* 96AAC 80095EAC 012A4823 */  subu       $t1, $t1, $t2
    /* 96AB0 80095EB0 BD350010 */  cache      0x15, 0x10($t1) /* handwritten instruction */
    /* 96AB4 80095EB4 0128082B */  sltu       $at, $t1, $t0
    /* 96AB8 80095EB8 14200005 */  bnez       $at, .L80095ED0_96AD0
    /* 96ABC 80095EBC 00000000 */   nop
  .L80095EC0_96AC0:
    /* 96AC0 80095EC0 BD110000 */  cache      0x11, 0x0($t0) /* handwritten instruction */
    /* 96AC4 80095EC4 0109082B */  sltu       $at, $t0, $t1
    /* 96AC8 80095EC8 1420FFFD */  bnez       $at, .L80095EC0_96AC0
    /* 96ACC 80095ECC 25080010 */   addiu     $t0, $t0, 0x10
  .L80095ED0_96AD0:
    /* 96AD0 80095ED0 03E00008 */  jr         $ra
    /* 96AD4 80095ED4 00000000 */   nop
  .L80095ED8_96AD8:
    /* 96AD8 80095ED8 3C088000 */  lui        $t0, 0x8000
    /* 96ADC 80095EDC 010B4821 */  addu       $t1, $t0, $t3
    /* 96AE0 80095EE0 2529FFF0 */  addiu      $t1, $t1, -0x10
  .L80095EE4_96AE4:
    /* 96AE4 80095EE4 BD010000 */  cache      0x01, 0x0($t0) /* handwritten instruction */
    /* 96AE8 80095EE8 0109082B */  sltu       $at, $t0, $t1
    /* 96AEC 80095EEC 1420FFFD */  bnez       $at, .L80095EE4_96AE4
    /* 96AF0 80095EF0 25080010 */   addiu     $t0, $t0, (0x80000010 & 0xFFFF)
    /* 96AF4 80095EF4 03E00008 */  jr         $ra
    /* 96AF8 80095EF8 00000000 */   nop
    /* 96AFC 80095EFC 00000000 */  nop
