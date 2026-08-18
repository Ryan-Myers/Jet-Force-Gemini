#include "audio.h"
#include "common.h"
#include "main.h"
#include "math/math.h"

const char D_800AAAE0[] = "OUT OF AUDIO POINTS\n";

#define SOUND_MASK_HEAP_COUNT 40

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amInitAudioMap.s")

void amAmbientPause(void) {
    amAmbientStop();
    D_800F2A04_F3604 = TRUE;
}

void amAmbientRestart(void) {
    D_800F2A04_F3604 = FALSE;
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amResetAudioMap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amPlayAudioMap.s")

s32 amCalcSfxStereo(f32 x, f32 y, f32 z) {
    s16 arctan;
    s32 ret;
    f32 ox;
    f32 oy;
    f32 oz;

    if (mainGetNumberOfCameras() >= 2) {
        return 64;
    }
    mathMtxXFMF(D_800F2A08_F3608, x, y, z, &ox, &oy, &oz);
    arctan = Arctanf(ox, oz);
    if (arctan >= -0x2000 && arctan <= 0x2000) {
        ret = 64 + ((arctan * 0x3F) / 8192);
    } else if (arctan < -0x2000 && arctan > -0x6000) {
        ret = (64 + ((s32) ((-arctan * 0x3F) + 0xFFE86000) / 16384)) | 0xFF000000;
    } else if (arctan > 0x2000 && arctan < 0x6000) {
        ret = (64 + ((s32) ((-arctan * 0x3F) + 0x17A000) / 16384)) | 0xFF000000;
    } else {
        ret = 64 | 0xFF000000;
    }
    return ret;
}

void amCreateAudioPoint(u16 soundId, f32 x, f32 y, f32 z, u8 arg4, u8 arg5, u8 volume, u16 distance, u8 arg8, u8 pitch,
                        u8 argA, u8 argB, SoundMask **soundMask);

extern SoundData *D_800F29F0_F35F0;

void amSndPlayXYZ(u16 soundId, f32 x, f32 y, f32 z, u8 arg4, SoundMask **soundMask) {

    if (soundMask == NULL || *soundMask == NULL) {
        amCreateAudioPoint(D_800F29F0_F35F0[soundId].soundBite, x, y, z, arg4, D_800F29F0_F35F0[soundId].minVolume,
                           D_800F29F0_F35F0[soundId].volume, D_800F29F0_F35F0[soundId].range, 0,
                           D_800F29F0_F35F0[soundId].pitch, D_800F29F0_F35F0[soundId].priority,
                           D_800F29F0_F35F0[soundId].unk5, soundMask);
    }
}

void amSndSetVolXYZ(SoundMask *soundMask, u8 volume) {
    if (soundMask != NULL) {
        soundMask->volume = volume;
    }
}

void amSndSetPitchXYZ(SoundMask *soundMask, u8 pitch) {
    if (soundMask != NULL) {
        soundMask->pitch = pitch;
    }
}

void amSndPlayDirectXYZ(u16 soundId, f32 x, f32 y, f32 z, u8 arg4, u8 volume, f32 pitch, u8 arg7,
                        SoundMask **soundMask) {
    amCreateAudioPoint(soundId, x, y, z, arg4, 100, volume, 15000, 0, pitch, 0x3F, 0, soundMask);
}

void amSndSetXYZ(SoundMask *soundMask, f32 x, f32 y, f32 z) {
    if (soundMask != NULL) {
        soundMask->pos.x = x;
        soundMask->pos.y = y;
        soundMask->pos.z = z;
    }
}

extern SoundMask **D_800F29F4_F35F4; // gSoundMaskHeapUsed
extern u16 D_800A0800_A1400;

void amSndStopXYZ(SoundMask *soundMask) {
    s32 i;

    if (soundMask != NULL) {
#ifdef VERSION_kiosk
        for (i = 0; i < SOUND_MASK_HEAP_COUNT; i++)
#else
        for (i = 0; i < D_800A0800_A1400; i++)
#endif
        {
            if (soundMask == D_800F29F4_F35F4[i]) {
                func_80003B74_4774(i);
                break;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndUnlinkHandleXYZ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amCreateAudioPoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndSetEcho.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_8000380C_440C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003994_4594.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003B14_4714.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003B74_4774.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndGetXYZVolume.s")
