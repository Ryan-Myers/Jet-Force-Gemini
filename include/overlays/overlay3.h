#ifndef _OVERLAYS_OVERLAY3_H_
#define _OVERLAYS_OVERLAY3_H_

#include <PR/ultratypes.h>
#include "structs.h"

void ReallyAddInterestingEvent(s32 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6);
void CopyStaticsToSquads(void *arg0, Object *arg1);
void SquaddieControl(Object *arg0, s32 arg1);
void squadsAddToActiveSquaddies(DisactivatedSquaddie *arg0);

#endif
