#ifndef _AUDIOMGR_H_
#define _AUDIOMGR_H_

#include <PR/ultratypes.h>

s32 amAudioMgrGetNextFrameCount(void);
void amStop(void);
void amAudioMgrSetScheduleMode(s32 arg0);

#endif
