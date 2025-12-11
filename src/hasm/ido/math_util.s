#ifdef MODERN_CC
.set gp=64
#endif

#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"
#include "asm_macros.h"

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
 * Official Name: mathMtxFastXFMF
 * Arguments:
 *   a0 = pointer to 4x4 matrix (float[4][4])
 *   a1 = pointer to input direction vector (float[3])
 *   a2 = pointer to output direction vector (float[3])
 */
LEAF(mathMtxFastXFMF)
    /* Load input vector components */
    lwc1       ft0, 0x0(a1)     /* x component */
    lwc1       ft1, 0x4(a1)     /* y component */
    lwc1       ft2, 0x8(a1)     /* z component */

    /* Load first column from matrix */
    lwc1       ft3, 0x0(a0)     /* mf[0][0] */
    lwc1       fa0, 0x10(a0)    /* mf[1][0] */
    lwc1       fa1, 0x20(a0)    /* mf[2][0] */

    /* Calculate result.x */
    mul.s      ft3, ft0, ft3    /* mf[0][0] * x */
    mul.s      fa0, ft1, fa0    /* mf[1][0] * y */
    mul.s      fa1, ft2, fa1    /* mf[2][0] * z */

    add.s      fa0, ft3, fa0
    add.s      ft4, fa0, fa1
    
    /* Load second column from matrix */
    lwc1       ft3, 0x4(a0)     /* mf[0][1] */
    lwc1       fa0, 0x14(a0)    /* mf[1][1] */
    lwc1       fa1, 0x24(a0)    /* mf[2][1] */

    /* Store result.x */
    swc1       ft4, 0x0(a2)

    /* Calculate result.y */
    mul.s      ft3, ft0, ft3    /* mf[0][1] * x */
    mul.s      fa0, ft1, fa0    /* mf[1][1] * y */
    mul.s      fa1, ft2, fa1    /* mf[2][1] * z */

    add.s      fa0, ft3, fa0
    add.s      ft4, fa0, fa1

    /* Load third column from matrix */
    lwc1       ft3, 0x8(a0)     /* mf[0][2] */
    lwc1       fa0, 0x18(a0)    /* mf[1][2] */
    lwc1       fa1, 0x28(a0)    /* mf[2][2] */

    /* Store result.y */
    swc1       ft4, 0x4(a2)

    /* Calculate result.z */
    mul.s      ft3, ft0, ft3    /* mf[0][2] * x */
    mul.s      fa0, ft1, fa0    /* mf[1][2] * y */
    mul.s      fa1, ft2, fa1    /* mf[2][2] * z */

    add.s      fa0, ft3, fa0
    add.s      fa1, fa0, fa1

    /* Store result.z */
    swc1       fa1, 0x8(a2)
    
    jr         ra
END(mathMtxFastXFMF)

/* Official Name: mathMtxCatF */
/* Multiplies two 4x4 floating-point matrices: result = m1 * m2 */
/* Arguments:
 *   a0 = pointer to first matrix (m1)
 *   a1 = pointer to second matrix (m2)
 *   a2 = pointer to result matrix
 */
LEAF(mathMtxCatF)
    addiu      sp, sp, -8
    sdc1       $f2, 0(sp)               /* Save $f2 register */
    ori        t0, zero, 4              /* Loop counter for 4 rows */
    
.mtxf_mul_row_loop:
    /* Load current row from first matrix (m1) */
    lwc1       $f2, 0x0(a0)             /* m1[row][0] */
    lwc1       $f4, 0x4(a0)             /* m1[row][1] */
    lwc1       $f6, 0x8(a0)             /* m1[row][2] */
    lwc1       $f8, 0xC(a0)             /* m1[row][3] */
    
    /* Load first column from second matrix (m2) */
    lwc1       $f10, 0x0(a1)            /* m2[0][0] */
    lwc1       $f12, 0x10(a1)           /* m2[1][0] */
    lwc1       $f14, 0x20(a1)           /* m2[2][0] */
    lwc1       $f16, 0x30(a1)           /* m2[3][0] */
    
    /* Calculate result[row][0] */
    mul.s      $f10, $f2, $f10          /* temp0 = m1[row][0] * m2[0][0] */
    mul.s      $f12, $f4, $f12          /* temp1 = m1[row][1] * m2[1][0] */
    mul.s      $f14, $f6, $f14          /* temp2 = m1[row][2] * m2[2][0] */
    mul.s      $f16, $f8, $f16          /* temp3 = m1[row][3] * m2[3][0] */
    add.s      $f14, $f12, $f14         /* temp2 = temp1 + temp2 */
    add.s      $f16, $f10, $f16         /* temp3 = temp0 + temp3 */
    add.s      $f18, $f14, $f16         /* result[row][0] = temp2 + temp3 */
    
    /* Load second column from second matrix (m2) */
    lwc1       $f10, 0x4(a1)            /* m2[0][1] */
    lwc1       $f12, 0x14(a1)           /* m2[1][1] */
    lwc1       $f14, 0x24(a1)           /* m2[2][1] */
    lwc1       $f16, 0x34(a1)           /* m2[3][1] */

    /* Store result[row][0] */
    swc1       $f18, 0x0(a2)

    /* Calculate result[row][1] */
    mul.s      $f10, $f2, $f10          /* temp0 = m1[row][0] * m2[0][1] */
    mul.s      $f12, $f4, $f12          /* temp1 = m1[row][1] * m2[1][1] */
    mul.s      $f14, $f6, $f14          /* temp2 = m1[row][2] * m2[2][1] */
    mul.s      $f16, $f8, $f16          /* temp3 = m1[row][3] * m2[3][1] */
    add.s      $f14, $f12, $f14         /* temp2 = temp1 + temp2 */
    add.s      $f16, $f10, $f16         /* temp3 = temp0 + temp3 */
    add.s      $f18, $f14, $f16         /* result[row][1] = temp2 + temp3 */
    
    /* Load third column from second matrix (m2) */
    lwc1       $f10, 0x08(a1)           /* m2[0][2] */
    lwc1       $f12, 0x18(a1)           /* m2[1][2] */
    lwc1       $f14, 0x28(a1)           /* m2[2][2] */
    lwc1       $f16, 0x38(a1)           /* m2[3][2] */

    /* Store result[row][1] */
    swc1       $f18, 0x4(a2)

    /* Calculate result[row][2] */
    mul.s      $f10, $f2, $f10          /* temp0 = m1[row][0] * m2[0][2] */
    mul.s      $f12, $f4, $f12          /* temp1 = m1[row][1] * m2[1][2] */
    mul.s      $f14, $f6, $f14          /* temp2 = m1[row][2] * m2[2][2] */
    mul.s      $f16, $f8, $f16          /* temp3 = m1[row][3] * m2[3][2] */
    add.s      $f14, $f12, $f14         /* temp2 = temp1 + temp2 */
    add.s      $f16, $f10, $f16         /* temp3 = temp0 + temp3 */
    add.s      $f18, $f14, $f16         /* result[row][2] = temp2 + temp3 */

    /* Load last column from second matrix (m2) */
    lwc1       $f10, 0x0C(a1)           /* m2[0][3] */
    lwc1       $f12, 0x1C(a1)           /* m2[1][3] */
    lwc1       $f14, 0x2C(a1)           /* m2[2][3] */
    lwc1       $f16, 0x3C(a1)           /* m2[3][3] */

    /* Store result[row][2] */
    swc1       $f18, 0x08(a2)
    
    /* Calculate result[row][3] */
    mul.s      $f10, $f2, $f10          /* temp0 = m1[row][0] * m2[0][3] */
    mul.s      $f12, $f4, $f12          /* temp1 = m1[row][1] * m2[1][3] */
    mul.s      $f14, $f6, $f14          /* temp2 = m1[row][2] * m2[2][3] */
    mul.s      $f16, $f8, $f16          /* temp3 = m1[row][3] * m2[3][3] */
    add.s      $f14, $f12, $f14         /* temp2 = temp1 + temp2 */
    add.s      $f16, $f10, $f16         /* temp3 = temp0 + temp3 */
    add.s      $f18, $f14, $f16         /* result[row][3] = temp2 + temp3 */

    /* Store result[row][3] */
    swc1       $f18, 0xC(a2)
    
    /* Move to next row */
    addiu      a0, 0x10                 /* Advance m1 pointer to next row */
    addiu      a2, 0x10                 /* Advance result pointer to next row */
    addiu      t0, -1                   /* Decrement loop counter */
    bnezl      t0, .mtxf_mul_row_loop
    
    ldc1       $f2, 0(sp)               /* Restore $f2 register */
    addiu      sp, sp, 8
    jr         ra
END(mathMtxCatF)

/* Official Name: mathMtxF2L */
/* Converts a 4x4 floating-point matrix to fixed-point integer matrix */
/* Arguments:
 *   a0 = pointer to source floating-point matrix (MtxF)
 *   a1 = pointer to destination fixed-point matrix (Mtx)
 * The function converts float values to 16.16 fixed-point format
 * and stores them in the N64s matrix format (split high/low words)
 */
LEAF(mathMtxF2L)
    ori        t0, zero, 4             /* Loop counter for 4 rows */
    li.s       fa0, 65536.0            /* Scaling factor to convert to 16.16 fixed-point */
    
.mtxf_to_mtx_row_loop:
    /* Load 4 float values from current row */
    lwc1       ft0, 0x0(a0)            /* Load element [row][0] */
    lwc1       ft1, 0x4(a0)            /* Load element [row][1] */
    lwc1       ft2, 0x8(a0)            /* Load element [row][2] */
    lwc1       ft3, 0xC(a0)            /* Load element [row][3] */

    /* Scale floats to fixed-point by multiplying by 65536.0 */
    mul.s      ft0, fa0                /* Scale [row][0] */
    mul.s      ft1, fa0                /* Scale [row][1] */
    mul.s      ft2, fa0                /* Scale [row][2] */
    mul.s      ft3, fa0                /* Scale [row][3] */

    /* Convert scaled floats to 32-bit integers */
    cvt.w.s    ft0                     /* Convert to word (integer) */
    cvt.w.s    ft1
    cvt.w.s    ft2
    cvt.w.s    ft3

    /* Move converted integers to general-purpose registers */
    mfc1       t1, ft0                 /* Get integer value of [row][0] */
    mfc1       t2, ft1                 /* Get integer value of [row][1] */
    mfc1       t3, ft2                 /* Get integer value of [row][2] */
    mfc1       t4, ft3                 /* Get integer value of [row][3] */
    
    /* Store fractional parts (low 16 bits) in second half of Mtx */
    sh         t1, 0x20(a1)            /* Store low 16 bits of [row][0] */
    sh         t2, 0x22(a1)            /* Store low 16 bits of [row][1] */
    sh         t3, 0x24(a1)            /* Store low 16 bits of [row][2] */
    sh         t4, 0x26(a1)            /* Store low 16 bits of [row][3] */
    
    /* Extract integer parts (high 16 bits) */
    srl        t1, 16                  /* Shift right to get high 16 bits */
    srl        t2, 16
    srl        t3, 16
    srl        t4, 16
    
    /* Store integer parts (high 16 bits) in first half of Mtx */
    sh         t1, 0(a1)               /* Store high 16 bits of [row][0] */
    sh         t2, 0x2(a1)             /* Store high 16 bits of [row][1] */
    sh         t3, 0x4(a1)             /* Store high 16 bits of [row][2] */
    sh         t4, 0x6(a1)             /* Store high 16 bits of [row][3] */
    
    /* Advance pointers to next row */
    addiu      a0, 0x10                /* Advance MtxF pointer to next row (16 bytes) */
    addiu      a1, 0x8                 /* Advance Mtx pointer by 8 bytes (interleaved format) */
    addiu      t0, -1                  /* Decrement loop counter */
    bnezl      t0, .mtxf_to_mtx_row_loop

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

/* Official Name: fastShortReflection
 * Reflects a vector across a given normal.
 * Fixed-point notes:
 *   - Inputs are 16-bit signed fixed-point values.
 *   - Dot product is accumulated in 32-bit, then shifted right by 12 to rescale.
 *   - Multiplication by N components is then shifted right by 13, giving final scale.
 * Arguments:
 *   a0 = pointer to vector struct containing:
 *        - input incident vector [x, y, z] at offsets 0x0, 0x2, 0x4
 *        - output reflected vector will be stored at offsets 0x6, 0x8, 0xA
 *   a1 = pointer to surface normal vector [nx, ny, nz] at offsets 0x0, 0x2, 0x4
 */
LEAF(fastShortReflection)
    /* Load incident vector components (16-bit signed) */
    lh         t0, 0x0(a0)              /* incident.x */
    lh         t1, 0x2(a0)              /* incident.y */
    lh         t2, 0x4(a0)              /* incident.z */
    
    /* Load normal vector components (16-bit signed) */
    lh         t3, 0x0(a1)              /* normal.x */
    lh         t4, 0x2(a1)              /* normal.y */
    lh         t5, 0x4(a1)              /* normal.z */
    
    /* Calculate dot product: dot = (incident.x * normal.x) + (incident.y * normal.y) + (incident.z * normal.z) */
    mult       t0, t3                   /* incident.x * normal.x */
    mflo       t6                       /* dot = result */
    
    mult       t1, t4                   /* incident.y * normal.y */
    mflo       t7                       /* temp = result */
    add        t6, t7                   /* dot += temp */
    
    mult       t2, t5                   /* incident.z * normal.z */
    mflo       t8                       /* temp = result */
    add        t6, t8                   /* dot += temp */
    
    /* Scale down accumulated dot product (fixed-point normalization) */
    sra        t6, 12                   /* dot >>= 12 */
    
    /* Calculate reflected.x = (dot * normal.x >> 13) - incident.x */
    mult       t6, t3                   /* (dot * normal.x) */
    mflo       t3                       /* scaled_normal_x = result */
    sra        t3, 13                   /* scaled_normal_x >>= 13 */
    sub        t3, t3, t0               /* reflected.x = scaled_normal_x - incident.x */
    sh         t3, 0x6(a0)              /* Store reflected.x */
    
    /* Calculate reflected.y = (dot * normal.y >> 13) - incident.y */
    mult       t6, t4                   /* (dot * normal.y) */
    mflo       t4                       /* scaled_normal_y = result */
    sra        t4, 13                   /* scaled_normal_y >>= 13 */
    sub        t4, t4, t1               /* reflected.y = scaled_normal_y - incident.y */
    sh         t4, 0x8(a0)              /* Store reflected.y */
    
    /* Calculate reflected.z = (dot * normal.z >> 13) - incident.z */
    mult       t6, t5                   /* (dot * normal.z) */
    mflo       t5                       /* scaled_normal_z = result */
    sra        t5, 13                   /* scaled_normal_z >>= 13 */
#ifdef AVOID_UB
    sub        t5, t5, t2               /* reflected.z = scaled_normal_z - incident.z */
#else
    sub        t5, t5, t0               /* !@bug: should subtract incident.z (t2), not incident.x (t0) */
#endif
    sh         t5, 0xA(a0)              /* Store reflected.z */
    
    jr         ra
END(fastShortReflection)

/**
 * Scales the Y axis of the given 4×4 transformation matrix by the specified factor.
 * If this is a model matrix, the operation is equivalent to stretching or squashing
 * the model along its local Y axis.
 *
 * Official Name: mathSquashY
 *
 * Arguments:
 *   a0 = pointer to 4×4 float matrix (MtxF)
 *   a1 = scale factor (passed as raw float bits in integer register)
 *
 * Operation:
 *   mtx[1][0] *= scale
 *   mtx[1][1] *= scale
 *   mtx[1][2] *= scale
 */
LEAF(mathSquashY)
    mtc1       a1, ft5                  /* ft5 = scale factor */

    /* Scale mtx[1][0] */
    lwc1       ft4, 0x10(a0)            /* ft4 = mtx[1][0] */
    mul.s      ft4, ft5                 /* ft4 *= scale */
    swc1       ft4, 0x10(a0)            /* store mtx[1][0] */

    /* Scale mtx[1][1] */
    lwc1       ft4, 0x14(a0)            /* ft4 = mtx[1][1] */
    mul.s      ft4, ft5                 /* ft4 *= scale */
    swc1       ft4, 0x14(a0)            /* store mtx[1][1] */

    /* Scale mtx[1][2] */
    lwc1       ft4, 0x18(a0)            /* ft4 = mtx[1][2] */
    mul.s      ft4, ft5                 /* ft4 *= scale */
    swc1       ft4, 0x18(a0)            /* store mtx[1][2] */

    jr         ra
END(mathSquashY)

/**
 * Modifies the matrix by translating its position along the local Y axis.
 * If this is a model matrix, the operation is equivalent to moving the model
 * along its local Y axis in model space.
 *
 * Official Name: mathTransY
 *
 * Arguments:
 *   a0 = pointer to 4×4 float matrix (MtxF)
 *   a1 = offset (passed as raw float bits in integer register)
 *
 * Operation:
 *   mtx[3][0] += mtx[1][0] * offset
 *   mtx[3][1] += mtx[1][1] * offset
 *   mtx[3][2] += mtx[1][2] * offset
 *
 * This moves the translation component of the matrix along the local Y axis
 * direction by the specified offset amount.
 */
LEAF(mathTransY)
    mtc1       a1, ft4                  /* ft4 = offset */

    /* Load Y-axis basis vector (row 1) */
    lwc1       fv0, 0x10(a0)            /* fv0 = mtx[1][0] */
    lwc1       ft0, 0x14(a0)            /* ft0 = mtx[1][1] */
    lwc1       ft2, 0x18(a0)            /* ft2 = mtx[1][2] */

    /* Load current translation (row 3) */
    lwc1       fv1, 0x30(a0)            /* fv1 = mtx[3][0] */
    lwc1       ft1, 0x34(a0)            /* ft1 = mtx[3][1] */
    lwc1       ft3, 0x38(a0)            /* ft3 = mtx[3][2] */

    /* Scale Y-axis by offset */
    mul.s      fv0, ft4                 /* fv0 = mtx[1][0] * offset */
    mul.s      ft0, ft4                 /* ft0 = mtx[1][1] * offset */
    mul.s      ft2, ft4                 /* ft2 = mtx[1][2] * offset */

    /* Add scaled Y-axis to translation */
    add.s      fv0, fv1                 /* fv0 = mtx[3][0] + mtx[1][0] * offset */
    add.s      ft0, ft1                 /* ft0 = mtx[3][1] + mtx[1][1] * offset */
    add.s      ft2, ft3                 /* ft2 = mtx[3][2] + mtx[1][2] * offset */

    /* Store updated translation */
    swc1       fv0, 0x30(a0)            /* mtx[3][0] = result */
    swc1       ft0, 0x34(a0)            /* mtx[3][1] = result */
    swc1       ft2, 0x38(a0)            /* mtx[3][2] = result */

    jr         ra
END(mathTransY)

/**
 * Builds an inverse 4×4 floating-point transformation matrix from an ObjectTransform struct.
 *
 * This function constructs the inverse of a rotation matrix from Euler angles (roll, pitch, yaw)
 * and applies the inverse translation. This is useful for transforming world-space coordinates
 * into object-local space (e.g., for camera view matrices).
 *
 * Official Name: mathRpyXyzMtx
 *
 * Arguments:
 *   a0 = pointer to destination 4×4 float matrix (MtxF)
 *   a1 = pointer to ObjectTransform struct:
 *          0x00: s16 rotation.x (roll)
 *          0x02: s16 rotation.y (pitch)
 *          0x04: s16 rotation.z (yaw)
 *          0x06: s16 (padding)
 *          0x08: f32 scale (UNUSED in this function)
 *          0x0C: f32 position.x
 *          0x10: f32 position.y
 *          0x14: f32 position.z
 *
 * The inverse rotation is computed by transposing the rotation matrix.
 * The inverse translation is: R^T * position (not negated here)
 *
 * Register usage after trig calculations:
 *   fv0 = sinRoll,  fv1 = cosRoll
 *   ft0 = sinPitch, ft1 = cosPitch
 *   ft2 = sinYaw,   ft3 = cosYaw
 *
 * Matrix layout (row-major):
 *   [ m00 m01 m02  0  ]   row 0
 *   [ m10 m11 m12  0  ]   row 1
 *   [ m20 m21 m22  0  ]   row 2
 *   [ tx  ty  tz   1  ]   row 3 (inverse translation + homogeneous)
 */
LEAF(mathRpyXyzMtx)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)

    move       a3, a0                   /* a3 = dest matrix pointer */
    li.s       ft5, 0.0000152587890625  /* ft5 = 1.0f / 0x10000 (s16 to float scale) */

    /* -----------------------------------------
     * Compute sin/cos for roll (rotation.x)
     * ----------------------------------------- */
    lh         a0, 0x0(a1)              /* a0 = transform->rotation.x (roll angle) */
    jal        mathSinInterp            /* v0 = sin(roll) as s16 */
    mtc1       v0, fv0                  /* fv0 = sin(roll) as int bits */
    cvt.s.w    fv0                      /* fv0 = (float)sin(roll) */
    mul.s      fv0, ft5                 /* fv0 = sinRoll (normalized) */
    lh         a0, 0x0(a1)              /* a0 = transform->rotation.x */
    jal        mathCosInterp            /* v0 = cos(roll) as s16 */
    mtc1       v0, fv1                  /* fv1 = cos(roll) as int bits */
    cvt.s.w    fv1                      /* fv1 = (float)cos(roll) */
    mul.s      fv1, ft5                 /* fv1 = cosRoll (normalized) */

    /* -----------------------------------------
     * Compute sin/cos for pitch (rotation.y)
     * ----------------------------------------- */
    lh         a0, 0x2(a1)              /* a0 = transform->rotation.y (pitch angle) */
    jal        mathSinInterp                 /* v0 = sin(pitch) as s16 */
    mtc1       v0, ft0                  /* ft0 = sin(pitch) as int bits */
    cvt.s.w    ft0                      /* ft0 = (float)sin(pitch) */
    mul.s      ft0, ft5                 /* ft0 = sinPitch (normalized) */
    lh         a0, 0x2(a1)              /* a0 = transform->rotation.y */
    jal        mathCosInterp            /* v0 = cos(pitch) as s16 */
    mtc1       v0, ft1                  /* ft1 = cos(pitch) as int bits */
    cvt.s.w    ft1                      /* ft1 = (float)cos(pitch) */
    mul.s      ft1, ft5                 /* ft1 = cosPitch (normalized) */

    /* -----------------------------------------
     * Compute sin/cos for yaw (rotation.z)
     * ----------------------------------------- */
    lh         a0, 0x4(a1)              /* a0 = transform->rotation.z (yaw angle) */
    jal        mathSinInterp            /* v0 = sin(yaw) as s16 */
    mtc1       v0, ft2                  /* ft2 = sin(yaw) as int bits */
    cvt.s.w    ft2                      /* ft2 = (float)sin(yaw) */
    mul.s      ft2, ft5                 /* ft2 = sinYaw (normalized) */
    lh         a0, 0x4(a1)              /* a0 = transform->rotation.z */
    jal        mathCosInterp            /* v0 = cos(yaw) as s16 */
    mtc1       v0, ft3                  /* ft3 = cos(yaw) as int bits */
    cvt.s.w    ft3                      /* ft3 = (float)cos(yaw) */
    mul.s      ft3, ft5                 /* ft3 = cosYaw (normalized) */

    /* -----------------------------------------
     * Build inverse rotation matrix (transpose of forward rotation)
     * 
     * Register usage at this point:
     *   fv0 = sinRoll,  fv1 = cosRoll
     *   ft0 = sinPitch, ft1 = cosPitch
     *   ft2 = sinYaw,   ft3 = cosYaw
     * ----------------------------------------- */

    /* Clear perspective column and set m[1][2] = sinPitch */
    sw         zero, 0xC(a3)            /* m[0][3] = 0.0 */
    swc1       ft0, 0x18(a3)            /* m[1][2] = sinPitch */
    sw         zero, 0x1C(a3)           /* m[1][3] = 0.0 */
    sw         zero, 0x2C(a3)           /* m[2][3] = 0.0 */

    /* -----------------------------------------
     * m[0][0] = cosRoll * cosYaw - sinPitch * sinYaw * sinRoll
     * ----------------------------------------- */
    mul.s      ft4, ft0, ft2            /* ft4 = sinPitch * sinYaw */
    mul.s      ft4, fv0                 /* ft4 = sinPitch * sinYaw * sinRoll */
    mul.s      ft5, fv1, ft3            /* ft5 = cosRoll * cosYaw */
    sub.s      ft4, ft5, ft4            /* ft4 = cosRoll*cosYaw - sinPitch*sinYaw*sinRoll */
    swc1       ft4, 0x0(a3)             /* m[0][0] = result */

    /* -----------------------------------------
     * m[0][1] = sinPitch * cosYaw * sinRoll + cosRoll * sinYaw
     * ----------------------------------------- */
    mul.s      ft4, ft0, ft3            /* ft4 = sinPitch * cosYaw */
    mul.s      ft4, fv0                 /* ft4 = sinPitch * cosYaw * sinRoll */
    mul.s      ft5, fv1, ft2            /* ft5 = cosRoll * sinYaw */
    add.s      ft4, ft5                 /* ft4 = sinPitch*cosYaw*sinRoll + cosRoll*sinYaw */
    swc1       ft4, 0x4(a3)             /* m[0][1] = result */

    /* -----------------------------------------
     * m[0][2] = -sinRoll * cosPitch
     * ----------------------------------------- */
    mul.s      ft4, fv0, ft1            /* ft4 = sinRoll * cosPitch */
    neg.s      ft4                      /* ft4 = -sinRoll * cosPitch */
    swc1       ft4, 0x8(a3)             /* m[0][2] = result */

    /* -----------------------------------------
     * m[1][0] = -cosPitch * sinYaw
     * ----------------------------------------- */
    mul.s      ft4, ft1, ft2            /* ft4 = cosPitch * sinYaw */
    neg.s      ft4                      /* ft4 = -cosPitch * sinYaw */
    swc1       ft4, 0x10(a3)            /* m[1][0] = result */

    /* -----------------------------------------
     * m[1][1] = cosPitch * cosYaw
     * ----------------------------------------- */
    mul.s      ft4, ft1, ft3            /* ft4 = cosPitch * cosYaw */
    swc1       ft4, 0x14(a3)            /* m[1][1] = result */

    /* -----------------------------------------
     * m[2][0] = sinPitch * sinYaw * cosRoll + sinRoll * cosYaw
     * ----------------------------------------- */
    mul.s      ft4, ft0, ft2            /* ft4 = sinPitch * sinYaw */
    mul.s      ft4, fv1                 /* ft4 = sinPitch * sinYaw * cosRoll */
    mul.s      ft5, fv0, ft3            /* ft5 = sinRoll * cosYaw */
    add.s      ft4, ft5                 /* ft4 = sinPitch*sinYaw*cosRoll + sinRoll*cosYaw */
    swc1       ft4, 0x20(a3)            /* m[2][0] = result */

    /* -----------------------------------------
     * m[2][1] = sinRoll * sinYaw - sinPitch * cosYaw * cosRoll
     * ----------------------------------------- */
    mul.s      ft4, ft0, ft3            /* ft4 = sinPitch * cosYaw */
    mul.s      ft4, fv1                 /* ft4 = sinPitch * cosYaw * cosRoll */
    mul.s      ft5, fv0, ft2            /* ft5 = sinRoll * sinYaw */
    sub.s      ft4, ft5, ft4            /* ft4 = sinRoll*sinYaw - sinPitch*cosYaw*cosRoll */
    swc1       ft4, 0x24(a3)            /* m[2][1] = result */

    /* -----------------------------------------
     * m[2][2] = cosRoll * cosPitch
     * ----------------------------------------- */
    mul.s      ft4, fv1, ft1            /* ft4 = cosRoll * cosPitch */
    swc1       ft4, 0x28(a3)            /* m[2][2] = result */

    /* -----------------------------------------
     * Load X / Y / Z into fv0, fv1, ft0
     * ----------------------------------------- */
    lwc1       fv0, 0xC(a1)             /* fv0 = position.x */
    lwc1       fv1, 0x10(a1)            /* fv1 = position.y */
    lwc1       ft0, 0x14(a1)            /* ft0 = position.z */

    /* Compute translation x: tx = m[0][0]*pos.x + m[1][0]*pos.y + m[2][0]*pos.z */
    lwc1       ft5, 0x0(a3)             /* ft5 = m[0][0] */
    lwc1       ft4, 0x10(a3)            /* ft4 = m[1][0] */
    mul.s      ft5, fv0                 /* ft5 = m[0][0] * position.x */
    mul.s      ft4, fv1                 /* ft4 = m[1][0] * position.y */
    add.s      ft4, ft5                 /* ft4 = m[0][0]*pos.x + m[1][0]*pos.y */
    lwc1       ft5, 0x20(a3)            /* ft5 = m[2][0] */
    mul.s      ft5, ft0                 /* ft5 = m[2][0] * position.z */
    add.s      ft4, ft5                 /* ft4 = m[0][0]*pos.x + m[1][0]*pos.y + m[2][0]*pos.z */
    swc1       ft4, 0x30(a3)            /* m[3][0] = translation x */

    /* Compute translation y: ty = m01*pos.x + m11*pos.y + m21*pos.z */
    lwc1       ft5, 0x4(a3)             /* ft5 = m[0][1] */
    lwc1       ft4, 0x14(a3)            /* ft4 = m[1][1] */
    mul.s      ft5, fv0                 /* ft5 = m[0][1] * position.x */
    mul.s      ft4, fv1                 /* ft4 = m[1][1] * position.y */
    add.s      ft4, ft5                 /* ft4 = m[0][1]*pos.x + m[1][1]*pos.y */
    lwc1       ft5, 0x24(a3)            /* ft5 = m[2][1] */
    mul.s      ft5, ft0                 /* ft5 = m[2][1] * position.z */
    add.s      ft4, ft5                 /* ft4 = m[0][1]*pos.x + m[1][1]*pos.y + m[2][1]*pos.z */
    swc1       ft4, 0x34(a3)            /* m[3][1] = translation y */

    /* Compute translation z: tz = m02*pos.x + m12*pos.y + m22*pos.z */
    lwc1       ft5, 0x8(a3)             /* ft5 = m[0][2] */
    lwc1       ft4, 0x18(a3)            /* ft4 = m[1][2] */
    mul.s      ft5, fv0                 /* ft5 = m[0][2] * position.x */
    mul.s      ft4, fv1                 /* ft4 = m[1][2] * position.y */
    add.s      ft4, ft5                 /* ft4 = m[0][2]*pos.x + m[1][2]*pos.y */
    lwc1       ft5, 0x28(a3)            /* ft5 = m[2][2] */
    mul.s      ft5, ft0                 /* ft5 = m[2][2] * position.z */
    add.s      ft4, ft5                 /* ft4 = m[0][2]*pos.x + m[1][2]*pos.y + m[2][2]*pos.z */
    swc1       ft4, 0x38(a3)            /* m[3][2] = translation z */

    li.s       ft4, 1.0
    swc1       ft4, 0x3C(a3)            /* m[3][3] = 1.0 */

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

/**
 * Rotates a 3D floating-point vector by Euler angles in RPY (Roll-Pitch-Yaw) order.
 *
 * Official Name: mathOneFloatRPY
 *
 * The rotation is applied in the order: Yaw (Z) -> Pitch (Y) -> Roll (X)
 * Each rotation is a 2D rotation in the corresponding plane.
 *
 * Arguments:
 *   a0 = pointer to Vec3s rotation angles:
 *          0x00: s16 roll  (rotation around X axis)
 *          0x02: s16 pitch (rotation around Y axis)
 *          0x04: s16 yaw   (rotation around Z axis)
 *   a1 = pointer to Vec3f vector (in/out):
 *          0x00: f32 x
 *          0x04: f32 y
 *          0x08: f32 z
 *
 * Math:
 *   First apply yaw (Z rotation):
 *     x1 = x * cos(yaw) - y * sin(yaw)
 *     y1 = y * cos(yaw) + x * sin(yaw)
 *   Then apply pitch (Y rotation):
 *     y2 = y1 * cos(pitch) - z * sin(pitch)
 *     z1  = z * cos(pitch) + y1 * sin(pitch)
 *   Finally apply roll (X rotation):
 *     x2 = x1 * cos(roll) + z1 * sin(roll)
 *     z2 = z1 * cos(roll) - x1 * sin(roll)
 */
LEAF(mathOneFloatRPY)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    move       a2, a0                   /* a2 = rotation angles pointer */

    /* Load input vector components */
    lwc1       ft0, 0x0(a1)             /* ft0 = x */
    lwc1       ft1, 0x4(a1)             /* ft1 = y */
    lwc1       ft2, 0x8(a1)             /* ft2 = z */

    /* -----------------------------------------
     * Apply yaw rotation (around Z axis)
     * x1 = x*cos - y*sin
     * y1 = y*cos + x*sin
     * ----------------------------------------- */
    lh         a0, 0x4(a2)              /* a0 = yaw angle */
    jal        Sinf                     /* fv0 = sin(yaw) */
    mul.s      ft3, ft0, fv0            /* ft3 = x * sin(yaw) */
    mul.s      fa0, ft1, fv0            /* fa0 = y * sin(yaw) */
    jal        Cosf                     /* fv0 = cos(yaw) */
    mul.s      ft0, ft0, fv0            /* ft0 = x * cos(yaw) */
    sub.s      ft0, ft0, fa0            /* ft0 = x1 = x*cos - y*sin */
    mul.s      ft1, ft1, fv0            /* ft1 = y * cos(yaw) */
    add.s      ft1, ft1, ft3            /* ft1 = y1 = y*cos + x*sin */

    /* -----------------------------------------
     * Apply pitch rotation (around Y axis)
     * y2 = y1*cos - z*sin
     * z1 = z*cos + y1*sin
     * ----------------------------------------- */
    lh         a0, 0x2(a2)              /* a0 = pitch angle */
    jal        Sinf                     /* fv0 = sin(pitch) */
    mul.s      ft3, ft1, fv0            /* ft3 = y1 * sin(pitch) */
    mul.s      fa0, ft2, fv0            /* fa0 = z * sin(pitch) */
    jal        Cosf                     /* fv0 = cos(pitch) */
    mul.s      ft1, ft1, fv0            /* ft1 = y1 * cos(pitch) */
    sub.s      ft1, ft1, fa0            /* ft1 = y2 = y1*cos - z*sin */
    mul.s      ft2, ft2, fv0            /* ft2 = z * cos(pitch) */
    add.s      ft2, ft2, ft3            /* ft2 = z1 = z*cos + y1*sin */

    /* -----------------------------------------
     * Apply roll rotation (around X axis)
     * x2 = x1*cos + z1*sin
     * z2 = z1*cos - x1*sin
     * ----------------------------------------- */
    lh         a0, 0x0(a2)              /* a0 = roll angle */
    jal        Sinf                     /* fv0 = sin(roll) */
    mul.s      ft3, ft0, fv0            /* ft3 = x1 * sin(roll) */
    mul.s      fa0, ft2, fv0            /* fa0 = z1 * sin(roll) */
    jal        Cosf                     /* fv0 = cos(roll) */
    mul.s      ft0, ft0, fv0            /* ft0 = x1 * cos(roll) */
    add.s      ft0, ft0, fa0            /* ft0 = x2 = x1*cos + z1*sin */
    mul.s      ft2, ft2, fv0            /* ft2 = z1 * cos(roll) */
    sub.s      ft2, ft2, ft3            /* ft2 = z2 = z1*cos - x1*sin */

    /* Store rotated vector */
    swc1       ft0, 0x0(a1)             /* store x2 */
    swc1       ft1, 0x4(a1)             /* store y2 */
    swc1       ft2, 0x8(a1)             /* store z2 */

    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatRPY)

/**
 * Rotates a 3D floating-point vector by Euler angles in YPR (Yaw-Pitch-Roll) order.
 *
 * Official Name: mathOneFloatYPR
 *
 * The rotation is applied in the order: Roll (X) -> Pitch (Y) -> Yaw (Z)
 * This is the reverse application order compared to vec3f_rotate (RPY).
 *
 * Arguments:
 *   a0 = pointer to Vec3s rotation angles:
 *          0x00: s16 roll  (rotation around X axis)
 *          0x02: s16 pitch (rotation around Y axis)
 *          0x04: s16 yaw   (rotation around Z axis)
 *   a1 = pointer to Vec3f vector (in/out):
 *          0x00: f32 x
 *          0x04: f32 y
 *          0x08: f32 z
 *
 * Math:
 *   First apply roll (X rotation):
 *     x1 = x * cos(roll) + z * sin(roll)
 *     z1 = z * cos(roll) - x * sin(roll)
 *   Then apply pitch (Y rotation):
 *     y1 = y * cos(pitch) - z1 * sin(pitch)
 *     z2 = z1 * cos(pitch) + y * sin(pitch)
 *   Finally apply yaw (Z rotation):
 *     x2 = x1 * cos(yaw) - y1 * sin(yaw)
 *     y2 = y1 * cos(yaw) + x1 * sin(yaw)
 */
LEAF(mathOneFloatYPR)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    move       a2, a0                   /* a2 = rotation angles pointer */

    /* Load input vector components */
    lwc1       ft0, 0x0(a1)             /* ft0 = x */
    lwc1       ft1, 0x4(a1)             /* ft1 = y */
    lwc1       ft2, 0x8(a1)             /* ft2 = z */

    /* -----------------------------------------
     * Apply roll rotation (around X axis)
     * x1 = x*cos + z*sin
     * z1 = z*cos - x*sin
     * ----------------------------------------- */
    lh         a0, 0x0(a2)              /* a0 = roll angle */
    jal        Sinf                     /* fv0 = sin(roll) */
    mul.s      ft3, ft0, fv0            /* ft3 = x * sin(roll) */
    mul.s      fa0, ft2, fv0            /* fa0 = z * sin(roll) */
    jal        Cosf                     /* fv0 = cos(roll) */
    mul.s      ft0, fv0                 /* ft0 = x * cos(roll) */
    add.s      ft0, fa0                 /* ft0 = x1 = x*cos + z*sin */
    mul.s      ft2, fv0                 /* ft2 = z * cos(roll) */
    sub.s      ft2, ft3                 /* ft2 = z1 = z*cos - x*sin */

    /* -----------------------------------------
     * Apply pitch rotation (around Y axis)
     * y1 = y*cos - z1*sin
     * z2 = z1*cos + y*sin
     * ----------------------------------------- */
    lh         a0, 0x2(a2)              /* a0 = pitch angle */
    jal        Sinf                     /* fv0 = sin(pitch) */
    mul.s      ft3, ft1, fv0            /* ft3 = y * sin(pitch) */
    mul.s      fa0, ft2, fv0            /* fa0 = z1 * sin(pitch) */
    jal        Cosf                     /* fv0 = cos(pitch) */
    mul.s      ft1, fv0                 /* ft1 = y * cos(pitch) */
    sub.s      ft1, fa0                 /* ft1 = y1 = y*cos - z1*sin */
    mul.s      ft2, fv0                 /* ft2 = z1 * cos(pitch) */
    add.s      ft2, ft3                 /* ft2 = z2 = z1*cos + y*sin */

    /* -----------------------------------------
     * Apply yaw rotation (around Z axis)
     * x2 = x1*cos - y1*sin
     * y2 = y1*cos + x1*sin
     * ----------------------------------------- */
    lh         a0, 0x4(a2)              /* a0 = yaw angle */
    jal        Sinf                     /* fv0 = sin(yaw) */
    mul.s      ft3, ft0, fv0            /* ft3 = x1 * sin(yaw) */
    mul.s      fa0, ft1, fv0            /* fa0 = y1 * sin(yaw) */
    jal        Cosf                     /* fv0 = cos(yaw) */
    mul.s      ft0, fv0                 /* ft0 = x1 * cos(yaw) */
    sub.s      ft0, fa0                 /* ft0 = x2 = x1*cos - y1*sin */
    mul.s      ft1, fv0                 /* ft1 = y1 * cos(yaw) */
    add.s      ft1, ft3                 /* ft1 = y2 = y1*cos + x1*sin */

    /* Store rotated vector */
    swc1       ft0, 0x0(a1)             /* store x2 */
    swc1       ft1, 0x4(a1)             /* store y2 */
    swc1       ft2, 0x8(a1)             /* store z2 */

    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatYPR)

/**
 * Rotates a 3D floating-point vector by Pitch and Yaw angles only.
 *
 * Official Name: mathOneFloatPY
 *
 * This is a simplified rotation function that only uses the Z input component
 * to generate a direction vector based on pitch and yaw angles. The input X and Y
 * components are ignored.
 *
 * Arguments:
 *   a0 = pointer to Vec3s rotation angles:
 *          0x00: s16 roll  (rotation around X axis) - used for final X/Z split
 *          0x02: s16 pitch (rotation around Y axis) - controls Y component
 *          0x04: s16 yaw   (unused in this function)
 *   a1 = pointer to Vec3f vector (in/out):
 *          0x00: f32 x (output only)
 *          0x04: f32 y (output only)
 *          0x08: f32 z (input: magnitude, output: transformed z)
 *
 * Math:
 *   First apply pitch (Y rotation) to get Y component:
 *     y = -(z * sin(pitch))
 *     z1 = z * cos(pitch)
 *   Then apply roll to split z1 into X and Z:
 *     x = z1 * sin(roll)
 *     z2 = z1 * cos(roll)
 *
 * This effectively converts a Z-magnitude into a direction vector based on
 * pitch and roll angles, useful for aiming or facing direction calculations.
 */
LEAF(mathOneFloatPY)
    addiu      sp, sp, -0x8
    sd         ra, 0x0(sp)
    move       a2, a0                   /* a2 = rotation angles pointer */

    /* Load input Z component (used as magnitude) */
    lwc1       ft2, 0x8(a1)             /* ft2 = z (input magnitude) */

    /* -----------------------------------------
     * Apply pitch rotation (around Y axis)
     * y = -z * sin(pitch)
     * z1 = z * cos(pitch)
     * ----------------------------------------- */
    lh         a0, 0x2(a2)              /* a0 = pitch angle */
    jal        Sinf                     /* fv0 = sin(pitch) */
    mul.s      ft1, ft2, fv0            /* ft1 = z * sin(pitch) */
    jal        Cosf                     /* fv0 = cos(pitch) */
    neg.s      ft1                      /* ft1 = y = -z * sin(pitch) */
    mul.s      ft2, fv0                 /* ft2 = z1 = z * cos(pitch) */

    /* -----------------------------------------
     * Apply roll rotation to split z1 into X and Z
     * x = z1 * sin(roll)
     * z2 = z1 * cos(roll)
     * ----------------------------------------- */
    lh         a0, 0x0(a2)              /* a0 = roll angle */
    jal        Sinf                     /* fv0 = sin(roll) */
    mul.s      ft0, ft2, fv0            /* ft0 = x = z1 * sin(roll) */
    jal        Cosf                     /* fv0 = cos(roll) */
    mul.s      ft2, fv0                 /* ft2 = z2 = z1 * cos(roll) */

    /* Store output direction vector */
    swc1       ft0, 0x0(a1)             /* store x */
    swc1       ft1, 0x4(a1)             /* store y */
    swc1       ft2, 0x8(a1)             /* store z */

    ld         ra, 0x0(sp)
    addiu      sp, sp, 0x8
    jr         ra
END(mathOneFloatPY)

/**
 * Tests if a 2D point (x, z) lies inside a triangle defined by three vertices.
 *
 * Official Name: mathXZInTri
 *
 * Uses the cross-product sign test: for each edge of the triangle, compute
 * the cross product of (point - vertex) × (next_vertex - vertex). If all
 * three cross products have the same sign (all positive or all negative),
 * the point is inside the triangle.
 *
 * Arguments:
 *   a0 = x coordinate of test point
 *   a1 = z coordinate of test point
 *   a2 = pointer to Vec3s pointA (uses x at offset 0x0, z at offset 0x4)
 *   a3 = pointer to Vec3s pointB (uses x at offset 0x0, z at offset 0x4)
 *   sp+0x10 = pointer to Vec3s pointC (uses x at offset 0x0, z at offset 0x4)
 *
 * Returns:
 *   v0 = 1 if point is inside triangle, 0 otherwise
 *
 * Note: The cross product for edge A->B is computed as:
 *   cross = (x - A.x) * (B.z - A.z) - (B.x - A.x) * (z - A.z)
 *   sign = (cross >= 0) ? 1 : 0
 */
LEAF(mathXZInTri)
    /* Load pointC from stack and extract all vertex coordinates */
    lw         t6, 0x10(sp)             /* t6 = pointC */
    move       v0, zero                 /* v0 = 0 (default: point outside) */
    .set noreorder
    lh         t0, 0x0(a2)              /* t0 = A.x */
    lh         t1, 0x4(a2)              /* t1 = A.z */
    lh         t2, 0x0(a3)              /* t2 = B.x */
    lh         t3, 0x4(a3)              /* t3 = B.z */
    lh         t4, 0x0(t6)              /* t4 = C.x */
    lh         t5, 0x4(t6)              /* t5 = C.z */
    .set reorder
    /* -----------------------------------------
     * Edge A->B: cross = (x - A.x) * (B.z - A.z) - (B.x - A.x) * (z - A.z)
     * ----------------------------------------- */
    sub        t6, t3, t1               /* t6 = B.z - A.z */
    sub        t7, a0, t0               /* t7 = x - A.x */
    MULS       (t7, t6, t7)             /* t7 = (B.z - A.z) * (x - A.x) */
    sub        t8, t2, t0               /* t8 = B.x - A.x */
    sub        t9, a1, t1               /* t9 = z - A.z */
    MULS       (t8, t8, t9)             /* t8 (B.x - A.x) * (z - A.z) */
    sub        t7, t8                   /* t7 = cross product for edge A->B */
    ori        a3, zero, 1              /* a3 = 1 (assume positive) */
    bgez       t7, .edge_ab_positive
    move       a3, zero                 /* a3 = 0 (cross was negative) */

.edge_ab_positive:
    /* -----------------------------------------
     * Edge B->C: cross = (x - B.x) * (C.z - B.z) - (C.x - B.x) * (z - B.z)
     * ----------------------------------------- */
    sub        t6, t5, t3               /* t6 = C.z - B.z */
    sub        t7, a0, t2               /* t7 = x - B.x */
    MULS       (t7, t6, t7)             /* t7 = (C.z - B.z) * (x - B.x) */
    sub        t8, t4, t2               /* t8 = C.x - B.x */
    sub        t9, a1, t3               /* t9 = z - B.z */
    MULS       (t8, t8, t9)             /* t8 = (C.x - B.x) * (z - B.z) */    
    sub        t7, t8                   /* t7 = cross product for edge B->C */
    ori        a2, zero, 1              /* a2 = 1 (assume positive) */
    bgez       t7, .edge_bc_positive
    move       a2, zero                 /* a2 = 0 (cross was negative) */

.edge_bc_positive:
    /* If sign of edge A->B != sign of edge B->C, point is outside */
    bne        a3, a2, .point_outside

    /* -----------------------------------------
     * Edge C->A: cross = (x - C.x) * (A.z - C.z) - (A.x - C.x) * (z - C.z)
     * ----------------------------------------- */
    sub        t6, t1, t5               /* t6 = A.z - C.z */
    sub        t7, a0, t4               /* t7 = x - C.x */
    MULS       (t7, t6, t7)             /* t7 = (A.z - C.z) * (x - C.x) */
    sub        t8, t0, t4               /* t8 = A.x - C.x */
    sub        t9, a1, t5               /* t9 = z - C.z */
    MULS       (t8, t8, t9)             /* t8 = (A.x - C.x) * (z - C.z) */    
    sub        t7, t8                   /* t7 = cross product for edge C->A */
    ori        a1, zero, 1              /* a1 = 1 (assume positive) */
    bgez       t7, .edge_ca_positive
    move       a1, zero                 /* a1 = 0 (cross was negative) */

.edge_ca_positive:
    /* If sign of edge C->A != sign of edge B->C, point is outside */
    bne        a2, a1, .point_outside
    ori        v0, zero, 1              /* All signs match: point is inside */

.point_outside:
    jr         ra
END(mathXZInTri)

/**
 * Creates a 4×4 translation matrix from X, Y, Z coordinates.
 *
 * Official Name: mathTranslateMtx
 *
 * The resulting matrix is an identity matrix with the translation
 * components set in the bottom row (row 3).
 *
 * Arguments:
 *   a0 = pointer to destination 4×4 float matrix (MtxF)
 *   a1 = x translation (passed as raw float bits in integer register)
 *   a2 = y translation (passed as raw float bits in integer register)
 *   a3 = z translation (passed as raw float bits in integer register)
 *
 * Resulting matrix:
 *   [ 1  0  0  0 ]
 *   [ 0  1  0  0 ]
 *   [ 0  0  1  0 ]
 *   [ x  y  z  1 ]
 */
LEAF(mathTranslateMtx)
    /* -----------------------------------------
     * Clear entire 64-byte matrix to zero
     * ----------------------------------------- */
    move       t0, a0                   /* t0 = current write pointer */
    addiu      t1, t0, 0x40             /* t1 = end pointer (64 bytes) */

.mtxf_from_translation_clear_loop:
    sw         zero, 0(t0)              /* clear 4 bytes */
    addiu      t0, 4                    /* advance pointer */
    bne        t1, t0, .mtxf_from_translation_clear_loop

    /* -----------------------------------------
     * Set diagonal elements to 1.0 (identity)
     * ----------------------------------------- */
    li.s       ft5, 1.0                 /* ft5 = 1.0f */
    swc1       ft5, 0x0(a0)             /* m[0][0] = 1.0 */
    swc1       ft5, 0x14(a0)            /* m[1][1] = 1.0 */
    swc1       ft5, 0x28(a0)            /* m[2][2] = 1.0 */
    swc1       ft5, 0x3C(a0)            /* m[3][3] = 1.0 */

    /* -----------------------------------------
     * Set translation components (row 3)
     * ----------------------------------------- */
    sw         a1, 0x30(a0)             /* m[3][0] = x */
    sw         a2, 0x34(a0)             /* m[3][1] = y */
    sw         a3, 0x38(a0)             /* m[3][2] = z */

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

/**
 * Computes the arctangent of y/x (atan2) and returns the angle in binary angle measurement (BAM).
 * BAM uses 16-bit unsigned values where 0x0000 = 0°, 0x4000 = 90°, 0x8000 = 180°, 0xC000 = 270°.
 * This function handles all quadrants correctly by examining the signs of both arguments.
 * 
 * Official name: Arctanf
 * s16 Arctanf(f32 y, f32 x);
 * 
 * Arguments:
 *   fa0 (a0) - y component
 *   fa1 (a1) - x component
 * Returns:
 *   v0 - angle in BAM format (0x0000 - 0xFFFF)
 */
LEAF(Arctanf)
    li         v0, 0              /* Initialize angle offset to 0 */
    mtc1       zero, fv0          /* Load 0.0 into floating point register */

    /* Check if y == 0 */
    c.eq.s     fa0, fv0
    bc1f       .y_not_zero

    /* Check if x == 0 (if both are zero, return 0) */
    c.eq.s     fa1, fv0
    bc1t       .return_angle

.y_not_zero:
    /* Check if y < 0 (y is negative) */
    c.lt.s     fa0, fv0
    bc1t       .y_negative

    /* y is positive, check if x < 0 (Quadrant II) */
    c.lt.s     fa1, fv0
    bc1f       .normalize_to_quadrant_i

    /* Quadrant II: y > 0, x < 0 */
    neg.s      fa1                /* Make x positive (absolute value) */
    li         v0, 0x4000         /* Base angle offset = 90° */
    j          .swap_xy           /* Swap to normalize for table lookup */

.y_negative:
    /* y is negative, make it positive (absolute value) */
    neg.s      fa0

    /* Check if x < 0 (Quadrant III) */
    c.lt.s     fa1, fv0
    bc1t       .quadrant_iii

    /* Quadrant IV: y < 0, x > 0 */
    li         v0, 0xC000         /* Base angle offset = 270° (-90°) */
.swap_xy:
    /* Swap x and y for table lookup optimization */
    mov.s      fa0f, fa0
    mov.s      fa0, fa1
    mov.s      fa1, fa0f
    b          .normalize_to_quadrant_i

.quadrant_iii:
    /* Quadrant III: y < 0, x < 0 */
    neg.s      fa1                /* Make x positive (absolute value) */
    li         v0, 0x8000         /* Base angle offset = 180° */

.normalize_to_quadrant_i:
    /* At this point, both values are positive */
    /* Compare y and x to determine lookup strategy */
    c.lt.s     fa0, fa1

    bc1f      .x_greater_than_y

    /* Case: y < x, compute atan(y/x) directly */
    div.s      ft3, fa0, fa1      /* ft3 = y / x */
    li.s       fv0f, 2048.0       /* Scale factor for table index */
    la         t0, gArcTanTable   /* Load arctangent lookup table address */
    mul.s      ft3, fv0f          /* Scale ratio to table range */
    cvt.w.s    ft3                /* Convert to integer */
    mfc1       t1, ft3            /* Move to CPU register */
    andi       t1, 0xFFE          /* Mask to even index (table is 16-bit halfwords) */
    add        t0, t1             /* Add offset to table base */
    lh         t0, 0x0(t0)        /* Load table value */
    addu       v0, t0             /* Add to angle offset */
    andi       v0, 0xFFFF         /* Mask to 16-bit */
    jr         ra

.x_greater_than_y:
    /* Case: y >= x, compute 90° - atan(x/y) for better precision */
    div.s      ft3, fa1, fa0      /* ft3 = x / y */
    li.s       fv0f, 2048.0       /* Scale factor for table index */
    la         t0, gArcTanTable   /* Load arctangent lookup table address */
    addiu      v0, 0x4000         /* Add 90° to offset */
    mul.s      ft3, fv0f          /* Scale ratio to table range */
    cvt.w.s    ft3                /* Convert to integer */
    mfc1       t1, ft3            /* Move to CPU register */
    andi       t1, 0xFFE          /* Mask to even index (table is 16-bit halfwords) */
    add        t0, t1             /* Add offset to table base */
    lh         t0, 0x0(t0)        /* Load table value */
    subu       v0, t0             /* Subtract table value from 90° + offset */
    andi       v0, 0xFFFF         /* Mask to 16-bit */

.return_angle:
    jr         ra
END(Arctanf)

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
