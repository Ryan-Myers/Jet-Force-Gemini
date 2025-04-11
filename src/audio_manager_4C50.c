#include "common.h"

const char D_800AAB00[] = "am: Unknown OSCILLATOR type %d\n";
const char D_800AAB20[] = "am: Out of oscillator states\n";

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_4C50/amVibratoInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_4C50/amInitOsc.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_4C50/amUpdateOsc.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_4C50/amStopOsc.s")

#if 0
f32 _depth2Cents(u8 arg0) {
    f32 var_f0;
    f32 var_f2;
    u32 temp_t8;

    var_f0 = 1.030992985f;
    var_f2 = 1.0f;
    if (arg0 != 0) {
        do {
            if (arg0 & 1) {
                var_f2 *= var_f0;
            }
            var_f0 *= var_f0;
            temp_t8 = (arg0 >> 1);
            arg0 = temp_t8 & 0xFF;
        } while (temp_t8 != 0);
    }
    return var_f2;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/audio_manager_4C50/_depth2Cents.s")
#endif
