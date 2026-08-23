#include "audio_manager_1050.h"
#include "audio.h"
#include "common.h"
#include "gsSnd.h"
#include "n_seqp.h"
#include "n_sndp.h"
#include "PR/n_libaudio.h"
#include "PR/os_reg.h"

#ifdef VERSION_us
const char D_800A9FB0[] = "No I'm not playing MIDI sequence %d, its over 32K\n";
#endif
const char D_800AA950[] = "WARNING: Sync arrived before wait - music will be out of sync with sequence\n";
#ifdef VERSION_us
const char D_800AA034[] = "I'm not playing any ditty over 4K nowadays (%d)\n";
#endif
const char D_800AA9A0[] = "amSndPlay: Illegal sound effects table index\n";
const char D_800AA9D0[] = "amSndPlayDirect: Somebody tried to play illegal sound %d\n";
const char D_800AAA0C[] = "Invalid midi sequence index\n";
#ifdef VERSION_kiosk
const char D_800AAA2C[] = "amTuneRestart:Jump failed - not previously pushed onto seq player %x\n";
#endif

void n_alCSPVoiceLimit(N_ALCSPlayer *seqp, u8 value);
u32 gsSndpGetGlobalVolume(void);                    // sndp_get_global_volume
void gsSndpSetMasterVolume(u8 channel, u16 volume); // sndp_set_group_volume
void amAmbientSetVolume(u8 volume);
extern u8 sfxRelativeVolume;
extern u8 gAudioVolumeSetting;
extern N_ALSeqPlayer *ambientSeqPlayer; // Official Name: ambientSeqPlayer
#define AL_SNDP_GROUP_VOLUME_MAX 32767
void music_sequence_init(u8 seqID, N_ALSeqPlayer *seqPlayer); // Could be different in kiosk

void amTuneStop(void);
void music_sequence_start(u8 seqID, N_ALSeqPlayer *seqPlayer);

extern u8 gCanPlayMusic;
extern s32 gBlockMusicChange;
extern s32 audioPrevCount;
extern s32 gMusicTempo;
extern u32 gDynamicMusicChannelMask;
extern N_ALSeqPlayer *tuneSeqPlayer;   // Official Name: tuneSeqPlayer
extern N_ALSeqPlayer **tuneSeqPlayers; // Official Name: tuneSeqPlayers
extern u8 gBlockVoiceLimitChange;
#define AUDIO_CHANNELS 16
#define MUSIC_CHAN_MASK_NONE 0xFFFFFFFF

/**
 * Changes the volume of each sound channel depending on what value is passed through.
 * Official Name: amSetMuteMode
 */
void amSetMuteMode(s32 behaviour) {
    switch (behaviour) {
        case VOLUME_LOWER: // Mute most sound effects and half the volume of music.
            gsSndpSetMasterVolume(0, 0);
            gsSndpSetMasterVolume(1, AL_SNDP_GROUP_VOLUME_MAX);
            n_alCSPSetVol(ambientSeqPlayer, 0);
            break;
        case VOLUME_LOWER_AMBIENT: // Mute the ambient channel, making course elements stop making noise.
            gsSndpSetMasterVolume(0, 0);
            break;
        default: // Restore sound back to normal.
            gsSndpSetMasterVolume(0, AL_SNDP_GROUP_VOLUME_MAX);
            gsSndpSetMasterVolume(1, AL_SNDP_GROUP_VOLUME_MAX);
            n_alCSPSetVol(ambientSeqPlayer, (s16) (gsSndpGetGlobalVolume() * sfxRelativeVolume));
            break;
    }
    gAudioVolumeSetting = behaviour;
}

/**
 * Queue a new music sequence to play if not blocked.
 * Stops any playing existing music beforehand.
 * Official Name: amTunePlay
 */
extern u32 *seqLen;
void amTunePlay(u8 seqID) {
#ifdef VERSION_kiosk
    if (gBlockMusicChange == FALSE) {
        if (gCanPlayMusic) {
            amTuneStop();
            music_sequence_start(seqID, tuneSeqPlayer);
        }
        gMusicTempo = n_alCSPGetTempo(tuneSeqPlayer);
        audioPrevCount = osGetCount();
        gDynamicMusicChannelMask = MUSIC_CHAN_MASK_NONE;
    }
#else
    if (gBlockMusicChange == FALSE && seqLen[seqID] <= 0x8000) {
        if (gCanPlayMusic) {
            amTuneStop();
            music_sequence_init(seqID, tuneSeqPlayer);
        }
        gMusicTempo = n_alCSPGetTempo(tuneSeqPlayer);
        audioPrevCount = osGetCount();
        gDynamicMusicChannelMask = MUSIC_CHAN_MASK_NONE;
    }
#endif
}

/**
 * Update the background music voice limit if not prevented from doing so.
 * Official Name: amTuneVoiceLimit
 */
void amTuneVoiceLimit(u8 voiceLimit) {
    if (gBlockVoiceLimitChange == FALSE) {
        n_alCSPVoiceLimit(tuneSeqPlayer, voiceLimit);
    }
}

void amTuneSetVolume(u8 volume);

extern u8 D_800A0668_A1268; // gMusicBaseVolume?
extern s32 gTuneFade;
extern s32 D_800E9524_EA124;
extern s32 D_800E9528_EA128;

void amTuneSetFade(f32 fade, u8 volume) {
    if (volume > 0x7F) {
        volume = 0x7F;
    }
    D_800E9524_EA124 = volume;
    if (osTvType == OS_TV_TYPE_PAL) {
        gTuneFade = fade * 50.0f;
    } else {
        gTuneFade = fade * 60.0f;
    }
    if (gTuneFade > 0) {
        D_800E9528_EA128 = ((D_800A0668_A1268 - volume) << 16) / gTuneFade;
    } else {
        amTuneSetVolume(volume);
    }
}

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetFadeScaled.s")
#endif

void amTuneResetFade(void) {
    gTuneFade = 0;
}

extern s32 gAmbientFade;
extern s32 D_800E952C_EA12C;
extern s32 D_800E9530_EA130;

void amAmbientSetFade(f32 fade, u8 volume) {
    if (volume > 0x7F) {
        volume = 0x7F;
    }
    D_800E952C_EA12C = volume;
    if (osTvType == OS_TV_TYPE_PAL) {
        gAmbientFade = fade * 50.0f;
    } else {
        gAmbientFade = fade * 60.0f;
    }
    if (gAmbientFade > 0) {
        D_800E9530_EA130 = ((D_800A0668_A1268 - volume) << 16) / gAmbientFade;
    } else {
        amTuneSetVolume(volume);
    }
}

void amAmbientResetFade(void) {
    gAmbientFade = 0;
}

// void music_sequence_init(N_ALCSPlayer *seqp, void *sequence, u8 *seqID, ALCSeq *seq);
void func_8000167C_227C(N_ALCSPlayer *arg0, u8 *arg1, ALCSeq *arg2);
extern ALCSeq *tuneCSeqp;
extern ALCSeq **tuneCSeqs;

extern void *gMusicSequenceData;
extern void *gJingleSequenceData;
extern ALCSeq gJingleSequence;
extern DelayedSound gDelayedSounds[8];
extern OSMesgQueue animCtrlQueue;
extern u8 D_800A0EF4;
extern s32 gDelayedSoundsCount;
extern u8 gMusicNextSeqID;
extern u8 gJingleNextSeqID;
extern u8 D_800A0F34;
extern s32 D_800A0F40;
extern s8 D_800A0F4C;
extern s16 sMusicTempo; // sMusicTempo
extern ALCSeq D_800EA2D8;
extern s32 D_800EA3D4;
extern OSMesgQueue animCtrlQueue;

extern u32 D_800A0690_A1290; // gTuneGlobalVol?
extern u8 D_800A06B0_A12B0;

#ifdef NON_EQUIVALENT
/**
 * Run every frame, this handles the transitions in and out of music sequences.
 * If there's something in the queue, then begin to play that.
 * Additionally, it also handles the delayed audio queue, counting down and playing any sounds.
 * Official Name: amAudioTick
 */
void amAudioTick(u8 updateRate) {
    s32 i, j;
    N_ALVoiceState *voiceState;
    ALChanState *chanState;
    OSMesg *unused;

    if (osRecvMesg(&animCtrlQueue, &unused, OS_MESG_NOBLOCK) == 0) {
        D_800EA3D4 = 1;
    }

    if (gTuneFade > 0 || D_800A0F40 != -1) {
        gTuneFade -= updateRate;
        if (gTuneFade < 0) {
            gTuneFade = 0;
        }
        amTuneSetVolume((((D_800E9528_EA128 * gTuneFade) >> 16) + D_800E9524_EA124) & 0xFF);
    }
    if (gAmbientFade > 0) {
        gAmbientFade -= updateRate;
        if (gAmbientFade < 0) {
            gAmbientFade = 0;
        }
        amAmbientSetVolume((((D_800E9530_EA130 * gAmbientFade) >> 16) + D_800E952C_EA12C) & 0xFF);
    }

    if (gDelayedSoundsCount > 0) {
        for (i = 0; i < gDelayedSoundsCount;) {
            gDelayedSounds[i].timer -= updateRate;
            if (gDelayedSounds[i].timer <= 0) {
                j = i;
                amSndPlay(gDelayedSounds[i].soundId, gDelayedSounds[i].handlePtr);

                gDelayedSoundsCount -= 1;
                while (j < gDelayedSoundsCount) {
                    gDelayedSounds[i].soundId = gDelayedSounds[i + 1].soundId;
                    gDelayedSounds[i].timer = gDelayedSounds[i + 1].timer;
                    gDelayedSounds[i].handlePtr = gDelayedSounds[i + 1].handlePtr;
                    j++;
                }
                j++;
            } else {
                i++;
            }
        }
    }

    if (D_800A0F34 != 0 || gMusicNextSeqID != 0) {
        voiceState = tuneSeqPlayer->vAllocHead;
        while (voiceState != NULL) {
            chanState = &tuneSeqPlayer->chanState[voiceState->channel];
            if (chanState->usechanparams) {
                __n_seqpReleaseVoice(tuneSeqPlayer, &voiceState->voice, chanState->releaseTime);
            } else if (voiceState->sound != NULL) {
                if (voiceState->sound->envelope != NULL) {
                    __n_seqpReleaseVoice(tuneSeqPlayer, &voiceState->voice, voiceState->sound->envelope->releaseTime);
                }
            }
            voiceState = voiceState->next;
        }
        n_alSynRemovePlayer(&tuneSeqPlayer->node);
        D_800A0EF4 ^= 1;
        tuneSeqPlayer = (&tuneSeqPlayers)[D_800A0EF4];
        n_alSynAddSeqPlayer(&tuneSeqPlayer->node);
        tuneCSeqp = &tuneCSeqs[(D_800A0EF4 * 62)];
    }

    if (D_800A0F34 != 0) {
        func_8000167C_227C(tuneSeqPlayer, &D_800A0F34, tuneCSeqp);
    } else {
        music_sequence_init(tuneSeqPlayer, &gMusicSequenceData, &gMusicNextSeqID, tuneCSeqp);
    }

    music_sequence_init(ambientSeqPlayer, &gJingleSequenceData, &gJingleNextSeqID, &gJingleSequence);

    if (sMusicTempo == -1 && tuneSeqPlayer->target != NULL) {
        sMusicTempo = (s16) (60000000 / n_alCSPGetTempo(tuneSeqPlayer));
    }

    D_800A06B0_A12B0 = 0;
    D_800A0F4C = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAudioTick.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amWaitForMidiSync.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amResetMidiSync.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetChlMask.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneMuteChl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneUnmuteChl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetChlVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneResetChls.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneScaleTempo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetTempoBPM.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneGetTempoBPM.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientSetTempoBPM.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientStop.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneGetSeqNo.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientGetSeqNo.s")

void amTuneSetVolume(u8 volume) {
    s32 vol;
    if (volume > 0x7F) {
        volume = 0x7F;
    }
    D_800A0668_A1268 = volume;
    vol = D_800A0690_A1290 * D_800A0668_A1268;
    n_alCSPSetVol(tuneSeqPlayer, vol);
    D_800A06B0_A12B0 = 1;
}

void amTuneSetGlobalVolume(u32 volume) {
    s32 vol;
    if (volume > 0x100) {
        volume = 0x100;
    }
    D_800A0690_A1290 = volume;
    vol = D_800A0690_A1290 * D_800A0668_A1268;
    n_alCSPSetVol(tuneSeqPlayer, vol);
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneGetVolume.s")

/**
 * Set the volume for the jingle player.
 * The jingle player scales with sfx volume rather than music volume.
 * Official Name: amAmbientSetVolume
 */
void amAmbientSetVolume(u8 volume) {
    sfxRelativeVolume = volume;
    n_alCSPSetVol(ambientSeqPlayer, (s16) (gsSndpGetGlobalVolume() * sfxRelativeVolume));
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientSetPan.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amDittyPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amDittyPlaying.s")

void amSndStop(ALSoundState *handle) {
    gsSndpStop(handle);
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndPlayDirect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndSetVol.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndSetPan.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndSetPitchDirect.s")

/**
 * Return the number of playable sounds in the audio table.
 * DKR name: sound_count
 */
extern ALBankFile *sfxBankPtr;
u16 amGetSfxCount(void) {
    return sfxBankPtr->bankArray[0]->instArray[0]->soundCount;
}

extern s32 maxSound;
extern s32 sfxIndex;
extern s32 sfxIndexSize;

void amGetSfxSettings(s32 *arg0, s32 *arg1, s32 *arg2) {
    if (arg0 != NULL) {
        *arg0 = sfxIndex;
    }
    if (arg1 != NULL) {
        *arg1 = sfxIndexSize;
    }
    if (arg2 != NULL) {
        *arg2 = maxSound;
    }
}

u8 amSoundIsLooped(u16 soundID) {
    if (soundID <= 0 || sfxBankPtr->bankArray[0]->instArray[0]->soundCount < soundID) {
        return 0;
    }
    return ((u32) (1 + sfxBankPtr->bankArray[0]->instArray[0]->soundArray[soundID - 1]->envelope->decayTime) == 0);
}

#ifdef VERSION_kiosk
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/music_sequence_start.s")
#endif

/**
 * If the sequence player is currently inactive, start a new sequence with the current properties.
 */
// void music_sequence_init(N_ALCSPlayer *seqp, void *sequence, u8 *seqID, ALCSeq *seq);
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/music_sequence_init.s")

// Could be an alternate version of the above without the sequence
void func_8000167C_227C(N_ALCSPlayer *arg0, u8 *arg1, ALCSeq *arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_8000167C_227C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/stop_ALSeqp.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetReverbOnOff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/forcelink.s")

s32 scalevol(s32 vol) {
    return (vol * 0.5f);
}
