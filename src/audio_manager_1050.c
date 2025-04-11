#include "common.h"

const char D_800AA950[] = "WARNING: Sync arrived before wait - music will be out of sync with sequence\n";
const char D_800AA9A0[] = "amSndPlay: Illegal sound effects table index\n";
const char D_800AA9D0[] = "amSndPlayDirect: Somebody tried to play illegal sound %d\n";
const char D_800AAA0C[] = "Invalid midi sequence index\n";
const char D_800AAA2C[] = "amTuneRestart:Jump failed - not previously pushed onto seq player %x\n";

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSetMuteMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTunePlay.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amGetSfxSettings.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amSoundIsLooped.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_8000169C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_8000170C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_80001990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/func_80001B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/amTuneSetReverbOnOff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_1050/forcelink.s")

s32 scalevol(s32 vol) {
    return (vol * 0.5f);
}
