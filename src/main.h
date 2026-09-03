#ifndef _MAIN_H_
#define _MAIN_H_

#include "enums.h"
#include "gbi.h"
#include "structs.h"
#include <PR/ultratypes.h>
#include <sched.h>

extern s8 D_800FD7BC_B3B0C;
extern s32 D_800A3290_A3E90;
extern s32 D_800A3530_A4130;
extern OSSched sc;
extern u64 Time[1024];
extern void *securitybuffer;
extern s32 mainGameMode;
extern u8 mainGameAutoSave;
extern s16 D_800A3230_A3E30; // mainGameWindowIsChanging
extern s16 D_800A3260_A3E60;
extern s32 D_800A331C_A3F1C; // RamTest1Result
extern s32 D_800A3320_A3F20; // RamTest2Result
extern s32 D_800FD7D0_B8010;
extern s32 load_save_flags;
extern s16 D_800A323C_A3E3C;
extern s16 D_800A3248_A3E48;
extern s16 D_800A3250_A3E50;
extern s16 D_800A3254_A3E54;
extern s16 D_800A325C_A3E5C;
extern u8 D_800A326C_A3E6C;
extern s16 D_800A32BC_A3EBC;
extern u8 numberOfCameras;
extern Game *gameplay;
extern Game *D_800FD7D8_B3B28;
extern s32 debugMenuEnable;
extern s32 debugInputCode[14];
extern s32 debugInputCount;
extern s8 D_800FD7BD_B3B0D;

// Window Size Vars
extern s16 mainGameWindowSizeX1;
extern s16 mainGameWindowSizeY1;
extern s16 mainGameWindowSizeX2;
extern s16 mainGameWindowSizeY2;

typedef struct UnkD_800FE217 {
    s8 ZBCheck;
    u8 pad1[0x7];
} UnkD_800FE217;
extern UnkD_800FE217 D_800FD757_B7F97[8];

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
s8 mainGetPauseMode(void);
s32 mainAddZBCheck(s32 arg0, s32 arg1, s32 arg2);
s8 mainGetZBCheck(s32 arg0);
s32 mainGameChanged(void);
void mainFrontInit(s32 arg0, s32 arg1, s32 arg2);

#endif
