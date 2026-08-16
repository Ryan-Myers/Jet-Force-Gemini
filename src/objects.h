
#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <PR/ultratypes.h>



/* Size: 8 bytes. Shared across every entry. */
typedef struct LevelObjectEntryCommon {
    u8 objectID; // 9-bit object ID to load (uses size's MSB).
    u8 size; // 7-bit total entry length (MSB is used in object_id).
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