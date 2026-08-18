#ifndef _WEATHER_H_
#define _WEATHER_H_

#include <PR/ultratypes.h>

void freeWeather(void);
void setWeatherLimits(s16 near, s16 far);
void setupWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

#endif
