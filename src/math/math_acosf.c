#include "math.h"

/**
 * Official Name: acosf
 */
float acosf(float x) {
    signed short signed_num;
    unsigned short unsigned_num;

    if (x >= 1.0f) {
        signed_num = 0x7FFF;
    } else if (x <= -1.0f) {
        signed_num = -0x7FFF;
    } else {
        signed_num = x * 0x7FFF;
    }
    unsigned_num = acoss(signed_num);
    return (unsigned_num * MATH_PI) / 0xFFFF;
}
