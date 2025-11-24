#include "gsSnd.h"
#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084848.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084910.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085CF8.s")

extern ALSoundState D_800A9F70;

u16 getSoundStateCounts(u16 *lastAllocListIndex, u16 *lastFreeListIndex) {
    u32 mask;
    u16 freeListNextIndex;
    u16 allocListNextIndex;
    u16 freeListLastIndex;
    ALSoundState *nextFreeList;
    ALSoundState *nextAllocList;
    ALSoundState *prevFreeList;

    mask = osSetIntMask(OS_IM_NONE);
    nextFreeList = D_800A9F70.next;
    nextAllocList = D_800A9F70.unk8;
    prevFreeList = D_800A9F70.prev;

    for (freeListNextIndex = 0; nextFreeList != NULL; freeListNextIndex++, nextFreeList = nextFreeList->next) {}
    for (allocListNextIndex = 0; nextAllocList != NULL; allocListNextIndex++, nextAllocList = nextAllocList->next) {}
    for (freeListLastIndex = 0; prevFreeList != NULL; freeListLastIndex++, prevFreeList = prevFreeList->prev) {}

    *lastAllocListIndex = allocListNextIndex;
    *lastFreeListIndex = freeListNextIndex;

    osSetIntMask(mask);

    return freeListLastIndex;
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085EF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80086120.s")

typedef struct {
    u8 pad[0x40];
    u8 unk40;
    u8 pad41[0x3];
    u8 unk44;
} UnkSndPriority;

void gsSndpSetPriority(UnkSndPriority *arg0, u8 arg1) {
    if (arg0 != NULL) {
        arg0->unk40 = (s16) arg1;
    }
}

u8 gsSndpGetState(UnkSndPriority *arg0) {
    if (arg0 != NULL) {
        return arg0->unk44;
    } else {
        return 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/ad_sndp_play.s")

typedef struct unk800DC6BC_40 {
    ALLink node;
    u8 pad0C[0x38];
} unk800DC6BC_40;
typedef struct unk800DC6BC {
    ALPlayer node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    ALSynth *drvr;
    u32 unk3C;
    unk800DC6BC_40 *unk40;
    s32 soundChannelsMax;
    s32 soundChannels;
    s32 frameTime;
    ALMicroTime nextDelta;
    ALMicroTime curTime;
} unk800DC6BC; // ALSndPlayer
extern unk800DC6BC *D_800A9F7C;
extern void osSyncPrintf(const char *fmt, ...);
void n_alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
extern const char D_800B002C[];

void gsSndpStop(ALSoundState *queue) {
    ALEvent alEvent;

    alEvent.type = 0x400; // Could be a custom Rare event type.
    alEvent.msg.sndpevent.soundState = queue;
    if (queue != NULL) {
        alEvent.msg.sndpevent.soundState->flags &= ~AL_SNDP_PITCH_EVT;
        n_alEvtqPostEvent(&D_800A9F7C->evtq, &alEvent, 0);
    } else {
        osSyncPrintf("WARNING: Attempt to stop NULL sound aborted\n");
    }
}

void func_80086624(u8 event) {
    OSIntMask mask;
    ALEvent evt;
    ALSoundState *queue;

    mask = osSetIntMask(OS_IM_NONE);
    queue = D_800A9F70.next;
    while (queue != NULL) {
        evt.type = AL_SNDP_UNK_10_EVT;
        evt.msg.sndpevent.soundState = queue;
        if ((queue->flags & event) == event) {
            evt.msg.sndpevent.soundState->flags &= ~AL_SNDP_PITCH_EVT;
            n_alEvtqPostEvent(&D_800A9F7C->evtq, &evt, 0);
        }
        queue = queue->next;
    }
    osSetIntMask(mask);
}

void gsSndpStopAll(void) {
    func_80086624(AL_SNDP_PLAY_EVT);
}

void gsSndpStopAllRetrigger(void) {
    func_80086624(AL_SNDP_PLAY_EVT | AL_SNDP_PITCH_EVT);
}

void gsSndpStopAllLooped(void) {
    func_80086624(AL_SNDP_PLAY_EVT | AL_SNDP_STOP_EVT);
}

typedef union {
    ALEvent msg;

    struct {
        s16 type;
        SoundMask *state;
        u32 param;
    } snd_event;

} ALEvent2;
extern const char D_800B005C[];
void gsSndpSetParam(s32 soundMask, s16 type, u32 volume) {
    ALEvent2 sndEvt;
    sndEvt.snd_event.type = type;
    sndEvt.snd_event.state = (void *) soundMask;
    sndEvt.snd_event.param = volume;
    if (soundMask != NULL) {
        n_alEvtqPostEvent(&D_800A9F7C->evtq, (ALEvent *) &sndEvt, 0);
    } else {
        osSyncPrintf("WARNING: Attempt to modify NULL sound aborted\n");
    }
}

extern u16 *D_80105C94;

u16 gsSndpGetMasterVolume(u8 channel) {
    return D_80105C94[channel];
}

void gsSndpSetMasterVolume(u8 channel, u16 volume) {
    OSIntMask mask;
    ALEventQueue *queue;
    s32 sp2C;
    ALEvent evt;

    mask = osSetIntMask(OS_IM_NONE);
    queue = (ALEventQueue *) D_800A9F70.next;
    D_80105C94[channel] = volume;

    for (sp2C = 0; queue != NULL;) {
        // This is almost definitely the wrong struct list, but it matches so I'm not going to complain
        if ((((ALInstrument *) queue->allocList.next->prev)->priority & 0x3F) == channel) {
            evt.type = AL_SNDP_UNK_11_EVT;
            evt.msg.spseq.seq = (void *) queue;
            n_alEvtqPostEvent(&D_800A9F7C->evtq, &evt, 0);
        }
        sp2C++, queue = (ALEventQueue *) queue->freeList.next;
    }

    osSetIntMask(mask);
}

extern u32 D_800A9F80; // u32 globalVolume = 0x100;

void gsSndpSetGlobalVolume(u32 arg0) {
    if (arg0 > 0x100) {
        arg0 = 0x100;
    }
    D_800A9F80 = arg0;
}

extern u32 D_800A9F80;

u32 gsSndpGetGlobalVolume(void) {
    return D_800A9F80;
}

void gsSndpLimitVoices(s32 arg0) {
    if (D_800A9F7C->soundChannelsMax >= arg0) {
        D_800A9F7C->curTime = arg0;
    } else {
        D_800A9F7C->curTime = D_800A9F7C->soundChannelsMax;
    }
}
