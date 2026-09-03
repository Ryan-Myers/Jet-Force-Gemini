#ifndef _MENU_H_
#define _MENU_H_

#include "structs.h"
#include <PR/ultratypes.h>

// These vars might need to be declared in a global file just for global vars.
// They're used in multiple files, there might be a globals.c file.
typedef struct Resbitfield {
    u32 bi31 : 1;
    u32 bit30 : 1;
    u32 rest : 30;
} Resbitfield;
extern Resbitfield someResVar;
extern u64 globalflags;
extern MultiPlayer multiPlayer[4];       // Size: 0xA0 - Total Size: 0x280
extern MultiCharacter multiCharacter[4]; // Size: 0x76 - Total Size: 0x1D8
extern u8 multiCharacterType[4];
extern u8 multiGameLife[4];
extern u8 multiPlayerGame; // Bool? Like isMultiPlayerGame?
extern u8 numberOfPlayers;
extern u8 racingInGame; // Bool? Like isRacingInGame?
extern s16 D_800A3240_A3E40;
extern u8 speakerSetting;
extern u8 D_800A5938_A6538[4];
extern s8 charselquitmode;
extern u16 SFXVolume;
extern u16 musicVolume;
extern s8 widescreenVOffset;
extern u8 D_800A51A0_A5DA0;
extern u8 D_800A51A4_A5DA4;
extern u8 D_800A51A8_A5DA8;
extern u8 D_800FF386_B1D86;
extern u8 frontEndMode;
extern u8 multiGameType;
extern u8 selectedControlModes[];

void frontFreeMode(void);
u8 frontGetMode(void);
s32 frontGameSelected(void);
s32 frontGetWorldLevel(void);
s8 frontGetWideAdjust(void);
s32 frontGetLanguage(void);
void frontStoreScreenMode(void);
u8 frontRecallScreenMode(void);
s8 frontGetWideAdjust(void);
void frontSetWideAdjust(s32 offset);
u8 frontGetStereoMode(void);
void frontSetStereoMode(s32 mode);
u16 frontGetSfxVolume(void);
void frontSetSfxVolume(s32 volume);
u16 frontGetBgmVolume(void);
void frontSetBgmVolume(s32 volume);
u8 frontGetTargetControl(s32 mode);
void frontSetTargetControl(s32 mode, s32 value);
void frontCharSelectSetQuitMode(s32 mode);
s32 frontGetScreenMode(void);
void setLanguage(s32 language);
void frontSetLanguage(s32 language);
void frontSetScreenMode(s32 mode);

#endif
