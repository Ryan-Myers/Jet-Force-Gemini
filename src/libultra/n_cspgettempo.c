#include "n_libaudio.h"
#include <PR/ultratypes.h>

// From DP

s32 n_alCSPGetTempo(N_ALCSPlayer *seqp) {
    /* sct 1/8/96 - If no target sequence has been set, just return zero (instead of crashing). */
    if (seqp->target == NULL) {
        return 0;
    }

    return seqp->uspt / seqp->target->qnpt;
}
