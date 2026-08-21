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
#include <ultraerror.h>

#include <os.h>
#include <os_internal.h>
// #include <stdio.h>
#include <assert.h>
#define RANGE 2.0f

#ifdef AUD_PROFILE
extern u32 cnt_index, reverb_num, reverb_cnt, reverb_max, reverb_min, lastCnt[];
extern u32 load_num, load_cnt, load_max, load_min, save_num, save_cnt, save_max, save_min;
#endif

/*
 * macros
 */
#define SWAP(in, out)	\
{			\
			  s16 t = out;	\
			    out = in;		\
			      in = t;		\
			      }

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 arg2, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(ALFx *r, s32 arg1, s16 *curr_ptr, s32 buff,s32 count, Acmd *p);
Acmd *_n_saveBuffer(ALFx *r, s32 arg1, s16 *curr_ptr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p);

f32  _doModFunc(ALDelay *d, s32 count);

/***********************************************************************
 * Reverb filter public interfaces
 ***********************************************************************/
Acmd *n_alFxPull(s32 sampleOffset,
	       Acmd *p, s32 arg2) 
{
  Acmd        *ptr = p;
  ALFx	*r = (ALFx *)n_syn->auxBus[arg2].fx;
  s16		i, buff1, buff2, input, output;
  s16		*in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
  ALDelay	*d, *pd;
  s32 sp58 = 1;
  u32 j;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif

  /*
   * pull channels going into this effect first
   */
  ptr = n_alAuxBusPull(sampleOffset, p, arg2, &sp58);
  
#ifndef N_MICRO
  input  = AL_AUX_L_OUT;
  output = AL_AUX_R_OUT;
  buff1  = AL_TEMP_0;
  buff2  = AL_TEMP_1;
#else
  input  = N_AL_AUX_L_OUT;
  output = N_AL_AUX_R_OUT;
  buff1  = N_AL_TEMP_0;
  buff2  = N_AL_TEMP_1;
#endif

  if (D_80105014_B1754[arg2] == 0) {
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);  /* set the buffer size */
    aMix(ptr++, 0, 0xda83, AL_AUX_L_OUT, input); /* .707L = L - .293L */
    aMix(ptr++, 0, 0x5a82, AL_AUX_R_OUT, input); /* mix the AuxL and AuxR into the AuxL */
#else
    aMix(ptr++, 0, 0xc000, N_AL_AUX_L_OUT, input);
    aMix(ptr++, 0, 0x4000, N_AL_AUX_R_OUT, input);
#endif
  }

  /* and write the mixed value to the delay line at r->input */
  ptr = _n_saveBuffer(r, 0, r->input[0], input, ptr);

  if (D_80105014_B1754[arg2]) {
    ptr = _n_saveBuffer(r, 1, r->input[1], 0x930, ptr);
  }
  for (j = 0; j <= D_80105014_B1754[arg2]; j++) {
    aClearBuffer(ptr++, output, FIXED_SAMPLE<<1); /* clear the AL_AUX_R_OUT */
    for (i = 0; i < r->section_count; i++) {
      d  = &r->delay[i];  /* get the ALDelay structure */
      in_ptr  = &r->input[j][-d->input];
      out_ptr = &r->input[j][-d->output];
  
      if (D_80105016_B1756[arg2] && D_80105014_B1754[arg2]) {
        d->ffcoef = -d->ffcoef;
        d->fbcoef = -d->fbcoef;
      }
  
      if (in_ptr == prev_out_ptr) {
        SWAP(buff1, buff2);
      } else {  /* load data at in_ptr into buff1 */
        ptr = _n_loadBuffer(r, j, in_ptr, buff1, FIXED_SAMPLE, ptr);
      }
      ptr = _n_loadOutputBuffer(r, d, j, buff2, ptr);
      
      if (d->ffcoef) {
        aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
        if (!d->rs && !d->lp) {
  	ptr = _n_saveBuffer(r, j, out_ptr, buff2, ptr);
        }
      }
      
      if (d->fbcoef) {
        aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
        ptr = _n_saveBuffer(r, j, in_ptr, buff1, ptr);
      }
      
      if (d->lp)
        ptr = _n_filterBuffer(d->lp, j, buff2, ptr);
      
      if (!d->rs)
        ptr = _n_saveBuffer(r, j, out_ptr, buff2, ptr);
      
      if (d->gain) {
        if (D_80105014_B1754[arg2]) {
          aMix(ptr++, 0, (u16)d->gain, buff2, output);
        } else {
          u32 sp34 = d->gain * 1.4141999483109f;
  
          if (sp34 > 0x7fff) {
            sp34 = 0x7fff;
          }
  
          aMix(ptr++, 0, (u16)sp34, buff2, output);
        }
      }
      
      prev_out_ptr = &r->input[j][d->output];
    }
  
    if (D_80105014_B1754[arg2] && j == 0) {
      ptr = _n_loadBuffer(r, 1, r->input[1], input, FIXED_SAMPLE, ptr);
  
      if (D_80105016_B1756[arg2]) {
        aMix(ptr++, 0, 0x5a82, output, 0x650);
      } else {
        aMix(ptr++, 0, 0x5a82, output, 0x4e0);
      }
    } else {
      // empty
    }
  
    /*
     * output already in AL_AUX_R_OUT
     *      just copy to AL_AUX_L_OUT
     */
#if 0
    aDMEMMove(ptr++, output, AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#else
    aDMEMMove(ptr++, output, N_AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#endif
    
    /*
     * bump the master delay line input pointer
     * modulo the length
     */
    r->input[j] += FIXED_SAMPLE;
    if (r->input[j] > &r->base[j][r->length])
      r->input[j] -= r->length;
  }

#ifdef AUD_PROFILE
  PROFILE_AUD(reverb_num, reverb_cnt, reverb_max, reverb_min);
#endif
  return ptr;
}

/*
 * This routine gets called by alSynSetFXParam. No checking takes place to 
 * verify the validity of the paramID or the param value. input and output 
 * values must be 8 byte aligned, so round down any param passed. 
 */
s32 n_alFxParamHdl(void *filter, s32 paramID, void *param)
{
  ALFx   *f = (ALFx *) filter;    
  s32    p = (paramID & 7); 
  s32    s = (paramID >> 3);
  s32    val = *(s32*)param;
  f32 rsgain;

  if (s >= f->section_count) {
    return 0;
  }

#define INPUT_PARAM         0
#define OUTPUT_PARAM        1
#define FBCOEF_PARAM        2
#define FFCOEF_PARAM        3
#define GAIN_PARAM          4
#define CHORUSRATE_PARAM    5
#define CHORUSDEPTH_PARAM   6
#define LPFILT_PARAM        7
  
  switch(p)
    {
    case INPUT_PARAM:
      f->delay[s].input = ((s32)val * n_syn->outputRate / 1000) & 0xFFFFFFF8;
      break;
    case OUTPUT_PARAM:
      f->delay[s].output = ((s32)val * n_syn->outputRate / 1000) & 0xFFFFFFF8;
      break;
    case FBCOEF_PARAM:
      f->delay[s].fbcoef = (s16)val;
      break;
    case FFCOEF_PARAM:
      f->delay[s].ffcoef = (s16)val;
      break;
    case GAIN_PARAM:
      f->delay[s].gain = (s16)val;
      break;
    case CHORUSRATE_PARAM:
      f->delay[s].rsinc = ((((f32)val)/1000) * RANGE)/n_syn->outputRate;
      break;
    case CHORUSDEPTH_PARAM:
	  rsgain = val;
      break;
    case LPFILT_PARAM:
      if (f->delay[s].lp) {
        f->delay[s].lp->fc = (s16)val;
          n_alFxInitlpfilter(f->delay[s].lp);
      }
      break;
  }

  if (f->delay[s].input >= f->length - 16) {
    f->delay[s].input = f->length - 16;
  }

  if (f->delay[s].input >= f->length - 8) {
    f->delay[s].input = f->length - 8;
  }

  if (f->delay[s].input >= f->delay[s].output) {
    f->delay[s].output = f->delay[s].input + 8;
  }

      /*
       * the following constant is derived from:
       *
       *      ratio = 2^(cents/1200)
       *
       * and therefore for hundredths of a cent
       *                     x
       *      ln(ratio) = ---------------
       *              (120,000)/ln(2)
       * where
       *      120,000/ln(2) = 173123.40...
       */
#define CONVERT 173123.404906676f
#define LENGTH  (f->delay[s].output - f->delay[s].input)

  if (f->delay[s].rs) {
    if (p != 6) {
      if (LENGTH != 0) {
        rsgain = (f32)f->delay[s].rsgain / (f->delay[s].output - f->delay[s].input) * CONVERT;
      } else {
        rsgain = 0;
      }
    }

    f->delay[s].rsgain = (f->delay[s].output - f->delay[s].input) * (rsgain / CONVERT);
  }

  return 0;
}

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 arg2, s32 buff, Acmd *p)
{
  Acmd        *ptr = p;
#ifndef N_MICRO
  s32         ratio, count, rbuff = AL_TEMP_2;
#else
  s32         ratio, count, rbuff = N_AL_TEMP_2;
#endif
  s16         *out_ptr;
  f32         fincount, fratio, delta;
  s32         ramalign = 0, length;
  s32 incount = FIXED_SAMPLE;

  if (d->rs) {
    length = d->output - d->input;
    delta = _doModFunc(d, incount);
    delta /= length;
    delta = (s32)(delta * UNITY_PITCH);
    delta = delta / UNITY_PITCH;
    fratio = 1.0f - delta;
    fincount = d->rs->delta + (fratio * (f32)incount);
    count = (s32) fincount;
    d->rs->delta = fincount - (f32)count;
    out_ptr = &r->input[arg2][-(d->output - d->rsdelta)];
    ramalign = ((s32)out_ptr & 0x7) >> 1;
    ptr = _n_loadBuffer(r, arg2, out_ptr - ramalign, rbuff, count + ramalign, ptr);
    
    ratio = (s32)(fratio * UNITY_PITCH);
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, rbuff + (ramalign<<1), buff, incount<<1);
    aResample(ptr++, d->rs->first, ratio, osVirtualToPhysical(d->rs->state));
#else
    {
        s16 tmp;

        tmp = buff >> 8;
        n_aResample(ptr++, osVirtualToPhysical(d->rs->state[arg2]), d->rs->first, ratio, rbuff + (ramalign<<1), tmp);
      }
#endif    
    d->rs->first = 0;
    d->rsdelta += count - incount;
  } else {
    out_ptr = &r->input[arg2][-d->output];
    ptr = _n_loadBuffer(r, arg2, out_ptr, buff, FIXED_SAMPLE, ptr);
  }
  
  return ptr;
}

Acmd *_n_loadBuffer(ALFx *r, s32 arg1, s16 *curr_ptr, s32 buff,s32 count, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[arg1][r->length];

#ifdef _DEBUG
    if(curr_ptr > delay_end)
        __osError(ERR_ALMODDELAYOVERFLOW, 1, delay_end - curr_ptr);
#endif

    if (curr_ptr < r->base[arg1])
    curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        
#ifndef N_MICRO
        aSetBuffer(ptr++, 0, buff, 0, before_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, buff+(before_end<<1), 0, after_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(r->base));
    } else {
        aSetBuffer(ptr++, 0, buff, 0, count<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }

    aSetBuffer(ptr++, 0, 0, 0, count<<1);
#else
        n_aLoadBuffer(ptr++, before_end<<1, buff, osVirtualToPhysical(curr_ptr));
        n_aLoadBuffer(ptr++, after_end<<1, buff+(before_end<<1), osVirtualToPhysical(r->base[arg1]));
   } else {
        n_aLoadBuffer(ptr++, count<<1, buff, osVirtualToPhysical(curr_ptr));
   }
#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(load_num, load_cnt, load_max, load_min);
#endif
    return ptr;
}

Acmd *_n_saveBuffer(ALFx *r, s32 arg1, s16 *curr_ptr, s32 buff, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[arg1][r->length];
    if (curr_ptr < r->base[arg1])      /* probably just security */
        curr_ptr += r->length;   /* shouldn't occur */
    updated_ptr = curr_ptr + FIXED_SAMPLE;

    if (updated_ptr > delay_end) { /* if the data wraps past end of r->base */
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;

#ifndef N_MICRO
        aSetBuffer(ptr++, 0, 0, buff, before_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, 0, buff+(before_end<<1), after_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(r->base));
        aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);
    } else {
        aSetBuffer(ptr++, 0, 0, buff, FIXED_SAMPLE<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }
#else
        n_aSaveBuffer(ptr++, before_end<<1, buff, osVirtualToPhysical(curr_ptr));
        n_aSaveBuffer(ptr++, after_end<<1, buff+(before_end<<1), osVirtualToPhysical(r->base[arg1]));
   } else {
        n_aSaveBuffer(ptr++, FIXED_SAMPLE<<1, buff, osVirtualToPhysical(curr_ptr));
   }
#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(save_num, save_cnt, save_max, save_min);
#endif
    return ptr;

}

Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p)
{
    Acmd	*ptr = p;
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, buff, buff, FIXED_SAMPLE<<1);
    aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    aPoleFilter(ptr++, lp->first, lp->fgain, osVirtualToPhysical(lp->fstate));
#else
{
  s16  tmp;

  tmp = count >> 8;
  n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
  n_aPoleFilter(ptr++, lp->first, lp->fgain, tmp, osVirtualToPhysical(lp->fstate[buff]));
}
#endif

    lp->first = 0;

    return ptr;
}

/**
 * Generate a triangle wave from -1 to 1, and find the current position
 * in the wave. (Rate of the wave is controlled by d->rsinc, which is chorus
 * rate) Multiply the current triangle wave value by d->rsgain, (chorus depth)
 * which is expressed in number of samples back from output pointer the chorus
 * should go at it's full chorus. In otherwords, this function returns a number
 * of samples the output pointer should modulate backwards.
 */
f32 _doModFunc(ALDelay *d, s32 count)
{
	f32 val;

	/*
	 * generate bipolar sawtooth
	 * from -RANGE to +RANGE
	 */
	d->rsval += d->rsinc * count;
	d->rsval = (d->rsval > RANGE) ? d->rsval - (RANGE * 2) : d->rsval;

	/*
	 * convert to monopolar triangle
	 * from 0 to RANGE
	 */
	val = d->rsval;
	val = (val < 0) ? -val : val;

	/*
	 * convert to bipolar triangle
	 * from -1 to 1
	 */
	val -= RANGE/2;

	return d->rsgain * val;
}
