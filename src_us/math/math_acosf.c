#include "math.h"

/**
 * Official Name: acosf
 */
f32 acosf(f32 value) {
    s16 intval;
    u16 uintval;

    if (value >= 1) {
        intval = 0x7FFF;
    } else if (value <= -1) {
        intval = -0x7FFF;
    } else {
        intval = value * 0x7FFF;
    }

    uintval = acoss(intval);
    return (uintval * MATH_PI) / 0xFFFF;
}
