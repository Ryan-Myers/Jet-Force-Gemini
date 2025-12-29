#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "common.h"
#include "PR/n_libaudio.h"
#include "PR/os_reg.h"
#include "naudio/n_seqp.h"
#include "naudio/n_sndp.h"

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
 u8 volume; // Multiplied by 256. 128 = 32768, max volume.
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

#endif
