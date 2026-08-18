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
void mathOneFloatPY(Vec3s*, Vec3f*);
void mathOneFloatRPY(Vec3s *rotation, Vec3f *vec);
void mathOneFloatYPR(Vec3s *rotation, Vec3f *vec);
float Powerf(float base, int exp);
void mathMtxXFMF(Matrix mf, float x, float y, float z, float *ox, float *oy, float *oz);
void mathMtxFastXFMF(Matrix* mf, f32* in, f32* out);
void pointListRPY(s32 count, Vec3s *rotation, f32 *in, f32 *out);

extern float sinf(float angle); //official name: fsin
extern float cosf(float angle);
extern float sqrtf(float value);


u32 disableInterrupts(void);
void enableInterrupts(u32 flags);
s32 mathRnd(s32, s32);

#endif
