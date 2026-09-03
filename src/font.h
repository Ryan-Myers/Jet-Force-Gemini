#ifndef _FONT_H_
#define _FONT_H_

#include "common.h"
#include "enums.h"

enum WindowFlags {
    DIALOGUE_BOX_UNUSED_01 = 0x0001,
    DIALOGUE_BOX_VERTS = 0x4000,
    WINDOW_CLOSED = 0x7FFF,
    WINDOW_OPEN = 0x8000,
    DIALOGUE_BOX_UNK_02 = 0xBFFF,
    DIALOGUE_BOX_UNUSED_02 = 0xFFFE
};

typedef enum TextFonts {
    FONT_COLOURFUL,
    FONT_SMALL,
    FONT_LARGE,
    FONT_UNK_FF = 255,
} TextFonts;

// typedef enum HorizontalAlignmentFlags {
//     HORZ_ALIGN_LEFT   = 0,
//     HORZ_ALIGN_RIGHT  = 1,
//     HORZ_ALIGN_CENTER = 4
// } HorizontalAlignmentFlags;

// typedef enum VerticalAlignmentFlags {
//     VERT_ALIGN_TOP    = 0,
//     VERT_ALIGN_BOTTOM = 2,
//     VERT_ALIGN_MIDDLE = 8
// } VerticalAlignmentFlags;

// typedef enum AlignmentFlags {
//     ALIGN_TOP_LEFT      = VERT_ALIGN_TOP    | HORZ_ALIGN_LEFT,
//     ALIGN_TOP_CENTER    = VERT_ALIGN_TOP    | HORZ_ALIGN_CENTER,
//     ALIGN_TOP_RIGHT     = VERT_ALIGN_TOP    | HORZ_ALIGN_RIGHT,
//     ALIGN_MIDDLE_LEFT   = VERT_ALIGN_MIDDLE | HORZ_ALIGN_LEFT,
//     ALIGN_MIDDLE_CENTER = VERT_ALIGN_MIDDLE | HORZ_ALIGN_CENTER,
//     ALIGN_MIDDLE_RIGHT  = VERT_ALIGN_MIDDLE | HORZ_ALIGN_RIGHT,
//     ALIGN_BOTTOM_LEFT   = VERT_ALIGN_BOTTOM | HORZ_ALIGN_LEFT,
//     ALIGN_BOTTOM_CENTER = VERT_ALIGN_BOTTOM | HORZ_ALIGN_CENTER,
//     ALIGN_BOTTOM_RIGHT  = VERT_ALIGN_BOTTOM | HORZ_ALIGN_RIGHT
// } AlignmentFlags;

typedef struct Asset46 {
    s32 unk0;
    s32 unk4;
    u8 unk8;
} Asset46;

extern char *combineBuffer;
extern char *printBuffer;
extern char *convertBuffer;
extern DialogueBoxBackground Window[];
extern u8 fontInUse;
extern u8 buttonMode;
extern s16 D_80103FB2_BE7F2;                   // Window->y2
extern s16 D_80103FB4_BE7F4;                   // Window->width
extern DialogueBoxBackground D_80103FD0_BE810; // Window[1]
// Window noise?
extern s32 D_800A6EA8_A7AA8;
extern s32 D_800A6EAC_A7AAC;
extern s32 D_800A6EB0_A7AB0;
// Descending powers of 10
extern s32 D_800A6EB4_A7AB4[9];
// s32 D_800A6EB4_A7AB4[9] = {
//     1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
// };

extern Gfx D_800A6E30_A7A30[7];
extern u32 D_A7858; // Font asset location?
extern u32 D_A7318; // Font asset location?
extern u32 *cache;
extern u8 *dxTable;
extern u8 squash;
extern DialogueTextElement String[]; // Sizeof(32)?
// dDialogueBoxBegin?
extern char D_800A6F04_A7B04[100];

/* Size: 10 bytes */
typedef struct FontCharDataAlt {
    u8 textureID; // Texture Pointer Index?
    u8 ulx;       // Upper Left Corner? Maybe only used when FontData->unk20 is 0 for some reason.
    u8 width;     // Font Char Width?
    u8 height;    // Font Char Height?
    u8 s;         // Upper left coordinate for the texture derived from X?
    u8 t;         // Upper left coordinate for the texture derived from Y?
    u8 lrx;       // Related to the lower right X Coordinate.
    u8 lry;       // Related to the lower right Y Coordinate.
    u8 pad[8];
} FontCharDataAlt;
extern FontCharDataAlt *Font;

#define WINDOW_COUNT 8
#define POS_CENTRED -0x8000
#define DIALOGUE_NUM_NULL 0xFF

void fontSetWindow0(s32 width, s32 height);
void fontSetButtonMode(s32 mode);
void fontSetWindowNoise(u8 arg0, u8 arg1, u8 arg2);
void fontUseFont(s32 font);
void fontColour(s32 red, s32 green, s32 blue, s32 alpha, s32 opacity);
void fontBackground(s32 red, s32 green, s32 blue, s32 alpha);
void fontPrintXY(Gfx **displayList, s32 xpos, s32 ypos, char *text, AlignmentFlags alignmentFlags);
void fontPrintWindowXY(Gfx **displayList, s32 windowId, s32 xpos, s32 ypos, char *text, AlignmentFlags alignmentFlags);
s32 fontStringWidth(char *text, s32 font, s32 convertString);
void fontWindowSize(s32 windowId, s32 x1, s32 y1, s32 x2, s32 y2);
void fontWindowUseFont(s32 windowId, s32 font);
void fontWindowColour(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha);
void fontWindowColour(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha);
void fontWindowFontColour(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity);
void fontWindowFontBackground(s32 windowId, s32 red, s32 green, s32 blue, s32 alpha);
void *fontWindowAddStringXY(s32 windowId, s32 posX, s32 posY, char *text, s32 number, s32 flags);
void fontWindowFlushStrings(s32 windowId);
void fontWindowEnable(s32 windowId);
void fontWindowDisable(s32 windowId);
void fontStringAddNumber(char **outString, s32 number);
void fontWindowsDraw(Gfx **dList, Mtx **mat, Vertex **verts);
void func_80070DE4_719E4(Gfx **dList, s32 ulx, s32 uly, s32 lrx, s32 lry);
void fontWindowDraw(Gfx **dList, Mtx **mat, Vertex **verts, s32 dialogueBoxID);
void func_8007128C_71E8C(char *input, char *output, s32 number);
void fontCreateDisplayList(Gfx *dList, Asset46 *asset, s32 width, s32 height);
void fontConvertString(char *inString, char *outString);
u8 *fontGetLine(s32 font, char *text, s32 arg2, u8 **outString);
u8 fontYSpacing(s32 font);

void func_8006FD98_70998(Gfx **dList, DialogueBoxBackground *box, char *text, AlignmentFlags alignmentFlags);
void *func_80071388_71F88(u8); // returns cacheline?

#endif
