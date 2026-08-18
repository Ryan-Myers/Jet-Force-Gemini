#ifndef _MAIN_H_
#define _MAIN_H_

#include <PR/ultratypes.h>
#include "enums.h"
#include "gbi.h"
#include "structs.h"

extern void segSetBase(Gfx **dlist, s32 segment, s32 base);

#ifdef VERSION_kiosk
// The kiosk doesn't have this function, but it can be used so many times in a single function, 
// so this is a macro to avoid having to ifdef it out multiple times.
#define mainPreNMI()
#else
void mainPreNMI(void);
#endif

void mainThread(void *);
u8 mainGetNumberOfPlayers(void);
void RevealReturnAddresses(void);
void func_80044FAC_45BAC(void);
void func_800468EC_474EC(s32 arg0);
void mainChangeLevel(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void mainInitGame(void);
void mainSetGameFlag(GameFlags arg0, s32 arg1);
Game *mainGetGame(void);
Game *mainGetGameArrayPtr(void);
s32 mainGetNumberOfCameras(void);
s32 mainResetPressed(void);
void mainSetDefaultCharacter(MultiCharacter *character, s32 arg1);
void mainInitMultiPlayer(void);

#endif
