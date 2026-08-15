#ifndef _WEATHER_H_
#define _WEATHER_H_

#include <PR/ultratypes.h>

extern s32 D_801008B4_BB0F4;

void initWeather(void);
void setWeatherLimits(s16 near, s16 far);
void freeWeather(void);
void setupWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void changeWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void doWeather(Gfx **currDisplayList, Mtx **currHudMat, Vertex **currHudVerts, Triangle **currHudTris, s32 updateRate);
void rainSetFog(void);
f32 rainDensity(void);

#endif