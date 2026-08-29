#ifndef _SQUADS_H_
#define _SQUADS_H_

#include <PR/ultratypes.h>
#include "structs.h"

//squads.c
extern s32 GlobalSizeOfRomdefList;
extern RomDefHeader *GlobalStartOfRomdefList;
extern s32 PlayersDisguise;

void squadsInitialiseAfterObjects(void);
void squadsInitialiseBeforeObjects(void);
void squadsPreInit(RomDefHeader *list, s32 listSize);
s32 ProcessNodeChange(u8 arg0);

#endif
