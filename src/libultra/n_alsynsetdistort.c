#include "common.h"

#include "n_synthInternals.h"
#include <n_libaudio.h>
#include <ultra64.h>

void n_alSynSetDistort(N_ALVoice *voice, u8 data) {
    ALParam *update;

    if (voice->pvoice != NULL) {
        update = __n_allocParam();
        if (update == NULL) {
            return;
        }
        update->delta = n_syn->paramSamples + voice->pvoice->offset;
        update->type = AL_FILTER_11;
        update->data.i = data;
        update->next = NULL;
        n_alEnvmixerParam(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}
