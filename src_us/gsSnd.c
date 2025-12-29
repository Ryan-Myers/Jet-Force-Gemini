#include "gsSnd.h"
#include "common.h"

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/gsSndpNew.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_80084468_85068.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_80084530_85130.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_8008582C_8642C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_8008589C_8649C.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_80085918_86518.s")

// ALSoundStateLists gSoundStateLists = { NULL, NULL, NULL };
extern ALSoundStateLists gSoundStateLists;

/**
 * Takes two pointers as arguments and writes the number of free and allocated sound states to them.
 * Additionally, it counts the number of allocated sound states in reverse order.
 * But why? It should be the same as in the forward order.
 * Official Name: getSoundStateCounts
 */
u16 getSoundStateCounts(u16 *numFree, u16 *numAllocated) {
    OSIntMask mask;
    u16 allocatedCounter;
    u16 freeCounter;
    u16 allocatedRevCounter;
    ALSoundState *allocatedPtr;
    ALSoundState *freePtr;
    ALSoundState *allocatedRevPtr;

    mask = osSetIntMask(OS_IM_NONE);
    allocatedPtr = gSoundStateLists.allocHead;
    freePtr = gSoundStateLists.freeHead;
    allocatedRevPtr = gSoundStateLists.allocTail;

    for (allocatedCounter = 0; allocatedPtr != NULL; allocatedCounter++, allocatedPtr = allocatedPtr->next) {}
    for (freeCounter = 0; freePtr != NULL; freeCounter++, freePtr = freePtr->next) {}
    for (allocatedRevCounter = 0; allocatedRevPtr != NULL; allocatedRevCounter++, allocatedRevPtr = allocatedRevPtr->prev) {}

    *numFree = freeCounter;
    *numAllocated = allocatedCounter;

    osSetIntMask(mask);

    return allocatedRevCounter;
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_80085B20_86720.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/func_80085D50_86950.s")

typedef struct {
    u8 pad[0x40];
    u8 unk40;
    u8 pad41[0x3];
    u8 unk44;
} UnkSndPriority;

/**
 * Sets the priority for voice allocation and for preempting another sound.
 * Official Name: gsSndpSetPriority
 */
void gsSndpSetPriority(UnkSndPriority *sndp, u8 priority) {
    if (sndp != NULL) {
        sndp->unk40 = (s16) priority;
    }
}
/**
 * Retrieves the current sound state, refer to the SoundStates enum.
 * Official Name: gsSndpGetState
 */
u8 gsSndpGetState(ALSoundState *sndp) {
    if (sndp != NULL) {
        return sndp->state;
    } else {
        return SOUND_STATE_NONE;
    }
}

#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/ad_sndp_play.s")

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
extern SoundPlayer *gSoundPlayerPtr;
extern void osSyncPrintf(const char *fmt, ...);
void n_alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
extern const char D_800B002C[];

void gsSndpStop(ALSoundState *state) {
    ALEvent alEvent;

    alEvent.type = AL_SNDP_STOP_EVT;
    alEvent.msg.sndpevent.soundState = state;
    if (state != NULL) {
        alEvent.msg.sndpevent.soundState->flags &= ~AL_SNDP_PITCH_EVT;
        n_alEvtqPostEvent(&gSoundPlayerPtr->evtq, &alEvent, 0);
    } else {
        osSyncPrintf("WARNING: Attempt to stop NULL sound aborted\n");
    }
}

void sndp_stop_with_flags(u8 event) {
    OSIntMask mask;
    ALEvent evt;
    ALSoundState *queue;

    mask = osSetIntMask(OS_IM_NONE);
    queue = gSoundStateLists.allocHead;
    while (queue != NULL) {
        evt.type = AL_SNDP_STOP_EVT;
        evt.msg.sndpevent.soundState = queue;
        if ((queue->flags & event) == event) {
            evt.msg.sndpevent.soundState->flags &= ~AL_SNDP_PITCH_EVT;
            n_alEvtqPostEvent(&gSoundPlayerPtr->evtq, &evt, 0);
        }
        queue = queue->next;
    }
    osSetIntMask(mask);
}

/**
 * Stops all sounds from playing.
 * Official Name: gsSndpStopAll
 */
void gsSndpStopAll(void) {
    sndp_stop_with_flags(SOUND_FLAG_FINAL_IN_SEQUENCE);
}

/**
 * Stops all retriggered sounds from playing.
 * Official Name: gsSndpStopAllRetrigger
 */
void gsSndpStopAllRetrigger(void) {
    sndp_stop_with_flags(SOUND_FLAG_FINAL_IN_SEQUENCE | SOUND_FLAG_RETRIGGER);
}

/**
 * Stops all looped sounds from playing.
 * Official Name: gsSndpStopAllLooped
 */
void gsSndpStopAllLooped(void) {
    sndp_stop_with_flags(SOUND_FLAG_FINAL_IN_SEQUENCE | SOUND_FLAG_LOOPED);
}

/**
 * Send a message to the sound player to update an existing property of the sound entry.
 * Official Name: gsSndpSetParam
 */
void gsSndpSetParam(SoundHandle soundMask, s16 type, u32 paramValue) {
    ALSndpEvent evt;
    evt.common.type = type;
    evt.common.state = soundMask;
    evt.common.param = paramValue;
    if (soundMask != NULL) {
        n_alEvtqPostEvent(&gSoundPlayerPtr->evtq, (ALEvent *) &evt, 0);
    } else {
        osSyncPrintf("WARNING: Attempt to modify NULL sound aborted\n");
    }
}

extern u16 *gSoundGroupVolume;

/**
 * Returns the volume level of the channel ID.
 * Official Name: gsSndpGetMasterVolume
 */
u16 gsSndpGetMasterVolume(u8 groupID) {
    return gSoundGroupVolume[groupID];
}

void gsSndpSetMasterVolume(u8 groupID, u16 volume) {
    OSIntMask mask;
    ALSoundState *state;
    s32 i;
    ALSndpEvent evt;

    mask = osSetIntMask(OS_IM_NONE);
    state = gSoundStateLists.allocHead;
    gSoundGroupVolume[groupID] = volume;

    for (i = 0; state != NULL;) {
        if (SOUND_PARAM_GROUP(state->sound->keyMap) == groupID) {
            evt.common.type = AL_SNDP_GROUP_VOL_EVT;
            evt.common.state = state;
            n_alEvtqPostEvent(&gSoundPlayerPtr->evtq, (ALEvent *) &evt, 0);
        }
        i++, state = state->next;
    }

    osSetIntMask(mask);
}

extern u32 globalVolume; // u32 globalVolume = 0x100;

void gsSndpSetGlobalVolume(u32 arg0) {
    if (arg0 > 0x100) {
        arg0 = 0x100;
    }
    globalVolume = arg0;
}

extern u32 globalVolume;

u32 gsSndpGetGlobalVolume(void) {
    return globalVolume;
}

#ifdef NON_MATCHING
void gsSndpLimitVoices(s32 arg0) {
    if (gSoundPlayerPtr->maxSystemSoundChannels >= arg0) {
        gSoundPlayerPtr->curTime = arg0;
    } else {
        gSoundPlayerPtr->curTime = gSoundPlayerPtr->maxSystemSoundChannels;
    }
}
#else
#pragma GLOBAL_ASM("asm_us/nonmatchings/gsSnd/gsSndpLimitVoices.s")
#endif
