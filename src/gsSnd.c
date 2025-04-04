#include "common.h"

// This file appears to be audiosfx.c in DKR.

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084848.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80084910.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085C7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085CF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/getSoundStateCounts.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80085EF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80086120.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetPriority.s")

void func_80086284(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpGetState.s")

void func_800862B4(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/ad_sndp_play.s")

#if 0
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
#else
#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStop.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/func_80086624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAllRetrigger.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpStopAllLooped.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetParam.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpGetMasterVolume.s")

void func_800867F0(void) {
}

void func_800867F8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetMasterVolume.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpSetGlobalVolume.s")

void func_80086900(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpGetGlobalVolume.s")

void func_80086914(void) {
}

void func_8008691C(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/gsSnd/gsSndpLimitVoices.s")

void func_80086964(void) {
}
