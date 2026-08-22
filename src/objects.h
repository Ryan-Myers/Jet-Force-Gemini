
#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <PR/ultratypes.h>



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

#endif