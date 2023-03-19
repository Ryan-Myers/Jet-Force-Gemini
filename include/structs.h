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

typedef struct huft {
  u8 e;                /* number of extra bits or operation */
  u8 b;                /* number of bits in this code or subcode */
  union {
    u16 n;              /* literal, length base, or distance base */
    struct huft *t;     /* pointer to next level of table */
  } v;
} huft;

#endif
