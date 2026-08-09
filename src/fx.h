#ifndef _FX_H_
#define _FX_H_

#include <PR/ultratypes.h>

typedef struct UnkScreenStruct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
} UnkScreenStruct;

extern s32 D_800A6784_A7384;

s32 fxFadeOn(void);
void setupClearScreen(UnkScreenStruct*);

#endif
