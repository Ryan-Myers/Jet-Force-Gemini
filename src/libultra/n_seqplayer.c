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

#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>
#include <assert.h>
#include "n_libaudio.h"
#include "n_seqp.h"

void __n_unmapVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice) 
{
    N_ALVoiceState *prev = 0;
    N_ALVoiceState *vs;
    
    /*
     * we could use doubly linked lists here and save some code and
     * execution time, but time spent here in negligible, so it won't
     * make much difference.
     */
    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
        if (&vs->voice == voice) {

            if (prev)
                prev->next = vs->next;
            else
                seqp->vAllocHead = vs->next;
            
            if (vs == seqp->vAllocTail) {
                seqp->vAllocTail = prev;
            }

            vs->next = seqp->vFreeList;
            seqp->vFreeList = vs;
			seqp->voicecount--;
            return;

        }
        prev = vs;
    }
#ifdef _DEBUG
    __osError(ERR_ALSEQPUNMAP, 1, voice);
#endif    
}

void __n_seqpReleaseVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime deltaTime)
{
	N_ALEvent evt;
	N_ALVoiceState *vs = (N_ALVoiceState *)voice->clientPrivate;

	/*
	 * if in attack phase, remove all pending volume
	 * events for this voice from the queue
	 */

	if (vs->envPhase == AL_PHASE_ATTACK) {
		ALLink *thisNode;
		ALLink *nextNode;
		N_ALEventListItem *thisItem, *nextItem;

		thisNode = seqp->evtq.allocList.next;

		while (thisNode != 0) {
			nextNode = thisNode->next;
			thisItem = (N_ALEventListItem *)thisNode;
			nextItem = (N_ALEventListItem *)nextNode;

			if (thisItem->evt.type == AL_SEQP_ENV_EVT) {
				if (thisItem->evt.msg.vol.voice == voice) {
					if (nextItem) {
						nextItem->delta += thisItem->delta;
					}

					alUnlink(thisNode);
					alLink(thisNode, &seqp->evtq.freeList);
				}
			}

			thisNode = nextNode;
		}
	}

	vs->velocity = 0;
	vs->envPhase = AL_PHASE_RELEASE;
	vs->envGain  = 0;
	vs->envEndTime = seqp->curTime + deltaTime;

	n_alSynSetPriority(voice, 0); /* make candidate for stealing */
	n_alSynSetVol(voice, 0, deltaTime);

	evt.type = AL_NOTE_END_EVT;
	evt.msg.note.voice = voice;

	deltaTime += AL_USEC_PER_FRAME * 2;

	n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
}



/*
  This special purpose routine is called only when processing
  a stopping event in order to properly kill all active voices.
  
  The routine searches through the seqp's event queue for an
  AL_NOTE_END_EVT for the given voice.  If the event's execution
  time is greater than kill time, it removes the event from the
  event queue and returns true that it needs to kill the voice.
  Otherwise, if the event's time is less than the kill time, it
  returns false that the voice needs to be killed.

  sct 1/3/96
*/

#define VOICENEEDSNOTEKILL_DEBUG	_DEBUG_INTERNAL&&0	/* For debugging voiceNeedsNoteKill routine. */

char __n_voiceNeedsNoteKill (N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime)
{
    ALLink              *thisNode;
    ALLink              *nextNode;
    N_ALEventListItem     *thisItem;
    ALMicroTime		itemTime = 0;
    char		needsNoteKill = TRUE;

#if VOICENEEDSNOTEKILL_DEBUG
    n_alEvtqPrintAllocEvts (&seqp->evtq);
#endif

    thisNode = seqp->evtq.allocList.next;
    while (thisNode != 0)
    {
	nextNode = thisNode->next;
	thisItem = (N_ALEventListItem *)thisNode;
	itemTime += thisItem->delta;

	if (thisItem->evt.type == AL_NOTE_END_EVT)
	{
	    if (thisItem->evt.msg.note.voice == voice)
	    {
		if (itemTime > killTime)
		{
		    if ((N_ALEventListItem *)nextNode)
			((N_ALEventListItem *)nextNode)->delta += thisItem->delta;
		    alUnlink(thisNode);
		    alLink(thisNode, &seqp->evtq.freeList);
		}
		else
		    needsNoteKill = FALSE;
		break;
	    }
	}
	thisNode = nextNode;
    }

#if VOICENEEDSNOTEKILL_DEBUG 
    if (thisNode)
	osSyncPrintf("vox 0x%0x: end time %d  kill time %d\n\n", voice, itemTime, killTime);
    else
	osSyncPrintf("vox 0x%0x: not found\n\n", voice);

    n_alEvtqPrintAllocEvts (&seqp->evtq);
#endif

   return needsNoteKill;
}


N_ALVoiceState *__n_mapVoice(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 channel)
{
    N_ALVoiceState  *vs = seqp->vFreeList;

	if (seqp->voicecount > seqp->voicelimit) {
		return 0;
	}

    if (vs) {

        seqp->vFreeList = vs->next;

        vs->next = 0;
        
        if (!seqp->vAllocHead)
            seqp->vAllocHead = vs;
        else
            seqp->vAllocTail->next = vs;
        
        seqp->vAllocTail = vs;

        vs->channel             = channel;
        vs->key                 = key;
        vs->velocity            = vel;
        vs->voice.clientPrivate = vs;

		seqp->voicecount++;
    }
    
    return vs;
}


N_ALVoiceState *__n_lookupVoice(N_ALSeqPlayer *seqp, u8 key, u8 channel)
{
    N_ALVoiceState  *vs;

    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
        if ((vs->key == key) && (vs->channel == channel) &&
            (vs->phase != AL_PHASE_RELEASE) && (vs->phase != AL_PHASE_SUSTREL))
            return vs;
    }
    
    return 0;
}


#if 0
ALSound *__n_lookupSound(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan)
{
    s32         i;
    ALInstrument  *inst = seqp->chanState[chan].instrument;
    ALSound       *snd = 0;
    
    for (i = 0; i < inst->soundCount; i++) {
        ALSound *sound = inst->soundArray[i];
        ALKeyMap *keymap = sound->keyMap;
        
        if ((key >= keymap->keyMin) && (key <= keymap->keyMax) &&
            (vel >= keymap->velocityMin) && (vel <= keymap->velocityMax)) {
            snd = sound;
            break;
        }
    }
    return snd;
}
#endif


ALSound *__n_lookupSoundQuick(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan)
{
    ALInstrument *inst = seqp->chanState[chan].instrument;
    s32 l = 1;
    s32 r = inst->soundCount;
    s32 i;
    ALKeyMap *keymap;

    assert(inst != NULL);       /* sct 10/31/95 - If inst is NULL, then the seqp probably wasn't setup correctly. */
    
    while (r >= l) {
        i = (l+r)/2;

        keymap = inst->soundArray[i-1]->keyMap;

        if ((key >= keymap->keyMin) && (key <= keymap->keyMax) &&
            (vel >= keymap->velocityMin) && (vel <= keymap->velocityMax)) {
            return inst->soundArray[i-1];
        } else if ((key < keymap->keyMin) ||
                   ((vel < keymap->velocityMin) && (key <= keymap->keyMax))) {
            r = i - 1;
        } else {
            l = i + 1;
        }
    }
    
    return 0;
}



/*
 * __n_vsVol calculates the target volume for the voice based on the
 * note on velocity, envelope, sampleVolume and controller.
 */
s16 __n_vsVol(N_ALVoiceState *vs, N_ALSeqPlayer *seqp) 
{

    u32     t1,t2;

    t1 = (vs->tremelo*vs->velocity*vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume*seqp->vol*
          seqp->chanState[vs->channel].vol) >> 14;

    if (seqp->chanState[vs->channel].fadevolcurrent != 0xff) {
        t2 = (seqp->chanState[vs->channel].fadevolcurrent * t2 + 1) >> 8;
	}

    t1 *= t2;
    t1 >>= 15;
    
    return( (s16)t1 );
    
}

u8 __n_vsMix(N_ALVoiceState *vs, N_ALCSPlayer *seqp)
{
	s32 sign = seqp->chanState[vs->channel].fxmix & 0x80;
	s32 fxmix = ((seqp->chanState[vs->channel].fxmix & 0x7f) + (s32)(seqp->fxmixmajor * 127)) * seqp->fxmixmega;

	return MAX(0, MIN(127, fxmix)) | sign;
}


ALMicroTime __n_vsDelta(N_ALVoiceState *vs, ALMicroTime t)
{
    /*
     * If we are interrupting a previously set envelope segment, we
     * need to recalculate the segment end time given the current
     * time. Note: this routine assumes that the voice is currently
     * playing.
     */
    
    s32 delta = vs->envEndTime - t;
    
    if (delta >= 0) {
        return delta;
    } else {
        return AL_GAIN_CHANGE_TIME;
    }
}


ALPan __n_vsPan(N_ALVoiceState *vs, N_ALSeqPlayer *seqp) 
{
    s32 tmp;

    tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER +
        vs->sound->samplePan;
    tmp = MAX(tmp, AL_PAN_LEFT);
    tmp = MIN(tmp, AL_PAN_RIGHT);

    return (ALPan) tmp;
}


void __n_initFromBank(N_ALSeqPlayer *seqp, ALBank *b)
{
    /*
     * init the chanState with the default instrument
     */
    s32 i;    
    ALInstrument *inst = 0;
    
    /* set to the first available instrument. */
    for(i = 0; !inst ; i++)
      inst = b->instArray[i];
    
    /* sct 11/6/95 - Setup the channel state for the given instrument. */
    /* There is some wasted effort here since both calls the same state vars */
    /* but it's safer. */
    for (i = 0; i < seqp->maxChannels; i++) {
      __n_resetPerfChanState(seqp, i);
      __n_setInstChanState(seqp, inst, i);
    }

    if (b->percussion) {
      __n_resetPerfChanState(seqp, i);
      __n_setInstChanState(seqp, b->percussion, 9);
    }
}


/*
  sct 11/6/95 - Called only when creating a new sequence player.
*/
void __n_initChanState(N_ALSeqPlayer *seqp) 
{
    int i;
    
    for (i = 0; i < seqp->maxChannels; i++)
    {
        seqp->chanState[i].instrument = 0;
	__n_resetPerfChanState (seqp, i);
    }
}


/*
  sct 11/6/95 -- Call this whenever a new sequence is to be played or when
  initializing a sequence player.
*/
void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan) 
{
  seqp->chanState[chan].fxId = AL_FX_NONE;
  seqp->chanState[chan].fxmix = AL_DEFAULT_FXMIX;
  seqp->chanState[chan].pan = AL_PAN_CENTER;
  seqp->chanState[chan].vol = AL_VOL_FULL;
  seqp->chanState[chan].priority = AL_DEFAULT_PRIORITY;
  seqp->chanState[chan].sustain = 0;
  seqp->chanState[chan].bendRange = 200;
  seqp->chanState[chan].pitchBend = 1.0f;
  seqp->chanState[chan].notemesgflags = 0;
  seqp->chanState[chan].fadevolcurrent = 255;
  seqp->chanState[chan].fadevoltarget = 255;
  seqp->chanState[chan].fadevolinc = 0;
  seqp->chanState[chan].fxbus = 0;
  seqp->chanState[chan].unk13 = 0;
  seqp->chanState[chan].unk12 = 0;
  seqp->chanState[chan].unk11 = 0;
  seqp->chanState[chan].instmajor = 0;
}


/*
  sct 11/6/95 - Call this whenever a new instrument gets assigned to a channel
  such as when changing banks or in response to a MIDI program change event.
  Currently also gets called when changing sequences.
*/
void __n_setInstChanState(N_ALSeqPlayer *seqp, ALInstrument *inst, s32 chan)
{
	ALSound *sound;

	seqp->chanState[chan].instrument = inst;
	seqp->chanState[chan].pan = inst->pan;
	seqp->chanState[chan].vol = inst->volume;
	seqp->chanState[chan].priority = inst->priority;
	seqp->chanState[chan].bendRange = inst->bendRange;

	if (inst->soundCount == 0) {
		return;
	}

	sound = inst->soundArray[0];

	seqp->chanState[chan].attackTime = sound->envelope->attackTime;
	seqp->chanState[chan].decayTime = sound->envelope->decayTime;
	seqp->chanState[chan].releaseTime = sound->envelope->releaseTime;
	seqp->chanState[chan].attackVolume = sound->envelope->attackVolume;
	seqp->chanState[chan].decayVolume = sound->envelope->decayVolume;

	seqp->chanState[chan].pitch = 0;
	seqp->chanState[chan].tremType = inst->tremType;
	seqp->chanState[chan].tremRate = inst->tremRate;
	seqp->chanState[chan].tremDepth = inst->tremDepth;
	seqp->chanState[chan].tremDelay = inst->tremDelay;
	seqp->chanState[chan].vibType = inst->vibType;
	seqp->chanState[chan].vibRate = inst->vibRate;
	seqp->chanState[chan].vibDepth = inst->vibDepth;
	seqp->chanState[chan].vibDelay = inst->vibDelay;

	seqp->chanState[chan].usechanparams = 0;
	seqp->chanState[chan].timeindex = 0;
}


void __n_seqpStopOsc(N_ALSeqPlayer *seqp, N_ALVoiceState *vs)
{
    N_ALEventListItem  *thisNode,*nextNode;
    s16              evtType;
    
    thisNode = (N_ALEventListItem*)seqp->evtq.allocList.next;
    while(thisNode)
    {
        nextNode = (N_ALEventListItem*)thisNode->node.next;
        evtType = thisNode->evt.type;
        if(evtType == AL_TREM_OSC_EVT || evtType == AL_VIB_OSC_EVT)
        {
            if(thisNode->evt.msg.osc.vs == vs)
            {
                (*seqp->stopOsc)(thisNode->evt.msg.osc.oscState);
                alUnlink((ALLink*)thisNode);
                if(nextNode)
                    nextNode->delta += thisNode->delta;
                alLink((ALLink*)thisNode, &seqp->evtq.freeList);
                if(evtType == AL_TREM_OSC_EVT)
                    vs->flags = vs->flags & 0xFE;
                else /* must be a AL_VIB_OSC_EVT */
                    vs->flags = vs->flags & 0xFD;
                if(!vs->flags)
                    return;  /* there should be no more events */
            }
        }
        
        thisNode = nextNode;
    }
}
