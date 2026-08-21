#include "common.h"
/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include "n_synthInternals.h"
#include <os.h>
// #include <stdio.h>
// #include <math.h>
#include <assert.h>

#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif

#define N_EQPOWER_LENGTH 128

s16 n_eqpower[ N_EQPOWER_LENGTH ] = {
    32767,  32764,  32757,  32744,  32727,  32704,
    32677,  32644,  32607,  32564,  32517,  32464,
    32407,  32344,  32277,  32205,  32127,  32045,
    31958,  31866,  31770,  31668,  31561,  31450,
    31334,  31213,  31087,  30957,  30822,  30682,
    30537,  30388,  30234,  30075,  29912,  29744,
    29572,  29395,  29214,  29028,  28838,  28643,
    28444,  28241,  28033,  27821,  27605,  27385,
    27160,  26931,  26698,  26461,  26220,  25975,
    25726,  25473,  25216,  24956,  24691,  24423,
    24151,  23875,  23596,  23313,  23026,  22736,
    22442,  22145,  21845,  21541,  21234,  20924,
    20610,  20294,  19974,  19651,  19325,  18997,
    18665,  18331,  17993,  17653,  17310,  16965,
    16617,  16266,  15913,  15558,  15200,  14840,
    14477,  14113,  13746,  13377,  13006,  12633,
    12258,  11881,  11503,  11122,  10740,  10357,
    9971,   9584,   9196,   8806,   8415,   8023,
    7630,   7235,   6839,   6442,   6044,   5646,
    5246,   4845,   4444,   4042,   3640,   3237,
    2833,   2429,   2025,   1620,   1216,    810,
    405,      0
};

#ifndef N_MICRO
extern	f64	__pow(f64, f64);
extern f64 _frexpf(f64 value, s32 *eptr);
extern f64 _ldexpf(f64 in, s32 ex);
#endif

/*
 * prototypes for private enveloper functions
 */
static  Acmd *_pullSubFrame(N_PVoice *pv, s16 *inp, s16 *outp, s32 outCount,
                             Acmd *p) ;

static  s16 _getRate(f64 vol, f64 tgt, s32 count, u16* ratel);
     
     
#ifndef N_MICRO
static  f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
#else
static  s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/n_env/n_alEnvmixerPull.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/n_env/n_alEnvmixerParam.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/n_env/_pullSubFrame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/n_env/_getRate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/n_env/_getVol.s")
