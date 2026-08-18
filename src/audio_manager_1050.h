#ifndef _AUDIO_MANAGER_1050_H_
#define _AUDIO_MANAGER_1050_H_

#include "structs.h"
#include "libultra_internal.h"

void *ad_sndp_play(ALBank *arg0, s16 arg1, u16 arg2, u8 arg3, f32 arg4, u8 arg5, void **arg6);
u16 amGetSfxCount(void);
s32 scalevol(s32 vol);

#endif
