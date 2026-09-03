#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "common.h"
#include "n_sndp.h"

#define AUDIO_CHANNELS 16
#define MUSIC_CHAN_MASK_NONE 0xFFFFFFFF

enum AudioVolumeBehaviour {
    VOLUME_NORMAL,
    VOLUME_LOWER,
    VOLUME_LOWER_AMBIENT,
    VOLUME_UNK03,
};

/* Size: 0x0A bytes */
typedef struct SoundData {
    u16 soundBite; // Audio file index.
    u8 volume;     // Multiplied by 256. 128 = 32768, max volume.
    u8 minVolume;
    u8 pitch; // Fractional. 100 = 1.0f.
    u8 unk5;
    u16 range; // Ingame units distance, same as any other.
    u8 priority;
    u8 unk9;
} SoundData;

/* Size: 0x03 bytes */
typedef struct MusicData {
    u8 volume;
    u8 tempo;
    u8 reverb;
} MusicData;

/* Size: 0x08 bytes */
typedef struct DelayedSound {
    /* 0x00 */ u16 soundId;
    /* 0x02 */ s16 timer;
    /* 0x04 */ SoundHandle *handlePtr;
} DelayedSound;

void amAmbientStop(void);
void amSndSetVolXYZ(SoundMask *soundMask, u8 volume);
void amSndSetXYZ(SoundMask *soundMask, f32 x, f32 y, f32 z);
void func_80003B74_4774(s32 arg0);
void amSndStopXYZ(SoundMask *soundMask);
void amSndStop(ALSoundState *handle);
void amSndPlayXYZ(u16 soundId, f32 x, f32 y, f32 z, u8 arg4, SoundMask **soundMask);
void amSndPlay(u16 soundID, SoundHandle *handlePtr);
void amTuneSetGlobalVolume(u32 volume);
void amResetAudioMap(void);
void amSetMuteMode(s32 behaviour); // 0x80000450 Start of .text
u8 amTuneGetSeqNo(void);
void amTunePlay(u8 seqID);
void amTuneResetChls(void);
void amTuneResetFade(void);
void amTuneScaleTempo(f32);
void amTuneSetChlMask(u16);
void amTuneStop(void);
void amTuneVoiceLimit(u8 voiceLimit);

#endif
