#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <PR/ultratypes.h>
#include "structs.h"

/* Size: 10 bytes. Shared across every entry. */
typedef struct LevelObjectEntryCommon {
    u16 objectID;
    u8 size;
    s16 x, y, z; // Position in level
} LevelObjectEntryCommon;

enum ObjectSpawnFlags {
    OBJECT_SPAWN_NONE,
    OBJECT_SPAWN_UNK01 = (1 << 0),
    OBJECT_SPAWN_UNK02 = (1 << 1),
    OBJECT_SPAWN_UNK04 = (1 << 2),
    OBJECT_SPAWN_UNK08 = (1 << 3),
    OBJECT_SPAWN_NO_LODS = (1 << 4) // Forces the object to have 1 model associate. Used to prevent LoD models of player model racers spawning.
};

extern s8 D_800F2C60_F3860;
extern s32 D_800F2C6C_F386C;
extern s32 D_800F2C70_F3870;
extern s32 D_800F2CAC_F38AC;
extern s32 D_800F2CB8_F38B8;
extern s32 D_800F2CBC_F38BC;
extern s16 D_800F2CC0_F38C0;
extern s16 D_800F2CC2_F38C2;
extern s32 D_800F2CC4_F38C4;
extern s32 D_800F2D08_F3908;
extern s32 D_800F2D10_F3910;
extern s32 D_800F2D1C_F391C;
extern s32 D_800F2D40_F3940;
extern s32 ObjListCount;
extern Object **ObjList; // Not sure about the number of elements
extern VertexPosition gEnvmapPos;
extern s32 *Ftables;
extern s32 *Findex;
extern s32 Fmax;
extern s8 D_800A0980_A1580;
extern f32 D_800F2F60_F3B60;

extern s32 D_800A089C_A149C; // Globally used in level.c

void objSetAnimGroup(s32);
s32 *objGetTable(s32 index);
void objUndoPlayerTumble(Object *obj);
void objDoPlayerTumble(Object *this);
s32 objTvTimes(s32 timer);

#endif
