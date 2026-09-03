#ifndef _RCPFAST3D_H_
#define _RCPFAST3D_H_

#include "sched.h"
#include <PR/ultratypes.h>

extern u8 sBackgroundPrimColourR;
extern u8 sBackgroundPrimColourG;
extern u8 sBackgroundPrimColourB;
extern s32 D_800A35E4_A41E4;         // gGfxTaskIsRunning
extern OSMesgQueue D_800FE4B8_B8CF8; // gGfxTaskMsgQueue
extern OSMesgQueue D_800FE918_B9158; // blurDoneMsgQueue
extern s32 blurTaskActive;
extern s32 cloneTaskActive;
extern OSMesgQueue refractDoneMsgQueue;
extern s32 refractTaskActive;

s32 rcpWaitDP(void);

void rcpInit(OSSched *sc);
void rcpInitDp(Gfx **dList);
void rcpSetScreenColour(u8 red, u8 green, u8 blue);

#endif
