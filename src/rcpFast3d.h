#ifndef _RCPFAST3D_H_
#define _RCPFAST3D_H_

#include <PR/ultratypes.h>
#include "sched.h"

s32 rcpWaitDP(void);

void rcpInit(OSSched *sc);
void rcpInitDp(Gfx **dList);
void rcpSetScreenColour(u8 red, u8 green, u8 blue);

#endif
