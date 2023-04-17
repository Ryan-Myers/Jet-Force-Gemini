#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"

//objects.c
extern s32 ObjListCount;
extern Object **ObjList; // Not sure about the number of elements
extern VertexPosition gEnvmapPos;
extern s32 *Ftables;
extern s32 *Findex;
extern s32 Fmax;

//boot.c

extern OSThread D_800F9B30_FA730; //bootOSThread
extern OSThread D_800F9D60_FA960; //mainOSThread
extern u64 D_800F7B20_F8720[1]; //bootThreadStack
extern u64 D_800F9B28_FA728[1]; //mainThreadStack
extern u64 D_800F7B28_F8728[1];

//diCpu.c
extern OSThread diCpuOSThread;
void diCpuThread(void *);
extern u64 diCpuThreadStack[1];
extern OSMesgQueue D_801031C0_103DC0;
extern void *D_801031D8_103DD8;
extern void *D_801031F8_103DF8;
extern OSMesgQueue D_80103218_103E18;
extern s32 D_800A6E88_A7A88;
extern s32 D_800A6E8C_A7A8C;
extern char D_800AF4EC_B00EC[26];// = "Fault in thread %d   (%s)";
extern char D_800AF508_B0108[9];// = "epc NULL";
extern char D_800AF650_B0250[16];// = "free ram 		%08x";

//prnBorders.c
extern Gfx D_800A3F90_A4B90[11][2]; //unknown number of graphics commands. This is a best guess.

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
extern s16 D_80104C82_105882; //Window->y2
extern s16 D_80104C84_105884; //Window->width
extern DialogueBoxBackground D_80104CA0_1058A0; //Window[1]
//Window noise?
extern s32 D_800A7868_A8468;
extern s32 D_800A786C_A846C;
extern s32 D_800A7870_A8470;
// Descending powers of 10
extern s32 D_800A7874_A8474[9];
// s32 D_800A7874_A8474[9] = {
//     1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
// };

extern Gfx D_800A77F0_A83F0[7];
extern u32 D_A7858; //Font asset location?
extern u32 *cache;
extern u8 *dxTable;
extern u8 squash;
extern DialogueTextElement String[]; //Sizeof(32)?
//dDialogueBoxBegin?
extern u8 D_800A78C4_A84C4[100];

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
extern s8 D_800FE26C_FEE6C;
extern OSSched sc;
extern u64 Time[1024];
extern void *securitybuffer;
extern u8 debugMenuEnable;
extern s32 mainGameMode;
extern u8 mainGameAutoSave;
extern s16 D_800A3A80_A4680; //mainGameWindowIsChanging
extern s16 D_800A3AB0_A46B0;
extern s32 D_800A3B70_A4770; //RamTest1Result
extern s32 D_800A3B74_A4774; //RamTest2Result
extern s32 D_800FE280_FEE80;
extern s32 load_save_flags;

//Window Size Vars
extern s16 mainGameWindowSizeX1;
extern s16 mainGameWindowSizeY1;
extern s16 mainGameWindowSizeX2;
extern s16 mainGameWindowSizeY2;

extern char D_800AD170_ADD70[]; // = "%5d  %5d  %5d";
extern char D_800AD180_ADD80[]; // = "FREE %d";
extern char D_800AD188_ADD88[]; // = "%d K";

typedef struct UnkD_800FE217_FEE17 {
    s8 ZBCheck;
    u8 pad1[0x7];
} UnkD_800FE217_FEE17;
extern UnkD_800FE217_FEE17 D_800FE217_FEE17[8];

//main.c?
extern u64 globalflags;

//diprint.c
typedef struct TexFontCoords {
    u8 u, v;
} TexFontCoords;
extern s32 D_80101F6C_102B6C; //gDebugFontTexture
extern TexFontCoords D_800A6D48_A7948[3][32]; //gDebugFontCoords
extern s32 D_80101F58_102B58; //D_80127CB8 in DKR
extern u16 D_80101F4C_102B4C; //D_80127CAC in DKR
extern u16 D_80101F4E_102B4E; //D_80127CAE in DKR
extern TextureHeader *D_80101F40_102B40; //gTexture0
extern TextureHeader *D_80101F44_102B44; //gTexture1
extern TextureHeader *D_80101F48_102B48; //gTexture2
extern s32 D_80101F5C_102B5C;
extern s32 D_80101F60_102B60;
extern s32 D_80101F64_102B64;
extern s32 D_80101F68_102B68;
extern u16 D_80101F70_102B70;
extern u16 D_80101F72_102B72;
extern char *D_800A6D44_A7944; //gDebugPrintBufferEnd
extern char D_80101640_102240[]; //gDebugPrintBufferStart
extern s32 D_800A6D40_A7940; // spacing codes
extern Gfx D_800A6E08_A7A08[]; //dDebugFontSettings
extern s32 D_80101F54_102B54; //D_80127CB4 in DKR
extern u16 D_80101F50_102B50; //D_80127CB0 in DKR
extern u16 D_80101F50_102B52; //D_80127CB2 in DKR

//sched.c
extern char D_800AD510_AE110[];// = "(Audio task)";
extern char D_800AD520_AE120[];// = "(Game task)";
extern char D_800AD52C_AE12C[];// = "(DI task)";
extern char D_800AD538_AE138[];// = "(DI benchmark test)";
extern char D_800AD550_AE150[];// = "(Clone task)";
extern char D_800AD560_AE160[];// = "(Refract task)";
extern char D_800AD570_AE170[];// = "(Blur task)";
extern char D_800AD580_AE180[];// = "(Unknown task type %d)";
extern char D_800AD598_AE198[];// = "RCP TASK INFO-------------"; //AND MUCH MORE STRING CONTENT
extern char D_800AD7D4_AE3D4[];// = "SP CRASHED, gfx=%x";
extern char D_800AD7E8_AE3E8[];// = "DP CRASHED, gfx=%x";
extern char D_800AD7FC_AE3FC[];// = "** GFX overflow **";
extern char D_800AD810_AE410[];// = "** MTX overflow **";
extern char D_800AD824_AE424[];// = "** VTX overflow **";
extern char D_800AD838_AE438[];// = "** POL overflow **";
extern char D_800AD84C_AE44C[];// = "Version %s";
extern char D_800AD7D0_AE3D0[];// = "7.2";
extern s32 D_800A4300_A4F00[];
extern s32 D_800A4308_A4F08[]; //gBootBlackoutMesg[] = { OSMESG_SWAP_BUFFER, MESG_SKIP_BUFFER_SWAP };
extern u8 D_800A3B28_A4728;
extern u8 D_800A3B2C_A472C;
extern u8 D_800A3B30_A4730;
extern u8 D_800A3B34_A4734;
extern s32 D_800A4310_A4F10; //gCurRSPTaskCounter
extern s32 D_800A4314_A4F14; //gCurRDPTaskCounter
extern s8 D_800A4318_A4F18; //gNextFrameCount
extern u64 D_800A4320_A4F20; //gRetraceCounter64
extern s32 D_800FF660_100260; //gCurRSPTaskIsSet
extern s32 D_800FF664_100264; //gCurRDPTaskIsSet
extern s32 currentScreen;
extern s32 otherScreen;
extern s32 otherZbuf;

//dicpu.c
//These two string are the file name and extensions 
//for what looks like debug data that gets written to a controller pak
extern char D_800AF400_B0000[];// = "CORE";
extern char D_800AF408_B0008[];// = "";
extern char D_800AF40C_B000C[];// = 
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
extern char D_800AF4AC_B00AC[];// = "Watchpoint exception at %x";

//lights.c
extern s32 D_800A1894_A2494;
extern unk800DC950 **D_800A1898_A2498;

//textures.c
extern u8 blinkMode;
extern u32 D_800A5830_A6430; //gTexColourTag = COLOUR_TAG_MAGENTA;
extern s32 D_800A5838_A6438; //D_80126378
extern u8 D_800A583C_A643C;
extern s32 D_800FF9F4_1005F4; //D_80126354 in DKR
extern s32 *D_800FF9C0_1005C0[2]; //D_80126320 in DKR
extern s32 D_800FFA10_100610;
extern s32 D_800FFA14_100614;
extern s32 D_800FFA18_100618;
extern s32 D_800FFA1C_10061C;
extern s32 D_800FFA20_100620;
extern s32 D_800FFA24_100624;
extern s32 D_800A584C_A644C; //A store of the texture currently loading?
extern s32 D_800A5844_A6444; //IA2ColOverride?
extern u8 D_800FFA28_100628;
extern u8 D_800FFA29_100629;
extern u8 D_800FFA2A_10062A;
extern u8 D_800FFA2B_10062B;
extern u8 D_800FFA2C_10062C;
extern u8 D_800FFA2D_10062D;
extern s32 D_800A5834_A6434; //textureFilter

//rcpFast3d.c
extern u8 sBackgroundPrimColourR;
extern u8 sBackgroundPrimColourG;
extern u8 sBackgroundPrimColourB;

//saves.c
extern OSPfs pfs[MAXCONTROLLERS];
extern OSMesgQueue *sControllerMesgQueue;
extern s32 D_800FEAC4_FF6C4; //UNUSED
extern u8 gN64FontCodes[];
extern s8 *sPackDirectory;
extern u8 sRumblePaksPresent;
extern u8 D_800A3EA8_A4AA8; //sControllerPakPresent?
extern OSMesgQueue flashEventQueue;
extern OSMesg flashEventBuf[1];
extern OSMesg cartEventBuf[1];
extern OSMesgQueue cartEventQueue;
extern OSIoMesg flashMesgReqBlock;
extern u8 D_800A3EC4_A4AC4;
extern u8 D_800A3ECC_A4ACC;

//Size: 0xA
typedef struct unkD_800FEC68_FF868 {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} unkD_800FEC68_FF868;
extern unkD_800FEC68_FF868 D_800FEC68_FF868;


typedef struct unkD_800A3EAC_A4AAC {
    s32 unk0;
    f32 unk4;
} unkD_800A3EAC_A4AAC;
extern unkD_800A3EAC_A4AAC D_800A3EAC_A4AAC[3];

//camera.c
extern ScreenViewport D_800A3728_A4328[4]; //gScreenViewports

//audiomgr.c
extern s32 nextFrameCount; // = 2;

#endif
