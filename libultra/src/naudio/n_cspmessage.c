#include "common.h"
#include "n_libaudio.h"

// Not 100% confident on the types used here, but it does pass the sniff test.

void n_alCSPSetMessageQ(N_ALCSPlayer *seqp, OSMesgQueue *mq) {
    seqp->queue = mq;
}

void n_alCSPSetMessage(N_ALCSPlayer *seqp, u8 index, u8 flag) {
    ALInstrument *inst;

    if (index < seqp->bank->instCount) {
        inst = seqp->bank->instArray[index];
        if (inst != NULL) {
            inst->flags = (flag << 1) | (inst->flags & 1);
        }
    }
}

void n_alCSPSetChanMessage(N_ALCSPlayer *seqp, s32 index, u8 flag) {
    seqp->chanState[index].notemesgflags = flag;
}
