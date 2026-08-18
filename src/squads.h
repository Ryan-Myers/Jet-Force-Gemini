#ifndef _SQUADS_H_
#define _SQUADS_H_

#include <PR/ultratypes.h>
#include "structs.h"

void squadsInitialiseAfterObjects(void);
void squadsInitialiseBeforeObjects(void);
void squadsPreInit(RomDefHeader *list, s32 listSize);

#endif
