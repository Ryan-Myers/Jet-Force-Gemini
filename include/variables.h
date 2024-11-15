#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"


extern ALBankFile *sfxBankPtr;

//spranim.c
extern s32 disablejoy;
extern s32 D_800F7868;
extern s32 D_800F7874;
extern s32 D_800F7878;
extern s32 controlDkeys;
extern s32 controlKeys;
extern s32 controlXjoy;
extern s32 controlYjoy;

//objects.c
extern s8 D_800F3860;
extern s32 D_800F386C;
extern s32 D_800F3870;
extern s32 D_800F38AC;
extern s32 D_800F38B8;
extern s32 D_800F38BC;
extern s16 D_800F38C0;
extern s16 D_800F38C2;
extern s32 D_800F38C4;
extern s32 D_800F3908;
extern s32 D_800F3910;
extern s32 D_800F391C;
extern s32 D_800F3948;
extern s32 ObjListCount;
extern Object **ObjList; // Not sure about the number of elements
extern VertexPosition gEnvmapPos;
extern s32 *Ftables;
extern s32 *Findex;
extern s32 Fmax;
extern s8 D_800A1208;
extern f32 D_800F3A20;

//boot.c

extern OSThread D_800F9B30; //bootOSThread
extern OSThread D_800F9D60; //mainOSThread
extern u64 D_800F7B20[1]; //bootThreadStack
extern u64 D_800F9B28[1]; //mainThreadStack
extern u64 D_800F7B28[1];

//diCpu.c
extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801031C0;
extern void *D_801031D8;
extern void *D_801031F8;
extern OSMesgQueue D_80103218;
extern s32 D_800A6E88;
extern s32 D_800A6E8C;
extern char D_800AF4EC[26];// = "Fault in thread %d   (%s)";
extern char D_800AF508[9];// = "epc NULL";
extern char D_800AF650[16];// = "free ram 		%08x";
extern s32 D_800A6EDC;

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
extern s16 D_80104C82; //Window->y2
extern s16 D_80104C84; //Window->width
extern DialogueBoxBackground D_80104CA0; //Window[1]
//Window noise?
extern s32 D_800A7868;
extern s32 D_800A786C;
extern s32 D_800A7870;
// Descending powers of 10
extern s32 D_800A7874[9];
// s32 D_800A7874[9] = {
//     1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
// };

extern Gfx D_800A77F0[7];
extern u32 D_A7858; //Font asset location?
extern u32 *cache;
extern u8 *dxTable;
extern u8 squash;
extern DialogueTextElement String[]; //Sizeof(32)?
//dDialogueBoxBegin?
extern u8 D_800A78C4[100];

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
extern s8 D_800FE26C;
extern OSSched sc;
extern u64 Time[1024];
extern void *securitybuffer;
extern u8 debugMenuEnable;
extern s32 mainGameMode;
extern u8 mainGameAutoSave;
extern s16 D_800A3A80; //mainGameWindowIsChanging
extern s16 D_800A3AB0;
extern s32 D_800A3B70; //RamTest1Result
extern s32 D_800A3B74; //RamTest2Result
extern s32 D_800FE280;
extern s32 load_save_flags;

//Window Size Vars
extern s16 mainGameWindowSizeX1;
extern s16 mainGameWindowSizeY1;
extern s16 mainGameWindowSizeX2;
extern s16 mainGameWindowSizeY2;

extern char D_800AD170[]; // = "%5d  %5d  %5d";
extern char D_800AD180[]; // = "FREE %d";
extern char D_800AD188[]; // = "%d K";

typedef struct UnkD_800FE217 {
    s8 ZBCheck;
    u8 pad1[0x7];
} UnkD_800FE217;
extern UnkD_800FE217 D_800FE217[8];

//main.c?
extern u64 globalflags;

//diprint.c
typedef struct TexFontCoords {
    u8 u, v;
} TexFontCoords;
extern s32 D_80101F6C; //gDebugFontTexture
extern TexFontCoords D_800A6D48[3][32]; //gDebugFontCoords
extern s32 D_80101F58; //D_80127CB8 in DKR
extern u16 D_80101F4C; //D_80127CAC in DKR
extern u16 D_80101F4E; //D_80127CAE in DKR
extern TextureHeader *D_80101F40; //gTexture0
extern TextureHeader *D_80101F44; //gTexture1
extern TextureHeader *D_80101F48; //gTexture2
extern s32 D_80101F5C;
extern s32 D_80101F60;
extern s32 D_80101F64;
extern s32 D_80101F68;
extern u16 D_80101F70;
extern u16 D_80101F72;
extern char *D_800A6D44; //gDebugPrintBufferEnd
extern char D_80101640[]; //gDebugPrintBufferStart
extern s32 D_800A6D40; // spacing codes
extern Gfx D_800A6E08[]; //dDebugFontSettings
extern s32 D_80101F54; //D_80127CB4 in DKR
extern u16 D_80101F50; //D_80127CB0 in DKR
extern u16 D_80101F52; //D_80127CB2 in DKR

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
extern s32 D_800A4300[]; // = { OSMESG_SWAP_BUFFER, OSMESG_SWAP_BUFFER };
extern s32 D_800A4308[]; //gBootBlackoutMesg[] = { OSMESG_SWAP_BUFFER, MESG_SKIP_BUFFER_SWAP };
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
extern s32 currentScreen;
extern s32 *otherScreen;
extern s32 otherZbuf;

//dicpu.c
//These two string are the file name and extensions 
//for what looks like debug data that gets written to a controller pak
extern char D_800AF400[];// = "CORE";
extern char D_800AF408[];// = "";
extern char D_800AF40C[];// = 
// "%08x:%08x
// MEMORY REGION %d
// MOD OFFSET
// SIZE
// ADDRESS
// Texture %d
// Module %d   %d
// %08x %d
// Module %d at %08x
// %1x %d lines logged
// Page %d/%d"
extern char D_800AF4AC[];// = "Watchpoint exception at %x";

//lights.c
extern s32 D_800A1890;
extern s32 D_800A189C;
extern s32 D_800A1894;
extern unk800DC950 **D_800A1898;

//textures.c
extern u8 blinkMode;
extern u32 D_800A5830; //gTexColourTag = COLOUR_TAG_MAGENTA;
extern s32 D_800A5838; //D_80126378
extern u8 D_800A583C;
extern s32 D_800FF9F4; //D_80126354 in DKR
extern s32 *D_800FF9C0[2]; //D_80126320 in DKR
extern s32 D_800FFA10;
extern s32 D_800FFA14;
extern s32 D_800FFA18;
extern s32 D_800FFA1C;
extern s32 D_800FFA20;
extern s32 D_800FFA24;
extern s32 D_800A584C; //A store of the texture currently loading?
extern s32 D_800A5844; //IA2ColOverride?
extern u8 D_800FFA28;
extern u8 D_800FFA29;
extern u8 D_800FFA2A;
extern u8 D_800FFA2B;
extern u8 D_800FFA2C;
extern u8 D_800FFA2D;
extern s32 D_800A5834; //textureFilter

//rcpFast3d.c
extern u8 sBackgroundPrimColourR;
extern u8 sBackgroundPrimColourG;
extern u8 sBackgroundPrimColourB;

//saves.c
extern OSPfs pfs[MAXCONTROLLERS];
extern OSMesgQueue *sControllerMesgQueue;
extern s32 D_800FEAC4; //UNUSED
extern u8 gN64FontCodes[];
extern s8 *sPackDirectory;
extern u8 sRumblePaksPresent;
extern u8 D_800A3EA8; //sControllerPakPresent?
extern OSMesgQueue flashEventQueue;
extern OSMesg flashEventBuf[1];
extern OSMesg cartEventBuf[1];
extern OSMesgQueue cartEventQueue;
extern OSIoMesg flashMesgReqBlock;
extern u8 D_800A3EC4;
extern u8 D_800A3ECC;
extern f32 D_800AD4FC;
extern RumbleStruct D_800FEC68[];
extern f32 D_800AD504;
extern f32 D_800AD500;
extern RumbleStruct D_800FEC6A[];

//Size: 0xA
//extern unkD_800FEC68 *D_800FEC68;


typedef struct unkD_800A3EAC {
    s32 unk0;
    f32 unk4;
} unkD_800A3EAC;
extern unkD_800A3EAC D_800A3EAC[3];

//camera.c
extern ScreenViewport D_800A3728[4]; //gScreenViewports

//audiomgr.c
extern s32 nextFrameCount; // = 2;

//squads.c
extern s32 GlobalSizeOfRomdefList;
extern RomDefHeader *GlobalStartOfRomdefList;
extern s32 PlayersDisguise;

//gameVi.c
extern s8 D_800FF988; //videoMode

#endif
