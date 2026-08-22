#ifndef _AUDIOMGR_H_
#define _AUDIOMGR_H_

#include <PR/ultratypes.h>

extern s32 D_800A06DC_A12DC;
extern s32 D_800A06E4_A12E4;

s32 amAudioMgrGetNextFrameCount(void);
void amStop(void);
void amAudioMgrSetScheduleMode(s32 arg0);

#endif
