.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

/* Handwritten function */
glabel _bcopy
    /* 9A490 80099890 10C0001A */  beqz       $a2, .L800998FC_9A4FC
    /* 9A494 80099894 00A03825 */   or        $a3, $a1, $zero
    /* 9A498 80099898 10850018 */  beq        $a0, $a1, .L800998FC_9A4FC
    /* 9A49C 8009989C 00A4082A */   slt       $at, $a1, $a0
    /* 9A4A0 800998A0 54200008 */  bnel       $at, $zero, .L800998C4_9A4C4
    /* 9A4A4 800998A4 28C10010 */   slti      $at, $a2, 0x10
    /* 9A4A8 800998A8 00861020 */  add        $v0, $a0, $a2 /* handwritten instruction */
    /* 9A4AC 800998AC 00A2082A */  slt        $at, $a1, $v0
    /* 9A4B0 800998B0 50200004 */  beql       $at, $zero, .L800998C4_9A4C4
    /* 9A4B4 800998B4 28C10010 */   slti      $at, $a2, 0x10
    /* 9A4B8 800998B8 1000005B */  b          .L80099A28_9A628
    /* 9A4BC 800998BC 28C10010 */   slti      $at, $a2, 0x10
    /* 9A4C0 800998C0 28C10010 */  slti       $at, $a2, 0x10
  .L800998C4_9A4C4:
    /* 9A4C4 800998C4 14200005 */  bnez       $at, .L800998DC_9A4DC
    /* 9A4C8 800998C8 00000000 */   nop
    /* 9A4CC 800998CC 30820003 */  andi       $v0, $a0, 0x3
    /* 9A4D0 800998D0 30A30003 */  andi       $v1, $a1, 0x3
    /* 9A4D4 800998D4 1043000B */  beq        $v0, $v1, .L80099904_9A504
    /* 9A4D8 800998D8 00000000 */   nop
  .L800998DC_9A4DC:
    /* 9A4DC 800998DC 10C00007 */  beqz       $a2, .L800998FC_9A4FC
    /* 9A4E0 800998E0 00000000 */   nop
    /* 9A4E4 800998E4 00861821 */  addu       $v1, $a0, $a2
  .L800998E8_9A4E8:
    /* 9A4E8 800998E8 80820000 */  lb         $v0, 0x0($a0)
    /* 9A4EC 800998EC 24840001 */  addiu      $a0, $a0, 0x1
    /* 9A4F0 800998F0 24A50001 */  addiu      $a1, $a1, 0x1
    /* 9A4F4 800998F4 1483FFFC */  bne        $a0, $v1, .L800998E8_9A4E8
    /* 9A4F8 800998F8 A0A2FFFF */   sb        $v0, -0x1($a1)
  .L800998FC_9A4FC:
    /* 9A4FC 800998FC 03E00008 */  jr         $ra
    /* 9A500 80099900 00E01025 */   or        $v0, $a3, $zero
  .L80099904_9A504:
    /* 9A504 80099904 10400018 */  beqz       $v0, .L80099968_9A568
    /* 9A508 80099908 24010001 */   addiu     $at, $zero, 0x1
    /* 9A50C 8009990C 1041000F */  beq        $v0, $at, .L8009994C_9A54C
    /* 9A510 80099910 24010002 */   addiu     $at, $zero, 0x2
    /* 9A514 80099914 50410008 */  beql       $v0, $at, .L80099938_9A538
    /* 9A518 80099918 84820000 */   lh        $v0, 0x0($a0)
    /* 9A51C 8009991C 80820000 */  lb         $v0, 0x0($a0)
    /* 9A520 80099920 24840001 */  addiu      $a0, $a0, 0x1
    /* 9A524 80099924 24A50001 */  addiu      $a1, $a1, 0x1
    /* 9A528 80099928 24C6FFFF */  addiu      $a2, $a2, -0x1
    /* 9A52C 8009992C 1000000E */  b          .L80099968_9A568
    /* 9A530 80099930 A0A2FFFF */   sb        $v0, -0x1($a1)
    /* 9A534 80099934 84820000 */  lh         $v0, 0x0($a0)
  .L80099938_9A538:
    /* 9A538 80099938 24840002 */  addiu      $a0, $a0, 0x2
    /* 9A53C 8009993C 24A50002 */  addiu      $a1, $a1, 0x2
    /* 9A540 80099940 24C6FFFE */  addiu      $a2, $a2, -0x2
    /* 9A544 80099944 10000008 */  b          .L80099968_9A568
    /* 9A548 80099948 A4A2FFFE */   sh        $v0, -0x2($a1)
  .L8009994C_9A54C:
    /* 9A54C 8009994C 80820000 */  lb         $v0, 0x0($a0)
    /* 9A550 80099950 84830001 */  lh         $v1, 0x1($a0)
    /* 9A554 80099954 24840003 */  addiu      $a0, $a0, 0x3
    /* 9A558 80099958 24A50003 */  addiu      $a1, $a1, 0x3
    /* 9A55C 8009995C 24C6FFFD */  addiu      $a2, $a2, -0x3
    /* 9A560 80099960 A0A2FFFD */  sb         $v0, -0x3($a1)
    /* 9A564 80099964 A4A3FFFE */  sh         $v1, -0x2($a1)
  .L80099968_9A568:
    /* 9A568 80099968 28C10020 */  slti       $at, $a2, 0x20
    /* 9A56C 8009996C 54200016 */  bnel       $at, $zero, .L800999C8_9A5C8
    /* 9A570 80099970 28C10010 */   slti      $at, $a2, 0x10
    /* 9A574 80099974 8C820000 */  lw         $v0, 0x0($a0)
    /* 9A578 80099978 8C830004 */  lw         $v1, 0x4($a0)
    /* 9A57C 8009997C 8C880008 */  lw         $t0, 0x8($a0)
    /* 9A580 80099980 8C89000C */  lw         $t1, 0xC($a0)
    /* 9A584 80099984 8C8A0010 */  lw         $t2, 0x10($a0)
    /* 9A588 80099988 8C8B0014 */  lw         $t3, 0x14($a0)
    /* 9A58C 8009998C 8C8C0018 */  lw         $t4, 0x18($a0)
    /* 9A590 80099990 8C8D001C */  lw         $t5, 0x1C($a0)
    /* 9A594 80099994 24840020 */  addiu      $a0, $a0, 0x20
    /* 9A598 80099998 24A50020 */  addiu      $a1, $a1, 0x20
    /* 9A59C 8009999C 24C6FFE0 */  addiu      $a2, $a2, -0x20
    /* 9A5A0 800999A0 ACA2FFE0 */  sw         $v0, -0x20($a1)
    /* 9A5A4 800999A4 ACA3FFE4 */  sw         $v1, -0x1C($a1)
    /* 9A5A8 800999A8 ACA8FFE8 */  sw         $t0, -0x18($a1)
    /* 9A5AC 800999AC ACA9FFEC */  sw         $t1, -0x14($a1)
    /* 9A5B0 800999B0 ACAAFFF0 */  sw         $t2, -0x10($a1)
    /* 9A5B4 800999B4 ACABFFF4 */  sw         $t3, -0xC($a1)
    /* 9A5B8 800999B8 ACACFFF8 */  sw         $t4, -0x8($a1)
    /* 9A5BC 800999BC 1000FFEA */  b          .L80099968_9A568
    /* 9A5C0 800999C0 ACADFFFC */   sw        $t5, -0x4($a1)
  .L800999C4_9A5C4:
    /* 9A5C4 800999C4 28C10010 */  slti       $at, $a2, 0x10
  .L800999C8_9A5C8:
    /* 9A5C8 800999C8 5420000E */  bnel       $at, $zero, .L80099A04_9A604
    /* 9A5CC 800999CC 28C10004 */   slti      $at, $a2, 0x4
    /* 9A5D0 800999D0 8C820000 */  lw         $v0, 0x0($a0)
    /* 9A5D4 800999D4 8C830004 */  lw         $v1, 0x4($a0)
    /* 9A5D8 800999D8 8C880008 */  lw         $t0, 0x8($a0)
    /* 9A5DC 800999DC 8C89000C */  lw         $t1, 0xC($a0)
    /* 9A5E0 800999E0 24840010 */  addiu      $a0, $a0, 0x10
    /* 9A5E4 800999E4 24A50010 */  addiu      $a1, $a1, 0x10
    /* 9A5E8 800999E8 24C6FFF0 */  addiu      $a2, $a2, -0x10
    /* 9A5EC 800999EC ACA2FFF0 */  sw         $v0, -0x10($a1)
    /* 9A5F0 800999F0 ACA3FFF4 */  sw         $v1, -0xC($a1)
    /* 9A5F4 800999F4 ACA8FFF8 */  sw         $t0, -0x8($a1)
    /* 9A5F8 800999F8 1000FFF2 */  b          .L800999C4_9A5C4
    /* 9A5FC 800999FC ACA9FFFC */   sw        $t1, -0x4($a1)
  .L80099A00_9A600:
    /* 9A600 80099A00 28C10004 */  slti       $at, $a2, 0x4
  .L80099A04_9A604:
    /* 9A604 80099A04 1420FFB5 */  bnez       $at, .L800998DC_9A4DC
    /* 9A608 80099A08 00000000 */   nop
    /* 9A60C 80099A0C 8C820000 */  lw         $v0, 0x0($a0)
    /* 9A610 80099A10 24840004 */  addiu      $a0, $a0, 0x4
    /* 9A614 80099A14 24A50004 */  addiu      $a1, $a1, 0x4
    /* 9A618 80099A18 24C6FFFC */  addiu      $a2, $a2, -0x4
    /* 9A61C 80099A1C 1000FFF8 */  b          .L80099A00_9A600
    /* 9A620 80099A20 ACA2FFFC */   sw        $v0, -0x4($a1)
    /* 9A624 80099A24 28C10010 */  slti       $at, $a2, 0x10
  .L80099A28_9A628:
    /* 9A628 80099A28 00862020 */  add        $a0, $a0, $a2 /* handwritten instruction */
    /* 9A62C 80099A2C 14200005 */  bnez       $at, .L80099A44_9A644
    /* 9A630 80099A30 00A62820 */   add       $a1, $a1, $a2 /* handwritten instruction */
    /* 9A634 80099A34 30820003 */  andi       $v0, $a0, 0x3
    /* 9A638 80099A38 30A30003 */  andi       $v1, $a1, 0x3
    /* 9A63C 80099A3C 1043000D */  beq        $v0, $v1, .L80099A74_9A674
    /* 9A640 80099A40 00000000 */   nop
  .L80099A44_9A644:
    /* 9A644 80099A44 10C0FFAD */  beqz       $a2, .L800998FC_9A4FC
    /* 9A648 80099A48 00000000 */   nop
    /* 9A64C 80099A4C 2484FFFF */  addiu      $a0, $a0, -0x1
    /* 9A650 80099A50 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* 9A654 80099A54 00861823 */  subu       $v1, $a0, $a2
  .L80099A58_9A658:
    /* 9A658 80099A58 80820000 */  lb         $v0, 0x0($a0)
    /* 9A65C 80099A5C 2484FFFF */  addiu      $a0, $a0, -0x1
    /* 9A660 80099A60 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* 9A664 80099A64 1483FFFC */  bne        $a0, $v1, .L80099A58_9A658
    /* 9A668 80099A68 A0A20001 */   sb        $v0, 0x1($a1)
    /* 9A66C 80099A6C 03E00008 */  jr         $ra
    /* 9A670 80099A70 00E01025 */   or        $v0, $a3, $zero
  .L80099A74_9A674:
    /* 9A674 80099A74 10400018 */  beqz       $v0, .L80099AD8_9A6D8
    /* 9A678 80099A78 24010003 */   addiu     $at, $zero, 0x3
    /* 9A67C 80099A7C 1041000F */  beq        $v0, $at, .L80099ABC_9A6BC
    /* 9A680 80099A80 24010002 */   addiu     $at, $zero, 0x2
    /* 9A684 80099A84 50410008 */  beql       $v0, $at, .L80099AA8_9A6A8
    /* 9A688 80099A88 8482FFFE */   lh        $v0, -0x2($a0)
    /* 9A68C 80099A8C 8082FFFF */  lb         $v0, -0x1($a0)
    /* 9A690 80099A90 2484FFFF */  addiu      $a0, $a0, -0x1
    /* 9A694 80099A94 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* 9A698 80099A98 24C6FFFF */  addiu      $a2, $a2, -0x1
    /* 9A69C 80099A9C 1000000E */  b          .L80099AD8_9A6D8
    /* 9A6A0 80099AA0 A0A20000 */   sb        $v0, 0x0($a1)
    /* 9A6A4 80099AA4 8482FFFE */  lh         $v0, -0x2($a0)
  .L80099AA8_9A6A8:
    /* 9A6A8 80099AA8 2484FFFE */  addiu      $a0, $a0, -0x2
    /* 9A6AC 80099AAC 24A5FFFE */  addiu      $a1, $a1, -0x2
    /* 9A6B0 80099AB0 24C6FFFE */  addiu      $a2, $a2, -0x2
    /* 9A6B4 80099AB4 10000008 */  b          .L80099AD8_9A6D8
    /* 9A6B8 80099AB8 A4A20000 */   sh        $v0, 0x0($a1)
  .L80099ABC_9A6BC:
    /* 9A6BC 80099ABC 8082FFFF */  lb         $v0, -0x1($a0)
    /* 9A6C0 80099AC0 8483FFFD */  lh         $v1, -0x3($a0)
    /* 9A6C4 80099AC4 2484FFFD */  addiu      $a0, $a0, -0x3
    /* 9A6C8 80099AC8 24A5FFFD */  addiu      $a1, $a1, -0x3
    /* 9A6CC 80099ACC 24C6FFFD */  addiu      $a2, $a2, -0x3
    /* 9A6D0 80099AD0 A0A20002 */  sb         $v0, 0x2($a1)
    /* 9A6D4 80099AD4 A4A30000 */  sh         $v1, 0x0($a1)
  .L80099AD8_9A6D8:
    /* 9A6D8 80099AD8 28C10020 */  slti       $at, $a2, 0x20
    /* 9A6DC 80099ADC 54200016 */  bnel       $at, $zero, .L80099B38_9A738
    /* 9A6E0 80099AE0 28C10010 */   slti      $at, $a2, 0x10
    /* 9A6E4 80099AE4 8C82FFFC */  lw         $v0, -0x4($a0)
    /* 9A6E8 80099AE8 8C83FFF8 */  lw         $v1, -0x8($a0)
    /* 9A6EC 80099AEC 8C88FFF4 */  lw         $t0, -0xC($a0)
    /* 9A6F0 80099AF0 8C89FFF0 */  lw         $t1, -0x10($a0)
    /* 9A6F4 80099AF4 8C8AFFEC */  lw         $t2, -0x14($a0)
    /* 9A6F8 80099AF8 8C8BFFE8 */  lw         $t3, -0x18($a0)
    /* 9A6FC 80099AFC 8C8CFFE4 */  lw         $t4, -0x1C($a0)
    /* 9A700 80099B00 8C8DFFE0 */  lw         $t5, -0x20($a0)
    /* 9A704 80099B04 2484FFE0 */  addiu      $a0, $a0, -0x20
    /* 9A708 80099B08 24A5FFE0 */  addiu      $a1, $a1, -0x20
    /* 9A70C 80099B0C 24C6FFE0 */  addiu      $a2, $a2, -0x20
    /* 9A710 80099B10 ACA2001C */  sw         $v0, 0x1C($a1)
    /* 9A714 80099B14 ACA30018 */  sw         $v1, 0x18($a1)
    /* 9A718 80099B18 ACA80014 */  sw         $t0, 0x14($a1)
    /* 9A71C 80099B1C ACA90010 */  sw         $t1, 0x10($a1)
    /* 9A720 80099B20 ACAA000C */  sw         $t2, 0xC($a1)
    /* 9A724 80099B24 ACAB0008 */  sw         $t3, 0x8($a1)
    /* 9A728 80099B28 ACAC0004 */  sw         $t4, 0x4($a1)
    /* 9A72C 80099B2C 1000FFEA */  b          .L80099AD8_9A6D8
    /* 9A730 80099B30 ACAD0000 */   sw        $t5, 0x0($a1)
  .L80099B34_9A734:
    /* 9A734 80099B34 28C10010 */  slti       $at, $a2, 0x10
  .L80099B38_9A738:
    /* 9A738 80099B38 5420000E */  bnel       $at, $zero, .L80099B74_9A774
    /* 9A73C 80099B3C 28C10004 */   slti      $at, $a2, 0x4
    /* 9A740 80099B40 8C82FFFC */  lw         $v0, -0x4($a0)
    /* 9A744 80099B44 8C83FFF8 */  lw         $v1, -0x8($a0)
    /* 9A748 80099B48 8C88FFF4 */  lw         $t0, -0xC($a0)
    /* 9A74C 80099B4C 8C89FFF0 */  lw         $t1, -0x10($a0)
    /* 9A750 80099B50 2484FFF0 */  addiu      $a0, $a0, -0x10
    /* 9A754 80099B54 24A5FFF0 */  addiu      $a1, $a1, -0x10
    /* 9A758 80099B58 24C6FFF0 */  addiu      $a2, $a2, -0x10
    /* 9A75C 80099B5C ACA2000C */  sw         $v0, 0xC($a1)
    /* 9A760 80099B60 ACA30008 */  sw         $v1, 0x8($a1)
    /* 9A764 80099B64 ACA80004 */  sw         $t0, 0x4($a1)
    /* 9A768 80099B68 1000FFF2 */  b          .L80099B34_9A734
    /* 9A76C 80099B6C ACA90000 */   sw        $t1, 0x0($a1)
  .L80099B70_9A770:
    /* 9A770 80099B70 28C10004 */  slti       $at, $a2, 0x4
  .L80099B74_9A774:
    /* 9A774 80099B74 1420FFB3 */  bnez       $at, .L80099A44_9A644
    /* 9A778 80099B78 00000000 */   nop
    /* 9A77C 80099B7C 8C82FFFC */  lw         $v0, -0x4($a0)
    /* 9A780 80099B80 2484FFFC */  addiu      $a0, $a0, -0x4
    /* 9A784 80099B84 24A5FFFC */  addiu      $a1, $a1, -0x4
    /* 9A788 80099B88 24C6FFFC */  addiu      $a2, $a2, -0x4
    /* 9A78C 80099B8C 1000FFF8 */  b          .L80099B70_9A770
    /* 9A790 80099B90 ACA20000 */   sw        $v0, 0x0($a1)
    /* 9A794 80099B94 00000000 */  nop
    /* 9A798 80099B98 00000000 */  nop
    /* 9A79C 80099B9C 00000000 */  nop
