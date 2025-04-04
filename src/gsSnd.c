#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084848.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084910.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085CF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/getSoundStateCounts.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085EF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80086120.s")

typedef struct {
    u8 pad[0x40];
    u8 unk40;
    u8 pad41[0x3];
    u8 unk44;
} UnkSndPriority;

void gsSndpSetPriority(UnkSndPriority *arg0, u8 arg1) {
    if (arg0 != NULL) {
        arg0->unk40 = (s16) arg1;
    }
}

u8 gsSndpGetState(UnkSndPriority *arg0) {
    if (arg0 != NULL) {
        return arg0->unk44;
    } else {
        return 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/ad_sndp_play.s")

typedef struct unk800DC6BC_40 {
    ALLink node;
    u8 pad0C[0x38];
} unk800DC6BC_40;
typedef struct unk800DC6BC {
    ALPlayer        node;
    ALEventQueue    evtq;
    ALEvent         nextEvent;
    ALSynth        *drvr;
    u32             unk3C;
    unk800DC6BC_40 *unk40;
    s32             soundChannelsMax;
    s32             soundChannels;
    s32             frameTime;
    ALMicroTime     nextDelta;
    ALMicroTime     curTime;
} unk800DC6BC; //ALSndPlayer
extern unk800DC6BC *D_800A9F7C;
extern void		osSyncPrintf(const char *fmt, ...);
void n_alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
extern const char D_800B002C[];

void gsSndpStop(AlMsgUnk_Unk0 *msg) {
    ALEvent alEvent;

    alEvent.type = 0x400; // Could be a custom Rare event type.
    alEvent.msg.unk.unk0 = msg;
    if (msg != NULL) {
        alEvent.msg.unk.unk0->flags &= ~(1 << 4);
        n_alEvtqPostEvent(&D_800A9F7C->evtq, &alEvent, 0);
    } else {
        osSyncPrintf((char *) &D_800B002C);
        //osSyncPrintf("WARNING: Attempt to stop NULL sound aborted\n");
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80086624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAllRetrigger.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAllLooped.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetParam.s")

extern u16 *D_80105C94;

u16 gsSndpGetMasterVolume(u8 arg0) {
    return D_80105C94[arg0];
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetMasterVolume.s")

extern u32 D_800A9F80;

void gsSndpSetGlobalVolume(u32 arg0) {
    if (arg0 > 0x100) {
        arg0 = 0x100;
    }
    D_800A9F80 = arg0;
}

extern u32 D_800A9F80;

u32 gsSndpGetGlobalVolume(void) {
    return D_800A9F80;
}

void gsSndpLimitVoices(s32 arg0) {
    if (D_800A9F7C->soundChannelsMax >= arg0) {
        D_800A9F7C->curTime = arg0;
    } else {
        D_800A9F7C->curTime = D_800A9F7C->soundChannelsMax;
    }
}
