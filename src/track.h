#ifndef _TRACK_H_
#define _TRACK_H_

#include "math/math.h"
#include <PR/ultratypes.h>

s32 trackPolyHeight(f32, f32, f32 *, s32);
void trackSetFog(s32 fogIdx, s16 near, s16 far, s16 arg3, u8 red, u8 green, u8 blue, s8 arg7);
void trackSetFogOff(s32 playerID);
void trackFadeFog(s32 fogIdx, s32 red, s32 green, s32 blue, s32 near, s32 far, f32 timer);
s32 trackNearestIntersection(s32 arg0, Vec3f *arg1, Vec3f *arg2, void *arg3, s32 arg4, s32 arg5);
void trackFreeAll(void);

#endif
