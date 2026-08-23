#include "n_synthInternals.h"
#include <gu.h>
#include <os.h>

// Taken from Perfect Dark's repo, and modified slightly.

void n_alFxInitlpfilter_mono(struct fx *fx, f32 outputrate);

Acmd *n_alLPFilterPull(N_PVoice *f, s16 *outp, s32 outCount, Acmd *p) {
    Acmd *ptr = p;
    s32 pad;
    f32 sp28;

    ptr = n_alResamplePull(f, outp, p);

    if (f->unk8c != 0 && f->unk8c < 64) {
        if (f->unk8c >= 6) {
            sp28 = 26755 / sqrtf(f->unk8c + 1.0f);
        } else {
            sp28 = 65536 / (f->unk8c + 1.0f);
        }

        if (sp28 < 7723) {
            sp28 = 7723;
        }

        n_aNoop(ptr++, *outp, sp28, f->unk8c + 1);
    }

    if (f->fx.unk02 > 0) {
        if (f->unkb8 != 0) {
            n_alFxInitlpfilter_mono(&f->fx, 22050);
        }

        n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(f->fx.unk08))

            if (f->unkb8 == 2) {
            f->unkb8 = 0;
        }

        n_aPoleFilter(ptr++, f->unkb8, *outp, 0, osVirtualToPhysical(f->unkbc) & 0xffffff);

        f->unkb8 = 0;
    }

    return ptr;
}

s32 n_alLPFilterParam(N_PVoice *filter, s32 paramID, void *param) {
    f32 *f = (f32 *) &param;

    switch (paramID) {
        case (AL_FILTER_RESET):
            filter->fx.unk02 = 0;
            n_alLoadParam(filter, AL_FILTER_RESET, param);
            break;
        case (AL_FILTER_12):
            filter->fx.unk02 = (s32) param;
            filter->unkb8 |= 2;
            break;
        case (AL_FILTER_13):
            filter->fx.unk00 = *f;
            filter->unkb8 |= 2;
            break;
        case (AL_FILTER_11):
            filter->unk8c = (u8) param;
            break;
        default:
            n_alLoadParam(filter, paramID, param);
            break;
    }

    return 0;
}
