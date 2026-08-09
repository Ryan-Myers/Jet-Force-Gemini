#ifndef _MATH_H_
#define _MATH_H_

#include "common.h"

#define MATH_PI 3.141592741f
#define DTOR(deg)  ((deg) * MATH_PI / 180.0f)

u16 acoss(s16 x);
s16 asins(s16 x);
f32 atan2f(f32 x, f32 z);
f32 acosf(f32 value);

f32 Sinf(s32);
u16 Arctanf(f32, f32);

extern float sinf(float angle); //official name: fsin
extern float cosf(float angle);
extern float sqrtf(float value);

#endif
