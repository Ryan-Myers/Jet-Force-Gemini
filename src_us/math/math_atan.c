#include "common.h"
#include "math.h"

/**
 * Official Name: atan2f
 */
f32 atan2f(f32 x, f32 z) {
    f32 result = 0;

    if (x == 0) {
        if (z >= 0) {
            result = 0;
        } else {
            result = DTOR(180);
        }
    } else if (z == 0) {
        if (x > 0) {
            result = DTOR(90);
        } else {
            result = DTOR(270);
        }
    } else {
        f32 sqrt = sqrtf(x * x + z * z);

        if (x > z) {
            result = acosf(z / sqrt);

            if (x < 0) {
                result = DTOR(360) - result;
            }
        } else {
            result = DTOR(90) - acosf(x / sqrt);

            if (z < 0) {
                result = DTOR(180) - result;
            }

            if (result < 0) {
                result += DTOR(360);
            }
        }
    }

    return result;
}
