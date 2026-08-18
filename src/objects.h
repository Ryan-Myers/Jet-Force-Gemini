#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <PR/ultratypes.h>
#include "structs.h"

extern s32 D_800A089C_A149C; // Globally used in level.c

void objSetAnimGroup(s32);
s32 *objGetTable(s32 index);
void objUndoPlayerTumble(Object *obj);
void objDoPlayerTumble(Object *this);
s32 objTvTimes(s32 timer);

#endif
