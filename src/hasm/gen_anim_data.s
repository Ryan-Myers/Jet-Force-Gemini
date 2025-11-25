.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .data

dlabel D_800A7980
.word 0x00000000

dlabel D_800A7984
.word 0x00000000

dlabel D_800A7988
.word 0x00000000

/* 288 short zeroes 0x240 bytes */
dlabel D_800A798C
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.short 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

/* 144 byte zeroes 0x240 bytes */
dlabel D_800A7BCC
.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

/* 44 byte zeroes 0x2C bytes */
dlabel D_800A7E0C
.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

dlabel gen_anim_data_jump_table
.word 0x00000000 /* NULL jump target? */
.word .gen_anim_data_jump_target

.section .text, "ax"

glabel gen_anim_data
    lui        $at, %hi(D_800A7984)
    sw         $a0, %lo(D_800A7984)($at)
    lw         $a0, 0x0($a0)
    addiu      $sp, $sp, -0x28
    sw         $ra, 0x0($sp)
    sw         $s0, 0x4($sp)
    sw         $s1, 0x8($sp)
    sw         $s2, 0xC($sp)
    sw         $s3, 0x10($sp)
    sw         $s4, 0x14($sp)
    sw         $s5, 0x18($sp)
    sw         $s6, 0x1C($sp)
    sw         $s7, 0x20($sp)
    addiu      $sp, $sp, -0x38
    sw         $a0, %lo(D_800A7980)($at)
    sw         $ra, 0x0($sp)
    swc1       $f30, 0x2C($sp)
    lui        $at, %hi(D_800A7988)
    swc1       $f20, 0x4($sp)
    swc1       $f21, 0x8($sp)
    swc1       $f22, 0xC($sp)
    swc1       $f23, 0x10($sp)
    swc1       $f24, 0x14($sp)
    swc1       $f25, 0x18($sp)
    swc1       $f26, 0x1C($sp)
    swc1       $f27, 0x20($sp)
    swc1       $f28, 0x24($sp)
    swc1       $f29, 0x28($sp)
    swc1       $f31, 0x30($sp)
    sw         $a3, %lo(D_800A7988)($at)
    mtc1       $zero, $f30
    lui        $t1, %hi(D_800A7BCC)
    lui        $t5, %hi(D_800A798C)
    lw         $a3, 0x40($a2)
    addiu      $t1, $t1, %lo(D_800A7BCC)
    addiu      $t5, $t5, %lo(D_800A798C)
    lw         $t4, 0x30($a2)
    lh         $s4, 0x38($a2)
    jal        func_80074B50
     lwc1      $f4, 0x8($a2)
    lh         $t3, 0x3E($a2)
    blez       $t3, .L80074EDC
     nop
    mov.s      $f10, $f22
    mov.s      $f11, $f23
    mov.s      $f12, $f24
    mtc1       $t5, $f13
    mtc1       $t1, $f14
    lw         $a3, 0x44($a2)
    lw         $t4, 0x34($a2)
    lh         $s4, 0x3A($a2)
    jal        func_80074B50
     lwc1      $f4, 0xC($a2)
    lh         $t6, 0x3E($a2)
    li.s       $at, 1024.0
    mfc1       $s7, $f14
    mtc1       $t6, $f20
    mfc1       $t4, $f13
    sub.s      $f13, $f10, $f22
    lw         $t1, 0x70($sp)
    lui        $t0, %hi(D_800A7988)
    cvt.s.w    $f20, $f20
    mtc1       $at, $f21
    li.s       $at, 1.0
    mtc1       $at, $f31
    lw         $t0, %lo(D_800A7988)($t0)
    sub.s      $f14, $f11, $f23
    li.s       $at, 2.0
    lui        $s5, %hi(D_800A7BCC)
    div.s      $f28, $f20, $f21
    lui        $t5, %hi(D_800A798C)
    lui        $s2, %hi(D_800A8754)
    sll        $t2, $t1, 4
    sub.s      $f15, $f12, $f24
    mtc1       $at, $f29
    addiu      $s5, $s5, %lo(D_800A7BCC)
    addiu      $t5, $t5, %lo(D_800A798C)
    addiu      $s2, $s2, %lo(D_800A8754)
    addiu      $t3, $zero, 0xFFC
    addiu      $s6, $zero, 0x1000
    addu       $s4, $t2, $t0
    sub.s      $f27, $f31, $f28
    mul.s      $f13, $f13, $f27
    nop
    mul.s      $f14, $f14, $f27
    nop
    mul.s      $f15, $f15, $f27
    nop
  .L80074760:
    lbu        $t2, 0x3($t0)
    sll        $s1, $t2, 1
    addu       $s1, $s1, $t2
    sll        $s1, $s1, 1
    addu       $s1, $s1, $s7
    lbu        $t2, 0x2($t0)
    sll        $t7, $t2, 1
    addu       $t7, $t7, $t2
    sll        $t7, $t7, 1
    addu       $s0, $t7, $s5
    addiu      $t1, $zero, 0x2
  .L8007478C:
    lhu        $t2, 0x0($s1)
    bnez       $t2, .L8007479C
     addi      $t2, $t2, 0x2
    ori        $t2, $t2, 0x8000
  .L8007479C:
    lhu        $t7, 0x0($s0)
    bnez       $t7, .L800747AC
     addiu     $s0, $s0, 0x2
    ori        $t7, $t7, 0x8000
  .L800747AC:
    subu       $t2, $t2, $t7
    mult       $t2, $t6
    mflo       $t2
    sra        $t2, $t2, 10
    addu       $t2, $t2, $t7
    sh         $t2, -0x2($s0)
    bnez       $t1, .L8007478C
     addiu     $t1, $t1, -0x1
    addiu      $t0, $t0, 0x10
    bne        $t0, $s4, .L80074760
     nop
    add.s      $f22, $f13, $f22
    lui        $t0, %hi(D_800A7988)
    lui        $s5, %hi(D_800A7BCC)
    lw         $t1, 0x70($sp)
    add.s      $f23, $f14, $f23
    lw         $t0, %lo(D_800A7988)($t0)
    addiu      $s5, $s5, %lo(D_800A7BCC)
    add.s      $f24, $f15, $f24
  .L800747F8:
    lbu        $s0, 0x2($t0)
    sll        $s1, $s0, 1
    addu       $s1, $s1, $s0
    sll        $s1, $s1, 1
    addu       $t9, $s5, $s1
    jal        func_80074A68
     addu      $s0, $s1, $t5
    lbu        $s0, 0x3($t0)
    sll        $s1, $s0, 1
    addu       $s1, $s1, $s0
    sll        $s1, $s1, 1
    addu       $s0, $s1, $t4
    mul.s      $f14, $f7, $f18
    add.s      $f20, $f10, $f11
    mul.s      $f15, $f8, $f19
    sub.s      $f21, $f12, $f13
    mul.s      $f16, $f6, $f19
    add.s      $f25, $f14, $f15
    mul.s      $f17, $f9, $f18
    jal        func_80074A68
     sub.s     $f26, $f16, $f17
    mul.s      $f14, $f7, $f18
    add.s      $f4, $f10, $f11
    mul.s      $f15, $f8, $f19
    sub.s      $f5, $f12, $f13
    mul.s      $f16, $f6, $f19
    add.s      $f6, $f14, $f15
    mul.s      $f17, $f9, $f18
    sub.s      $f7, $f16, $f17
    mul.s      $f10, $f20, $f4
    nop
    mul.s      $f11, $f21, $f5
    nop
    mul.s      $f12, $f25, $f6
    add.s      $f10, $f10, $f11
    mul.s      $f13, $f26, $f7
    add.s      $f10, $f10, $f12
    mul.s      $f20, $f20, $f27
    add.s      $f10, $f10, $f13
    mul.s      $f21, $f21, $f27
    c.olt.s    $f10, $f30
    mul.s      $f25, $f25, $f27
    bc1f       .L800748B8
     nop
    sub.s      $f4, $f30, $f4
    sub.s      $f5, $f30, $f5
    sub.s      $f6, $f30, $f6
    sub.s      $f7, $f30, $f7
  .L800748B8:
    mul.s      $f26, $f26, $f27
    nop
    mul.s      $f4, $f4, $f28
    lbu        $s4, 0x1($t0)
    mul.s      $f5, $f5, $f28
    add.s      $f10, $f20, $f4
    mul.s      $f6, $f6, $f28
    add.s      $f11, $f21, $f5
    mul.s      $f7, $f7, $f28
    add.s      $f12, $f25, $f6
    mul.s      $f0, $f11, $f29
    add.s      $f13, $f26, $f7
    mul.s      $f1, $f12, $f29
    sll        $s4, $s4, 6
    mul.s      $f2, $f13, $f29
    lui        $t7, %hi(D_800A7980)
    lw         $t7, %lo(D_800A7980)($t7)
    mul.s      $f3, $f10, $f0
    addu       $t7, $t7, $s4
    mul.s      $f4, $f10, $f1
    lw         $t6, 0x4($t0)
    mul.s      $f5, $f10, $f2
    lw         $s0, 0x8($t0)
    mul.s      $f6, $f11, $f0
    lw         $s3, 0xC($t0)
    mul.s      $f7, $f11, $f1
    nop
    mul.s      $f8, $f11, $f2
    add.s      $f20, $f7, $f5
    mul.s      $f17, $f13, $f2
    sub.s      $f21, $f8, $f4
    mul.s      $f15, $f12, $f1
    add.s      $f25, $f8, $f4
    mul.s      $f16, $f12, $f2
    add.s      $f19, $f15, $f17
    sub.s      $f19, $f31, $f19
    sub.s      $f26, $f16, $f3
    add.s      $f10, $f6, $f15
    sub.s      $f10, $f31, $f10
    add.s      $f4, $f16, $f3
    sub.s      $f2, $f7, $f5
    add.s      $f3, $f6, $f17
    sub.s      $f3, $f31, $f3
    li.s       $at, 0.000030517578 /* (1.0 / 32768.0) */
    mtc1       $at, $f1
    lhu        $t2, 0x0($t9)
    bnez       $t2, .L800749CC
     nop
    swc1       $f19, 0x0($t7)
    lhu        $t2, 0x2($t9)
    swc1       $f20, 0x4($t7)
    bnez       $t2, .L80074A00
     swc1      $f21, 0x8($t7)
  .L8007498C:
    swc1       $f2, 0x10($t7)
    lhu        $t2, 0x4($t9)
    swc1       $f3, 0x14($t7)
    bnez       $t2, .L80074A34
     swc1      $f4, 0x18($t7)
  .L800749A0:
    swc1       $f25, 0x20($t7)
    swc1       $f26, 0x24($t7)
    swc1       $f10, 0x28($t7)
  alabel func_800749AC
    addiu      $t0, $t0, 0x10
    sw         $t6, 0x30($t7)
    addiu      $t1, $t1, -0x1
    sw         $s0, 0x34($t7)
    bnez       $t1, .L800747F8
     sw        $s3, 0x38($t7)
    j          func_8007524C
     nop
  .L800749CC:
    mtc1       $t2, $f0
    nop
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f0, $f1
    lhu        $t2, 0x2($t9)
    mul.s      $f19, $f19, $f0
    nop
    swc1       $f19, 0x0($t7)
    mul.s      $f20, $f20, $f0
    swc1       $f20, 0x4($t7)
    mul.s      $f21, $f21, $f0
    beqz       $t2, .L8007498C
     swc1      $f21, 0x8($t7)
  .L80074A00:
    mtc1       $t2, $f0
    nop
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f0, $f1
    lhu        $t2, 0x4($t9)
    mul.s      $f2, $f2, $f0
    nop
    swc1       $f2, 0x10($t7)
    mul.s      $f3, $f3, $f0
    swc1       $f3, 0x14($t7)
    mul.s      $f4, $f4, $f0
    beqz       $t2, .L800749A0
     swc1      $f4, 0x18($t7)
  .L80074A34:
    mtc1       $t2, $f0
    nop
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f0, $f1
    nop
    mul.s      $f25, $f25, $f0
    nop
    swc1       $f25, 0x20($t7)
    mul.s      $f26, $f26, $f0
    swc1       $f26, 0x24($t7)
    mul.s      $f10, $f10, $f0
    j          func_800749AC
     swc1      $f10, 0x28($t7)
endlabel gen_anim_data

glabel func_80074A68
    lh         $t7, 0x0($s0)
    srl        $t2, $t7, 5
    sll        $t2, $t2, 2
    and        $s3, $t2, $t3
    addu       $t6, $s3, $s2
    bltz       $t7, .L80074A94
     subu      $s3, $s6, $s3
    lwc1       $f1, 0x0($t6)
    addu       $t6, $s3, $s2
    j          func_80074AA4
     lwc1      $f0, 0x0($t6)
  .L80074A94:
    lwc1       $f0, 0x0($t6)
    addu       $t6, $s3, $s2
    lwc1       $f1, 0x0($t6)
    sub.s      $f0, $f30, $f0
  alabel func_80074AA4
    lh         $t7, 0x2($s0)
    srl        $t2, $t7, 5
    sll        $t2, $t2, 2
    and        $s3, $t2, $t3
    addu       $t6, $s3, $s2
    bltz       $t7, .L80074AD0
     subu      $s3, $s6, $s3
    lwc1       $f3, 0x0($t6)
    addu       $t6, $s3, $s2
    j          func_80074AE0
     lwc1      $f2, 0x0($t6)
  .L80074AD0:
    lwc1       $f2, 0x0($t6)
    addu       $t6, $s3, $s2
    lwc1       $f3, 0x0($t6)
    sub.s      $f2, $f30, $f2
  alabel func_80074AE0
    lh         $t7, 0x4($s0)
    mul.s      $f6, $f0, $f2
    srl        $t2, $t7, 5
    mul.s      $f7, $f0, $f3
    sll        $t2, $t2, 2
    mul.s      $f8, $f1, $f2
    and        $s3, $t2, $t3
    mul.s      $f9, $f1, $f3
    addu       $t6, $s3, $s2
    bltz       $t7, .L80074B1C
     subu      $s3, $s6, $s3
    lwc1       $f19, 0x0($t6)
    addu       $t6, $s3, $s2
    j          func_80074B2C
     lwc1      $f18, 0x0($t6)
  .L80074B1C:
    lwc1       $f18, 0x0($t6)
    addu       $t6, $s3, $s2
    lwc1       $f19, 0x0($t6)
    sub.s      $f18, $f30, $f18
  alabel func_80074B2C
    mul.s      $f10, $f6, $f18
    nop
    mul.s      $f11, $f9, $f19
    nop
    mul.s      $f12, $f8, $f18
    nop
    mul.s      $f13, $f7, $f19
    jr         $ra
     nop
endlabel func_80074A68

glabel func_80074B50
    bnel       $t4, $zero, .L80074B64
     floor.w.s $f6, $f4
    lui        $t4, %hi(D_800A798C)
    addiu      $t4, $t4, %lo(D_800A798C)
    floor.w.s  $f6, $f4
  .L80074B64:
    li.s       $at, 1024.0
    lbu        $t6, 0x1($a3)
    lbu        $s1, 0x6($a3)
    cvt.s.w    $f6, $f6
    addiu      $t6, $t6, -0x1
    sll        $t7, $t6, 1
    addu       $t6, $t6, $t7
    lb         $t7, 0x0($a3)
    addu       $s4, $s4, $t4
    sub.s      $f4, $f4, $f6
    mtc1       $at, $f5
    ldl        $s3, 0x0($t4)
    ldl        $s7, 0x0($s4)
    mul.s      $f4, $f4, $f5
    or         $t0, $t5, $zero
    srl        $s0, $s1, 4
    addiu      $t3, $a3, 0x8
    addiu      $s6, $zero, 0x40
    or         $s2, $zero, $zero
    sll        $t7, $t7, 11
    cvt.w.s    $f4, $f4
    ldr        $s3, 0x7($t4)
    ldr        $s7, 0x7($s4)
    mfc1       $t9, $f4
    beql       $s0, $zero, .L80074C00
     mtc1      $t7, $f22
    subu       $t2, $s6, $s0
    dsrlv      $s5, $s3, $t2
    dsrlv      $t8, $s7, $t2
    sub        $t8, $t8, $s5
    mult       $t8, $t9
    sll        $s5, $s5, 10
    addu       $s2, $s2, $s0
    dsllv      $s7, $s7, $s0
    mflo       $t8
    addu       $s5, $s5, $t8
    addu       $t7, $t7, $s5
    dsllv      $s3, $s3, $s0
    mtc1       $t7, $f22
  .L80074C00:
    lb         $t7, 0x2($a3)
    andi       $s0, $s1, 0xF
    cvt.s.w    $f22, $f22
    beqz       $s0, .L80074C44
     sll       $t7, $t7, 11
    subu       $t2, $s6, $s0
    dsrlv      $s5, $s3, $t2
    dsrlv      $t8, $s7, $t2
    sub        $t8, $t8, $s5
    mult       $t8, $t9
    sll        $s5, $s5, 10
    addu       $s2, $s2, $s0
    dsllv      $s7, $s7, $s0
    mflo       $t8
    addu       $s5, $s5, $t8
    addu       $t7, $t7, $s5
    dsllv      $s3, $s3, $s0
  .L80074C44:
    lbu        $s0, 0x7($a3)
    mtc1       $t7, $f23
    lb         $t7, 0x4($a3)
    andi       $s0, $s0, 0xF
    cvt.s.w    $f23, $f23
    beqz       $s0, .L80074C90
     sll       $t7, $t7, 11
    subu       $t2, $s6, $s0
    dsrlv      $s5, $s3, $t2
    dsrlv      $t8, $s7, $t2
    sub        $t8, $t8, $s5
    mult       $t8, $t9
    sll        $s5, $s5, 10
    addu       $s2, $s2, $s0
    dsllv      $s7, $s7, $s0
    mflo       $t8
    addu       $s5, $s5, $t8
    addu       $t7, $t7, $s5
    dsllv      $s3, $s3, $s0
  .L80074C90:
    mtc1       $t7, $f24
    ori        $t7, $zero, 0xFFF0
    cvt.s.w    $f24, $f24
  .L80074C9C:
    lhu        $s0, 0x0($t3)
    andi       $s1, $s0, 0xF
    beqz       $s1, .L80074D24
     and       $s0, $s0, $t7
    addu       $s2, $s2, $s1
    slti       $at, $s2, 0x41
    bnel       $at, $zero, .L80074CF0
     subu      $t2, $s6, $s1
    subu       $s2, $s2, $s1
    srl        $s3, $s2, 3
    addu       $s4, $s4, $s3
    addu       $t4, $t4, $s3
    ldl        $s3, 0x0($t4)
    ldl        $s7, 0x0($s4)
    ldr        $s3, 0x7($t4)
    ldr        $s7, 0x7($s4)
    andi       $s2, $s2, 0x7
    dsllv      $s3, $s3, $s2
    dsllv      $s7, $s7, $s2
    addu       $s2, $s2, $s1
    subu       $t2, $s6, $s1
  .L80074CF0:
    dsrlv      $s5, $s3, $t2
    dsrlv      $t8, $s7, $t2
    subu       $t8, $t8, $s5
    sll        $t8, $t8, 21
    sra        $t8, $t8, 21
    mult       $t8, $t9
    dsllv      $s7, $s7, $s1
    mflo       $t8
    sra        $t8, $t8, 10
    addu       $s5, $s5, $t8
    sll        $s5, $s5, 5
    addu       $s0, $s0, $s5
    dsllv      $s3, $s3, $s1
  .L80074D24:
    sh         $s0, 0x0($t5)
    andi       $s0, $s0, 0x10
    bnel       $s0, $zero, .L80074E3C
     lbu       $s1, 0x3($t3)
    addiu      $t6, $t6, -0x1
    sh         $s0, 0x0($t1)
    addiu      $t1, $t1, 0x2
    addiu      $t5, $t5, 0x2
    bnez       $t6, .L80074C9C
     addiu     $t3, $t3, 0x2
  alabel func_80074D4C
    lw         $s3, 0x74($sp)
    bnel       $s3, $zero, .L80074D64
     lhu       $s0, 0x0($s3)
    jr         $ra
     nop
  alabel func_80074D60
    lhu        $s0, 0x0($s3)
  .L80074D64:
    addiu      $at, $zero, 0x1000
    lhu        $s5, 0x2($s3)
    andi       $t8, $s0, 0xF000
    addiu      $s3, $s3, 0x4
    bne        $t8, $at, .L80074D84
     xor       $s0, $s0, $t8
    jr         $ra
     nop
  .L80074D84:
    bnel       $t8, $zero, .L80074DA4
     addiu     $at, $zero, 0x2000
    addu       $s0, $s0, $t0
    lh         $t8, 0x0($s0)
    add        $s5, $s5, $t8
    j          func_80074D60
     sh        $s5, 0x0($s0)
    addiu      $at, $zero, 0x2000
  .L80074DA4:
    bnel       $t8, $at, .L80074DBC
     addiu     $at, $zero, 0x3000
    addu       $s0, $s0, $t0
    j          func_80074D60
     sh        $s5, 0x0($s0)
    addiu      $at, $zero, 0x3000
  .L80074DBC:
    bnel       $t8, $at, .L80074DF8
     addiu     $at, $zero, 0x4000
    addu       $s0, $s0, $t0
    lh         $t8, 0x0($s0)
    lh         $s0, 0x0($s3)
    addiu      $s3, $s3, 0x2
    mult       $t8, $s5
    addu       $s0, $s0, $t0
    lh         $t8, 0x0($s0)
    mflo       $s5
    sra        $s5, $s5, 10
    add        $s5, $s5, $t8
    j          func_80074D60
     sh        $s5, 0x0($s0)
    addiu      $at, $zero, 0x4000
  .L80074DF8:
    bnel       $t8, $at, .L80074E18
     addiu     $at, $zero, 0x5000
    lui        $t1, %hi(D_800A7BCC)
    addiu      $t1, $t1, %lo(D_800A7BCC)
    addu       $s0, $s0, $t1
    j          func_80074D60
     sh        $s5, 0x0($s0)
    addiu      $at, $zero, 0x5000
  .L80074E18:
    bne        $t8, $at, .L80074E30
     nop
    addu       $s0, $s0, $t0
    lh         $t8, 0x0($s0)
    j          func_80074D60
     sh        $t8, -0x2($s3)
  .L80074E30:
    jr         $ra
     nop
    lbu        $s1, 0x3($t3)
  .L80074E3C:
    lbu        $s0, 0x2($t3)
    addiu      $t3, $t3, 0x4
    andi       $s1, $s1, 0xF
    addiu      $t5, $t5, 0x2
    beqz       $s1, .L80074EC4
     sll       $s0, $s0, 8
    addu       $s2, $s2, $s1
    slti       $at, $s2, 0x41
    bnel       $at, $zero, .L80074E98
     subu      $t2, $s6, $s1
    subu       $s2, $s2, $s1
    srl        $s3, $s2, 3
    addu       $s4, $s4, $s3
    addu       $t4, $t4, $s3
    ldl        $s3, 0x0($t4)
    ldl        $s7, 0x0($s4)
    ldr        $s3, 0x7($t4)
    ldr        $s7, 0x7($s4)
    andi       $s2, $s2, 0x7
    dsllv      $s3, $s3, $s2
    dsllv      $s7, $s7, $s2
    addu       $s2, $s2, $s1
    subu       $t2, $s6, $s1
  .L80074E98:
    dsrlv      $s5, $s3, $t2
    dsrlv      $t8, $s7, $t2
    sub        $t8, $t8, $s5
    mult       $t8, $t9
    sll        $s5, $s5, 8
    dsllv      $s7, $s7, $s1
    mflo       $t8
    srl        $t8, $t8, 2
    addu       $s5, $s5, $t8
    addu       $s0, $s0, $s5
    dsllv      $s3, $s3, $s1
  .L80074EC4:
    addiu      $t6, $t6, -0x1
    sh         $s0, 0x0($t1)
    bnez       $t6, .L80074C9C
     addiu     $t1, $t1, 0x2
    j          func_80074D4C
     nop
  .L80074EDC:
    lui        $t0, %hi(D_800A7988)
    lw         $t0, %lo(D_800A7988)($t0)
    lui        $a2, %hi(D_800A798C)
    addiu      $a2, $a2, %lo(D_800A798C)
    lui        $a3, %hi(D_800A7980)
    lw         $a3, %lo(D_800A7980)($a3)
    lw         $s0, 0x70($sp)
    sll        $s0, $s0, 4
    addu       $s5, $s0, $t0
    addiu      $s2, $zero, 0x400
    sll        $s3, $s2, 1
    addu       $s4, $s2, $s3
    li.s       $at, 0.000030517578 /* (1.0 / 32768.0) */
    mtc1       $at, $f21
    addiu      $t3, $zero, 0xFFC
    addiu      $s6, $zero, 0x1000
    lui        $t5, %hi(D_800A8754)
    addiu      $t5, $t5, %lo(D_800A8754)
    lui        $t8, %hi(D_800A7BCC)
    addiu      $t8, $t8, %lo(D_800A7BCC)
  .L80074F2C:
    lbu        $t7, 0x1($t0)
    sll        $t2, $t7, 6
    lbu        $t7, 0x2($t0)
    addu       $a0, $t2, $a3
    sll        $t1, $t7, 1
    addu       $t1, $t1, $t7
    sll        $t1, $t1, 1
    addu       $s7, $t8, $t1
    addu       $t1, $a2, $t1
    lhu        $t2, 0x0($t1)
    srl        $t2, $t2, 4
    and        $t7, $t2, $s4
    sll        $t2, $t2, 2
    beq        $t7, $s4, .L80074F9C
     and       $t4, $t2, $t3
    beq        $t7, $s3, .L80074FB8
     addu      $t6, $t4, $t5
    beq        $t7, $s2, .L80074F88
     subu      $t4, $s6, $t4
    lwc1       $f1, 0x0($t6)
    addu       $t6, $t4, $t5
    j          func_80074FD0
     lwc1      $f0, 0x0($t6)
  .L80074F88:
    lwc1       $f0, 0x0($t6)
    addu       $t6, $t4, $t5
    lwc1       $f1, 0x0($t6)
    j          func_80074FD0
     sub.s     $f0, $f30, $f0
  .L80074F9C:
    addu       $t6, $t4, $t5
    lwc1       $f0, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f1, 0x0($t6)
    j          func_80074FD0
     sub.s     $f1, $f30, $f1
  .L80074FB8:
    lwc1       $f1, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f0, 0x0($t6)
    sub.s      $f1, $f30, $f1
    sub.s      $f0, $f30, $f0
  alabel func_80074FD0
    lhu        $t2, 0x4($t1)
    srl        $t2, $t2, 4
    and        $t7, $t2, $s4
    sll        $t2, $t2, 2
    beq        $t7, $s4, .L8007501C
     and       $t4, $t2, $t3
    beq        $t7, $s3, .L80075038
     addu      $t6, $t4, $t5
    beq        $t7, $s2, .L80075008
     subu      $t4, $s6, $t4
    lwc1       $f5, 0x0($t6)
    addu       $t6, $t4, $t5
    j          func_80075050
     lwc1      $f4, 0x0($t6)
  .L80075008:
    lwc1       $f4, 0x0($t6)
    addu       $t6, $t4, $t5
    lwc1       $f5, 0x0($t6)
    j          func_80075050
     sub.s     $f4, $f30, $f4
  .L8007501C:
    addu       $t6, $t4, $t5
    lwc1       $f4, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f5, 0x0($t6)
    j          func_80075050
     sub.s     $f5, $f30, $f5
  .L80075038:
    lwc1       $f5, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f4, 0x0($t6)
    sub.s      $f5, $f30, $f5
    sub.s      $f4, $f30, $f4
  alabel func_80075050
    mul.s      $f6, $f1, $f5
    lhu        $t2, 0x2($t1)
    mul.s      $f7, $f0, $f5
    lw         $s0, 0x4($t0)
    srl        $t2, $t2, 4
    mul.s      $f8, $f1, $f4
    lw         $s1, 0x8($t0)
    and        $t7, $t2, $s4
    sw         $s0, 0x30($a0)
    mul.s      $f9, $f0, $f4
    lw         $s0, 0xC($t0)
    sll        $t2, $t2, 2
    beq        $t7, $s4, .L800750BC
     and       $t4, $t2, $t3
    beq        $t7, $s3, .L800750D8
     addu      $t6, $t4, $t5
    beq        $t7, $s2, .L800750A8
     subu      $t4, $s6, $t4
    lwc1       $f3, 0x0($t6)
    addu       $t6, $t4, $t5
    j          func_800750F0
     lwc1      $f2, 0x0($t6)
  .L800750A8:
    lwc1       $f2, 0x0($t6)
    addu       $t6, $t4, $t5
    lwc1       $f3, 0x0($t6)
    j          func_800750F0
     sub.s     $f2, $f30, $f2
  .L800750BC:
    addu       $t6, $t4, $t5
    lwc1       $f2, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f3, 0x0($t6)
    j          func_800750F0
     sub.s     $f3, $f30, $f3
  .L800750D8:
    lwc1       $f3, 0x0($t6)
    subu       $t4, $s6, $t4
    addu       $t6, $t4, $t5
    lwc1       $f2, 0x0($t6)
    sub.s      $f3, $f30, $f3
    sub.s      $f2, $f30, $f2
  alabel func_800750F0
    mul.s      $f12, $f2, $f4
    sw         $s0, 0x38($a0)
    addiu      $t1, $t1, 0x6
    lhu        $t7, 0x0($s7)
    mul.s      $f13, $f2, $f5
    bnez       $t7, .L80075184
     sw        $s1, 0x34($a0)
    swc1       $f12, 0x0($a0)
    sub.s      $f14, $f30, $f3
    swc1       $f13, 0x4($a0)
  .L80075118:
    mul.s      $f15, $f8, $f3
    swc1       $f14, 0x8($a0)
    sub.s      $f15, $f15, $f7
    lhu        $t7, 0x2($s7)
    mul.s      $f16, $f6, $f3
    bnez       $t7, .L800751C8
     nop
    swc1       $f15, 0x10($a0)
    add.s      $f16, $f16, $f9
    mul.s      $f17, $f1, $f2
    swc1       $f16, 0x14($a0)
  .L80075144:
    mul.s      $f18, $f9, $f3
    swc1       $f17, 0x18($a0)
    add.s      $f18, $f18, $f6
    lhu        $t7, 0x4($s7)
    mul.s      $f19, $f7, $f3
    bnez       $t7, .L80075210
     nop
    swc1       $f18, 0x20($a0)
    sub.s      $f19, $f19, $f8
    mul.s      $f20, $f0, $f2
    swc1       $f19, 0x24($a0)
    addiu      $t0, $t0, 0x10
    bne        $t0, $s5, .L80074F2C
     swc1      $f20, 0x28($a0)
    j          func_8007524C
     nop
  .L80075184:
    mtc1       $t7, $f10
    nop
    cvt.s.w    $f10, $f10
    mul.s      $f10, $f10, $f21
    sub.s      $f14, $f30, $f3
    mul.s      $f12, $f12, $f10
    nop
    swc1       $f12, 0x0($a0)
    mul.s      $f13, $f13, $f10
    lhu        $t7, 0x2($s7)
    mul.s      $f14, $f14, $f10
    beqz       $t7, .L80075118
     swc1      $f13, 0x4($a0)
    mul.s      $f15, $f8, $f3
    swc1       $f14, 0x8($a0)
    sub.s      $f15, $f15, $f7
    mul.s      $f16, $f6, $f3
  .L800751C8:
    mtc1       $t7, $f10
    nop
    cvt.s.w    $f10, $f10
    mul.s      $f10, $f10, $f21
    add.s      $f16, $f16, $f9
    mul.s      $f15, $f15, $f10
    nop
    mul.s      $f17, $f1, $f2
    swc1       $f15, 0x10($a0)
    mul.s      $f16, $f16, $f10
    lhu        $t7, 0x4($s7)
    mul.s      $f17, $f17, $f10
    beqz       $t7, .L80075144
     swc1      $f16, 0x14($a0)
    mul.s      $f18, $f9, $f3
    swc1       $f17, 0x18($a0)
    add.s      $f18, $f18, $f6
    mul.s      $f19, $f7, $f3
  .L80075210:
    mtc1       $t7, $f10
    nop
    cvt.s.w    $f10, $f10
    mul.s      $f10, $f10, $f21
    sub.s      $f19, $f19, $f8
    mul.s      $f18, $f18, $f10
    nop
    mul.s      $f20, $f0, $f2
    swc1       $f18, 0x20($a0)
    mul.s      $f19, $f19, $f10
    swc1       $f19, 0x24($a0)
    mul.s      $f20, $f20, $f10
    addiu      $t0, $t0, 0x10
    bne        $t0, $s5, .L80074F2C
     swc1      $f20, 0x28($a0)
  alabel func_8007524C
    lui        $v0, %hi(D_800A7E0C)
    addiu      $v0, $v0, %lo(D_800A7E0C)
    or         $v1, $a1, $zero
    or         $s1, $v0, $zero
    lwc1       $f0, 0x0($v1)
    lui        $s5, %hi(D_800A7980)
    lw         $s5, %lo(D_800A7980)($s5)
    lwc1       $f1, 0x4($v1)
    lui        $s0, %hi(D_800A7988)
    lw         $s0, %lo(D_800A7988)($s0)
    lwc1       $f2, 0x8($v1)
    lwc1       $f3, 0x10($v1)
    li.s       $at, 0.0009765625 /* (1.0 / 1024.0) */
    mtc1       $at, $f12
    lwc1       $f4, 0x14($v1)
    mul.s      $f22, $f22, $f12
    lwc1       $f5, 0x18($v1)
    lwc1       $f6, 0x20($v1)
    mul.s      $f23, $f23, $f12
    lwc1       $f7, 0x24($v1)
    lwc1       $f8, 0x28($v1)
    mul.s      $f24, $f24, $f12
    lwc1       $f9, 0x30($v1)
    mul.s      $f12, $f22, $f0
    lwc1       $f10, 0x34($v1)
    mul.s      $f15, $f23, $f3
    lwc1       $f11, 0x38($v1)
    add.s      $f12, $f12, $f15
    lbu        $s6, 0x1($s0)
    mul.s      $f15, $f24, $f6
    lw         $s2, 0x70($sp)
    sll        $s4, $s2, 6
    addu       $s4, $s4, $s5
    lui        $s7, %hi(D_800A7984)
    lw         $s7, %lo(D_800A7984)($s7)
    sw         $s4, 0x0($s7)
    sll        $s4, $s6, 6
    addu       $s7, $s4, $s5
    mul.s      $f13, $f22, $f1
    add.s      $f12, $f12, $f15
    mul.s      $f15, $f23, $f4
    add.s      $f9, $f9, $f12
    add.s      $f13, $f13, $f15
    mul.s      $f15, $f24, $f7
    lwc1       $f21, 0x0($s7)
    mul.s      $f14, $f22, $f2
    add.s      $f13, $f13, $f15
    mul.s      $f15, $f23, $f5
    lui        $t1, %hi(D_800A7BCC)
    addiu      $t1, $t1, %lo(D_800A7BCC)
    add.s      $f14, $f14, $f15
    mul.s      $f15, $f24, $f8
    add.s      $f10, $f10, $f13
    add.s      $f14, $f14, $f15
    j          func_800753BC
     add.s     $f11, $f11, $f14
  alabel func_8007532C
    lbu        $s3, 0x1($s0)
    sll        $s4, $s3, 6
    addu       $s7, $s4, $s5
    addu       $s1, $v0, $s3
    lbu        $s4, 0x0($s0)
    beq        $s4, $s6, .L80075388
     or        $s6, $s3, $zero
    lwc1       $f21, 0x0($s7)
    sll        $v1, $s4, 6
    addu       $v1, $v1, $s5
    lwc1       $f0, 0x0($v1)
    lwc1       $f1, 0x4($v1)
    lwc1       $f2, 0x8($v1)
    lwc1       $f3, 0x10($v1)
    lwc1       $f4, 0x14($v1)
    lwc1       $f5, 0x18($v1)
    lwc1       $f6, 0x20($v1)
    lwc1       $f7, 0x24($v1)
    lwc1       $f8, 0x28($v1)
    lwc1       $f9, 0x30($v1)
    lwc1       $f10, 0x34($v1)
    j          func_800753BC
     lwc1      $f11, 0x38($v1)
  .L80075388:
    mov.s      $f0, $f12
    mov.s      $f1, $f13
    mov.s      $f2, $f14
    mov.s      $f3, $f15
    mov.s      $f4, $f16
    mov.s      $f5, $f17
    mov.s      $f6, $f18
    mov.s      $f7, $f19
    mov.s      $f8, $f20
    mov.s      $f9, $f21
    mov.s      $f10, $f22
    lwc1       $f21, 0x0($s7)
    mov.s      $f11, $f23
  alabel func_800753BC
    lwc1       $f22, 0x4($s7)
    mul.s      $f24, $f21, $f0
    lwc1       $f30, 0x8($s7)
    mul.s      $f26, $f21, $f1
    lwc1       $f28, 0x10($s7)
    mul.s      $f25, $f22, $f3
    lwc1       $f29, 0x14($s7)
    mul.s      $f27, $f22, $f4
    add.s      $f12, $f24, $f25
    mul.s      $f24, $f30, $f6
    nop
    mul.s      $f25, $f30, $f7
    add.s      $f12, $f12, $f24
    mul.s      $f24, $f21, $f2
    swc1       $f12, 0x0($s7)
    add.s      $f13, $f25, $f26
    mul.s      $f25, $f22, $f5
    add.s      $f13, $f13, $f27
    add.s      $f14, $f24, $f25
    mul.s      $f25, $f30, $f8
    add.s      $f14, $f14, $f25
    swc1       $f13, 0x4($s7)
    mul.s      $f24, $f28, $f0
    lwc1       $f30, 0x18($s7)
    mul.s      $f26, $f28, $f1
    lwc1       $f21, 0x20($s7)
    mul.s      $f25, $f29, $f3
    lwc1       $f22, 0x24($s7)
    mul.s      $f27, $f29, $f4
    add.s      $f15, $f24, $f25
    mul.s      $f24, $f30, $f6
    swc1       $f14, 0x8($s7)
    mul.s      $f25, $f30, $f7
    add.s      $f15, $f15, $f24
    mul.s      $f24, $f28, $f2
    swc1       $f15, 0x10($s7)
    add.s      $f16, $f25, $f26
    mul.s      $f25, $f29, $f5
    add.s      $f16, $f16, $f27
    add.s      $f17, $f24, $f25
    mul.s      $f25, $f30, $f8
    add.s      $f17, $f17, $f25
    swc1       $f16, 0x14($s7)
    mul.s      $f24, $f21, $f0
    lwc1       $f30, 0x28($s7)
    mul.s      $f26, $f21, $f1
    lwc1       $f28, 0x30($s7)
    mul.s      $f25, $f22, $f3
    lwc1       $f29, 0x34($s7)
    mul.s      $f27, $f22, $f4
    add.s      $f18, $f24, $f25
    mul.s      $f24, $f30, $f6
    swc1       $f17, 0x18($s7)
    mul.s      $f25, $f30, $f7
    add.s      $f18, $f18, $f24
    mul.s      $f24, $f21, $f2
    add.s      $f19, $f25, $f26
    mul.s      $f25, $f22, $f5
    add.s      $f19, $f19, $f27
    swc1       $f18, 0x20($s7)
    add.s      $f20, $f24, $f25
    mul.s      $f25, $f30, $f8
    add.s      $f20, $f20, $f25
    swc1       $f19, 0x24($s7)
    mul.s      $f24, $f28, $f0
    lwc1       $f30, 0x38($s7)
    mul.s      $f26, $f28, $f1
    add.s      $f24, $f24, $f9
    mul.s      $f25, $f29, $f3
    add.s      $f26, $f26, $f10
    mul.s      $f27, $f29, $f4
    add.s      $f21, $f24, $f25
    add.s      $f22, $f26, $f27
    mul.s      $f24, $f30, $f6
    swc1       $f20, 0x28($s7)
    mul.s      $f25, $f30, $f7
    add.s      $f21, $f21, $f24
    mul.s      $f24, $f28, $f2
    swc1       $f21, 0x30($s7)
    add.s      $f22, $f22, $f25
    add.s      $f24, $f24, $f11
    mul.s      $f25, $f29, $f5
    swc1       $f22, 0x34($s7)
    add.s      $f23, $f24, $f25
    mul.s      $f25, $f30, $f8
    add.s      $f23, $f23, $f25
    swc1       $f23, 0x38($s7)
    lbu        $s3, 0x0($s1)
    addiu      $s2, $s2, -0x1
    bnez       $s3, .L8007559C
     sll       $s3, $s3, 2
  alabel func_80075528
    bnez       $s2, func_8007532C
     addiu     $s0, $s0, 0x10
    lw         $ra, 0x0($sp)
    lwc1       $f20, 0x4($sp)
    lwc1       $f21, 0x8($sp)
    lwc1       $f22, 0xC($sp)
    lwc1       $f23, 0x10($sp)
    lwc1       $f24, 0x14($sp)
    lwc1       $f25, 0x18($sp)
    lwc1       $f26, 0x1C($sp)
    lwc1       $f27, 0x20($sp)
    lwc1       $f28, 0x24($sp)
    lwc1       $f29, 0x28($sp)
    lwc1       $f30, 0x2C($sp)
    lwc1       $f31, 0x30($sp)
    addiu      $sp, $sp, 0x38
    lw         $ra, 0x0($sp)
    lw         $s0, 0x4($sp)
    lw         $s1, 0x8($sp)
    lw         $s2, 0xC($sp)
    lw         $s3, 0x10($sp)
    lw         $s4, 0x14($sp)
    lw         $s5, 0x18($sp)
    lw         $s6, 0x1C($sp)
    lw         $s7, 0x20($sp)
    lui        $v0, %hi(D_800A7E0C)
    addiu      $sp, $sp, 0x28
    jr         $ra
     addiu     $v0, $v0, %lo(D_800A7E0C)
  .L8007559C:
    lui        $s1, %hi(gen_anim_data_jump_table)
    addiu      $s1, $s1, %lo(gen_anim_data_jump_table)
    addu       $s1, $s1, $s3
    lw         $s1, 0x0($s1)
    jr         $s1
     nop
  .gen_anim_data_jump_target:
    li.s       $at, 30.0
    lwc1       $f22, 0x34($s7)
    mtc1       $at, $f0
    nop
    add.s      $f22, $f22, $f0
    j          func_80075528
     swc1      $f22, 0x34($s7)
endlabel func_80074B50
