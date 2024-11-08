.include "macro.inc"

/* assembler directives */
.set noat      /* allow manual use of $at */
.set noreorder /* don't insert nops after branches */
.set gp=64     /* allow use of 64-bit general purpose registers */

.section .text, "ax"

/* Generated by spimdisasm 1.30.2 */

glabel matrix_SCL_RPY_XYZ
    /* 4ACC0 8004A0C0 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 4ACC4 8004A0C4 AFBF0004 */  sw         $ra, 0x4($sp)
    /* 4ACC8 8004A0C8 00803025 */  or         $a2, $a0, $zero
    /* 4ACCC 8004A0CC 0C0127CD */  jal        Sinf
    /* 4ACD0 8004A0D0 84C40000 */   lh        $a0, 0x0($a2)
    /* 4ACD4 8004A0D4 0C0127CC */  jal        Cosf
    /* 4ACD8 8004A0D8 46000086 */   mov.s     $f2, $f0
    /* 4ACDC 8004A0DC 460000C6 */  mov.s      $f3, $f0
    /* 4ACE0 8004A0E0 0C0127CD */  jal        Sinf
    /* 4ACE4 8004A0E4 84C40002 */   lh        $a0, 0x2($a2)
    /* 4ACE8 8004A0E8 0C0127CC */  jal        Cosf
    /* 4ACEC 8004A0EC 46000106 */   mov.s     $f4, $f0
    /* 4ACF0 8004A0F0 46000146 */  mov.s      $f5, $f0
    /* 4ACF4 8004A0F4 0C0127CD */  jal        Sinf
    /* 4ACF8 8004A0F8 84C40004 */   lh        $a0, 0x4($a2)
    /* 4ACFC 8004A0FC 0C0127CC */  jal        Cosf
    /* 4AD00 8004A100 46000186 */   mov.s     $f6, $f0
    /* 4AD04 8004A104 460001C6 */  mov.s      $f7, $f0
    /* 4AD08 8004A108 46023202 */  mul.s      $f8, $f6, $f2
    /* 4AD0C 8004A10C C4D30008 */  lwc1       $f19, 0x8($a2)
    /* 4AD10 8004A110 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 4AD14 8004A114 46033242 */  mul.s      $f9, $f6, $f3
    /* 4AD18 8004A118 ACA0000C */  sw         $zero, 0xC($a1)
    /* 4AD1C 8004A11C ACA0001C */  sw         $zero, 0x1C($a1)
    /* 4AD20 8004A120 46071282 */  mul.s      $f10, $f2, $f7
    /* 4AD24 8004A124 ACA0002C */  sw         $zero, 0x2C($a1)
    /* 4AD28 8004A128 ACA00030 */  sw         $zero, 0x30($a1)
    /* 4AD2C 8004A12C 46033AC2 */  mul.s      $f11, $f7, $f3
    /* 4AD30 8004A130 ACA00034 */  sw         $zero, 0x34($a1)
    /* 4AD34 8004A134 ACA00038 */  sw         $zero, 0x38($a1)
    /* 4AD38 8004A138 46023302 */  mul.s      $f12, $f6, $f2
    /* 4AD3C 8004A13C 00000000 */  nop
    /* 4AD40 8004A140 46053342 */  mul.s      $f13, $f6, $f5
    /* 4AD44 8004A144 00000000 */  nop
    /* 4AD48 8004A148 46053B82 */  mul.s      $f14, $f7, $f5
    /* 4AD4C 8004A14C 00000000 */  nop
    /* 4AD50 8004A150 460513C2 */  mul.s      $f15, $f2, $f5
    /* 4AD54 8004A154 46002407 */  neg.s      $f16, $f4
    /* 4AD58 8004A158 46032C42 */  mul.s      $f17, $f5, $f3
    /* 4AD5C 8004A15C 00000000 */  nop
    /* 4AD60 8004A160 46044002 */  mul.s      $f0, $f8, $f4
    /* 4AD64 8004A164 00000000 */  nop
    /* 4AD68 8004A168 46044842 */  mul.s      $f1, $f9, $f4
    /* 4AD6C 8004A16C 00000000 */  nop
    /* 4AD70 8004A170 46045082 */  mul.s      $f2, $f10, $f4
    /* 4AD74 8004A174 460B0000 */  add.s      $f0, $f0, $f11
    /* 4AD78 8004A178 460458C2 */  mul.s      $f3, $f11, $f4
    /* 4AD7C 8004A17C 460A0841 */  sub.s      $f1, $f1, $f10
    /* 4AD80 8004A180 46130002 */  mul.s      $f0, $f0, $f19
    /* 4AD84 8004A184 00000000 */  nop
    /* 4AD88 8004A188 46136B42 */  mul.s      $f13, $f13, $f19
    /* 4AD8C 8004A18C 46091081 */  sub.s      $f2, $f2, $f9
    /* 4AD90 8004A190 46130842 */  mul.s      $f1, $f1, $f19
    /* 4AD94 8004A194 E4A00000 */  swc1       $f0, 0x0($a1)
    /* 4AD98 8004A198 460C18C0 */  add.s      $f3, $f3, $f12
    /* 4AD9C 8004A19C 44812000 */  mtc1       $at, $f4
    /* 4ADA0 8004A1A0 E4AD0004 */  swc1       $f13, 0x4($a1)
    /* 4ADA4 8004A1A4 46131082 */  mul.s      $f2, $f2, $f19
    /* 4ADA8 8004A1A8 E4A10008 */  swc1       $f1, 0x8($a1)
    /* 4ADAC 8004A1AC 46137382 */  mul.s      $f14, $f14, $f19
    /* 4ADB0 8004A1B0 00000000 */  nop
    /* 4ADB4 8004A1B4 461318C2 */  mul.s      $f3, $f3, $f19
    /* 4ADB8 8004A1B8 E4A20010 */  swc1       $f2, 0x10($a1)
    /* 4ADBC 8004A1BC 46137BC2 */  mul.s      $f15, $f15, $f19
    /* 4ADC0 8004A1C0 E4AE0014 */  swc1       $f14, 0x14($a1)
    /* 4ADC4 8004A1C4 E4A30018 */  swc1       $f3, 0x18($a1)
    /* 4ADC8 8004A1C8 E4A4003C */  swc1       $f4, 0x3C($a1)
    /* 4ADCC 8004A1CC 46138402 */  mul.s      $f16, $f16, $f19
    /* 4ADD0 8004A1D0 E4AF0020 */  swc1       $f15, 0x20($a1)
    /* 4ADD4 8004A1D4 46138C42 */  mul.s      $f17, $f17, $f19
    /* 4ADD8 8004A1D8 E4B00024 */  swc1       $f16, 0x24($a1)
    /* 4ADDC 8004A1DC E4B10028 */  swc1       $f17, 0x28($a1)
    /* 4ADE0 8004A1E0 8CC8000C */  lw         $t0, 0xC($a2)
    /* 4ADE4 8004A1E4 ACA80030 */  sw         $t0, 0x30($a1)
    /* 4ADE8 8004A1E8 8CC80010 */  lw         $t0, 0x10($a2)
    /* 4ADEC 8004A1EC ACA80034 */  sw         $t0, 0x34($a1)
    /* 4ADF0 8004A1F0 8CC80014 */  lw         $t0, 0x14($a2)
    /* 4ADF4 8004A1F4 ACA80038 */  sw         $t0, 0x38($a1)
    /* 4ADF8 8004A1F8 8FBF0004 */  lw         $ra, 0x4($sp)
    /* 4ADFC 8004A1FC 27BD0008 */  addiu      $sp, $sp, 0x8
    /* 4AE00 8004A200 03E00008 */  jr         $ra
    /* 4AE04 8004A204 00000000 */   nop

glabel matrix_RPY_XYZ
    /* 4AE08 8004A208 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 4AE0C 8004A20C AFBF0004 */  sw         $ra, 0x4($sp)
    /* 4AE10 8004A210 00803025 */  or         $a2, $a0, $zero
    /* 4AE14 8004A214 0C0127CD */  jal        Sinf
    /* 4AE18 8004A218 84C40000 */   lh        $a0, 0x0($a2)
    /* 4AE1C 8004A21C 0C0127CC */  jal        Cosf
    /* 4AE20 8004A220 46000086 */   mov.s     $f2, $f0
    /* 4AE24 8004A224 460000C6 */  mov.s      $f3, $f0
    /* 4AE28 8004A228 0C0127CD */  jal        Sinf
    /* 4AE2C 8004A22C 84C40002 */   lh        $a0, 0x2($a2)
    /* 4AE30 8004A230 0C0127CC */  jal        Cosf
    /* 4AE34 8004A234 46000106 */   mov.s     $f4, $f0
    /* 4AE38 8004A238 46000146 */  mov.s      $f5, $f0
    /* 4AE3C 8004A23C 0C0127CD */  jal        Sinf
    /* 4AE40 8004A240 84C40004 */   lh        $a0, 0x4($a2)
    /* 4AE44 8004A244 0C0127CC */  jal        Cosf
    /* 4AE48 8004A248 46000186 */   mov.s     $f6, $f0
    /* 4AE4C 8004A24C 460001C6 */  mov.s      $f7, $f0
    /* 4AE50 8004A250 46023202 */  mul.s      $f8, $f6, $f2
    /* 4AE54 8004A254 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 4AE58 8004A258 ACA0000C */  sw         $zero, 0xC($a1)
    /* 4AE5C 8004A25C 46033242 */  mul.s      $f9, $f6, $f3
    /* 4AE60 8004A260 ACA0001C */  sw         $zero, 0x1C($a1)
    /* 4AE64 8004A264 ACA0002C */  sw         $zero, 0x2C($a1)
    /* 4AE68 8004A268 46071282 */  mul.s      $f10, $f2, $f7
    /* 4AE6C 8004A26C ACA00030 */  sw         $zero, 0x30($a1)
    /* 4AE70 8004A270 ACA00034 */  sw         $zero, 0x34($a1)
    /* 4AE74 8004A274 46033AC2 */  mul.s      $f11, $f7, $f3
    /* 4AE78 8004A278 ACA00038 */  sw         $zero, 0x38($a1)
    /* 4AE7C 8004A27C 46023302 */  mul.s      $f12, $f6, $f2
    /* 4AE80 8004A280 00000000 */  nop
    /* 4AE84 8004A284 46053342 */  mul.s      $f13, $f6, $f5
    /* 4AE88 8004A288 00000000 */  nop
    /* 4AE8C 8004A28C 46053B82 */  mul.s      $f14, $f7, $f5
    /* 4AE90 8004A290 00000000 */  nop
    /* 4AE94 8004A294 460513C2 */  mul.s      $f15, $f2, $f5
    /* 4AE98 8004A298 46002407 */  neg.s      $f16, $f4
    /* 4AE9C 8004A29C E4AD0004 */  swc1       $f13, 0x4($a1)
    /* 4AEA0 8004A2A0 46032C42 */  mul.s      $f17, $f5, $f3
    /* 4AEA4 8004A2A4 E4AE0014 */  swc1       $f14, 0x14($a1)
    /* 4AEA8 8004A2A8 E4B00024 */  swc1       $f16, 0x24($a1)
    /* 4AEAC 8004A2AC 46044002 */  mul.s      $f0, $f8, $f4
    /* 4AEB0 8004A2B0 E4AF0020 */  swc1       $f15, 0x20($a1)
    /* 4AEB4 8004A2B4 46044842 */  mul.s      $f1, $f9, $f4
    /* 4AEB8 8004A2B8 E4B10028 */  swc1       $f17, 0x28($a1)
    /* 4AEBC 8004A2BC 46045082 */  mul.s      $f2, $f10, $f4
    /* 4AEC0 8004A2C0 460B0000 */  add.s      $f0, $f0, $f11
    /* 4AEC4 8004A2C4 460458C2 */  mul.s      $f3, $f11, $f4
    /* 4AEC8 8004A2C8 E4A00000 */  swc1       $f0, 0x0($a1)
    /* 4AECC 8004A2CC 460A0841 */  sub.s      $f1, $f1, $f10
    /* 4AED0 8004A2D0 46091081 */  sub.s      $f2, $f2, $f9
    /* 4AED4 8004A2D4 E4A10008 */  swc1       $f1, 0x8($a1)
    /* 4AED8 8004A2D8 460C18C0 */  add.s      $f3, $f3, $f12
    /* 4AEDC 8004A2DC 44812000 */  mtc1       $at, $f4
    /* 4AEE0 8004A2E0 E4A20010 */  swc1       $f2, 0x10($a1)
    /* 4AEE4 8004A2E4 E4A30018 */  swc1       $f3, 0x18($a1)
    /* 4AEE8 8004A2E8 E4A4003C */  swc1       $f4, 0x3C($a1)
    /* 4AEEC 8004A2EC 8CC8000C */  lw         $t0, 0xC($a2)
    /* 4AEF0 8004A2F0 ACA80030 */  sw         $t0, 0x30($a1)
    /* 4AEF4 8004A2F4 8CC80010 */  lw         $t0, 0x10($a2)
    /* 4AEF8 8004A2F8 ACA80034 */  sw         $t0, 0x34($a1)
    /* 4AEFC 8004A2FC 8CC80014 */  lw         $t0, 0x14($a2)
    /* 4AF00 8004A300 ACA80038 */  sw         $t0, 0x38($a1)
    /* 4AF04 8004A304 8FBF0004 */  lw         $ra, 0x4($sp)
    /* 4AF08 8004A308 27BD0008 */  addiu      $sp, $sp, 0x8
    /* 4AF0C 8004A30C 03E00008 */  jr         $ra
    /* 4AF10 8004A310 00000000 */   nop

glabel matrix_XYZ_YPR_SCL
    /* 4AF14 8004A314 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 4AF18 8004A318 AFBF0004 */  sw         $ra, 0x4($sp)
    /* 4AF1C 8004A31C 00803025 */  or         $a2, $a0, $zero
    /* 4AF20 8004A320 0C0127CD */  jal        Sinf
    /* 4AF24 8004A324 84C40000 */   lh        $a0, 0x0($a2)
    /* 4AF28 8004A328 0C0127CC */  jal        Cosf
    /* 4AF2C 8004A32C 46000086 */   mov.s     $f2, $f0
    /* 4AF30 8004A330 460000C6 */  mov.s      $f3, $f0
    /* 4AF34 8004A334 0C0127CD */  jal        Sinf
    /* 4AF38 8004A338 84C40002 */   lh        $a0, 0x2($a2)
    /* 4AF3C 8004A33C 0C0127CC */  jal        Cosf
    /* 4AF40 8004A340 46000106 */   mov.s     $f4, $f0
    /* 4AF44 8004A344 46000146 */  mov.s      $f5, $f0
    /* 4AF48 8004A348 0C0127CD */  jal        Sinf
    /* 4AF4C 8004A34C 84C40004 */   lh        $a0, 0x4($a2)
    /* 4AF50 8004A350 0C0127CC */  jal        Cosf
    /* 4AF54 8004A354 46000186 */   mov.s     $f6, $f0
    /* 4AF58 8004A358 460001C6 */  mov.s      $f7, $f0
    /* 4AF5C 8004A35C 46071A02 */  mul.s      $f8, $f3, $f7
    /* 4AF60 8004A360 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 4AF64 8004A364 46061A42 */  mul.s      $f9, $f3, $f6
    /* 4AF68 8004A368 00000000 */  nop
    /* 4AF6C 8004A36C 46072A82 */  mul.s      $f10, $f5, $f7
    /* 4AF70 8004A370 00000000 */  nop
    /* 4AF74 8004A374 460712C2 */  mul.s      $f11, $f2, $f7
    /* 4AF78 8004A378 00000000 */  nop
    /* 4AF7C 8004A37C 46061302 */  mul.s      $f12, $f2, $f6
    /* 4AF80 8004A380 00000000 */  nop
    /* 4AF84 8004A384 46051B42 */  mul.s      $f13, $f3, $f5
    /* 4AF88 8004A388 00000000 */  nop
    /* 4AF8C 8004A38C 46041382 */  mul.s      $f14, $f2, $f4
    /* 4AF90 8004A390 00000000 */  nop
    /* 4AF94 8004A394 46041BC2 */  mul.s      $f15, $f3, $f4
    /* 4AF98 8004A398 46002C07 */  neg.s      $f16, $f5
    /* 4AF9C 8004A39C 46101442 */  mul.s      $f17, $f2, $f16
    /* 4AFA0 8004A3A0 00000000 */  nop
    /* 4AFA4 8004A3A4 46068482 */  mul.s      $f18, $f16, $f6
    /* 4AFA8 8004A3A8 C4D30008 */  lwc1       $f19, 0x8($a2)
    /* 4AFAC 8004A3AC ACA0000C */  sw         $zero, 0xC($a1)
    /* 4AFB0 8004A3B0 46067002 */  mul.s      $f0, $f14, $f6
    /* 4AFB4 8004A3B4 ACA0001C */  sw         $zero, 0x1C($a1)
    /* 4AFB8 8004A3B8 ACA0002C */  sw         $zero, 0x2C($a1)
    /* 4AFBC 8004A3BC 46077042 */  mul.s      $f1, $f14, $f7
    /* 4AFC0 8004A3C0 00000000 */  nop
    /* 4AFC4 8004A3C4 46067882 */  mul.s      $f2, $f15, $f6
    /* 4AFC8 8004A3C8 46004201 */  sub.s      $f8, $f8, $f0
    /* 4AFCC 8004A3CC 460778C2 */  mul.s      $f3, $f15, $f7
    /* 4AFD0 8004A3D0 46014A40 */  add.s      $f9, $f9, $f1
    /* 4AFD4 8004A3D4 46134202 */  mul.s      $f8, $f8, $f19
    /* 4AFD8 8004A3D8 44810800 */  mtc1       $at, $f1
    /* 4AFDC 8004A3DC 46025AC0 */  add.s      $f11, $f11, $f2
    /* 4AFE0 8004A3E0 E4A1003C */  swc1       $f1, 0x3C($a1)
    /* 4AFE4 8004A3E4 46036301 */  sub.s      $f12, $f12, $f3
    /* 4AFE8 8004A3E8 46134A42 */  mul.s      $f9, $f9, $f19
    /* 4AFEC 8004A3EC E4A80000 */  swc1       $f8, 0x0($a1)
    /* 4AFF0 8004A3F0 46138C42 */  mul.s      $f17, $f17, $f19
    /* 4AFF4 8004A3F4 E4A90004 */  swc1       $f9, 0x4($a1)
    /* 4AFF8 8004A3F8 46139482 */  mul.s      $f18, $f18, $f19
    /* 4AFFC 8004A3FC E4B10008 */  swc1       $f17, 0x8($a1)
    /* 4B000 8004A400 46135282 */  mul.s      $f10, $f10, $f19
    /* 4B004 8004A404 E4B20010 */  swc1       $f18, 0x10($a1)
    /* 4B008 8004A408 46132102 */  mul.s      $f4, $f4, $f19
    /* 4B00C 8004A40C 00000000 */  nop
    /* 4B010 8004A410 46135AC2 */  mul.s      $f11, $f11, $f19
    /* 4B014 8004A414 E4AA0014 */  swc1       $f10, 0x14($a1)
    /* 4B018 8004A418 E4A40018 */  swc1       $f4, 0x18($a1)
    /* 4B01C 8004A41C 46136302 */  mul.s      $f12, $f12, $f19
    /* 4B020 8004A420 E4AB0020 */  swc1       $f11, 0x20($a1)
    /* 4B024 8004A424 46136B42 */  mul.s      $f13, $f13, $f19
    /* 4B028 8004A428 E4AC0024 */  swc1       $f12, 0x24($a1)
    /* 4B02C 8004A42C E4AD0028 */  swc1       $f13, 0x28($a1)
    /* 4B030 8004A430 C4C5000C */  lwc1       $f5, 0xC($a2)
    /* 4B034 8004A434 C4C60010 */  lwc1       $f6, 0x10($a2)
    /* 4B038 8004A438 C4C70014 */  lwc1       $f7, 0x14($a2)
    /* 4B03C 8004A43C 46082802 */  mul.s      $f0, $f5, $f8
    /* 4B040 8004A440 00000000 */  nop
    /* 4B044 8004A444 46123042 */  mul.s      $f1, $f6, $f18
    /* 4B048 8004A448 46010000 */  add.s      $f0, $f0, $f1
    /* 4B04C 8004A44C 460B3842 */  mul.s      $f1, $f7, $f11
    /* 4B050 8004A450 46010000 */  add.s      $f0, $f0, $f1
    /* 4B054 8004A454 E4A00030 */  swc1       $f0, 0x30($a1)
    /* 4B058 8004A458 46092802 */  mul.s      $f0, $f5, $f9
    /* 4B05C 8004A45C 00000000 */  nop
    /* 4B060 8004A460 460A3042 */  mul.s      $f1, $f6, $f10
    /* 4B064 8004A464 46010000 */  add.s      $f0, $f0, $f1
    /* 4B068 8004A468 460C3842 */  mul.s      $f1, $f7, $f12
    /* 4B06C 8004A46C 46010000 */  add.s      $f0, $f0, $f1
    /* 4B070 8004A470 E4A00034 */  swc1       $f0, 0x34($a1)
    /* 4B074 8004A474 46112802 */  mul.s      $f0, $f5, $f17
    /* 4B078 8004A478 00000000 */  nop
    /* 4B07C 8004A47C 46043042 */  mul.s      $f1, $f6, $f4
    /* 4B080 8004A480 46010000 */  add.s      $f0, $f0, $f1
    /* 4B084 8004A484 460D3842 */  mul.s      $f1, $f7, $f13
    /* 4B088 8004A488 46010000 */  add.s      $f0, $f0, $f1
    /* 4B08C 8004A48C E4A00038 */  swc1       $f0, 0x38($a1)
    /* 4B090 8004A490 8FBF0004 */  lw         $ra, 0x4($sp)
    /* 4B094 8004A494 27BD0008 */  addiu      $sp, $sp, 0x8
    /* 4B098 8004A498 03E00008 */  jr         $ra
    /* 4B09C 8004A49C 00000000 */   nop

glabel matrix_XYZ_YPR
    /* 4B0A0 8004A4A0 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 4B0A4 8004A4A4 AFBF0004 */  sw         $ra, 0x4($sp)
    /* 4B0A8 8004A4A8 00803025 */  or         $a2, $a0, $zero
    /* 4B0AC 8004A4AC 0C0127CD */  jal        Sinf
    /* 4B0B0 8004A4B0 84C40000 */   lh        $a0, 0x0($a2)
    /* 4B0B4 8004A4B4 0C0127CC */  jal        Cosf
    /* 4B0B8 8004A4B8 46000086 */   mov.s     $f2, $f0
    /* 4B0BC 8004A4BC 460000C6 */  mov.s      $f3, $f0
    /* 4B0C0 8004A4C0 0C0127CD */  jal        Sinf
    /* 4B0C4 8004A4C4 84C40002 */   lh        $a0, 0x2($a2)
    /* 4B0C8 8004A4C8 0C0127CC */  jal        Cosf
    /* 4B0CC 8004A4CC 46000106 */   mov.s     $f4, $f0
    /* 4B0D0 8004A4D0 46000146 */  mov.s      $f5, $f0
    /* 4B0D4 8004A4D4 0C0127CD */  jal        Sinf
    /* 4B0D8 8004A4D8 84C40004 */   lh        $a0, 0x4($a2)
    /* 4B0DC 8004A4DC 0C0127CC */  jal        Cosf
    /* 4B0E0 8004A4E0 46000186 */   mov.s     $f6, $f0
    /* 4B0E4 8004A4E4 460001C6 */  mov.s      $f7, $f0
    /* 4B0E8 8004A4E8 46071A02 */  mul.s      $f8, $f3, $f7
    /* 4B0EC 8004A4EC 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 4B0F0 8004A4F0 ACA0000C */  sw         $zero, 0xC($a1)
    /* 4B0F4 8004A4F4 46061A42 */  mul.s      $f9, $f3, $f6
    /* 4B0F8 8004A4F8 ACA0001C */  sw         $zero, 0x1C($a1)
    /* 4B0FC 8004A4FC ACA0002C */  sw         $zero, 0x2C($a1)
    /* 4B100 8004A500 46072A82 */  mul.s      $f10, $f5, $f7
    /* 4B104 8004A504 00000000 */  nop
    /* 4B108 8004A508 460712C2 */  mul.s      $f11, $f2, $f7
    /* 4B10C 8004A50C 00000000 */  nop
    /* 4B110 8004A510 46061302 */  mul.s      $f12, $f2, $f6
    /* 4B114 8004A514 00000000 */  nop
    /* 4B118 8004A518 46051B42 */  mul.s      $f13, $f3, $f5
    /* 4B11C 8004A51C 00000000 */  nop
    /* 4B120 8004A520 46041382 */  mul.s      $f14, $f2, $f4
    /* 4B124 8004A524 00000000 */  nop
    /* 4B128 8004A528 46041BC2 */  mul.s      $f15, $f3, $f4
    /* 4B12C 8004A52C 46002C07 */  neg.s      $f16, $f5
    /* 4B130 8004A530 46101442 */  mul.s      $f17, $f2, $f16
    /* 4B134 8004A534 00000000 */  nop
    /* 4B138 8004A538 46068482 */  mul.s      $f18, $f16, $f6
    /* 4B13C 8004A53C 00000000 */  nop
    /* 4B140 8004A540 46067002 */  mul.s      $f0, $f14, $f6
    /* 4B144 8004A544 E4B10008 */  swc1       $f17, 0x8($a1)
    /* 4B148 8004A548 46077042 */  mul.s      $f1, $f14, $f7
    /* 4B14C 8004A54C E4B20010 */  swc1       $f18, 0x10($a1)
    /* 4B150 8004A550 46067882 */  mul.s      $f2, $f15, $f6
    /* 4B154 8004A554 46004201 */  sub.s      $f8, $f8, $f0
    /* 4B158 8004A558 460778C2 */  mul.s      $f3, $f15, $f7
    /* 4B15C 8004A55C E4A80000 */  swc1       $f8, 0x0($a1)
    /* 4B160 8004A560 46025AC0 */  add.s      $f11, $f11, $f2
    /* 4B164 8004A564 E4A40018 */  swc1       $f4, 0x18($a1)
    /* 4B168 8004A568 46014A40 */  add.s      $f9, $f9, $f1
    /* 4B16C 8004A56C 44810800 */  mtc1       $at, $f1
    /* 4B170 8004A570 E4AB0020 */  swc1       $f11, 0x20($a1)
    /* 4B174 8004A574 46036301 */  sub.s      $f12, $f12, $f3
    /* 4B178 8004A578 E4A90004 */  swc1       $f9, 0x4($a1)
    /* 4B17C 8004A57C E4AA0014 */  swc1       $f10, 0x14($a1)
    /* 4B180 8004A580 E4A1003C */  swc1       $f1, 0x3C($a1)
    /* 4B184 8004A584 E4AC0024 */  swc1       $f12, 0x24($a1)
    /* 4B188 8004A588 E4AD0028 */  swc1       $f13, 0x28($a1)
    /* 4B18C 8004A58C C4C5000C */  lwc1       $f5, 0xC($a2)
    /* 4B190 8004A590 C4C60010 */  lwc1       $f6, 0x10($a2)
    /* 4B194 8004A594 C4C70014 */  lwc1       $f7, 0x14($a2)
    /* 4B198 8004A598 46082802 */  mul.s      $f0, $f5, $f8
    /* 4B19C 8004A59C 00000000 */  nop
    /* 4B1A0 8004A5A0 46123042 */  mul.s      $f1, $f6, $f18
    /* 4B1A4 8004A5A4 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1A8 8004A5A8 460B3842 */  mul.s      $f1, $f7, $f11
    /* 4B1AC 8004A5AC 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1B0 8004A5B0 E4A00030 */  swc1       $f0, 0x30($a1)
    /* 4B1B4 8004A5B4 46092802 */  mul.s      $f0, $f5, $f9
    /* 4B1B8 8004A5B8 00000000 */  nop
    /* 4B1BC 8004A5BC 460A3042 */  mul.s      $f1, $f6, $f10
    /* 4B1C0 8004A5C0 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1C4 8004A5C4 460C3842 */  mul.s      $f1, $f7, $f12
    /* 4B1C8 8004A5C8 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1CC 8004A5CC E4A00034 */  swc1       $f0, 0x34($a1)
    /* 4B1D0 8004A5D0 46112802 */  mul.s      $f0, $f5, $f17
    /* 4B1D4 8004A5D4 00000000 */  nop
    /* 4B1D8 8004A5D8 46043042 */  mul.s      $f1, $f6, $f4
    /* 4B1DC 8004A5DC 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1E0 8004A5E0 460D3842 */  mul.s      $f1, $f7, $f13
    /* 4B1E4 8004A5E4 46010000 */  add.s      $f0, $f0, $f1
    /* 4B1E8 8004A5E8 E4A00038 */  swc1       $f0, 0x38($a1)
    /* 4B1EC 8004A5EC 8FBF0004 */  lw         $ra, 0x4($sp)
    /* 4B1F0 8004A5F0 27BD0008 */  addiu      $sp, $sp, 0x8
    /* 4B1F4 8004A5F4 03E00008 */  jr         $ra
    /* 4B1F8 8004A5F8 00000000 */   nop

glabel matrixTransposeVectorMultiply
    /* 4B1FC 8004A5FC C4A40000 */  lwc1       $f4, 0x0($a1)
    /* 4B200 8004A600 C4880000 */  lwc1       $f8, 0x0($a0)
    /* 4B204 8004A604 C4A50004 */  lwc1       $f5, 0x4($a1)
    /* 4B208 8004A608 C4890004 */  lwc1       $f9, 0x4($a0)
    /* 4B20C 8004A60C 46082402 */  mul.s      $f16, $f4, $f8
    /* 4B210 8004A610 C4A60008 */  lwc1       $f6, 0x8($a1)
    /* 4B214 8004A614 C48A0008 */  lwc1       $f10, 0x8($a0)
    /* 4B218 8004A618 46092C42 */  mul.s      $f17, $f5, $f9
    /* 4B21C 8004A61C C4A7000C */  lwc1       $f7, 0xC($a1)
    /* 4B220 8004A620 C48B000C */  lwc1       $f11, 0xC($a0)
    /* 4B224 8004A624 46118480 */  add.s      $f18, $f16, $f17
    /* 4B228 8004A628 460A3402 */  mul.s      $f16, $f6, $f10
    /* 4B22C 8004A62C 46109480 */  add.s      $f18, $f18, $f16
    /* 4B230 8004A630 460B3C02 */  mul.s      $f16, $f7, $f11
    /* 4B234 8004A634 46109480 */  add.s      $f18, $f18, $f16
    /* 4B238 8004A638 E4D20000 */  swc1       $f18, 0x0($a2)
    /* 4B23C 8004A63C C4880010 */  lwc1       $f8, 0x10($a0)
    /* 4B240 8004A640 C4890014 */  lwc1       $f9, 0x14($a0)
    /* 4B244 8004A644 C48A0018 */  lwc1       $f10, 0x18($a0)
    /* 4B248 8004A648 46082402 */  mul.s      $f16, $f4, $f8
    /* 4B24C 8004A64C C48B001C */  lwc1       $f11, 0x1C($a0)
    /* 4B250 8004A650 46092C42 */  mul.s      $f17, $f5, $f9
    /* 4B254 8004A654 46118480 */  add.s      $f18, $f16, $f17
    /* 4B258 8004A658 460A3402 */  mul.s      $f16, $f6, $f10
    /* 4B25C 8004A65C 46109480 */  add.s      $f18, $f18, $f16
    /* 4B260 8004A660 460B3C02 */  mul.s      $f16, $f7, $f11
    /* 4B264 8004A664 46109480 */  add.s      $f18, $f18, $f16
    /* 4B268 8004A668 E4D20004 */  swc1       $f18, 0x4($a2)
    /* 4B26C 8004A66C C4880020 */  lwc1       $f8, 0x20($a0)
    /* 4B270 8004A670 C4890024 */  lwc1       $f9, 0x24($a0)
    /* 4B274 8004A674 C48A0028 */  lwc1       $f10, 0x28($a0)
    /* 4B278 8004A678 46082402 */  mul.s      $f16, $f4, $f8
    /* 4B27C 8004A67C C48B002C */  lwc1       $f11, 0x2C($a0)
    /* 4B280 8004A680 46092C42 */  mul.s      $f17, $f5, $f9
    /* 4B284 8004A684 46118480 */  add.s      $f18, $f16, $f17
    /* 4B288 8004A688 460A3402 */  mul.s      $f16, $f6, $f10
    /* 4B28C 8004A68C 46109480 */  add.s      $f18, $f18, $f16
    /* 4B290 8004A690 460B3C02 */  mul.s      $f16, $f7, $f11
    /* 4B294 8004A694 46109480 */  add.s      $f18, $f18, $f16
    /* 4B298 8004A698 E4D20008 */  swc1       $f18, 0x8($a2)
    /* 4B29C 8004A69C C4880030 */  lwc1       $f8, 0x30($a0)
    /* 4B2A0 8004A6A0 C4890034 */  lwc1       $f9, 0x34($a0)
    /* 4B2A4 8004A6A4 C48A0038 */  lwc1       $f10, 0x38($a0)
    /* 4B2A8 8004A6A8 46082402 */  mul.s      $f16, $f4, $f8
    /* 4B2AC 8004A6AC C48B003C */  lwc1       $f11, 0x3C($a0)
    /* 4B2B0 8004A6B0 46092C42 */  mul.s      $f17, $f5, $f9
    /* 4B2B4 8004A6B4 46118480 */  add.s      $f18, $f16, $f17
    /* 4B2B8 8004A6B8 460A3402 */  mul.s      $f16, $f6, $f10
    /* 4B2BC 8004A6BC 46109480 */  add.s      $f18, $f18, $f16
    /* 4B2C0 8004A6C0 460B3C02 */  mul.s      $f16, $f7, $f11
    /* 4B2C4 8004A6C4 46109480 */  add.s      $f18, $f18, $f16
    /* 4B2C8 8004A6C8 03E00008 */  jr         $ra
    /* 4B2CC 8004A6CC E4D2000C */   swc1      $f18, 0xC($a2)
