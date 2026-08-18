#ifndef _MENU_H_
#define _MENU_H_

#include <PR/ultratypes.h>
#include "structs.h"

// These vars might need to be declared in a global file just for global vars.
// They're used in multiple files, there might be a globals.c file.
typedef struct Resbitfield {
    u32 bi31 : 1;
    u32 bit30 : 1;
    u32 rest : 30;
} Resbitfield;
extern Resbitfield someResVar;
extern u64 globalflags;
extern MultiPlayer multiPlayer[4]; // Size: 0xA0 - Total Size: 0x280
extern MultiCharacter multiCharacter[4]; // Size: 0x76 - Total Size: 0x1D8
extern u8 multiCharacterType[4];
extern u8 multiGameLife[4];
extern u8 multiPlayerGame; // Bool? Like isMultiPlayerGame?
extern u8 numberOfPlayers;
extern u8 racingInGame; // Bool? Like isRacingInGame?
extern s16 D_800A3240_A3E40;

s32 frontGetLanguage(void);

#endif
