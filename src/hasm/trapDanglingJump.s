.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

glabel TrapDanglingJump
    /* 55D50 80055150 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* 55D54 80055154 3C088010 */  lui        $t0, %hi(D_800FF78C)
    /* 55D58 80055158 3C098010 */  lui        $t1, %hi(D_800FF780)
    /* 55D5C 8005515C AFA40014 */  sw         $a0, 0x14($sp)
    /* 55D60 80055160 AFA50018 */  sw         $a1, 0x18($sp)
    /* 55D64 80055164 AFA6001C */  sw         $a2, 0x1C($sp)
    /* 55D68 80055168 AFA70020 */  sw         $a3, 0x20($sp)
    /* 55D6C 8005516C E7AC0024 */  swc1       $f12, 0x24($sp)
    /* 55D70 80055170 E7AD0028 */  swc1       $f13, 0x28($sp)
    /* 55D74 80055174 E7AE002C */  swc1       $f14, 0x2C($sp)
    /* 55D78 80055178 E7AF0030 */  swc1       $f15, 0x30($sp)
    /* 55D7C 8005517C AFBF0034 */  sw         $ra, 0x34($sp)
    /* 55D80 80055180 AFA20058 */  sw         $v0, 0x58($sp)
    /* 55D84 80055184 AFA3005C */  sw         $v1, 0x5C($sp)
    /* 55D88 80055188 27EDFFF8 */  addiu      $t5, $ra, -0x8
    /* 55D8C 8005518C 8D08F78C */  lw         $t0, %lo(D_800FF78C)($t0)
    /* 55D90 80055190 8D29F780 */  lw         $t1, %lo(D_800FF780)($t1)
  .L80055194_55D94:
    /* 55D94 80055194 3C028010 */  lui        $v0, %hi(D_800FF78C)
    /* 55D98 80055198 8C42F78C */  lw         $v0, %lo(D_800FF78C)($v0)
    /* 55D9C 8005519C 00481023 */  subu       $v0, $v0, $t0
    /* 55DA0 800551A0 5040000E */  beql       $v0, $zero, .L800551DC_55DDC
    /* 55DA4 800551A4 3C0A8000 */   lui       $t2, (0x80000450 >> 16)
    /* 55DA8 800551A8 8D2A0000 */  lw         $t2, 0x0($t1)
    /* 55DAC 800551AC 5140005A */  beql       $t2, $zero, .L80055318_55F18
    /* 55DB0 800551B0 2508FFFF */   addiu     $t0, $t0, -0x1
    /* 55DB4 800551B4 8D220008 */  lw         $v0, 0x8($t1)
    /* 55DB8 800551B8 852C0014 */  lh         $t4, 0x14($t1)
    /* 55DBC 800551BC 01425821 */  addu       $t3, $t2, $v0
    /* 55DC0 800551C0 8D22000C */  lw         $v0, 0xC($t1)
    /* 55DC4 800551C4 000C60C3 */  sra        $t4, $t4, 3
    /* 55DC8 800551C8 01625821 */  addu       $t3, $t3, $v0
    /* 55DCC 800551CC 8D220010 */  lw         $v0, 0x10($t1)
    /* 55DD0 800551D0 10000007 */  b          .L800551F0_55DF0
    /* 55DD4 800551D4 01625821 */   addu      $t3, $t3, $v0
    /* 55DD8 800551D8 3C0A8000 */  lui        $t2, (0x80000450 >> 16)
  .L800551DC_55DDC:
    /* 55DDC 800551DC 3C0C8010 */  lui        $t4, %hi(D_800FF790)
    /* 55DE0 800551E0 3C0B8010 */  lui        $t3, %hi(D_800FF784)
    /* 55DE4 800551E4 354A0450 */  ori        $t2, $t2, (0x80000450 & 0xFFFF)
    /* 55DE8 800551E8 8D8CF790 */  lw         $t4, %lo(D_800FF790)($t4)
    /* 55DEC 800551EC 8D6BF784 */  lw         $t3, %lo(D_800FF784)($t3)
  .L800551F0_55DF0:
    /* 55DF0 800551F0 59800049 */  blezl      $t4, .L80055318_55F18
    /* 55DF4 800551F4 2508FFFF */   addiu     $t0, $t0, -0x1
  .L800551F8_55DF8:
    /* 55DF8 800551F8 8D620004 */  lw         $v0, 0x4($t3)
    /* 55DFC 800551FC 00021202 */  srl        $v0, $v0, 8
    /* 55E00 80055200 004A1021 */  addu       $v0, $v0, $t2
    /* 55E04 80055204 544D0041 */  bnel       $v0, $t5, .L8005530C_55F0C
    /* 55E08 80055208 258CFFFF */   addiu     $t4, $t4, -0x1
    /* 55E0C 8005520C 8D6F0000 */  lw         $t7, 0x0($t3)
    /* 55E10 80055210 3C028010 */  lui        $v0, %hi(D_800FF788)
    /* 55E14 80055214 8C42F788 */  lw         $v0, %lo(D_800FF788)($v0)
    /* 55E18 80055218 000F7080 */  sll        $t6, $t7, 2
    /* 55E1C 8005521C AFA80038 */  sw         $t0, 0x38($sp)
    /* 55E20 80055220 01C27021 */  addu       $t6, $t6, $v0
    /* 55E24 80055224 AFA9003C */  sw         $t1, 0x3C($sp)
    /* 55E28 80055228 AFAA0040 */  sw         $t2, 0x40($sp)
    /* 55E2C 8005522C AFAB0044 */  sw         $t3, 0x44($sp)
    /* 55E30 80055230 AFAC0048 */  sw         $t4, 0x48($sp)
    /* 55E34 80055234 AFAE004C */  sw         $t6, 0x4C($sp)
    /* 55E38 80055238 AFAD0050 */  sw         $t5, 0x50($sp)
    /* 55E3C 8005523C AFAF0054 */  sw         $t7, 0x54($sp)
    /* 55E40 80055240 8DC40000 */  lw         $a0, 0x0($t6)
    /* 55E44 80055244 0C014E4F */  jal        runlinkDownloadCode
    /* 55E48 80055248 00042502 */   srl       $a0, $a0, 20
    /* 55E4C 8005524C 50400022 */  beql       $v0, $zero, .L800552D8_55ED8
    /* 55E50 80055250 8FBF0034 */   lw        $ra, 0x34($sp)
    /* 55E54 80055254 8FAE004C */  lw         $t6, 0x4C($sp)
    /* 55E58 80055258 3C038010 */  lui        $v1, %hi(D_800FF780)
    /* 55E5C 8005525C 8C63F780 */  lw         $v1, %lo(D_800FF780)($v1)
    /* 55E60 80055260 8DD90000 */  lw         $t9, 0x0($t6)
    /* 55E64 80055264 3C01000F */  lui        $at, (0xFFFFF >> 16)
    /* 55E68 80055268 3421FFFF */  ori        $at, $at, (0xFFFFF & 0xFFFF)
    /* 55E6C 8005526C 00191502 */  srl        $v0, $t9, 20
    /* 55E70 80055270 00021140 */  sll        $v0, $v0, 5
    /* 55E74 80055274 00431021 */  addu       $v0, $v0, $v1
    /* 55E78 80055278 8C580000 */  lw         $t8, 0x0($v0)
    /* 55E7C 8005527C 03211024 */  and        $v0, $t9, $at
    /* 55E80 80055280 8FA80038 */  lw         $t0, 0x38($sp)
    /* 55E84 80055284 0302C021 */  addu       $t8, $t8, $v0
    /* 55E88 80055288 8FA9003C */  lw         $t1, 0x3C($sp)
    /* 55E8C 8005528C 8FAA0040 */  lw         $t2, 0x40($sp)
    /* 55E90 80055290 8FAB0044 */  lw         $t3, 0x44($sp)
    /* 55E94 80055294 8FAC0048 */  lw         $t4, 0x48($sp)
    /* 55E98 80055298 8FAD0050 */  lw         $t5, 0x50($sp)
    /* 55E9C 8005529C 8FAF0054 */  lw         $t7, 0x54($sp)
    /* 55EA0 800552A0 8FA40014 */  lw         $a0, 0x14($sp)
    /* 55EA4 800552A4 8FA50018 */  lw         $a1, 0x18($sp)
    /* 55EA8 800552A8 8FA6001C */  lw         $a2, 0x1C($sp)
    /* 55EAC 800552AC 8FA70020 */  lw         $a3, 0x20($sp)
    /* 55EB0 800552B0 C7AC0024 */  lwc1       $f12, 0x24($sp)
    /* 55EB4 800552B4 C7AD0028 */  lwc1       $f13, 0x28($sp)
    /* 55EB8 800552B8 C7AE002C */  lwc1       $f14, 0x2C($sp)
    /* 55EBC 800552BC C7AF0030 */  lwc1       $f15, 0x30($sp)
    /* 55EC0 800552C0 8FBF0034 */  lw         $ra, 0x34($sp)
    /* 55EC4 800552C4 8FA20058 */  lw         $v0, 0x58($sp)
    /* 55EC8 800552C8 8FA3005C */  lw         $v1, 0x5C($sp)
    /* 55ECC 800552CC 03000008 */  jr         $t8
    /* 55ED0 800552D0 27BD0060 */   addiu     $sp, $sp, 0x60
    /* 55ED4 800552D4 8FBF0034 */  lw         $ra, 0x34($sp)
  .L800552D8_55ED8:
    /* 55ED8 800552D8 8FA40014 */  lw         $a0, 0x14($sp)
    /* 55EDC 800552DC 8FA50018 */  lw         $a1, 0x18($sp)
    /* 55EE0 800552E0 8FA6001C */  lw         $a2, 0x1C($sp)
    /* 55EE4 800552E4 8FA70020 */  lw         $a3, 0x20($sp)
    /* 55EE8 800552E8 C7AC0024 */  lwc1       $f12, 0x24($sp)
    /* 55EEC 800552EC C7AD0028 */  lwc1       $f13, 0x28($sp)
    /* 55EF0 800552F0 C7AE002C */  lwc1       $f14, 0x2C($sp)
    /* 55EF4 800552F4 C7AF0030 */  lwc1       $f15, 0x30($sp)
    /* 55EF8 800552F8 00001025 */  or         $v0, $zero, $zero
    /* 55EFC 800552FC 00001825 */  or         $v1, $zero, $zero
    /* 55F00 80055300 03E00008 */  jr         $ra
    /* 55F04 80055304 27BD0060 */   addiu     $sp, $sp, 0x60
    /* 55F08 80055308 258CFFFF */  addiu      $t4, $t4, -0x1
  .L8005530C_55F0C:
    /* 55F0C 8005530C 1580FFBA */  bnez       $t4, .L800551F8_55DF8
    /* 55F10 80055310 256B0008 */   addiu     $t3, $t3, 0x8
    /* 55F14 80055314 2508FFFF */  addiu      $t0, $t0, -0x1
  .L80055318_55F18:
    /* 55F18 80055318 1D00FF9E */  bgtz       $t0, .L80055194_55D94
    /* 55F1C 8005531C 25290020 */   addiu     $t1, $t1, 0x20
    /* 55F20 80055320 8FBF0034 */  lw         $ra, 0x34($sp)
    /* 55F24 80055324 27BD0060 */  addiu      $sp, $sp, 0x60
    /* 55F28 80055328 03E00008 */  jr         $ra
    /* 55F2C 8005532C 00000000 */   nop
