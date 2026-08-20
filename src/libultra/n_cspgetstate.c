#include "n_libaudio.h"
#include <libaudio.h>

s32 n_alCSPGetState(N_ALCSPlayer *seqp) {
    return seqp->state;
}
