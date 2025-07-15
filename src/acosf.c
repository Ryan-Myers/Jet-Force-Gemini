#include "common.h"

u16 acoss(s16);

f32 acosf(f32 x) {
    s16 signed_num;
    u16 unsigned_num;

    if (x >= 1.0f) {
        signed_num = 0x7FFF;
    } else if (x <= -1.0f) {
        signed_num = -0x7FFF;
    } else {
        signed_num = x * 0x7FFF;
    }
    unsigned_num = acoss(signed_num);
    return (unsigned_num * 3.141592741f) / 0xFFFF;
}
