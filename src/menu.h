#ifndef _MENU_H_
#define _MENU_H_

#include <PR/ultratypes.h>
#include "structs.h"

// These vars might need to be declared in a global file just for global vars.
// They're used in multiple files, there might be a globals.c file.
typedef struct Resbitfield {
    u32 bi31 : 1;
    u32 bit30 : 1; /* screen mode bit 0 */
    u32 bit29 : 1; /* screen mode bit 1 */
    u32 bit28 : 1; /* 2-player split orientation */
    u32 rest : 28;
} Resbitfield;
extern Resbitfield someResVar;
typedef struct GlobalFlags {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ Resbitfield res;
} GlobalFlags;
extern GlobalFlags globalflags;
extern MultiPlayer multiPlayer[4]; // Size: 0xA0 - Total Size: 0x280
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
extern u8 D_800A51A8_A5DA8; /* current screen mode, 0-3; bit0/bit1 mirror res.bit30/bit29 */
extern u8 D_800FF386_B1916;
extern u8 frontEndMode;
extern u8 multiGameType;
extern u8 multiObjectList;
extern u8 selectedControlModes[];
extern char **front_text;    /* language string table; entries are ROM-relative until
                                fixed up to pointers by setLanguage() */
extern u16 D_800A5918_A6518[];

/* ---- front end (src/menu.c) ------------------------------------------- */

/* One entry of the front end's object table.  currentobjects[] is the live
   copy that frontDrawObj renders; D_800A51DC_A5DDC[] is the template it is
   restored from. */
typedef struct FrontEndObject {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ s8 unk1F;
} FrontEndObject;

/* The front end builds a SHORTER spawn record than CreateStaticInstance's
   StaticInstanceSpawn: only fields through 0x0C are written and the local is
   0x10 bytes.  Keep the size -- it is what places the local at sp+0x28 in
   loadFrontEndItem's 0x38 frame. */
typedef struct FrontEndSpawn {
    /* 0x0 */ s16 objectId;
    /* 0x2 */ s8 unk2;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s8 unkA;
    /* 0xB */ s8 unkB;
    /* 0xC */ s8 unkC;
    /* 0xD */ u8 padD[0x3];
} FrontEndSpawn;

/* Rectangle list consumed by frontDrawRectangles. */
typedef struct FrontRect {
    /* 0x0 */ s16 x1;
    /* 0x2 */ s16 y1;
    /* 0x4 */ s16 x2;
    /* 0x6 */ s16 y2;
    /* 0x8 */ u32 colour;
} FrontRect;

/* Front-end object table and the loaded-asset bookkeeping that goes with it.
   frontendptrs[] holds one loaded asset per entry (a TextureHeader, a
   TextureSprite, an Object or a ModelInstance_JFG depending on the type bits
   in D_800A51D0_A5DD0[]); D_800FF6C8_B1C58[] is the matching loaded flag and
   D_800A51D8_A5DD8 the count of live entries. */
extern FrontEndObject D_800A51DC_A5DDC[];
extern FrontEndObject currentobjects[];
extern void *frontendptrs[];
extern u8 D_800FF6C8_B1C58[];  /* per-item loaded flag */
extern s16 *D_800A51D0_A5DD0; /* per-item type word: 0xC000 texture, 0x8000 sprite,
                                 0x4000 object, else model; low 14 bits = asset id */
extern s16 D_800A51D8_A5DD8;   /* number of loaded items */
extern s16 D_800A51D4_A5DD4;
extern u8 *D_800FF3A8_B1938[];
extern u32 *D_800A51AC_A5DAC;  /* language offset table (ROM asset 7); entry i is the
                                  byte offset of language i's string block */
extern s16 D_800A508C_A5C8C[];
extern u8 D_800A5194_A5D94;

/* Display-list cursors the front end appends to. */
extern Gfx *frontgfx;
extern Mtx *frontmtx;
extern Vtx *frontvtx;
extern void *frontpol;

/* Per-player input state, latched once a frame by func_80059A98_5A698. */
extern s32 frontJoyHeld[4];
extern s32 frontJoyPressed[4];
extern s8 frontJoyDx[4];
extern s8 frontJoyDy[4];
extern s8 frontJoyDxRepeat[4]; /* auto-repeat countdown, reset to 20 then 15 */
extern s32 D_800FF3E8_B1978[4];
extern s8 D_800FF3E4_B1974[4];
extern u8 okayed;
extern u8 disable;

/* Front-end draw colour, applied as the RDP prim colour by frontDrawObj.
   frR/D_800A51B8_A5DB8/frB are the R/G/B bytes; objtrans is the alpha, and is
   also tested against 0xFF to decide whether the translucent path is used. */
extern u8 frR;
extern u8 D_800A51B8_A5DB8;
extern u8 frB;
extern s32 objtrans;
extern s32 D_800A51C4_A5DC4;

/* Static display lists the front end prepends before its own commands. */
extern Gfx D_800A58A0_A64A0[];
extern Gfx D_800A58F8_A64F8[];
extern Gfx D_A58E8[];

/* Mode dispatch: frontUpdate/frontInitMode switch on frontEndMode. */
void frontInitMode(void);
void frontRarepage(s32 updateRate);
void frontStartScreen(s32 updateRate);
void frontOptionsPage(s32 updateRate);
void frontMap(s32 updateRate);
void frontCharSelect(s32 updateRate);
void frontMultiSelect(s32 updateRate);
void frontMultiModeSelect(s32 updateRate);
void frontMultiStats(s32 updateRate);
void frontCredits(s32 updateRate);
void frontKeyboard(s32 updateRate);
void frontInitRarepage(void);
void frontInitStartScreen(void);
void frontInitOptionsPage(s32 arg0);
void frontInitMap(s32 arg0);
void frontInitCharSelect(void);
void frontInitMultiSelect(void);
void frontInitMultiModeSelect(void);
void frontInitMultiStats(void);
void frontCreditsInit(void);
void frontKeyboardInit(void);
void frontInitMenuFrame(void);
void frontMenuFrameDraw(void);
void frontMenuFrameTick(s32 updateRate);
void func_80059A04_5A604(void);
void func_80059A98_5A698(s32 updateRate);

/* Asset list load/free/setup.  Each list is a -1 terminated array of indices
   into frontendptrs[]. */
void freeFrontEndItem(s32 item);
void loadFrontEndItem(s32 item);
void setupFrontEndObject(s32 index);
void freeFrontEndList(s16 *list);
void loadFrontEndList(s16 *list);
void setupFrontEndList(s16 *list);

/* Drawing helpers. */
void frontDrawRectangles(Gfx **dList, s32 count, FrontRect *rects, s32 arg3);
void frontPlayerScreenLimits(s32 player, s32 *x1, s32 *y1, s32 *x2, s32 *y2);
s32 frontGet2PlayerSplit(void);
void frontInstruments(s32 updateRate);
s32 frontInitMultiInstruments(void);
void frontInitInstruments(void);
void frontSingleInstruments(Object *player, s32 updateRate);
void frontDeathMatchScores(s32 count, Object **players, s32 updateRate);
void setLanguage(s32 language);

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
