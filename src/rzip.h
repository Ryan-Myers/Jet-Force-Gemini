#ifndef _RZIP_H_
#define _RZIP_H_

#include "structs.h"
#include <PR/ultratypes.h>

s32 inflate_block(void);      // hasm file
void _blkclr(void *, size_t); // bzero hasm alternate name

void rzipInit(void);
s32 rzipUncompressSize(u8 *arg0);
s32 rzipUncompressSizeROM(s32 assetIndex, s32 assetOffset);
u8 *rzipUncompress(u8 *compressedInput, u8 *decompressedOutput);
void huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, huft **t, s32 *m);

#endif
