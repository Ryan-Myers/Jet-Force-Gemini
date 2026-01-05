#include "common.h"

typedef struct FmvPlayer {
    s32 unk0;
    u8 pad4[0x10];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} FmvPlayer;

u32 *Table[2];
FmvPlayer Player[2];

#pragma GLOBAL_ASM("asm_us/nonmatchings/fmvInit/fmvInit.s")
