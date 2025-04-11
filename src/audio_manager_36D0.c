#include "common.h"

UNUSED const char D_800AAAE0[] = "OUT OF AUDIO POINTS\n";

#define SOUND_MASK_HEAP_COUNT 40

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amInitAudioMap.s")

void amAmbientPause(void) {
    amAmbientStop();
    D_800F3604 = TRUE;
}

void amAmbientRestart(void) {
    D_800F3604 = FALSE;
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
    mathMtxXFMF(D_800F3608, x, y, z, &ox, &oy, &oz);
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

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndPlayXYZ.s")

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

void amCreateAudioPoint(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, 
    u8 arg5, u8 arg6, u16 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB, void **argC);

#if 0
void amSndPlayDirectXYZ(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5, f32 arg6, u16 arg7, u8 arg8, u8 arg9, void **argA) {
    amCreateAudioPoint(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, 0x3F, 0, argA);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndPlayDirectXYZ.s")
#endif

void amSndSetXYZ(SoundMask *soundMask, f32 x, f32 y, f32 z) {
    if (soundMask != NULL) {
        soundMask->pos.x = x;
        soundMask->pos.y = y;
        soundMask->pos.z = z;
    }
}

extern SoundMask **D_800F35F4; //gSoundMaskHeapUsed

void amSndStopXYZ(SoundMask* soundMask) {
    s32 i;

    if (soundMask != NULL) {
        for (i = 0; i < SOUND_MASK_HEAP_COUNT; i++) {
            if (soundMask == D_800F35F4[i]) {
                func_80003E24(i);
                break;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndUnlinkHandleXYZ.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amCreateAudioPoint.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndSetEcho.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003AC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003C44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003DC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/func_80003E24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_36D0/amSndGetXYZVolume.s")
