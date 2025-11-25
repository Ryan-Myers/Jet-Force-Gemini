.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

.section .data

dlabel gIntDisFlag
.byte 0x00

dlabel rngSeed
.word 0x5141564D /* 'QAVM' */

/* Unused in this game, but it's used in DKR */
dlabel gPrevRNGSeed
.word 0x5141564D /* 'QAVM' */

.section .rodata

dlabel D_800AD190
.float -0.56
dlabel D_800AD194
.float 1.44
dlabel D_800AD198
.float -1.44
dlabel D_800AD19C
.float 0.56
dlabel D_800AD1A0
.float 1.12
dlabel D_800AD1A4
.float -2.44
dlabel D_800AD1A8
.float 1.88
dlabel D_800AD1AC
.float -0.56
dlabel D_800AD1B0
.float -0.56
dlabel D_800AD1B4
.float 0.56

.section .text, "ax"

glabel disableInterrupts
lui        $t0, %hi(gIntDisFlag)
lb         $t0, %lo(gIntDisFlag)($t0)
beqz       $t0, .L80048E24
mfc0       $t0, $12
addiu      $at, $zero, -0x2
and        $t1, $t0, $at
mtc0       $t1, $12
andi       $v0, $t0, 0x1
nop
.L80048E24:
jr         $ra
nop
endlabel disableInterrupts

glabel enableInterrupts
lui        $t0, %hi(gIntDisFlag)
lb         $t0, %lo(gIntDisFlag)($t0)
beqz       $t0, .L80048E4C
mfc0       $t0, $12
or         $t0, $t0, $a0
mtc0       $t0, $12
nop
nop
.L80048E4C:
jr         $ra
nop
endlabel enableInterrupts

glabel setIntDisFlag
lui        $at, %hi(gIntDisFlag)
jr         $ra
sb         $a0, %lo(gIntDisFlag)($at)
endlabel setIntDisFlag

glabel mathMtxXFMF
mtc1       $a1, $f12
lwc1       $f4, 0x0($a0)
mtc1       $a2, $f14
lwc1       $f8, 0x10($a0)
mul.s      $f6, $f4, $f12
mtc1       $a3, $f16
lwc1       $f4, 0x20($a0)
mul.s      $f10, $f8, $f14
lw         $t6, 0x10($sp)
mul.s      $f8, $f4, $f16
add.s      $f18, $f6, $f10
lwc1       $f10, 0x30($a0)
add.s      $f6, $f18, $f8
add.s      $f4, $f10, $f6
swc1       $f4, 0x0($t6)
lwc1       $f18, 0x4($a0)
lwc1       $f10, 0x14($a0)
lw         $t7, 0x14($sp)
mul.s      $f8, $f18, $f12
lwc1       $f18, 0x24($a0)
mul.s      $f6, $f10, $f14
add.s      $f4, $f8, $f6
mul.s      $f10, $f18, $f16
lwc1       $f6, 0x34($a0)
add.s      $f8, $f4, $f10
add.s      $f18, $f6, $f8
swc1       $f18, 0x0($t7)
lwc1       $f4, 0x8($a0)
lwc1       $f6, 0x18($a0)
lw         $t8, 0x18($sp)
mul.s      $f10, $f4, $f12
lwc1       $f4, 0x28($a0)
mul.s      $f8, $f6, $f14
add.s      $f18, $f10, $f8
mul.s      $f6, $f4, $f16
lwc1       $f8, 0x38($a0)
add.s      $f10, $f18, $f6
add.s      $f4, $f8, $f10
jr         $ra
swc1       $f4, 0x0($t8)
endlabel mathMtxXFMF

glabel mathMtxFastXFMF
lwc1       $f4, 0x0($a1)
lwc1       $f10, 0x0($a0)
lwc1       $f6, 0x4($a1)
lwc1       $f12, 0x10($a0)
mul.s      $f10, $f4, $f10
lwc1       $f8, 0x8($a1)
lwc1       $f14, 0x20($a0)
mul.s      $f12, $f6, $f12
add.s      $f12, $f10, $f12
mul.s      $f14, $f8, $f14
lwc1       $f10, 0x4($a0)
mul.s      $f10, $f4, $f10
add.s      $f16, $f12, $f14
lwc1       $f12, 0x14($a0)
lwc1       $f14, 0x24($a0)
mul.s      $f12, $f6, $f12
swc1       $f16, 0x0($a2)
mul.s      $f14, $f8, $f14
add.s      $f12, $f10, $f12
lwc1       $f10, 0x8($a0)
add.s      $f16, $f12, $f14
mul.s      $f10, $f4, $f10
lwc1       $f12, 0x18($a0)
lwc1       $f14, 0x28($a0)
swc1       $f16, 0x4($a2)
mul.s      $f12, $f6, $f12
add.s      $f12, $f10, $f12
mul.s      $f14, $f8, $f14
add.s      $f14, $f12, $f14
jr         $ra
swc1       $f14, 0x8($a2)
endlabel mathMtxFastXFMF

glabel mathMtxCatF
addiu      $sp, $sp, -0x8
sdc1       $f2, 0x0($sp)
ori        $t0, $zero, 0x4
.L80048F88:
lwc1       $f2, 0x0($a0)
lwc1       $f10, 0x0($a1)
lwc1       $f4, 0x4($a0)
lwc1       $f12, 0x10($a1)
mul.s      $f10, $f2, $f10
lwc1       $f6, 0x8($a0)
lwc1       $f14, 0x20($a1)
mul.s      $f12, $f4, $f12
lwc1       $f8, 0xC($a0)
lwc1       $f16, 0x30($a1)
mul.s      $f14, $f6, $f14
addiu      $a0, $a0, 0x10
addiu      $a2, $a2, 0x10
mul.s      $f16, $f8, $f16
addiu      $t0, $t0, -0x1
add.s      $f14, $f12, $f14
lwc1       $f12, 0x14($a1)
add.s      $f16, $f10, $f16
lwc1       $f10, 0x4($a1)
mul.s      $f10, $f2, $f10
add.s      $f18, $f14, $f16
lwc1       $f14, 0x24($a1)
mul.s      $f12, $f4, $f12
lwc1       $f16, 0x34($a1)
swc1       $f18, -0x10($a2)
mul.s      $f14, $f6, $f14
nop
mul.s      $f16, $f8, $f16
add.s      $f14, $f12, $f14
lwc1       $f12, 0x18($a1)
add.s      $f16, $f10, $f16
lwc1       $f10, 0x8($a1)
mul.s      $f10, $f2, $f10
add.s      $f18, $f14, $f16
lwc1       $f14, 0x28($a1)
mul.s      $f12, $f4, $f12
lwc1       $f16, 0x38($a1)
swc1       $f18, -0xC($a2)
mul.s      $f14, $f6, $f14
nop
mul.s      $f16, $f8, $f16
add.s      $f14, $f12, $f14
lwc1       $f12, 0x1C($a1)
add.s      $f16, $f10, $f16
lwc1       $f10, 0xC($a1)
mul.s      $f10, $f2, $f10
add.s      $f18, $f14, $f16
lwc1       $f14, 0x2C($a1)
mul.s      $f12, $f4, $f12
lwc1       $f16, 0x3C($a1)
swc1       $f18, -0x8($a2)
mul.s      $f14, $f6, $f14
nop
mul.s      $f16, $f8, $f16
add.s      $f14, $f12, $f14
add.s      $f16, $f10, $f16
add.s      $f18, $f14, $f16
swc1       $f18, -0x4($a2)
bnel       $t0, $zero, .L80048F88
nop
ldc1       $f2, 0x0($sp)
jr         $ra
addiu      $sp, $sp, 0x8
endlabel mathMtxCatF

glabel mathMtxF2L
li.s       $at, 65536.0
mtc1       $at, $f12
ori        $t0, $zero, 0x4
.L80049090:
lwc1       $f4, 0x0($a0)
lwc1       $f6, 0x4($a0)
lwc1       $f8, 0x8($a0)
mul.s      $f4, $f4, $f12
lwc1       $f10, 0xC($a0)
addiu      $a0, $a0, 0x10
mul.s      $f6, $f6, $f12
addiu      $a1, $a1, 0x8
addiu      $t0, $t0, -0x1
mul.s      $f8, $f8, $f12
nop
mul.s      $f10, $f10, $f12
cvt.w.s    $f4, $f4
cvt.w.s    $f6, $f6
mfc1       $t1, $f4
cvt.w.s    $f8, $f8
mfc1       $t2, $f6
sh         $t1, 0x18($a1)
cvt.w.s    $f10, $f10
mfc1       $t3, $f8
sh         $t2, 0x1A($a1)
srl        $t1, $t1, 16
mfc1       $t4, $f10
sh         $t3, 0x1C($a1)
srl        $t2, $t2, 16
sh         $t4, 0x1E($a1)
srl        $t3, $t3, 16
srl        $t4, $t4, 16
sh         $t1, -0x8($a1)
sh         $t2, -0x6($a1)
sh         $t3, -0x4($a1)
sh         $t4, -0x2($a1)
bnel       $t0, $zero, .L80049090
nop
jr         $ra
nop
endlabel mathMtxF2L

glabel mathSeed
lui        $at, %hi(rngSeed)
jr         $ra
sw         $a0, %lo(rngSeed)($at)
endlabel mathSeed

glabel mathRnd
lui        $t0, %hi(rngSeed)
lw         $t0, %lo(rngSeed)($t0)
lui        $at, %hi(rngSeed)
sub        $a1, $a1, $a0
dsll32     $t1, $t0, 31
dsll       $t2, $t0, 31
dsrl       $t1, $t1, 31
dsrl32     $t2, $t2, 0
dsll32     $t3, $t0, 12
or         $t1, $t1, $t2
dsrl32     $t3, $t3, 0
xor        $t1, $t1, $t3
dsrl       $t3, $t1, 20
andi       $t3, $t3, 0xFFF
xor        $t0, $t3, $t1
sw         $t0, %lo(rngSeed)($at)
addi       $a1, $a1, 0x1
subu       $t0, $t0, $a0
divu       $zero, $t0, $a1
mflo       $t0
mfhi       $v0
add        $v0, $v0, $a0
bnez       $a1, .L80049190
nop
break      7
.L80049190:
jr         $ra
nop
endlabel mathRnd

glabel fastShortReflection
lh         $t0, 0x0($a0)
lh         $t3, 0x0($a1)
lh         $t1, 0x2($a0)
lh         $t4, 0x2($a1)
mult       $t0, $t3
lh         $t2, 0x4($a0)
lh         $t5, 0x4($a1)
mflo       $t6
nop
nop
mult       $t1, $t4
mflo       $t7
add        $t6, $t6, $t7
nop
mult       $t2, $t5
mflo       $t8
add        $t6, $t6, $t8
sra        $t6, $t6, 12
mult       $t6, $t3
mflo       $t3
sra        $t3, $t3, 13
sub        $t3, $t3, $t0
mult       $t6, $t4
sh         $t3, 0x6($a0)
mflo       $t4
sra        $t4, $t4, 13
sub        $t4, $t4, $t1
mult       $t6, $t5
sh         $t4, 0x8($a0)
mflo       $t5
sra        $t5, $t5, 13
sub        $t5, $t5, $t0
jr         $ra
sh         $t5, 0xA($a0)
endlabel fastShortReflection

glabel mathSquashY
mtc1       $a1, $f18
lwc1       $f16, 0x10($a0)
mul.s      $f16, $f16, $f18
swc1       $f16, 0x10($a0)
lwc1       $f16, 0x14($a0)
mul.s      $f16, $f16, $f18
swc1       $f16, 0x14($a0)
lwc1       $f16, 0x18($a0)
mul.s      $f16, $f16, $f18
jr         $ra
swc1       $f16, 0x18($a0)
endlabel mathSquashY

glabel mathTransY
mtc1       $a1, $f16
lwc1       $f0, 0x10($a0)
lwc1       $f4, 0x14($a0)
lwc1       $f8, 0x18($a0)
mul.s      $f0, $f0, $f16
lwc1       $f2, 0x30($a0)
lwc1       $f6, 0x34($a0)
mul.s      $f4, $f4, $f16
lwc1       $f10, 0x38($a0)
mul.s      $f8, $f8, $f16
add.s      $f0, $f0, $f2
add.s      $f4, $f4, $f6
swc1       $f0, 0x30($a0)
add.s      $f8, $f8, $f10
swc1       $f4, 0x34($a0)
jr         $ra
swc1       $f8, 0x38($a0)
endlabel mathTransY

glabel mathRpyXyzMtx
addiu      $sp, $sp, -0x8
li.s       $at, 0.0000152587890625 /* (1.0f / 0x10000) */
sd         $ra, 0x0($sp)
or         $a3, $a0, $zero
mtc1       $at, $f18
jal        mathSinInterp
lh         $a0, 0x0($a1)
mtc1       $v0, $f0
lh         $a0, 0x0($a1)
cvt.s.w    $f0, $f0
mul.s      $f0, $f0, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f2
lh         $a0, 0x2($a1)
cvt.s.w    $f2, $f2
mul.s      $f2, $f2, $f18
jal        mathSinInterp
nop
mtc1       $v0, $f4
lh         $a0, 0x2($a1)
cvt.s.w    $f4, $f4
mul.s      $f4, $f4, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f6
lh         $a0, 0x4($a1)
cvt.s.w    $f6, $f6
mul.s      $f6, $f6, $f18
jal        mathSinInterp
nop
mtc1       $v0, $f8
lh         $a0, 0x4($a1)
cvt.s.w    $f8, $f8
mul.s      $f8, $f8, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f10
sw         $zero, 0xC($a3)
swc1       $f4, 0x18($a3)
cvt.s.w    $f10, $f10
sw         $zero, 0x1C($a3)
sw         $zero, 0x2C($a3)
li.s       $at, 1.0
mul.s      $f10, $f10, $f18
nop
mul.s      $f16, $f4, $f8
nop
mul.s      $f16, $f16, $f0
nop
mul.s      $f18, $f2, $f10
sub.s      $f16, $f18, $f16
swc1       $f16, 0x0($a3)
mul.s      $f16, $f4, $f10
nop
mul.s      $f16, $f16, $f0
nop
mul.s      $f18, $f2, $f8
add.s      $f16, $f16, $f18
swc1       $f16, 0x4($a3)
mul.s      $f16, $f0, $f6
neg.s      $f16, $f16
swc1       $f16, 0x8($a3)
mul.s      $f16, $f6, $f8
neg.s      $f16, $f16
swc1       $f16, 0x10($a3)
mul.s      $f16, $f6, $f10
swc1       $f16, 0x14($a3)
mul.s      $f16, $f4, $f8
nop
mul.s      $f16, $f16, $f2
nop
mul.s      $f18, $f0, $f10
add.s      $f16, $f16, $f18
swc1       $f16, 0x20($a3)
mul.s      $f16, $f4, $f10
nop
mul.s      $f16, $f16, $f2
nop
mul.s      $f18, $f0, $f8
sub.s      $f16, $f18, $f16
lwc1       $f18, 0x0($a3)
swc1       $f16, 0x24($a3)
mul.s      $f16, $f2, $f6
swc1       $f16, 0x28($a3)
lwc1       $f0, 0xC($a1)
lwc1       $f2, 0x10($a1)
lwc1       $f16, 0x10($a3)
mul.s      $f18, $f18, $f0
lwc1       $f4, 0x14($a1)
mul.s      $f16, $f16, $f2
add.s      $f16, $f16, $f18
lwc1       $f18, 0x20($a3)
mul.s      $f18, $f18, $f4
add.s      $f16, $f16, $f18
lwc1       $f18, 0x4($a3)
swc1       $f16, 0x30($a3)
mul.s      $f18, $f18, $f0
lwc1       $f16, 0x14($a3)
mul.s      $f16, $f16, $f2
add.s      $f16, $f16, $f18
lwc1       $f18, 0x24($a3)
mul.s      $f18, $f18, $f4
add.s      $f16, $f16, $f18
lwc1       $f18, 0x8($a3)
swc1       $f16, 0x34($a3)
mul.s      $f18, $f18, $f0
lwc1       $f16, 0x18($a3)
mul.s      $f16, $f16, $f2
add.s      $f16, $f16, $f18
lwc1       $f18, 0x28($a3)
mul.s      $f18, $f18, $f4
add.s      $f16, $f16, $f18
swc1       $f16, 0x38($a3)
mtc1       $at, $f16
nop
swc1       $f16, 0x3C($a3)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel mathRpyXyzMtx

glabel func_80049474
addiu      $sp, $sp, -0x8
li.s       $at, 0.0000152587890625 /* (1.0f / 0x10000) */
sd         $ra, 0x0($sp)
or         $a3, $a0, $zero
mtc1       $at, $f18
jal        mathSinInterp
lh         $a0, 0x0($a1)
mtc1       $v0, $f0
lh         $a0, 0x0($a1)
cvt.s.w    $f0, $f0
mul.s      $f0, $f0, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f2
lh         $a0, 0x2($a1)
cvt.s.w    $f2, $f2
mul.s      $f2, $f2, $f18
jal        mathSinInterp
nop
mtc1       $v0, $f4
lh         $a0, 0x2($a1)
cvt.s.w    $f4, $f4
mul.s      $f4, $f4, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f6
lh         $a0, 0x4($a1)
cvt.s.w    $f6, $f6
mul.s      $f6, $f6, $f18
jal        mathSinInterp
nop
mtc1       $v0, $f8
lh         $a0, 0x4($a1)
cvt.s.w    $f8, $f8
mul.s      $f8, $f8, $f18
jal        mathCosInterp
nop
mtc1       $v0, $f10
li.s       $at, 1.0
sw         $zero, 0xC($a3)
cvt.s.w    $f10, $f10
sw         $zero, 0x1C($a3)
sw         $zero, 0x2C($a3)
mul.s      $f10, $f10, $f18
mtc1       $at, $f18
mul.s      $f12, $f0, $f6
swc1       $f18, 0x3C($a3)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
mul.s      $f14, $f2, $f10
nop
mul.s      $f16, $f0, $f8
jr         $ra
nop
endlabel func_80049474

glabel mathRSMtx
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
jal        Sinf
nop
jal        Cosf
mov.s      $f2, $f0
mtc1       $a1, $f7
mov.s      $f3, $f0
mtc1       $a2, $f8
mul.s      $f2, $f2, $f7
li.s       $at, 1.0
mtc1       $at, $f6
sw         $zero, 0x8($a3)
sw         $zero, 0xC($a3)
sw         $zero, 0x18($a3)
sw         $zero, 0x1C($a3)
mul.s      $f3, $f3, $f7
neg.s      $f4, $f2
sw         $zero, 0x20($a3)
mul.s      $f2, $f2, $f8
swc1       $f4, 0x10($a3)
sw         $zero, 0x24($a3)
sw         $zero, 0x2C($a3)
sw         $zero, 0x30($a3)
sw         $zero, 0x34($a3)
sw         $zero, 0x38($a3)
mul.s      $f5, $f3, $f8
swc1       $f3, 0x0($a3)
swc1       $f2, 0x4($a3)
swc1       $f7, 0x28($a3)
swc1       $f5, 0x14($a3)
swc1       $f6, 0x3C($a3)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel mathRSMtx

glabel mathOneFloatRPY
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
or         $a2, $a0, $zero
lwc1       $f4, 0x0($a1)
lwc1       $f6, 0x4($a1)
lwc1       $f8, 0x8($a1)
jal        Sinf
lh         $a0, 0x4($a2)
mul.s      $f10, $f4, $f0
nop
mul.s      $f12, $f6, $f0
jal        Cosf
nop
mul.s      $f4, $f4, $f0
lh         $a0, 0x2($a2)
mul.s      $f6, $f6, $f0
sub.s      $f4, $f4, $f12
jal        Sinf
add.s      $f6, $f6, $f10
mul.s      $f10, $f6, $f0
nop
mul.s      $f12, $f8, $f0
jal        Cosf
nop
mul.s      $f6, $f6, $f0
lh         $a0, 0x0($a2)
mul.s      $f8, $f8, $f0
sub.s      $f6, $f6, $f12
jal        Sinf
add.s      $f8, $f8, $f10
mul.s      $f10, $f4, $f0
nop
mul.s      $f12, $f8, $f0
jal        Cosf
nop
mul.s      $f4, $f4, $f0
swc1       $f6, 0x4($a1)
mul.s      $f8, $f8, $f0
add.s      $f4, $f4, $f12
sub.s      $f8, $f8, $f10
swc1       $f4, 0x0($a1)
swc1       $f8, 0x8($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel mathOneFloatRPY

glabel mathOneFloatYPR
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
or         $a2, $a0, $zero
lwc1       $f4, 0x0($a1)
lwc1       $f6, 0x4($a1)
lwc1       $f8, 0x8($a1)
jal        Sinf
lh         $a0, 0x0($a2)
mul.s      $f10, $f4, $f0
nop
mul.s      $f12, $f8, $f0
jal        Cosf
nop
mul.s      $f4, $f4, $f0
lh         $a0, 0x2($a2)
mul.s      $f8, $f8, $f0
add.s      $f4, $f4, $f12
jal        Sinf
sub.s      $f8, $f8, $f10
mul.s      $f10, $f6, $f0
nop
mul.s      $f12, $f8, $f0
jal        Cosf
nop
mul.s      $f6, $f6, $f0
lh         $a0, 0x4($a2)
mul.s      $f8, $f8, $f0
sub.s      $f6, $f6, $f12
jal        Sinf
add.s      $f8, $f8, $f10
mul.s      $f10, $f4, $f0
nop
mul.s      $f12, $f6, $f0
jal        Cosf
nop
mul.s      $f4, $f4, $f0
swc1       $f8, 0x8($a1)
mul.s      $f6, $f6, $f0
sub.s      $f4, $f4, $f12
add.s      $f6, $f6, $f10
swc1       $f4, 0x0($a1)
swc1       $f6, 0x4($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel mathOneFloatYPR

glabel mathOneFloatPY
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
or         $a2, $a0, $zero
lwc1       $f8, 0x8($a1)
jal        Sinf
lh         $a0, 0x2($a2)
mul.s      $f6, $f8, $f0
jal        Cosf
nop
mul.s      $f8, $f8, $f0
neg.s      $f6, $f6
jal        Sinf
lh         $a0, 0x0($a2)
mul.s      $f4, $f8, $f0
jal        Cosf
nop
mul.s      $f8, $f8, $f0
swc1       $f4, 0x0($a1)
swc1       $f6, 0x4($a1)
swc1       $f8, 0x8($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel mathOneFloatPY

glabel mathXZInTri
lw         $t6, 0x10($sp)
or         $v0, $zero, $zero
lh         $t0, 0x0($a2)
lh         $t1, 0x4($a2)
lh         $t2, 0x0($a3)
lh         $t3, 0x4($a3)
lh         $t4, 0x0($t6)
lh         $t5, 0x4($t6)
sub        $t6, $t3, $t1
sub        $t7, $a0, $t0
mult       $t6, $t7
sub        $t8, $t2, $t0
sub        $t9, $a1, $t1
ori        $a3, $zero, 0x1
mflo       $t7
nop
nop
mult       $t8, $t9
mflo       $t8
sub        $t7, $t7, $t8
bgezl      $t7, .L8004980C
sub        $t6, $t5, $t3
or         $a3, $zero, $zero
sub        $t6, $t5, $t3
.L8004980C:
sub        $t7, $a0, $t2
mult       $t6, $t7
sub        $t8, $t4, $t2
sub        $t9, $a1, $t3
ori        $a2, $zero, 0x1
mflo       $t7
nop
nop
mult       $t8, $t9
mflo       $t8
sub        $t7, $t7, $t8
bgez       $t7, .L80049844
nop
or         $a2, $zero, $zero
.L80049844:
bne        $a3, $a2, .L80049894
nop
sub        $t6, $t1, $t5
sub        $t7, $a0, $t4
mult       $t6, $t7
sub        $t8, $t0, $t4
sub        $t9, $a1, $t5
ori        $a1, $zero, 0x1
mflo       $t7
nop
nop
mult       $t8, $t9
mflo       $t8
sub        $t7, $t7, $t8
bgez       $t7, .L80049888
nop
or         $a1, $zero, $zero
.L80049888:
bne        $a2, $a1, .L80049894
nop
ori        $v0, $zero, 0x1
.L80049894:
jr         $ra
nop
endlabel mathXZInTri

glabel mathTranslateMtx
or         $t0, $a0, $zero
addiu      $t1, $t0, 0x40
.L800498A4:
addiu      $t0, $t0, 0x4
bne        $t1, $t0, .L800498A4
sw         $zero, -0x4($t0)
li.s       $at, 1.0
mtc1       $at, $f18
sw         $a1, 0x30($a0)
sw         $a2, 0x34($a0)
sw         $a3, 0x38($a0)
swc1       $f18, 0x0($a0)
swc1       $f18, 0x14($a0)
swc1       $f18, 0x28($a0)
jr         $ra
swc1       $f18, 0x3C($a0)
endlabel mathTranslateMtx

glabel mathScaleMtx
or         $t0, $a0, $zero
addiu      $t1, $t0, 0x40
.L800498E0:
addiu      $t0, $t0, 0x4
bne        $t1, $t0, .L800498E0
sw         $zero, -0x4($t0)
li.s       $at, 1.0
mtc1       $at, $f18
sw         $a1, 0x0($a0)
sw         $a2, 0x14($a0)
sw         $a3, 0x28($a0)
jr         $ra
swc1       $f18, 0x3C($a0)
endlabel mathScaleMtx

glabel mathCosInterp
addiu      $a0, $a0, 0x4000
/* Fall through */

alabel mathSinInterp
sll        $v0, $a0, 17
bgezl      $v0, .L80049920
srl        $t2, $a0, 3
xori       $a0, $a0, 0x7FFF
srl        $t2, $a0, 3
.L80049920:
lui        $v0, %hi(gSineTable)
andi       $t2, $t2, 0x7FE
addiu      $v0, $v0, %lo(gSineTable)
addu       $v0, $v0, $t2
lhu        $t2, 0x2($v0)
lhu        $v0, 0x0($v0)
andi       $t1, $a0, 0xF
sll        $a0, $a0, 16
subu       $t2, $t2, $v0
multu      $t2, $t1
sll        $v0, $v0, 1
mflo       $t2
srl        $t2, $t2, 3
bgez       $a0, .L80049960
addu       $v0, $v0, $t2
negu       $v0, $v0
.L80049960:
jr         $ra
nop
endlabel mathCosInterp
.size mathSinInterp, . - mathSinInterp

glabel mathCos
addiu      $a0, $a0, 0x4000
/* Fall through */

alabel mathSin
sll        $v0, $a0, 17
bgezl      $v0, .L80049980
srl        $t2, $a0, 3
xori       $a0, $a0, 0x7FFF
srl        $t2, $a0, 3
.L80049980:
lui        $v0, %hi(gSineTable)
andi       $t2, $t2, 0x7FE
addiu      $v0, $v0, %lo(gSineTable)
addu       $v0, $v0, $t2
lhu        $v0, 0x0($v0)
sll        $a0, $a0, 16
bgez       $a0, .L800499A4
sll        $v0, $v0, 1
negu       $v0, $v0
.L800499A4:
jr         $ra
nop
endlabel mathCos
.size mathSin, . - mathSin

glabel mathBreakPoint
break      7
endlabel mathBreakPoint

glabel pointListRPY
addiu      $sp, $sp, -0x8
blez       $a0, .L80049A88
sd         $ra, 0x0($sp)
or         $t9, $a0, $zero
mtc1       $zero, $f0
jal        Sinf
lh         $a0, 0x0($a1)
jal        Cosf
mov.s      $f4, $f0
mov.s      $f5, $f0
jal        Sinf
lh         $a0, 0x2($a1)
jal        Cosf
mov.s      $f6, $f0
mov.s      $f7, $f0
jal        Sinf
lh         $a0, 0x4($a1)
jal        Cosf
mov.s      $f8, $f0
mov.s      $f9, $f0
.L80049A00:
lwc1       $f10, 0x0($a2)
lwc1       $f11, 0x4($a2)
lwc1       $f12, 0x8($a2)
mul.s      $f14, $f10, $f9
addiu      $t9, $t9, -0x1
addiu      $a2, $a2, 0xC
mul.s      $f15, $f11, $f8
addiu      $a3, $a3, 0xC
mul.s      $f16, $f11, $f9
nop
mul.s      $f17, $f10, $f8
sub.s      $f10, $f14, $f15
add.s      $f11, $f16, $f17
mul.s      $f14, $f11, $f7
nop
mul.s      $f15, $f12, $f6
nop
mul.s      $f16, $f12, $f7
nop
mul.s      $f17, $f11, $f6
sub.s      $f11, $f14, $f15
mul.s      $f14, $f10, $f5
add.s      $f12, $f16, $f17
mul.s      $f15, $f12, $f4
nop
mul.s      $f16, $f12, $f5
nop
mul.s      $f17, $f10, $f4
add.s      $f10, $f14, $f15
swc1       $f10, -0xC($a3)
sub.s      $f12, $f16, $f17
swc1       $f11, -0x8($a3)
bnez       $t9, .L80049A00
swc1       $f12, -0x4($a3)
.L80049A88:
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel pointListRPY

glabel splinePos
lwc1       $f10, 0x10($sp)
lui        $at, %hi(D_800AD190)
lwc1       $f0, %lo(D_800AD190)($at)
mul.s      $f11, $f10, $f10
lui        $at, %hi(D_800AD190 + 0x4)
lwc1       $f1, %lo(D_800AD190 + 0x4)($at)
lui        $at, %hi(D_800AD198)
lwc1       $f2, %lo(D_800AD198)($at)
mtc1       $a2, $f16
lui        $at, %hi(D_800AD198 + 0x4)
mul.s      $f0, $f0, $f12
lwc1       $f3, %lo(D_800AD198 + 0x4)($at)
mtc1       $a3, $f18
lui        $at, %hi(D_800AD1A0)
lwc1       $f4, %lo(D_800AD1A0)($at)
lui        $at, %hi(D_800AD1A0 + 0x4)
lwc1       $f5, %lo(D_800AD1A0 + 0x4)($at)
mul.s      $f1, $f1, $f14
lui        $at, %hi(D_800AD1A8)
lwc1       $f6, %lo(D_800AD1A8)($at)
lui        $at, %hi(D_800AD1A8 + 0x4)
lwc1       $f7, %lo(D_800AD1A8 + 0x4)($at)
lui        $at, %hi(D_800AD1B0)
lwc1       $f8, %lo(D_800AD1B0)($at)
mul.s      $f2, $f2, $f16
lui        $at, %hi(D_800AD1B0 + 0x4)
lwc1       $f9, %lo(D_800AD1B0 + 0x4)($at)
add.s      $f0, $f0, $f1
mul.s      $f3, $f3, $f18
nop
mul.s      $f4, $f4, $f12
add.s      $f2, $f2, $f3
add.s      $f0, $f0, $f2
mul.s      $f5, $f5, $f14
nop
mul.s      $f6, $f6, $f16
add.s      $f4, $f4, $f5
mul.s      $f7, $f7, $f18
nop
mul.s      $f15, $f10, $f11
nop
mul.s      $f8, $f8, $f12
add.s      $f6, $f6, $f7
add.s      $f4, $f4, $f6
mul.s      $f9, $f9, $f16
add.s      $f8, $f8, $f9
mul.s      $f8, $f8, $f10
nop
mul.s      $f15, $f15, $f0
nop
mul.s      $f11, $f11, $f4
add.s      $f14, $f14, $f8
add.s      $f15, $f15, $f11
jr         $ra
add.s      $f0, $f14, $f15
endlabel splinePos

glabel matrixScale
lwc1       $f0, 0x0($a3)
lwc1       $f1, 0x4($a3)
lwc1       $f2, 0x8($a3)
mul.s      $f0, $f0, $f12
lwc1       $f3, 0x10($a3)
lwc1       $f4, 0x14($a3)
lwc1       $f5, 0x18($a3)
lwc1       $f6, 0x20($a3)
mtc1       $a2, $f10
lwc1       $f7, 0x24($a3)
mul.s      $f1, $f1, $f12
lwc1       $f8, 0x28($a3)
swc1       $f0, 0x0($a3)
mul.s      $f2, $f2, $f12
swc1       $f1, 0x4($a3)
mul.s      $f3, $f3, $f14
swc1       $f2, 0x8($a3)
mul.s      $f4, $f4, $f14
swc1       $f3, 0x10($a3)
mul.s      $f5, $f5, $f14
swc1       $f4, 0x14($a3)
mul.s      $f6, $f6, $f10
swc1       $f5, 0x18($a3)
mul.s      $f7, $f7, $f10
swc1       $f6, 0x20($a3)
mul.s      $f8, $f8, $f10
swc1       $f7, 0x24($a3)
jr         $ra
swc1       $f8, 0x28($a3)
endlabel matrixScale

glabel matrixTranslate
lwc1       $f0, 0x0($a3)
lwc1       $f1, 0x4($a3)
lwc1       $f3, 0x10($a3)
mul.s      $f0, $f0, $f12
lwc1       $f4, 0x14($a3)
lwc1       $f6, 0x20($a3)
mul.s      $f3, $f3, $f12
lwc1       $f7, 0x24($a3)
lwc1       $f2, 0x8($a3)
mul.s      $f6, $f6, $f12
lwc1       $f5, 0x18($a3)
lwc1       $f8, 0x28($a3)
mul.s      $f1, $f1, $f14
mtc1       $a2, $f13
lwc1       $f9, 0x30($a3)
mul.s      $f4, $f4, $f14
lwc1       $f10, 0x34($a3)
lwc1       $f11, 0x38($a3)
mul.s      $f7, $f7, $f14
nop
mul.s      $f2, $f2, $f13
add.s      $f0, $f0, $f3
mul.s      $f5, $f5, $f13
add.s      $f0, $f0, $f6
mul.s      $f8, $f8, $f13
add.s      $f0, $f0, $f9
add.s      $f1, $f1, $f4
swc1       $f0, 0x30($a3)
add.s      $f1, $f1, $f7
add.s      $f1, $f1, $f10
add.s      $f2, $f2, $f5
swc1       $f1, 0x34($a3)
add.s      $f2, $f2, $f8
add.s      $f2, $f2, $f11
jr         $ra
swc1       $f2, 0x38($a3)
endlabel matrixTranslate

glabel matrixRoll
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
jal        Sinf
nop
jal        Cosf
mov.s      $f1, $f0
lwc1       $f2, 0x0($a1)
lwc1       $f5, 0x10($a1)
lwc1       $f3, 0x4($a1)
mul.s      $f8, $f2, $f0
lwc1       $f6, 0x14($a1)
lwc1       $f4, 0x8($a1)
mul.s      $f9, $f5, $f1
lwc1       $f7, 0x18($a1)
mul.s      $f10, $f3, $f0
nop
mul.s      $f11, $f6, $f1
nop
mul.s      $f12, $f4, $f0
nop
mul.s      $f13, $f7, $f1
add.s      $f10, $f10, $f11
mul.s      $f5, $f5, $f0
nop
mul.s      $f2, $f2, $f1
swc1       $f10, 0x4($a1)
add.s      $f12, $f12, $f13
mul.s      $f6, $f6, $f0
swc1       $f12, 0x8($a1)
mul.s      $f3, $f3, $f1
nop
mul.s      $f7, $f7, $f0
nop
mul.s      $f4, $f4, $f1
add.s      $f8, $f8, $f9
sub.s      $f5, $f5, $f2
sub.s      $f6, $f6, $f3
swc1       $f5, 0x10($a1)
sub.s      $f7, $f7, $f4
swc1       $f6, 0x14($a1)
swc1       $f8, 0x0($a1)
swc1       $f7, 0x18($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel matrixRoll

glabel matrixPitch
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
jal        Sinf
nop
jal        Cosf
mov.s      $f1, $f0
lwc1       $f2, 0x10($a1)
lwc1       $f5, 0x20($a1)
lwc1       $f3, 0x14($a1)
mul.s      $f8, $f2, $f0
lwc1       $f6, 0x24($a1)
lwc1       $f4, 0x18($a1)
mul.s      $f9, $f5, $f1
lwc1       $f7, 0x28($a1)
mul.s      $f10, $f3, $f0
nop
mul.s      $f11, $f6, $f1
nop
mul.s      $f12, $f4, $f0
nop
mul.s      $f13, $f7, $f1
add.s      $f10, $f10, $f11
mul.s      $f5, $f5, $f0
nop
mul.s      $f2, $f2, $f1
swc1       $f10, 0x14($a1)
add.s      $f12, $f12, $f13
mul.s      $f6, $f6, $f0
swc1       $f12, 0x18($a1)
mul.s      $f3, $f3, $f1
nop
mul.s      $f7, $f7, $f0
nop
mul.s      $f4, $f4, $f1
add.s      $f8, $f8, $f9
sub.s      $f5, $f5, $f2
sub.s      $f6, $f6, $f3
swc1       $f5, 0x20($a1)
sub.s      $f7, $f7, $f4
swc1       $f6, 0x24($a1)
swc1       $f8, 0x10($a1)
swc1       $f7, 0x28($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel matrixPitch

glabel matrixYaw
addiu      $sp, $sp, -0x8
sd         $ra, 0x0($sp)
jal        Sinf
nop
jal        Cosf
mov.s      $f1, $f0
lwc1       $f2, 0x0($a1)
lwc1       $f5, 0x20($a1)
lwc1       $f3, 0x4($a1)
mul.s      $f8, $f2, $f0
lwc1       $f6, 0x24($a1)
lwc1       $f4, 0x8($a1)
mul.s      $f9, $f5, $f1
lwc1       $f7, 0x28($a1)
mul.s      $f10, $f3, $f0
nop
mul.s      $f11, $f6, $f1
nop
mul.s      $f12, $f4, $f0
nop
mul.s      $f13, $f7, $f1
sub.s      $f10, $f10, $f11
mul.s      $f5, $f5, $f0
nop
mul.s      $f2, $f2, $f1
swc1       $f10, 0x4($a1)
sub.s      $f12, $f12, $f13
mul.s      $f6, $f6, $f0
swc1       $f12, 0x8($a1)
mul.s      $f3, $f3, $f1
nop
mul.s      $f7, $f7, $f0
nop
mul.s      $f4, $f4, $f1
sub.s      $f8, $f8, $f9
add.s      $f5, $f5, $f2
add.s      $f6, $f6, $f3
swc1       $f5, 0x20($a1)
add.s      $f7, $f7, $f4
swc1       $f6, 0x24($a1)
swc1       $f8, 0x0($a1)
swc1       $f7, 0x28($a1)
ld         $ra, 0x0($sp)
addiu      $sp, $sp, 0x8
jr         $ra
nop
endlabel matrixYaw

glabel matrixID
li.s       $at, 1.0
mtc1       $at, $f0
sw         $zero, 0x4($a0)
sw         $zero, 0x8($a0)
sw         $zero, 0xC($a0)
sw         $zero, 0x10($a0)
sw         $zero, 0x18($a0)
sw         $zero, 0x1C($a0)
sw         $zero, 0x20($a0)
sw         $zero, 0x24($a0)
sw         $zero, 0x2C($a0)
sw         $zero, 0x30($a0)
sw         $zero, 0x34($a0)
sw         $zero, 0x38($a0)
swc1       $f0, 0x0($a0)
swc1       $f0, 0x14($a0)
swc1       $f0, 0x28($a0)
jr         $ra
swc1       $f0, 0x3C($a0)
endlabel matrixID

glabel Powerf
li.s       $at, 1.0
mtc1       $at, $f0
beqz       $a1, .L80049F28
nop
blezl      $a1, .L80049F20
addiu      $a1, $a1, 0x1
addiu      $a1, $a1, -0x1
.L80049F08:
mul.s      $f0, $f0, $f12
bnel       $a1, $zero, .L80049F08
addiu      $a1, $a1, -0x1
jr         $ra
nop
.L80049F1C:
addiu      $a1, $a1, 0x1
.L80049F20:
bnez       $a1, .L80049F1C
div.s      $f0, $f0, $f12
.L80049F28:
jr         $ra
nop
endlabel Powerf

glabel Cosf
addiu      $a0, $a0, 0x4000
/* Fall through */

alabel Sinf
lui        $t2, %hi(D_800A8754)
srl        $t0, $a0, 4
andi       $t1, $a0, 0x4000
addiu      $t2, $t2, %lo(D_800A8754)
beqz       $t1, .L80049F64
 andi      $t0, $t0, 0x3FF
andi       $t1, $a0, 0xF
beql       $t1, $zero, .L80049F60
 ori       $t1, $zero, 0x400
addiu      $t0, $t0, 0x1
ori        $t1, $zero, 0x400
.L80049F60:
sub        $t0, $t1, $t0
.L80049F64:
sll        $t0, $t0, 2
addu       $t2, $t2, $t0
andi       $t1, $a0, 0x8000
beqz       $t1, .L80049F7C
 lwc1      $f0, 0x0($t2)
neg.s      $f0, $f0
.L80049F7C:
jr         $ra
nop
endlabel Cosf
.size Sinf, . - Sinf

glabel Arctanf
mtc1       $zero, $f0
addiu      $v0, $zero, 0x0
c.eq.s     $f12, $f0
bc1fl      .L80049FA8
c.lt.s     $f12, $f0
c.eq.s     $f14, $f0
bc1t       .L8004A078
nop
c.lt.s     $f12, $f0
.L80049FA8:
bc1tl      .L80049FCC
c.lt.s     $f14, $f0
c.lt.s     $f14, $f0
bc1fl      .L80049FF8
c.lt.s     $f12, $f14
neg.s      $f14, $f14
j          .L80049FDC
addiu      $v0, $zero, 0x4000
c.lt.s     $f14, $f0
.L80049FCC:
neg.s      $f12, $f12
bc1tl      .L80049FF0
neg.s      $f14, $f14
ori        $v0, $zero, 0xC000
.L80049FDC:
mov.s      $f13, $f12
mov.s      $f12, $f14
b          .L80049FF4
mov.s      $f14, $f13
neg.s      $f14, $f14
.L80049FF0:
ori        $v0, $zero, 0x8000
.L80049FF4:
c.lt.s     $f12, $f14
.L80049FF8:
bc1fl      .L8004A040
div.s      $f10, $f14, $f12
div.s      $f10, $f12, $f14
li.s       $at, 2048.0
mtc1       $at, $f1
lui        $t0, %hi(gArcTanTable)
addiu      $t0, $t0, %lo(gArcTanTable)
mul.s      $f10, $f10, $f1
cvt.w.s    $f10, $f10
mfc1       $t1, $f10
nop
andi       $t1, $t1, 0xFFE
add        $t0, $t0, $t1
lh         $t0, 0x0($t0)
addu       $v0, $v0, $t0
jr         $ra
andi       $v0, $v0, 0xFFFF
div.s      $f10, $f14, $f12
.L8004A040:
li.s       $at, 2048.0
mtc1       $at, $f1
lui        $t0, %hi(gArcTanTable)
addiu      $t0, $t0, %lo(gArcTanTable)
addiu      $v0, $v0, 0x4000
mul.s      $f10, $f10, $f1
cvt.w.s    $f10, $f10
mfc1       $t1, $f10
nop
andi       $t1, $t1, 0xFFE
add        $t0, $t0, $t1
lh         $t0, 0x0($t0)
subu       $v0, $v0, $t0
andi       $v0, $v0, 0xFFFF
.L8004A078:
jr         $ra
nop
endlabel Arctanf

glabel mathDiffAngle
sub        $v0, $a1, $a0
ori        $at, $zero, 0x8000
slt        $at, $v0, $at
beqz       $at, .L8004A0A8
lui        $t0, 1
slti       $at, $v0, -0x7FFF
bnez       $at, .L8004A0B0
nop
jr         $ra
nop
.L8004A0A8:
jr         $ra
sub        $v0, $v0, $t0
.L8004A0B0:
jr         $ra
add        $v0, $v0, $t0
endlabel mathDiffAngle
