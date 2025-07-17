#include "common.h"
#include "PR/n_libaudio.h"
#include "PR/os_reg.h"

u32 gsSndpGetGlobalVolume(void); //sndp_get_global_volume
void gsSndpSetMasterVolume(u8 channel, u16 volume); //sndp_set_group_volume
extern u8 sfxRelativeVolume;
extern u8 gAudioVolumeSetting;
extern N_ALCSPlayer *ambientSeqPlayer; // Official Name: ambientSeqPlayer
enum AudioVolumeBehaviour {
    VOLUME_NORMAL,
    VOLUME_LOWER,
    VOLUME_LOWER_AMBIENT,
    VOLUME_UNK03,
};
#define AL_SNDP_GROUP_VOLUME_MAX 32767

void amTuneStop(void);
void music_sequence_start(u8 seqID, N_ALCSPlayer *seqPlayer);

extern u8 gCanPlayMusic;
extern s32 gBlockMusicChange;
extern s32 audioPrevCount;
extern s32 gMusicTempo;
extern u32 gDynamicMusicChannelMask;
extern N_ALCSPlayer *tuneSeqPlayer; // Official Name: tuneSeqPlayer
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

void amTunePlay(u8 seqID) {
    if (gBlockMusicChange == FALSE) {
        if (gCanPlayMusic) {
            amTuneStop();
            music_sequence_start(seqID, tuneSeqPlayer);
        }
        gMusicTempo = n_alCSPGetTempo(tuneSeqPlayer);
        audioPrevCount = osGetCount();
        gDynamicMusicChannelMask = MUSIC_CHAN_MASK_NONE;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneVoiceLimit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetFade.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneResetFade.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientSetFade.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientResetFade.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAudioTick.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetGlobalVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneGetVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientSetVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amAmbientSetPan.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amDittyPlay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amDittyPlaying.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSndStop.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/music_sequence_start.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_8000170C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_80001990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_80001B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetReverbOnOff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/forcelink.s")

s32 scalevol(s32 vol) {
    return (vol * 0.5f);
}
