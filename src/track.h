#ifndef _TRACK_H_
#define _TRACK_H_

#include <PR/ultratypes.h>

void trackSetFog(s32 fogIdx, s16 near, s16 far, s16 arg3, u8 red, u8 green, u8 blue, s8 arg7);
void trackSetFogOff(s32 playerID);
void trackFadeFog(s32 fogIdx, s32 red, s32 green, s32 blue, s32 near, s32 far, f32 timer);
void trackFreeAll(void);

#endif
