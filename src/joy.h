#ifndef _JOY_H_
#define _JOY_H_

#include <PR/ultratypes.h>
#include <PR/os_message.h>

OSMesgQueue *joyMessageQ(void);
s32 joyInit(void);
s32 joyRead(s32 saveDataFlags, s32 updateRate);
void joyResetMap(void);
void joyDisable(s32 player);
void joyEnable(s32 player);
void joyCreateMap(s8 *activePlayers);
u8 joyGetController(s32 player);
u16 joyGetButtons(s32 player);
u16 joyGetPressed(s32 player);
u16 joyGetReleased(s32 player);
s8 joyGetStickX(s32 player);
s8 joyGetAbsX(s32 player);
s8 joyGetStickY(s32 player);
s8 joyGetAbsY(s32 player);
s8 joyClamp(s8 stickMag);
void joySetSecurity(void);
s32 joyCharVal(void);

#endif
