#ifndef _AUDIOSFX_H_
#define _AUDIOSFX_H_

#include "common.h"

#define SOUND_PARAM_DURATION(m) (m->velocityMax * 33333)
#define SOUND_PARAM_NEXT_SOUND(m) (m->velocityMin + (m->keyMin & 0xC0) * 4)
#define SOUND_PARAM_GROUP(m) (m->keyMin & 0x3F)
#define SOUND_PARAM_FLAGS(m) (m->keyMax & 0xF0)
#define SOUND_PARAM_FXMIX(m) (m->keyMax & 0xF)
#define SOUND_PARAM_PITCH_SLIDE_RATE(m) (m->detune)
#define SOUND_PARAM_PITCH(m) (m->keyBase * 100 - 6000)

#define AL_SNDP_PLAY_EVT (1 << 0)
#define AL_SNDP_RELEASE_EVT (1 << 1)
#define AL_SNDP_PAN_EVT (1 << 2)
#define AL_SNDP_VOL_EVT (1 << 3)
#define AL_SNDP_PITCH_EVT (1 << 4)
#define AL_SNDP_API_EVT (1 << 5)
#define AL_SNDP_DECAY_EVT (1 << 6)
#define AL_SNDP_END_EVT (1 << 7)
#define AL_SNDP_FX_EVT (1 << 8)
#define AL_SNDP_RETRIGGER_EVT (1 << 9)
#define AL_SNDP_STOP_EVT (1 << 10)
#define AL_SNDP_GROUP_VOL_EVT (1 << 11)
#define AL_SNDP_RELEASE_NEXT_EVT (1 << 12)

enum SoundFlags {
    SOUND_FLAG_FINAL_IN_SEQUENCE = 1 << 0,
    SOUND_FLAG_LOOPED = 1 << 1,
    SOUND_FLAG_PLAYING = 1 << 2,
    SOUND_FLAG_RETRIGGER = 1 << 4,
    SOUND_FLAG_PITCH_SLIDE = 1 << 5
};

enum SoundStates {
    SOUND_STATE_NONE,
    SOUND_STATE_PLAYING,
    SOUND_STATE_STOPPING,
    SOUND_STATE_PREEMPT,
    SOUND_STATE_WAIT_VOICE,
    SOUND_STATE_INIT
};

typedef struct ALSoundState* SoundHandle;

typedef struct ALSoundStateLists {
    /* 0x00 */ ALSoundState *allocHead;
    /* 0x04 */ ALSoundState *allocTail;
    /* 0x08 */ ALSoundState *freeHead;
} ALSoundStateLists;

/* Size: 0x64? */
typedef struct SoundPlayer {
    ALPlayer        node;
    ALEventQueue    evtq;
    ALEvent         nextEvent;
    ALSynth        *drvr;
    ALSoundState   *lastSoundState;
    ALSoundState   *soundStatesArray;
    s32             maxSystemSoundChannels;
    s32             maxActiveSounds;
    s32             frameTime;
    ALMicroTime     nextDelta;
    ALMicroTime     curTime;
} SoundPlayer;

typedef union {

    ALEvent             msg;

    struct {
        u16             type;
        ALSoundState    *state;
        s32             param;
    } common;

    struct {
        s16             type;
        ALSoundState    *state;
        s32             soundIndex;
        ALBank          *bank;
    } retrigger;
    
} ALSndpEvent;


#endif
