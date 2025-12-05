#ifdef MODERN_CC
.set gp=64
#endif

#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.data

EXPORT(gIntDisFlag)
    .byte 0x00

EXPORT(rngSeed)
    .word 0x5141564D /* 'QAVM' */

/* Unused in this game, but its used in DKR */
EXPORT(gPrevRNGSeed)
    .word 0x5141564D /* 'QAVM' */

.text

LEAF(disableInterrupts)
    lb         t0, gIntDisFlag
    .set noreorder
    beqz       t0, .L80048E24
    .set reorder
    MFC0(      t0, C0_SR)
    and        t1, t0, -2
    MTC0(      t1, C0_SR)
    andi       v0, t0, 0x1
    NOP
    .L80048E24:
    jr         ra
END(disableInterrupts)

LEAF(enableInterrupts)
    lb         t0, gIntDisFlag
    .set noreorder
    beqz       t0, .L80048E4C
    .set reorder
    MFC0(      t0, C0_SR)
    or         t0, t0, a0
    MTC0(      t0, C0_SR)
    NOP
    NOP
    .L80048E4C:
    jr         ra
END(enableInterrupts)

LEAF(setIntDisFlag)
    sb         a0, gIntDisFlag
    jr         ra
END(setIntDisFlag)

/**
 * Transforms a 3D vector using a 4×4 transformation matrix.
 * Perfect match to libultra compiled guMtxXFMF using -O3 -mips2
 * Official name: mathMtxXFMF
 * void mathMtxXFMF(float mf[4][4], float x, float y, float z, float *ox, float *oy, float *oz);
 */
LEAF(mathMtxXFMF)
    mtc1       a1, fa0
    mtc1       a2, fa1
    mtc1       a3, ft4
    lwc1       ft0, 0x0(a0)
    lwc1       ft2, 0x10(a0)
    mul.s      ft1, ft0, fa0
    lwc1       ft0, 0x20(a0)
    mul.s      ft3, ft2, fa1
    mul.s      ft2, ft0, ft4
    add.s      ft5, ft1, ft3
    lwc1       ft3, 0x30(a0)
    add.s      ft1, ft5, ft2
    add.s      ft0, ft3, ft1
    lw         t6,  0x10($sp)
    swc1       ft0, 0x0(t6)
    lwc1       ft5, 0x4(a0)
    lwc1       ft3, 0x14(a0)
    mul.s      ft2, ft5, fa0
    lwc1       ft5, 0x24(a0)
    mul.s      ft1, ft3, fa1
    add.s      ft0, ft2, ft1
    mul.s      ft3, ft5, ft4
    lwc1       ft1, 0x34(a0)
    add.s      ft2, ft0, ft3
    add.s      ft5, ft1, ft2
    lw         t7,  0x14($sp)
    swc1       ft5, 0x0(t7)
    lwc1       ft0, 0x8(a0)
    lwc1       ft1, 0x18(a0)
    mul.s      ft3, ft0, fa0
    lwc1       ft0, 0x28(a0)
    mul.s      ft2, ft1, fa1
    add.s      ft5, ft3, ft2
    mul.s      ft1, ft0, ft4
    lwc1       ft2, 0x38(a0)
    add.s      ft3, ft5, ft1
    add.s      ft0, ft2, ft3
    lw         t8,  0x18($sp)
    swc1       ft0, 0x0(t8)
    jr         ra
END(mathMtxXFMF)


/**
 * Transforms a direction vector in 3D space using the rotation part of a 4×4 matrix.
 * This function multiplies the input vector by the upper-left 3×3 portion of the matrix mf,
 * ignoring the translation component. It is used for transforming directions, such as normals,
 * rather than points.
 * Official name: mathMtxFastXFMF
 * void mathMtxFastXFMF(MtxF *mf, Vec3f *in, Vec3f *out);
 */
LEAF(mathMtxFastXFMF)
    lwc1       ft0, 0x0(a1)
    lwc1       ft1, 0x4(a1)
    lwc1       ft3, 0x0(a0)
    lwc1       fa0, 0x10(a0)
    mul.s      ft3, ft0, ft3
    lwc1       ft2, 0x8(a1)
    lwc1       fa1, 0x20(a0)
    mul.s      fa0, ft1, fa0
    add.s      fa0, ft3, fa0
    mul.s      fa1, ft2, fa1
    lwc1       ft3, 0x4(a0)
    mul.s      ft3, ft0, ft3
    add.s      ft4, fa0, fa1
    lwc1       fa0, 0x14(a0)
    lwc1       fa1, 0x24(a0)
    mul.s      fa0, ft1, fa0
    swc1       ft4, 0x0(a2)
    mul.s      fa1, ft2, fa1
    add.s      fa0, ft3, fa0
    lwc1       ft3, 0x8(a0)
    add.s      ft4, fa0, fa1
    mul.s      ft3, ft0, ft3
    lwc1       fa0, 0x18(a0)
    lwc1       fa1, 0x28(a0)
    swc1       ft4, 0x4(a2)
    mul.s      fa0, ft1, fa0
    add.s      fa0, ft3, fa0
    mul.s      fa1, ft2, fa1
    add.s      fa1, fa0, fa1
    swc1       fa1, 0x8(a2)
    jr         ra
END(mathMtxFastXFMF)

/**
 * Multiplies two 4×4 matrices.
 * Official name: mathMtxCatF
 * void mathMtxCatF(MtxF *mat1, MtxF *mat2, MtxF *output);
 */
LEAF(mathMtxCatF)
    addiu      sp, sp, -8
    sdc1       fv1, 0(sp)
    ori        t0, zero, 4
    .L80048F88:
    lwc1       fv1, 0x0(a0)
    lwc1       ft0, 0x4(a0)
    lwc1       ft1, 0x8(a0)
    lwc1       ft2, 0xC(a0)
    lwc1       ft3, 0x0(a1)
    lwc1       fa0, 0x10(a1)
    lwc1       fa1, 0x20(a1)
    lwc1       ft4, 0x30(a1)
    mul.s      ft3, fv1, ft3
    mul.s      fa0, ft0, fa0
    mul.s      fa1, ft1, fa1
    mul.s      ft4, ft2, ft4
    add.s      fa1, fa0, fa1
    add.s      ft4, ft3, ft4
    lwc1       ft3, 0x4(a1)
    mul.s      ft3, fv1, ft3
    add.s      ft5, fa1, ft4
    lwc1       fa0, 0x14(a1)
    mul.s      fa0, ft0, fa0
    lwc1       fa1, 0x24(a1)
    mul.s      fa1, ft1, fa1
    add.s      fa1, fa0, fa1
    lwc1       ft4, 0x34(a1)
    swc1       ft5, 0x0(a2)
    lwc1       fa0, 0x18(a1)
    mul.s      ft4, ft2, ft4
    add.s      ft4, ft3, ft4
    lwc1       ft3, 0x08(a1)
    mul.s      ft3, fv1, ft3
    add.s      ft5, fa1, ft4
    mul.s      fa0, ft0, fa0
    lwc1       fa1, 0x28(a1)
    lwc1       ft4, 0x38(a1)
    swc1       ft5, 0x4(a2)
    mul.s      fa1, ft1, fa1
    mul.s      ft4, ft2, ft4
    add.s      fa1, fa0, fa1
    add.s      ft4, ft3, ft4
    lwc1       ft3, 0xC(a1)
    lwc1       fa0, 0x1C(a1)
    mul.s      ft3, fv1, ft3
    add.s      ft5, fa1, ft4
    mul.s      fa0, ft0, fa0
    lwc1       fa1, 0x2C(a1)
    lwc1       ft4, 0x3C(a1)
    swc1       ft5, 0x8(a2)
    mul.s      fa1, ft1, fa1
    mul.s      ft4, ft2, ft4
    add.s      fa1, fa0, fa1
    add.s      ft4, ft3, ft4
    add.s      ft5, fa1, ft4
    swc1       ft5, 0xC(a2)
    addiu      a0, 0x10
    addiu      a2, 0x10
    addiu      t0, -1
    bnezl      t0, .L80048F88
    ldc1       fv1, 0(sp)
    addiu      sp, sp, 8
    jr         ra
END(mathMtxCatF)

/**
 * Converts a floating-point 4×4 matrix to a Mtx fixed-point matrix.
 * Official name: mathMtxF2L
 * void mathMtxF2L(MtxF *mf, Mtx *m);
 */
LEAF(mathMtxF2L)
    li.s       fa0, 65536.0
    ori        t0, zero, 4 /* Loop counter */
    .L80049090:
    lwc1       ft0, 0x0(a0)
    lwc1       ft1, 0x4(a0)
    lwc1       ft2, 0x8(a0)
    lwc1       ft3, 0xC(a0)
    mul.s      ft0, ft0, fa0
    mul.s      ft1, ft1, fa0
    mul.s      ft2, ft2, fa0
    mul.s      ft3, ft3, fa0
    cvt.w.s    ft0, ft0
    cvt.w.s    ft1, ft1
    cvt.w.s    ft2, ft2
    cvt.w.s    ft3, ft3
    mfc1       t1, ft0
    mfc1       t2, ft1
    mfc1       t3, ft2
    mfc1       t4, ft3
    sh         t1, 0x20(a1)
    sh         t2, 0x22(a1)
    sh         t3, 0x24(a1)
    sh         t4, 0x26(a1)
    srl        t1, t1, 16
    srl        t2, t2, 16
    srl        t3, t3, 16
    srl        t4, t4, 16
    sh         t1, 0(a1)
    sh         t2, 0x2(a1)
    sh         t3, 0x4(a1)
    sh         t4, 0x6(a1)
    addiu      a0, 0x10 /* Increment MtxF pointer by 16 bytes */
    addiu      a1, 0x8  /* Increment Mtx pointer by 8 bytes */
    addiu      t0, -1   /* Decrement loop counter */
    bnezl      t0, .L80049090
    jr         ra
END(mathMtxF2L)

LEAF(mathSeed)
    sw         a0, rngSeed
    jr         ra
END(mathSeed)

LEAF(mathRnd)
    lw         t0, rngSeed
    sub        a1, a0
    dsll32     t1, t0, 31
    dsll       t2, t0, 31
    dsrl       t1, 31
    dsrl32     t2, 0
    dsll32     t3, t0, 12
    or         t1, t2
    dsrl32     t3, 0
    xor        t1, t3
    dsrl       t3, t1, 20
    andi       t3, 0xFFF
    xor        t0, t3, t1
    sw         t0, rngSeed
    addi       a1, 1
    subu       t0, a0
    divu       t0, a1
    mfhi       v0
    add        v0, a0
    jr         ra
END(mathRnd)

/* Does t6 even do anything? */
LEAF(fastShortReflection)
    lh         t0, 0x0(a0)
    lh         t1, 0x2(a0)
    lh         t2, 0x4(a0)
    lh         t3, 0x0(a1)
    lh         t4, 0x2(a1)
    lh         t5, 0x4(a1)
    mult       t0, t3
    mflo       t6
    mult       t1, t4
    mflo       t7
    add        t6, t7
    mult       t2, t5
    mflo       t8
    add        t6, t8
    sra        t6, 12
    mult       t6, t3
    mflo       t3
    sra        t3, 13
    sub        t3, t0
    sh         t3, 0x6(a0)
    mult       t6, t4
    mflo       t4
    sra        t4, 13
    sub        t4, t1
    mult       t6, t5
    sh         t4, 0x8(a0)
    mflo       t5
    sra        t5, 13
    sub        t5, t0
    sh         t5, 0xA(a0)
    jr         ra
END(fastShortReflection)

/**
 * Scales the Y axis of the given 4×4 transformation matrix by the specified factor.
 * If this is a model matrix, the operation is equivalent to stretching or squashing
 * the model along its local Y axis.
 * Official name: mathSquashY
 * void mathSquashY(MtxF *input, f32 scale);
 */
LEAF(mathSquashY)
    mtc1       a1, ft5

    lwc1       ft4, 0x10(a0)
    mul.s      ft4, ft5
    swc1       ft4, 0x10(a0)

    lwc1       ft4, 0x14(a0)
    mul.s      ft4, ft5
    swc1       ft4, 0x14(a0)

    lwc1       ft4, 0x18(a0)
    mul.s      ft4, ft5
    swc1       ft4, 0x18(a0)

    jr         ra
END(mathSquashY)

/**
 * Modifies the matrix by translating its position along the local Y axis.
 * If this is a model matrix, the operation is equivalent to moving the model
 * along its local Y axis in model space.
 * Official name: mathTransY
 * void mathTransY(MtxF *input, f32 offset);
 */
LEAF(mathTransY)
    mtc1       a1, ft4
    lwc1       fv0, 0x10(a0)
    lwc1       ft0, 0x14(a0)
    lwc1       ft2, 0x18(a0)
    lwc1       fv1, 0x30(a0)
    lwc1       ft1, 0x34(a0)
    lwc1       ft3, 0x38(a0)
    mul.s      fv0, ft4
    mul.s      ft0, ft4
    mul.s      ft2, ft4
    add.s      fv0, fv1
    add.s      ft0, ft1
    add.s      ft2, ft3
    swc1       fv0, 0x30(a0)
    swc1       ft0, 0x34(a0)
    swc1       ft2, 0x38(a0)
    jr         ra
END(mathTransY)

LEAF(mathRpyXyzMtx)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    move       a3, a0
    li.s       ft5, 0.0000152587890625 /* (1.0f / 0x10000) */
    lh         a0, 0x0(a1)
    jal        mathSinInterp
    mtc1       v0, fv0
    cvt.s.w    fv0
    mul.s      fv0, ft5
    lh         a0, 0x0(a1)
    jal        mathCosInterp
    mtc1       v0, fv1
    cvt.s.w    fv1
    mul.s      fv1, ft5
    lh         a0, 0x2(a1)
    jal        mathSinInterp
    mtc1       v0, ft0
    cvt.s.w    ft0
    mul.s      ft0, ft5
    lh         a0, 0x2(a1)
    jal        mathCosInterp
    mtc1       v0, ft1
    cvt.s.w    ft1
    mul.s      ft1, ft5
    lh         a0, 0x4(a1)
    jal        mathSinInterp
    mtc1       v0, ft2
    cvt.s.w    ft2
    mul.s      ft2, ft5
    lh         a0, 0x4(a1)
    jal        mathCosInterp
    mtc1       v0, ft3
    cvt.s.w    ft3
    sw         zero, 0xC(a3)
    swc1       ft0, 0x18(a3)
    sw         zero, 0x1C(a3)
    sw         zero, 0x2C(a3)
    mul.s      ft3, ft5
    mul.s      ft4, ft0, ft2
    mul.s      ft4, fv0
    mul.s      ft5, fv1, ft3
    sub.s      ft4, ft5, ft4
    swc1       ft4, 0x0(a3)
    mul.s      ft4, ft0, ft3
    mul.s      ft4, fv0
    mul.s      ft5, fv1, ft2
    add.s      ft4, ft5
    swc1       ft4, 0x4(a3)
    mul.s      ft4, fv0, ft1
    neg.s      ft4
    swc1       ft4, 0x8(a3)
    mul.s      ft4, ft1, ft2
    neg.s      ft4
    swc1       ft4, 0x10(a3)
    mul.s      ft4, ft1, ft3
    swc1       ft4, 0x14(a3)
    mul.s      ft4, ft0, ft2
    mul.s      ft4, fv1
    mul.s      ft5, fv0, ft3
    add.s      ft4, ft5
    swc1       ft4, 0x20(a3)
    mul.s      ft4, ft0, ft3
    mul.s      ft4, fv1
    mul.s      ft5, fv0, ft2
    sub.s      ft4, ft5, ft4
    lwc1       ft5, 0x0(a3)
    swc1       ft4, 0x24(a3)
    mul.s      ft4, fv1, ft1
    swc1       ft4, 0x28(a3)
    lwc1       fv0, 0xC(a1)
    lwc1       fv1, 0x10(a1)
    lwc1       ft4, 0x10(a3)
    mul.s      ft5, fv0
    lwc1       ft0, 0x14(a1)
    mul.s      ft4, fv1
    add.s      ft4, ft5
    lwc1       ft5, 0x20(a3)
    mul.s      ft5, ft0
    add.s      ft4, ft5
    lwc1       ft5, 0x4(a3)
    swc1       ft4, 0x30(a3)
    mul.s      ft5, fv0
    lwc1       ft4, 0x14(a3)
    mul.s      ft4, fv1
    add.s      ft4, ft5
    lwc1       ft5, 0x24(a3)
    mul.s      ft5, ft0
    add.s      ft4, ft5
    lwc1       ft5, 0x8(a3)
    swc1       ft4, 0x34(a3)
    mul.s      ft5, fv0
    lwc1       ft4, 0x18(a3)
    mul.s      ft4, fv1
    add.s      ft4, ft5
    lwc1       ft5, 0x28(a3)
    mul.s      ft5, ft0
    add.s      ft4, ft5
    swc1       ft4, 0x38(a3)
    li.s       ft4, 1.0
    swc1       ft4, 0x3C(a3)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathRpyXyzMtx)

LEAF(func_80049474)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    or         a3, a0, zero
    li.s       ft5, 0.0000152587890625 /* (1.0f / 0x10000) */
    lh         a0, 0x0(a1)
    jal        mathSinInterp
    mtc1       v0, fv0
    cvt.s.w    fv0
    mul.s      fv0, ft5
    lh         a0, 0x0(a1)
    jal        mathCosInterp
    mtc1       v0, fv1
    cvt.s.w    fv1
    mul.s      fv1, ft5
    lh         a0, 0x2(a1)
    jal        mathSinInterp
    mtc1       v0, ft0
    cvt.s.w    ft0
    mul.s      ft0, ft5
    lh         a0, 0x2(a1)
    jal        mathCosInterp
    mtc1       v0, ft1
    cvt.s.w    ft1
    mul.s      ft1, ft5
    lh         a0, 0x4(a1)
    jal        mathSinInterp
    mtc1       v0, ft2
    cvt.s.w    ft2
    mul.s      ft2, ft5
    lh         a0, 0x4(a1)
    jal        mathCosInterp
    mtc1       v0, ft3
    cvt.s.w    ft3
    mul.s      ft3, ft5
    mul.s      fa0, fv0, ft1
    mul.s      fa1, fv1, ft3
    mul.s      ft4, fv0, ft2
    li.s       ft5, 1.0
    sw         zero, 0xC(a3)
    sw         zero, 0x1C(a3)
    sw         zero, 0x2C(a3)
    swc1       ft5, 0x3C(a3)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(func_80049474)

LEAF(mathRSMtx)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    jal        Sinf
    mov.s      fv1, fv0
    jal        Cosf
    mtc1       a1, ft1f
    mov.s      fv1f, fv0
    mtc1       a2, ft2
    mul.s      fv1, ft1f
    li.s       ft1, 1.0
    mul.s      fv1f, ft1f
    neg.s      ft0, fv1
    mul.s      fv1, ft2
    mul.s      ft0f, fv1f, ft2

    swc1       fv1f, 0x0(a3)
    swc1       fv1,  0x4(a3)
    sw         zero, 0x8(a3)
    sw         zero, 0xC(a3)
    swc1       ft0,  0x10(a3)
    swc1       ft0f, 0x14(a3)
    sw         zero, 0x18(a3)
    sw         zero, 0x1C(a3)
    sw         zero, 0x20(a3)
    sw         zero, 0x24(a3)
    swc1       ft1f, 0x28(a3)
    sw         zero, 0x2C(a3)
    sw         zero, 0x30(a3)
    sw         zero, 0x34(a3)
    sw         zero, 0x38(a3)
    swc1       ft1,  0x3C(a3)

    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathRSMtx)

LEAF(mathOneFloatRPY)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    lwc1       ft0, 0x0(a1)
    lwc1       ft1, 0x4(a1)
    lwc1       ft2, 0x8(a1)
    move       a2, a0
    lh         a0, 0x4(a2)
    jal        Sinf
    mul.s      ft3, ft0, fv0
    mul.s      fa0, ft1, fv0
    jal        Cosf
    mul.s      ft0, fv0
    lh         a0, 0x2(a2)
    mul.s      ft1, fv0
    sub.s      ft0, fa0
    add.s      ft1, ft3
    jal        Sinf
    mul.s      ft3, ft1, fv0
    mul.s      fa0, ft2, fv0
    jal        Cosf
    mul.s      ft1, fv0
    lh         a0, 0x0(a2)
    mul.s      ft2, fv0
    sub.s      ft1, fa0
    add.s      ft2, ft3
    jal        Sinf
    mul.s      ft3, ft0, fv0
    mul.s      fa0, ft2, fv0
    jal        Cosf
    mul.s      ft0, fv0
    swc1       ft1, 0x4(a1)
    mul.s      ft2, fv0
    add.s      ft0, fa0
    sub.s      ft2, ft3
    swc1       ft0, 0x0(a1)
    swc1       ft2, 0x8(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatRPY)

LEAF(mathOneFloatYPR)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    lwc1       ft0, 0x0(a1)
    lwc1       ft1, 0x4(a1)
    lwc1       ft2, 0x8(a1)
    move       a2, a0
    lh         a0, 0x0(a2)
    jal        Sinf
    mul.s      ft3, ft0, fv0
    mul.s      fa0, ft2, fv0
    jal        Cosf
    mul.s      ft0, fv0
    lh         a0, 0x2(a2)
    mul.s      ft2, fv0
    add.s      ft0, fa0
    sub.s      ft2, ft3
    jal        Sinf
    mul.s      ft3, ft1, fv0
    mul.s      fa0, ft2, fv0
    jal        Cosf
    mul.s      ft1, fv0
    lh         a0, 0x4(a2)
    mul.s      ft2, fv0
    sub.s      ft1, fa0
    add.s      ft2, ft3
    jal        Sinf
    mul.s      ft3, ft0, fv0
    mul.s      fa0, ft1, fv0
    jal        Cosf
    mul.s      ft0, fv0
    swc1       ft2, 0x8(a1)
    mul.s      ft1, fv0
    sub.s      ft0, fa0
    add.s      ft1, ft3
    swc1       ft0, 0x0(a1)
    swc1       ft1, 0x4(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatYPR)

LEAF(mathOneFloatPY)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    move       a2, a0
    lwc1       ft2, 0x8(a1)
    lh         a0, 0x2(a2)
    jal        Sinf
    mul.s      ft1, ft2, fv0
    jal        Cosf
    neg.s      ft1
    mul.s      ft2, fv0
    lh         a0, 0x0(a2)
    jal        Sinf
    mul.s      ft0, ft2, fv0
    jal        Cosf
    mul.s      ft2, fv0
    swc1       ft0, 0x0(a1)
    swc1       ft1, 0x4(a1)
    swc1       ft2, 0x8(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatPY)

LEAF(mathXZInTri)
    lw         t6, 0x10(sp)
    move       v0, zero
    .set noreorder
    lh         t0, 0x0(a2)
    lh         t1, 0x4(a2)
    lh         t2, 0x0(a3)
    lh         t3, 0x4(a3)
    lh         t4, 0x0(t6)
    lh         t5, 0x4(t6)
    .set reorder
    sub        t6, t3, t1
    sub        t7, a0, t0
    mult       t6, t7
    sub        t8, t2, t0
    sub        t9, a1, t1
    ori        a3, zero, 1
    mflo       t7
    mult       t8, t9
    mflo       t8
    sub        t7, t8
    bgez       t7, .L8004980C
    move       a3, zero
    .L8004980C:
    sub        t6, t5, t3
    sub        t7, a0, t2
    mult       t6, t7
    sub        t8, t4, t2
    sub        t9, a1, t3
    ori        a2, zero, 1
    mflo       t7
    mult       t8, t9
    mflo       t8
    sub        t7, t8
    bgez       t7, .L80049844
    move       a2, zero
    .L80049844:
    bne        a3, a2, .L80049894
    sub        t6, t1, t5
    sub        t7, a0, t4
    mult       t6, t7
    sub        t8, t0, t4
    sub        t9, a1, t5
    ori        a1, zero, 1
    mflo       t7
    mult       t8, t9
    mflo       t8
    sub        t7, t7, t8
    bgez       t7, .L80049888
    move       a1, zero
    .L80049888:
    bne        a2, a1, .L80049894
    ori        v0, zero, 1
    .L80049894:
    jr         ra
END(mathXZInTri)

LEAF(mathTranslateMtx)
    /* Clear matrix */
    move       t0, a0
    addiu      t1, t0, 0x40
    .L800498A4:
    sw         zero, 0(t0)
    addiu      t0, 4
    bne        t1, t0, .L800498A4

    li.s       ft5, 1.0
    swc1       ft5, 0x0(a0)
    swc1       ft5, 0x14(a0)
    swc1       ft5, 0x28(a0)
    sw         a1, 0x30(a0)
    sw         a2, 0x34(a0)
    sw         a3, 0x38(a0)
    swc1       ft5, 0x3C(a0)
    jr         ra
END(mathTranslateMtx)

LEAF(mathScaleMtx)
    /* Clear matrix */
    move       t0, a0
    addiu      t1, t0, 0x40
    .L800498E0:
    sw         zero, 0(t0)
    addiu      t0, 4
    bne        t1, t0, .L800498E0

    li.s       ft5, 1.0
    sw         a1, 0x0(a0)
    sw         a2, 0x14(a0)
    sw         a3, 0x28(a0)
    swc1       ft5, 0x3C(a0)
    jr         ra
END(mathScaleMtx)

LEAF(mathCosInterp)
    addiu      a0, 0x4000
    /* Fall through */

XLEAF(mathSinInterp)
    sll        v0, a0, 17
    bgez       v0, .L80049920
    xori       a0, 0x7FFF
    .L80049920:
    srl        t2, a0, 3
    andi       t2, 0x7FE
    la         v0, gSineTable
    addu       v0, t2
    lhu        t2, 0x2(v0)
    lhu        v0, 0x0(v0)
    andi       t1, a0, 0xF
    sll        a0, 16
    subu       t2, v0
    multu      t2, t1
    sll        v0, 1
    mflo       t2
    srl        t2, 3
    addu       v0, t2
    bgez       a0, .L80049960
    negu       v0
    .L80049960:
    jr         ra
END(mathCosInterp)
#ifdef MODERN_CC
.size mathSinInterp, . - mathSinInterp
#endif

LEAF(mathCos)
    addiu      a0, 0x4000
    /* Fall through */

XLEAF(mathSin)
    sll        v0, a0, 17
    bgez       v0, .L80049980
    xori       a0, 0x7FFF
    .L80049980:
    srl        t2, a0, 3
    andi       t2, t2, 0x7FE
    la         v0, gSineTable
    addu       v0, t2
    lhu        v0, 0x0(v0)
    sll        a0, 16
    sll        v0, 1
    bgez       a0, .L800499A4
    negu       v0
    .L800499A4:
    jr         ra
END(mathCos)
#ifdef MODERN_CC
.size mathSin, . - mathSin
#endif

LEAF(mathBreakPoint)
    break      7
END(mathBreakPoint)

LEAF(pointListRPY)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    blez       a0, .L80049A88
    move       t9, a0
    mtc1       zero, fv0
    lh         a0, 0x0(a1)
    jal        Sinf
    mov.s      ft0, fv0
    jal        Cosf
    mov.s      ft0f, fv0
    lh         a0, 0x2(a1)
    jal        Sinf
    mov.s      ft1, fv0
    jal        Cosf
    mov.s      ft1f, fv0
    lh         a0, 0x4(a1)
    jal        Sinf
    mov.s      ft2, fv0
    jal        Cosf
    mov.s      ft2f, fv0
    .L80049A00:
    lwc1       ft3, 0x0(a2)
    lwc1       ft3f, 0x4(a2)
    lwc1       fa0, 0x8(a2)
    addiu      t9, -1
    addiu      a2, 0xC
    mul.s      fa1, ft3, ft2f
    mul.s      fa1f, ft3f, ft2
    mul.s      ft4, ft3f, ft2f
    mul.s      ft4f, ft3, ft2
    sub.s      ft3, fa1, fa1f
    add.s      ft3f, ft4, ft4f
    mul.s      fa1, ft3f, ft1f
    mul.s      fa1f, fa0, ft1
    mul.s      ft4, fa0, ft1f
    mul.s      ft4f, ft3f, ft1
    sub.s      ft3f, fa1, fa1f
    mul.s      fa1, ft3, ft0f
    add.s      fa0, ft4, ft4f
    mul.s      fa1f, fa0, ft0
    mul.s      ft4, fa0, ft0f
    mul.s      ft4f, ft3, ft0
    add.s      ft3, fa1, fa1f
    sub.s      fa0, ft4, ft4f
    swc1       ft3, 0(a3)
    swc1       ft3f, 4(a3)
    swc1       fa0, 8(a3)
    addiu      a3, 0xC
    bnez       t9, .L80049A00
    .L80049A88:
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(pointListRPY)

LEAF(splinePos)
    lwc1       ft3, 0x10(sp)
    mtc1       a2, ft4
    mtc1       a3, ft5
    li.s       fv0, -0.56
    li.s       fv0f, 1.44
    li.s       fv1, -1.44
    li.s       fv1f, 0.56
    li.s       ft0, 1.12
    li.s       ft0f, -2.44
    li.s       ft1, 1.88
    li.s       ft1f, -0.56
    li.s       ft2, -0.56
    li.s       ft2f, 0.56
    mul.s      ft3f, ft3, ft3
    mul.s      fv0, fa0
    mul.s      fv0f, fv0f, fa1
    mul.s      fv1, ft4
    mul.s      fv1f, ft5
    mul.s      ft0, fa0
    add.s      fv0, fv0f
    add.s      fv1, fv1f
    add.s      fv0, fv1
    mul.s      ft0f, fa1
    mul.s      ft1, ft4
    add.s      ft0, ft0f
    mul.s      ft1f, ft5
    mul.s      fa1f, ft3, ft3f
    mul.s      ft2, fa0
    add.s      ft1, ft1f
    add.s      ft0, ft1
    mul.s      ft2f, ft4
    add.s      ft2, ft2f
    mul.s      ft2, ft3
    mul.s      fa1f, fv0
    mul.s      ft3f, ft0
    add.s      fa1, ft2
    add.s      fa1f, ft3f
    add.s      fv0, fa1, fa1f
    jr         ra
END(splinePos)

LEAF(matrixScale)
    mtc1       a2, ft3
    lwc1       fv0, 0x0(a3)
    lwc1       fv0f, 0x4(a3)
    lwc1       fv1, 0x8(a3)
    lwc1       fv1f, 0x10(a3)
    lwc1       ft0, 0x14(a3)
    lwc1       ft0f, 0x18(a3)
    lwc1       ft1, 0x20(a3)
    lwc1       ft1f, 0x24(a3)
    lwc1       ft2, 0x28(a3)
    mul.s      fv0, fa0
    mul.s      fv0f, fa0
    mul.s      fv1, fa0
    mul.s      fv1f, fa1
    mul.s      ft0, fa1
    mul.s      ft0f, fa1
    mul.s      ft1, ft3
    mul.s      ft1f, ft3
    mul.s      ft2, ft3
    swc1       fv0, 0x0(a3)
    swc1       fv0f, 0x4(a3)
    swc1       fv1, 0x8(a3)
    swc1       fv1f, 0x10(a3)
    swc1       ft0, 0x14(a3)
    swc1       ft0f, 0x18(a3)
    swc1       ft1, 0x20(a3)
    swc1       ft1f, 0x24(a3)
    swc1       ft2, 0x28(a3)
    jr         ra
END(matrixScale)

LEAF(matrixTranslate)
    lwc1       fv0, 0x0(a3)
    lwc1       fv0f, 0x4(a3)
    lwc1       fv1, 0x8(a3)
    lwc1       fv1f, 0x10(a3)
    lwc1       ft0, 0x14(a3)
    lwc1       ft0f, 0x18(a3)
    lwc1       ft1, 0x20(a3)
    lwc1       ft1f, 0x24(a3)
    lwc1       ft2, 0x28(a3)
    lwc1       ft2f, 0x30(a3)
    lwc1       ft3, 0x34(a3)
    lwc1       ft3f, 0x38(a3)
    mtc1       a2, fa0f
    mul.s      fv0, fa0
    mul.s      fv1f, fa0
    mul.s      ft1, fa0
    mul.s      fv0f, fa1
    mul.s      ft0, fa1
    mul.s      ft1f, fa1
    mul.s      fv1, fa0f
    mul.s      ft0f, fa0f
    mul.s      ft2, fa0f
    add.s      fv0, fv1f
    add.s      fv0, ft1
    add.s      fv0, ft2f
    add.s      fv0f, ft0
    add.s      fv0f, ft1f
    add.s      fv0f, ft3
    add.s      fv1, ft0f
    add.s      fv1, ft2
    add.s      fv1, ft3f
    swc1       fv0, 0x30(a3)
    swc1       fv0f, 0x34(a3)
    swc1       fv1, 0x38(a3)
    jr         ra
END(matrixTranslate)

LEAF(matrixRoll)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    jal        Sinf
    mov.s      fv0f, fv0
    jal        Cosf
    lwc1       fv1, 0x0(a1)
    lwc1       fv1f, 0x4(a1)
    lwc1       ft0, 0x8(a1)
    lwc1       ft0f, 0x10(a1)
    lwc1       ft1, 0x14(a1)
    lwc1       ft1f, 0x18(a1)
    mul.s      ft2, fv1, fv0
    mul.s      ft2f, ft0f, fv0f
    mul.s      ft3, fv1f, fv0
    mul.s      ft3f, ft1, fv0f
    mul.s      fa0, ft0, fv0
    mul.s      fa0f, ft1f, fv0f
    add.s      ft3, ft3f
    mul.s      ft0f, fv0
    mul.s      fv1, fv0f
    add.s      fa0, fa0f
    mul.s      ft1, fv0
    mul.s      fv1f, fv0f
    mul.s      ft1f, fv0
    mul.s      ft0, fv0f
    add.s      ft2, ft2f
    sub.s      ft0f, fv1
    sub.s      ft1, fv1f
    sub.s      ft1f, ft0
    swc1       ft2, 0x0(a1)
    swc1       ft3, 0x4(a1)
    swc1       fa0, 0x8(a1)
    swc1       ft0f, 0x10(a1)
    swc1       ft1, 0x14(a1)
    swc1       ft1f, 0x18(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(matrixRoll)

LEAF(matrixPitch)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    jal        Sinf
    mov.s      fv0f, fv0
    jal        Cosf
    lwc1       fv1, 0x10(a1)
    lwc1       ft0f, 0x20(a1)
    lwc1       fv1f, 0x14(a1)
    lwc1       ft0, 0x18(a1)
    lwc1       ft1, 0x24(a1)
    lwc1       ft1f, 0x28(a1)
    mul.s      ft2, fv1, fv0
    mul.s      ft2f, ft0f, fv0f
    mul.s      ft3, fv1f, fv0
    mul.s      ft3f, ft1, fv0f
    mul.s      fa0, ft0, fv0
    mul.s      fa0f, ft1f, fv0f
    add.s      ft3, ft3f
    mul.s      ft0f, fv0
    mul.s      fv1, fv0f
    add.s      fa0, fa0f
    mul.s      ft1, fv0
    mul.s      fv1f, fv0f
    mul.s      ft1f, fv0
    mul.s      ft0, fv0f
    add.s      ft2, ft2f
    sub.s      ft0f, fv1
    sub.s      ft1, fv1f
    sub.s      ft1f, ft0
    swc1       ft3, 0x14(a1)
    swc1       fa0, 0x18(a1)
    swc1       ft0f, 0x20(a1)
    swc1       ft1, 0x24(a1)
    swc1       ft2, 0x10(a1)
    swc1       ft1f, 0x28(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(matrixPitch)

LEAF(matrixYaw)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    jal        Sinf
    mov.s      fv0f, fv0
    jal        Cosf
    lwc1       fv1, 0x0(a1)
    lwc1       fv1f, 0x4(a1)
    lwc1       ft0, 0x8(a1)
    lwc1       ft0f, 0x20(a1)
    lwc1       ft1, 0x24(a1)
    lwc1       ft1f, 0x28(a1)
    mul.s      ft2, fv1, fv0
    mul.s      ft2f, ft0f, fv0f
    mul.s      ft3, fv1f, fv0
    mul.s      ft3f, ft1, fv0f
    mul.s      fa0, ft0, fv0
    mul.s      fa0f, ft1f, fv0f
    sub.s      ft3, ft3f
    mul.s      ft0f, fv0
    mul.s      fv1, fv0f
    sub.s      fa0, fa0f
    mul.s      ft1, fv0
    mul.s      fv1f, fv0f
    mul.s      ft1f, fv0
    mul.s      ft0, fv0f
    sub.s      ft2, ft2f
    add.s      ft0f, fv1
    add.s      ft1, fv1f
    add.s      ft1f, ft0
    swc1       ft2, 0x0(a1)
    swc1       ft3, 0x4(a1)
    swc1       fa0, 0x8(a1)
    swc1       ft0f, 0x20(a1)
    swc1       ft1, 0x24(a1)
    swc1       ft1f, 0x28(a1)
    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(matrixYaw)

LEAF(matrixID)
    li.s       fv0, 1.0
    swc1       fv0, 0x0(a0)
    sw         zero, 0x4(a0)
    sw         zero, 0x8(a0)
    sw         zero, 0xC(a0)
    sw         zero, 0x10(a0)
    swc1       fv0, 0x14(a0)
    sw         zero, 0x18(a0)
    sw         zero, 0x1C(a0)
    sw         zero, 0x20(a0)
    sw         zero, 0x24(a0)
    swc1       fv0, 0x28(a0)
    sw         zero, 0x2C(a0)
    sw         zero, 0x30(a0)
    sw         zero, 0x34(a0)
    sw         zero, 0x38(a0)
    swc1       fv0, 0x3C(a0)
    jr         ra
END(matrixID)

LEAF(Powerf)
    li.s       fv0, 1.0
    beqz       a1, .L80049F28
    blez       a1, .L80049F20
    .L80049F08:
    addiu      a1, -0x1
    mul.s      fv0, fa0
    bnez       a1, .L80049F08
    jr         ra
    .L80049F20:
    addiu      a1, 0x1
    div.s      fv0, fa0
    bnez       a1, .L80049F20
    .L80049F28:
    jr         ra
END(Powerf)

LEAF(Cosf)
    addiu      a0, a0, 0x4000
    /* Fall through */

XLEAF(Sinf)
    la         t2, D_800A8754
    srl        t0, a0, 4
    andi       t1, a0, 0x4000
    andi       t0, 0x3FF
    beqz       t1, .L80049F64
    andi       t1, a0, 0xF
    beqz       t1, .L80049F60
    addiu      t0, 0x1
    .L80049F60:
    ori        t1, zero, 0x400
    sub        t0, t1, t0
    .L80049F64:
    sll        t0, 2
    addu       t2, t0
    andi       t1, a0, 0x8000
    lwc1       fv0, 0x0(t2)
    beqz       t1, .L80049F7C
    neg.s      fv0
    .L80049F7C:
    jr         ra
END(Cosf)
#ifdef MODERN_CC
.size Sinf, . - Sinf
#endif

LEAF(Arctanf)
    mtc1       zero, fv0
    li         v0, 0
    c.eq.s     fa0, fv0
    bc1f       .L80049FA8
    c.eq.s     fa1, fv0
    bc1t       .L8004A078
    .L80049FA8:
    c.lt.s     fa0, fv0
    bc1t       .L80049FCC
    c.lt.s     fa1, fv0
    bc1f       .L80049FF8
    neg.s      fa1
    li         v0, 0x4000
    j          .L80049FDC
    .L80049FCC:
    c.lt.s     fa1, fv0
    neg.s      fa0
    bc1t       .L80049FF0
    li         v0, 0xC000
    .L80049FDC:
    mov.s      fa0f, fa0
    mov.s      fa0, fa1
    mov.s      fa1, fa0f
    b          .L80049FF4
    .L80049FF0:
    neg.s      fa1
    li         v0, 0x8000
    .L80049FF4:
    .L80049FF8:
    c.lt.s     fa0, fa1
    bc1f      .L8004A040
    div.s      ft3, fa0, fa1
    li.s       fv0f, 2048.0
    la         t0, gArcTanTable
    mul.s      ft3, fv0f
    cvt.w.s    ft3
    mfc1       t1, ft3
    andi       t1, 0xFFE
    add        t0, t1
    lh         t0, 0x0(t0)
    addu       v0, t0
    andi       v0, 0xFFFF
    jr         ra
    .L8004A040:
    div.s      ft3, fa1, fa0
    li.s       fv0f, 2048.0
    la         t0, gArcTanTable
    addiu      v0, 0x4000
    mul.s      ft3, fv0f
    cvt.w.s    ft3
    mfc1       t1, ft3
    andi       t1, 0xFFE
    add        t0, t1
    lh         t0, 0x0(t0)
    subu       v0, t0
    andi       v0, 0xFFFF
    .L8004A078:
    jr         ra
END(Arctanf)

.set reorder

/**
s16 mathDiffAngle(s16 angle1, s16 angle2) {
    s16 ret;

    ret = angle2 - angle1;
    if (ret < 0x8000) {
        if (ret > -0x8000) {
            return ret;
        } else {
            return ret + 1;
        }
    } else {
        return ret - 1;
    }
}
*/

/* s16 mathDiffAngle(s16 angle1, s16 angle2); */
LEAF(mathDiffAngle)
    lui        t0, 1
    sub        v0, a1, a0
    bge        v0,  0x8000, .greater_than_8000
    blt        v0, -0x7FFF, .not_greater_than_minus_8000
    jr         ra     /* Return v0 as is */
    .greater_than_8000:
    sub        v0, t0
    jr         ra     /* Return v0 - 1 */
    .not_greater_than_minus_8000:
    add        v0, t0
    jr         ra     /* Return v0 + 1 */
END(mathDiffAngle)
