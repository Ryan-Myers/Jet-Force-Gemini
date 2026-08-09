#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"
#include "enums.h"


extern ALBankFile *sfxBankPtr;

//spranim.c
extern s32 disablejoy;
extern s32 controlReleasedKeys;
extern s32 controlAbsXjoy;
extern s32 controlAbsYjoy;
extern s32 controlDkeys;
extern s32 controlKeys;
extern s32 controlXjoy;
extern s32 controlYjoy;
extern f32 controlchr_gravity;
extern s32 D_800A2DB4_A39B4[];

//objects.c
extern s8 D_800F2C60_F3860;
extern s32 D_800F2C6C_F386C;
extern s32 D_800F2C70_F3870;
extern s32 D_800F2CAC_F38AC;
extern s32 D_800F2CB8_F38B8;
extern s32 D_800F2CBC_F38BC;
extern s16 D_800F2CC0_F38C0;
extern s16 D_800F2CC2_F38C2;
extern s32 D_800F2CC4_F38C4;
extern s32 D_800F2D08_F3908;
extern s32 D_800F2D10_F3910;
extern s32 D_800F2D1C_F391C;
extern s32 D_800F2D40_F3940;
extern s32 ObjListCount;
extern Object **ObjList; // Not sure about the number of elements
extern VertexPosition gEnvmapPos;
extern s32 *Ftables;
extern s32 *Findex;
extern s32 Fmax;
extern s8 D_800A0980_A1580;
extern f32 D_800F2F60_F3B60;

//diCpu.c
extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801024E0_BCD20;
extern OSMesg D_801024F8_BCD38[8];
extern OSMesg D_80102518_BCD58[8];
extern OSMesgQueue D_80102538_BCD78;
extern s32 D_800A64C8_A70C8;
extern s32 D_800A64CC_A70CC;
// extern char D_800AF4EC[26];// = "Fault in thread %d   (%s)";
// extern char D_800AF508[9];// = "epc NULL";
// extern char D_800AF650[16];// = "free ram 		%08x";
extern s32 D_800A651C_A711C;

//prnBorders.c
extern Gfx D_800A3F90[11][2]; //unknown number of graphics commands. This is a best guess.

//font.c
typedef enum TextFonts {
    FONT_COLOURFUL,
    FONT_SMALL,
    FONT_LARGE,
    FONT_UNK_FF = 255
} TextFonts;
extern char *combineBuffer;
extern char *printBuffer;
extern char *convertBuffer;
extern DialogueBoxBackground Window[];
extern u8 fontInUse;
extern u8 buttonMode;
extern s16 D_80103FB2_BE7F2; //Window->y2
extern s16 D_80103FB4_BE7F4; //Window->width
extern DialogueBoxBackground D_80103FD0_BE810; //Window[1]
//Window noise?
extern s32 D_800A6EA8_A7AA8;
extern s32 D_800A6EAC_A7AAC;
extern s32 D_800A6EB0_A7AB0;
// Descending powers of 10
extern s32 D_800A6EB4_A7AB4[9];
// s32 D_800A6EB4_A7AB4[9] = {
//     1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
// };

extern Gfx D_800A6E30_A7A30[7];
extern u32 D_A7858; //Font asset location?
extern u32 D_A7318; //Font asset location?
extern u32 *cache;
extern u8 *dxTable;
extern u8 squash;
extern DialogueTextElement String[]; //Sizeof(32)?
//dDialogueBoxBegin?
extern char D_800A6F04_A7B04[100];

/* Size: 10 bytes */
typedef struct FontCharDataAlt {
    u8 textureID; // Texture Pointer Index?
    u8 ulx; // Upper Left Corner? Maybe only used when FontData->unk20 is 0 for some reason.
    u8 width; // Font Char Width?
    u8 height; // Font Char Height?
    u8 s; // Upper left coordinate for the texture derived from X?
    u8 t; // Upper left coordinate for the texture derived from Y?
    u8 lrx; // Related to the lower right X Coordinate.
    u8 lry; // Related to the lower right Y Coordinate.
    u8 pad[8];
} FontCharDataAlt;
extern FontCharDataAlt *Font;

//main.c
extern s8 D_800FD7BC_B3B0C;
extern s32 D_800A3290_A3E90;
extern s32 D_800A3530_A4130;
extern OSSched sc;
extern u64 Time[1024];
extern void *securitybuffer;
extern s32 mainGameMode;
extern u8 mainGameAutoSave;
extern s16 D_800A3230_A3E30; //mainGameWindowIsChanging
extern s16 D_800A3260_A3E60;
extern s32 D_800A331C_A3F1C; //RamTest1Result
extern s32 D_800A3320_A3F20; //RamTest2Result
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
extern u8 D_800A32A0_A3EA0;
extern s32 gSuppressWidescreen;

typedef struct Resbitfield {
    u32 bi31 : 1;
    u32 bit30 : 1;
    u32 rest : 30;
} Resbitfield;
extern Resbitfield someResVar;

//Window Size Vars
extern s16 mainGameWindowSizeX1;
extern s16 mainGameWindowSizeY1;
extern s16 mainGameWindowSizeX2;
extern s16 mainGameWindowSizeY2;

// extern char D_800AD170[]; // = "%5d  %5d  %5d";
// extern char D_800AD180[]; // = "FREE %d";
// extern char D_800AD188[]; // = "%d K";

typedef struct UnkD_800FE217 {
    s8 ZBCheck;
    u8 pad1[0x7];
} UnkD_800FE217;
extern UnkD_800FE217 D_800FD757_B7F97[8];

//main.c?
extern u64 globalflags;

//sched.c
extern char D_800AD510[];// = "(Audio task)";
extern char D_800AD520[];// = "(Game task)";
extern char D_800AD52C[];// = "(DI task)";
extern char D_800AD538[];// = "(DI benchmark test)";
extern char D_800AD550[];// = "(Clone task)";
extern char D_800AD560[];// = "(Refract task)";
extern char D_800AD570[];// = "(Blur task)";
extern char D_800AD580[];// = "(Unknown task type %d)";
extern char D_800AD598[];// = "RCP TASK INFO-------------"; //AND MUCH MORE STRING CONTENT
extern char D_800AD7D4[];// = "SP CRASHED, gfx=%x";
extern char D_800AD7E8[];// = "DP CRASHED, gfx=%x";
extern char D_800AD7FC[];// = "** GFX overflow **";
extern char D_800AD810[];// = "** MTX overflow **";
extern char D_800AD824[];// = "** VTX overflow **";
extern char D_800AD838[];// = "** POL overflow **";
extern char D_800AD84C[];// = "Version %s";
extern char D_800AD7D0[];// = "7.2";
extern s32 D_800A38B0_A44B0[]; // = { OSMESG_SWAP_BUFFER, OSMESG_SWAP_BUFFER };
extern s32 D_800A38B8_A44B8[]; //gBootBlackoutMesg[] = { OSMESG_SWAP_BUFFER, MESG_SKIP_BUFFER_SWAP };
extern u8 gGfxOverflowed;
extern u8 gMtxOverflowed;
extern u8 gVtxOverflowed;
extern u8 gPolOverflowed;
extern s32 gCurRSPTaskCounter;
extern s32 gCurRDPTaskCounter;
extern s8 gNextFrameCount;
extern u64 gRetraceCounter64;
extern s32 gCurRSPTaskIsSet;
extern s32 gCurRDPTaskIsSet;
extern s32 *currentScreen;
extern s32 *otherScreen;
extern s32 *otherZbuf;

//dicpu.c
//These two string are the file name and extensions 
//for what looks like debug data that gets written to a controller pak
// extern char D_800AE850_AF450[];// = "CORE";
// extern char D_800AE858_AF458[];// = "";
// extern char D_800AF40C[];// = 
// // "%08x:%08x
// // MEMORY REGION %d
// // MOD OFFSET
// // SIZE
// // ADDRESS
// // Texture %d
// // Module %d   %d
// // %08x %d
// // Module %d at %08x
// // %1x %d lines logged
// // Page %d/%d"
// extern char D_800AF4AC[];// = "Watchpoint exception at %x";
// extern char D_800AF4C8[]; // = "Module %d at %08x";

//lights.c
extern s32 D_800A1000_A1C00; //gMaxLights
extern ObjectLight *D_800A100C_A1C0C;
extern s32 D_800A1004_A1C04;
extern ObjectLight **D_800A1008_A1C08; //gActiveLights

//textures.c
extern u8 blinkMode;
extern u32 D_800A4E60_A5A60; //gTexColourTag = COLOUR_TAG_MAGENTA;
extern s32 D_800A4E68_A5A68; //D_80126378
extern u8 D_800A4E6C_A5A6C;
extern s32 D_800FED14_B9554; //D_80126354 in DKR
extern s32 *D_800FECE0_B9520[2]; //D_80126320 in DKR
extern s32 D_800FED30_B9570;
extern s32 D_800FED34_B9574;
extern s32 D_800FED38_B9578;
extern s32 D_800FED3C_B957C;
extern s32 D_800FED40_B9580;
extern s32 D_800FED44_B9584;
extern s32 D_800A4E7C_A5A7C; //A store of the texture currently loading?
extern s32 D_800A4E74_A5A74; //IA2ColOverride?
extern u8 D_800FED48_B9588;
extern u8 D_800FED49_B9589;
extern u8 D_800FED4A_B958A;
extern u8 D_800FED4B_B958B;
extern u8 D_800FED4C_B958C;
extern u8 D_800FED4D_B958D;
extern s32 D_800A4E64_A5A64; //textureFilter

//rcpFast3d.c
extern u8 sBackgroundPrimColourR;
extern u8 sBackgroundPrimColourG;
extern u8 sBackgroundPrimColourB;
extern s32 D_800A35E4_A41E4; // gGfxTaskIsRunning
extern OSMesgQueue D_800FE4B8_B8CF8; //gGfxTaskMsgQueue
extern OSMesgQueue D_800FE918_B9158; //blurDoneMsgQueue
extern s32 blurTaskActive;
extern s32 cloneTaskActive;
extern OSMesgQueue refractDoneMsgQueue;
extern s32 refractTaskActive;


//saves.c
extern OSPfs pfs[MAXCONTROLLERS];
extern OSMesgQueue *sControllerMesgQueue;
extern s32 D_800FDDB4_B85F4; //UNUSED
extern u8 gN64FontCodes[];
extern s8 *sPackDirectory;
extern u8 sRumblePaksPresent;
extern u8 D_800A3448_A4048; //sControllerPakPresent?
extern OSMesgQueue flashEventQueue;
extern OSMesg flashEventBuf[1];
extern OSMesg cartEventBuf[1];
extern OSMesgQueue cartEventQueue;
extern OSIoMesg flashMesgReqBlock;
extern u8 D_800A3464_A4064;
extern u8 D_800A346C_A406C;
extern f32 D_800AD4FC;
extern RumbleStruct rumbleStructArray[];
extern f32 D_800AD504;
extern f32 D_800AD500;
extern RumbleStruct D_800FDF5A_B879A[];

//Size: 0xA
//extern unkD_800FEC68 *rumbleStructArray;


typedef struct unkD_800A3EAC {
    s32 unk0;
    f32 unk4;
} unkD_800A3EAC;
extern unkD_800A3EAC D_800A344C_A404C[3];

//camera.c
extern ScreenViewport D_800A3728[4]; //gScreenViewports

//audiomgr.c
extern s32 nextFrameCount; // = 2;

//squads.c
extern s32 GlobalSizeOfRomdefList;
extern RomDefHeader *GlobalStartOfRomdefList;
extern s32 PlayersDisguise;

//gameVi.c
extern s8 sResolutionIndex;

// audio_manager_36D0.c
extern Matrix D_800F2A08_F3608; //.data
extern s8 D_800F2A04_F3604; //.data

//menu.c
extern MultiPlayer multiPlayer[4]; // Size: 0xA0 - Total Size: 0x280
extern MultiCharacter multiCharacter[4]; // Size: 0x76 - Total Size: 0x1D8
extern u8 multiCharacterType[4];
extern u8 multiGameLife[4];
extern u8 multiPlayerGame; // Bool? Like isMultiPlayerGame?
extern u8 numberOfPlayers;
extern u8 racingInGame; // Bool? Like isRacingInGame?
extern s16 D_800A3240_A3E40;

#endif
