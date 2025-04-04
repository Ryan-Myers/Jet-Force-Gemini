#ifndef _LANGUAGE_C
// Hack as vs code just wasn't seeing this defined for some reason
#define _LANGUAGE_C
#endif

#include <ultra64.h>
#include "structs.h"
#include "variables.h"
#include "macros.h"
#include "functions.h"
#include "f3ddkr.h"
#include "PRinternal/viint.h"

// Animation related?
#define COLOUR_TAG_RED 0xFF0000FF
// Object related?
#define COLOUR_TAG_GREEN 0x00FF00FF
// ???
#define COLOUR_TAG_BLUE 0x0000FFFF
// ???
#define COLOUR_TAG_YELLOW 0xFFFF00FF
// ???
#define COLOUR_TAG_MAGENTA 0xFF00FFFF
// Audio related?
#define COLOUR_TAG_CYAN 0x00FFFFFF
// ???
#define COLOUR_TAG_WHITE 0xFFFFFFFF
// ???
#define COLOUR_TAG_GREY 0x7F7F7FFF
// ???
#define COLOUR_TAG_SEMITRANS_GREY 0x80808080
// ???
#define COLOUR_TAG_ORANGE 0xFF7F7FFF
// ???
#define COLOUR_TAG_BLACK 0x000000FF
// ???
#define COLOUR_TAG_NONE 0x00000000

#define RAM_END 0x80400000


#define CAST_TO_S16(var) (*((s16 *)&var))
#define CAST_TO_U8(var) (*((u8 *)&var))
