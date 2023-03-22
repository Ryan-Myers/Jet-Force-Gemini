#include <ultra64.h>
#include "structs.h"
#include "variables.h"
#include "macros.h"


void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);
void mmFree(void *data);
u8 *rzipUncompress(u8 *compressedInput, u8 *decompressedOutput);
s32 rzipUncompressSize(u8 *arg0);
s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
s32 inflate_block(void);
void _blkclr(void *, size_t);

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
