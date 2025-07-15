#include "common.h"

f32 acosf(f32);

f32 atan2f(f32 y, f32 x) {
    f32 z;

    z = 0.0f;
    if (y == 0.0f) {
        if (x >= 0.0f) {
            z = 0.0f;
        } else {
            z = 3.141592741f;
        }
    } else if (x == 0.0f) {
        if (y > 0.0f) {
            z = 1.570796371f;
        } else {
            z = 4.712388992f;
        }
    } else {
        f32 sqrt = sqrtf((y * y) + (x * x));
        if (y > x) {
            z = acosf(x / sqrt);
            if (y < 0.0f) {
                z = 6.283185482f - z;
            }
        } else {
            z = 1.570796371f - acosf(y / sqrt);
            if (x < 0.0f) {
                z = 3.141592741f - z;
            }
            if (z < 0.0f) {
                z += 6.283185482f;
            }
        }
    }
    return z;
}
