#ifndef _STRUCTS_H_
#define _STRUCTS_H_

/* Note: Structs are not complete, take them with a grain of salt. */

#include "types.h"

typedef struct Object {
	s32 unk0;
} Object;

typedef struct VertexPosition {
/* 0x00 */ s16 x;
/* 0x02 */ s16 y;
/* 0x04 */ s16 z;
} VertexPosition;

#endif
