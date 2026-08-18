#ifndef _CHARCONTROL_H
#define _CHARCONTROL_H

#include "common.h"

typedef struct ControlPlayer {
    /* 0x000 */ u8 unk00;
    /* 0x001 */ s8 gravityType; // Possibly a bitfield.
    /* 0x002 */ u8 pad02[0x18 - 0x02];
    /* 0x018 */ Vec3f unk18;
    /* 0x024 */ u8 pad24[0x96 - 0x24];
    /* 0x096 */ s16 sidekickTiltList; // Type unknown
    /* 0x098 */ u8 pad98[0xF8 - 0x98];
    /* 0x0F8 */ s16 playerTiltList; // Type unknown
    /* 0x0FA */ u8 padFA[0x194 - 0xFA];
    /* 0x194 */ u8 gravityDisabled;
    /* 0x195 */ u8 pad195[0x575 - 0x195];
    /* 0x575 */ s8 gravityScaleIndex;
} ControlPlayer;

s16 dAngle(s16 arg0, s16 arg1, f32 arg2);
void controlDecapitatePlayer(s32 arg0);
void controlMakeGravity(ControlPlayer *player);
void controlFSUvels(s16 *rotation, ControlPlayer *player);
void controlDisableJoypad(s32 arg0);
void controlReadJoypad(s32 player);
s16 *controlPlayerTiltList(Object *obj);
s16 *controlSidekickTiltList(Object *obj);
void controlClearPlayerSetup(void);
void *controlGetWeaponDef(s32);

#endif
