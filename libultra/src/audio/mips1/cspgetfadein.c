#include <libaudio.h>

u8 alCSPGetFadeIn(ALCSPlayer *seqp, u8 chan) {
    return seqp->chanState[chan].notemesgflags;
}
