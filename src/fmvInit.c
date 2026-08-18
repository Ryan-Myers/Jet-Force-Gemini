#include "common.h"

typedef struct FmvPlayer {
    s8 unk0;
    u8 pad4[0x10];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} FmvPlayer;

u32 *Table[2];
FmvPlayer Player[2];

void fmvInit(void) {
    FmvPlayer* player;
    s32 i;

    Table[0] = piRomLoad(0x45);
    player = Player;

    i = 2;
    while (i--) {
        player->unk0 = -1;
        player->unk14 = 0;
        player->unk18 = 0;
        player->unk1C = 0;
        player->unk20 = 0;
        player++;
    }
}
