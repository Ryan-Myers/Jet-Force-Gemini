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
#include <assert.h>

#ifdef AUD_PROFILE
#include <os.h>
extern u32 cnt_index, drvr_num, drvr_cnt, drvr_max, drvr_min, lastCnt[];
extern u32 client_num, client_cnt, client_max, client_min;
#endif

#ifndef MIN
#   define MIN(a,b) (((a)<(b))?(a):(b))
#endif

static s32 __n_nextSampleTime(ALPlayer **client);
static s32 _n_timeToSamplesNoRound( s32 micros);

/***********************************************************************
 * Synthesis driver public interfaces
 ***********************************************************************/
void n_alSynNew(ALSynConfig *c)
{
  s32         i, j;
  N_ALVoice  *vv;
  N_PVoice   *pv;
  N_ALVoice  *vvoices;
  N_PVoice   *pvoices;
  ALHeap      *hp = c->heap;
  ALSave      *save;
  ALParam     *params;
  ALParam     *paramPtr;
  
  n_syn->head            = NULL;

  n_syn->numPVoices      = c->maxPVoices;
  n_syn->curSamples      = 0;
  n_syn->paramSamples    = 0;
  n_syn->outputRate      = c->outputRate;
#ifndef N_MICRO
  n_syn->maxOutSamples   = AL_MAX_RSP_SAMPLES;
#else
  n_syn->maxOutSamples   = FIXED_SAMPLE;
#endif
  n_syn->dma             = (ALDMANew) c->dmaproc;
  
  /******* save new *******************************/
  n_syn->sv_dramout = 0;
  n_syn->sv_first = 1;

  if (c->maxFXbusses > 2) {
    n_syn->maxAuxBusses = 2;
  } else {
    n_syn->maxAuxBusses = c->maxFXbusses;
  }
  
  /******* aux new *******************************/
  n_syn->auxBus = (N_ALAuxBus *)alHeapAlloc(hp, n_syn->maxAuxBusses, sizeof(N_ALAuxBus));
  
  for (i = 0; i < n_syn->maxAuxBusses; i++) {
    n_syn->auxBus[i].sourceCount = 0;
    n_syn->auxBus[i].maxSources = c->maxPVoices;
    n_syn->auxBus[i].sources = alHeapAlloc(hp, c->maxPVoices, sizeof(void *));

    if (c->fxType[i]) {
        n_syn->auxBus[i].fx = n_alSynAllocFX(i, c, hp);
    } else {
        n_syn->auxBus[i].fx = NULL;
    }

    n_syn->auxBus[i].unk44 = alHeapAlloc(hp, 1, sizeof(struct auxbus44));
    n_syn->auxBus[i].unk44->fx.unk02 = 0;
    n_syn->auxBus[i].unk44->unk2c = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
    n_syn->auxBus[i].unk44->unk30 = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
  }
  
  /******* main new *******************************/
  n_syn->mainBus = (N_ALMainBus *)alHeapAlloc(hp, 1, sizeof(N_ALMainBus));
  
  /******* fx new *******************************/
  n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alFxPull;
  

  
  n_syn->pFreeList.next = 0;
  n_syn->pFreeList.prev = 0;
  n_syn->pLameList.next = 0;
  n_syn->pLameList.prev = 0;
  n_syn->pAllocList.next = 0;
  n_syn->pAllocList.prev = 0;

  pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
  for (i = 0; i < c->maxPVoices; i++) {
    pv = &pvoices[i];
    alLink((ALLink *)pv, &n_syn->pFreeList);
    pv->vvoice = 0;
    alN_PVoiceNew(pv, n_syn->dma, hp);
	for (j = 0; j < n_syn->maxAuxBusses; j++) {
	  n_syn->auxBus[j].sources[n_syn->auxBus[j].sourceCount++] = pv;
	}
  }
  
  params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
  n_syn->paramList = 0;
  for (i = 0; i < c->maxUpdates; i++) {
    paramPtr= &params[i];
    paramPtr->next = n_syn->paramList;
    n_syn->paramList = paramPtr;
  }
  n_syn->heap = hp;
}

Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
  ALPlayer    *client;
  s16         tmp = 0;        /* Starting buffer in DMEM */
  Acmd        *cmdlEnd = cmdList;
  Acmd        *cmdPtr;
  s32         nOut;
  s16         *lOutBuf = outBuf;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  if (n_syn->head == 0) {
    *cmdLen = 0;
    return cmdList;         /* nothing to do */
  }    

  /*
   * run down list of clients and execute callback if needed this
   * subframe. Here we do all the work for the frame at the
   * start. Time offsets that occur before the next frame are
   * executed "early".
   */
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * paramSamples = time of next parameter change.
   * curSamples = current sample time.
   * so paramSamples - curSamples is the time until the next parameter change.
   * if the next parameter change occurs within this frame time (outLen),
   * then call back the client that contains the parameter change.
   * Note, paramSamples must be rounded down to 16 sample boundary for use
   * during the client handler.
   */
  
  for (n_syn->paramSamples = __n_nextSampleTime( &client);
       n_syn->paramSamples - n_syn->curSamples < outLen;
       n_syn->paramSamples = __n_nextSampleTime( &client))
    {
      n_syn->paramSamples &= ~0xf;
      client->samplesLeft += _n_timeToSamplesNoRound((*client->handler)(client));
    }
  
  /* for safety's sake, always store paramSamples aligned to 16 sample
     boundary.
   * this way, if an voice handler routine gets called outside
   the N_ALVoiceHandler
   * routine (alSynAllocVoice) it will get timestamped with an
   aligned value and
   * will be processed immediately next audio frame.
   */
  n_syn->paramSamples &= ~0xf;
  
#ifdef AUD_PROFILE
  PROFILE_AUD(client_num, client_cnt, client_max, client_min);
#endif
  
  /*
   * Now build the command list in small chunks
   */
  while (outLen > 0){
    nOut = MIN(n_syn->maxOutSamples, outLen);
    
    /*
     * construct the command list for each physical voice by calling
     * the head of the filter chain.
     */
    cmdPtr = cmdlEnd;
#ifndef N_MICRO
    aSegment(cmdPtr++, 0, 0);
#endif
    n_syn->sv_dramout = (s32)lOutBuf;
    cmdlEnd = n_alSavePull(n_syn->curSamples, cmdPtr);
    
    outLen -= nOut;
    lOutBuf += nOut<<1;     /* For Stereo */
    n_syn->curSamples += nOut;
    
  }
  *cmdLen = (s32) (cmdlEnd - cmdList);
  
  _n_collectPVoices(); /* collect free physical voices */
  
#ifdef AUD_PROFILE
  PROFILE_AUD(drvr_num, drvr_cnt, drvr_max, drvr_min);
#endif
  return cmdlEnd;
}

/***********************************************************************
 * Synthesis driver private interfaces
 ***********************************************************************/

#ifdef VERSION_us
s32 n_currParam = 0;
s32 n_paramMax = 0;
#endif

ALParam *__n_allocParam() 
{
  ALParam *update = 0;
  
  if (n_syn->paramList) {        
    update = n_syn->paramList;
    n_syn->paramList = n_syn->paramList->next;
    update->next = 0;
#ifdef VERSION_us
    n_currParam++;
    if (n_currParam > n_paramMax) {
      n_paramMax = n_currParam;
    }
#endif
  }
  return update;
}

void _n_freeParam(ALParam *param) 
{
  param->next = n_syn->paramList;
  n_syn->paramList = param;
#ifdef VERSION_us
  n_currParam--;
#endif
}

void _n_collectPVoices(void)
{
  ALLink       *dl;
  N_PVoice      *pv;
  
  while ((dl = n_syn->pLameList.next) != 0) {
    pv = (N_PVoice *)dl;
    
    /* ### remove from mixer */
    
    alUnlink(dl);
    alLink(dl, &n_syn->pFreeList);        
  }
}

void _n_freePVoice(N_PVoice *pvoice) 
{
  /*
   * move the voice from the allocated list to the lame list
   */
  alUnlink((ALLink *)pvoice);
  alLink((ALLink *)pvoice, &n_syn->pLameList);
}


/*
  Add 0.5 to adjust the average affect of
  the truncation error produced by casting
  a float to an int.
  */
s32 _n_timeToSamplesNoRound( s32 micros)
{
  f32 tmp = ((f32)micros) * n_syn->outputRate / 1000000.0f + 0.5f;
  
  return (s32)tmp;
}

s32 _n_timeToSamples( s32 micros)
{
  return _n_timeToSamplesNoRound( micros) & ~0xf;
}

static s32 __n_nextSampleTime(ALPlayer **client) 
{
  ALMicroTime temp,delta = 0x7fffffff;     /* max delta for s32 */
  ALPlayer *cl;
  
  /*    assert(n_syn->head); */
  *client = 0;

#if 1  
  for (cl = n_syn->head; cl != 0; cl = cl->next) {
    if ((cl->samplesLeft - n_syn->curSamples) < delta) {
      *client = cl;
      delta = cl->samplesLeft - n_syn->curSamples;
    }
  }
#endif
  
  return (*client)->samplesLeft;
}



