#ifndef _PI_H_
#define _PI_H_

#include "structs.h"

void piInit(void);
u32 *piRomLoad(u32 assetIndex);
u8 *piRomLoadCompressed(u32 assetIndex, s32 extraMemory);
s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
u8 *piRomGetSectionPtr(u32 assetIndex, u32 assetOffset);
s32 piRomGetFileSize(u32 assetIndex);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);

#endif
