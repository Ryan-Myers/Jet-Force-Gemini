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

#endif
