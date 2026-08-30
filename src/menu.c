#include "menu.h"
#include "audio.h"
#include "common.h"
#include "font.h"
#include "gameVi.h"
#include "gsSnd.h"
#include "overlays/overlay12.h"
#include "overlays/overlay14.h"
#include "overlays/overlay31.h"
#include "overlays/overlay37.h"
#include "overlays/overlay40.h"
#include "overlays/overlay42.h"
#include "overlays/overlay57.h"
#include "overlays/overlay58.h"
#include "overlays/overlay61.h"
#include "overlays/overlay62.h"
#include "overlays/overlay63.h"
#include "overlays/overlay9.h"
#include "PR/n_libaudio.h"
#include "runLink.h"

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

extern FrontEndObject D_800A51DC_A5DDC[];
extern FrontEndObject currentobjects[];
extern s32 currentGameTime;
s8 mainGetPauseMode(void);
void frontInstruments(Gfx **dl);
void diRcpTrace(Gfx *gdl, char *file, s32 line);
extern const char D_800AD370_ADF70[];
extern const char D_800AD380_ADF80[];
extern void *frontpol;
void func_80059A98_5A698(Gfx **dl);
s32 frontInitMultiInstruments(void);
void frontRarepage(Gfx **dl);
void frontStartScreen(Gfx **dl);
void frontOptionsPage(Gfx **dl);
void frontMap(Gfx **dl);
void frontCharSelect(Gfx **dl);
void frontMultiSelect(Gfx **dl);
void frontMultiModeSelect(Gfx **dl);
void frontMultiStats(Gfx **dl);
void frontCredits(Gfx **dl);
void frontKeyboard(Gfx **dl);
void frontMenuFrameDraw(void);
void frontMenuFrameTick(Gfx **dl);
void frontInitMenuFrame(void);
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
void frontInitInstruments(void);
void sprintInitInstruments(void);
void sparkUpdate(void);
extern Gfx *frontgfx;
extern Mtx *frontmtx;
extern Vtx *frontvtx;
Object **objGetPlayerlist(s32 *count);
void frontDeathMatchScores(s32 count, Object **players, Gfx **dl);
void duckshootDrawTargets(Gfx **gfx, Mtx **mtx, Vtx **vtx, s32 players);
void sprintDrawInstruments(Gfx **gfx, Mtx **mtx, Vtx **vtx, s32 players);
void frontSingleInstruments(Object *player, Gfx **dl);
extern u8 D_800A5194_A5D94;
void setLanguage(s32 language);
void fxScreenEffect(s32 arg0, s32 *screen, s32 width, s32 height, s32 x1, s32 y1, s32 x2, s32 y2, s32 arg8);
void fxQueueScreenEffect(s32 *screen, s32 width, s32 height, s32 x1, s32 y1, s32 x2, s32 y2, s32 arg7);
void frontPlayerScreenLimits(s32 player, s32 *x1, s32 *y1, s32 *x2, s32 *y2);
extern s16 D_800A508C_A5C8C[];
s32 frontGet2PlayerSplit(void);

typedef struct FrontRect {
    /* 0x0 */ s16 x1;
    /* 0x2 */ s16 y1;
    /* 0x4 */ s16 x2;
    /* 0x6 */ s16 y2;
    /* 0x8 */ u32 colour;
} FrontRect;

void frontDrawRectangles(s32 arg0, s32 count, FrontRect *rects, s32 arg3);
extern s32 D_800FF3E8_B1DE8[4];
extern u8 frontJoyDxRepeat[4];
extern u8 D_800FF3E4_B1DE4[4];
extern u8 okayed;
extern u8 disable;
extern u8 numberOfCameras;
s32 levelGetScreenMode(void);
void frontInitMode(void);
void func_80059A04_5A604(void);

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setLanguage.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/initFront.s")

void frontFreeMode(void) {
    if (runlinkIsModuleLoaded(12) != 0) {
        frontFreeMenuFrame();
    }
    if (D_800A51A0_A5DA0 != 0) {
        switch (frontEndMode) {
            case 0:
                fontWindowFlushStrings(1);
                break;
            case 2:
                frontCleanupRarepage();
                break;
            case 3:
                frontCleanupStartScreen();
                break;
            case 4:
                frontCleanupOptionsPage();
                break;
            case 5:
                frontCleanupCharSelect();
                break;
            case 6:
                frontCleanupMultiSelect();
                break;
            case 8:
                frontCleanupMultiModeSelect();
                break;
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                frontCleanupMultiStats();
                break;
            case 24:
                frontKeyboardCleanup();
                break;
            case 16:
                if (
#ifdef VERSION_kiosk
                    multiGameType == 4 || racingInGame
#else
                    (multiPlayerGame && multiGameType == 4) || racingInGame
#endif
                ) {
                    sprintFreeInstruments();
                } else if (numberOfPlayers == 1) {
                    frontCleanupInstruments();
                } else {
                    frontCleanupMultiInstruments();
                }
                break;
            case 17:
                frontCleanupMap();
                runlinkFreeCode(9);
                break;
        }
        D_800A51A0_A5DA0 = 0;
    }
}

void frontInitMode(void) {
    frontInitMenuFrame();
    if (D_800A51A0_A5DA0 == 0) {
        switch (frontEndMode) {
            case 0:
                break;
            case 2:
                frontInitRarepage();
                break;
            case 3:
                frontInitStartScreen();
                break;
            case 4:
                frontInitOptionsPage(0);
                break;
            case 17:
                frontInitMap(0);
                break;
            case 5:
                frontInitCharSelect();
                break;
            case 6:
                frontInitMultiSelect();
                break;
            case 8:
                frontInitMultiModeSelect();
                break;
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                frontInitMultiStats();
                break;
            case 23:
                frontCreditsInit();
                break;
            case 24:
                frontKeyboardInit();
                break;
            case 16:
                if ((multiPlayerGame && multiGameType == 4) || racingInGame) {
                    sprintInitInstruments();
                } else {
                    runlinkDownloadCode(6);
                    if (numberOfPlayers == 1) {
                        frontInitInstruments();
                    } else {
                        sparkUpdate();
                    }
                }
                break;
        }
        D_800A51A0_A5DA0 = 1;
    }
}

void frontSetMode(s32 mode) {
    frontFreeMode();
    frontEndMode = mode;
    frontInitMode();
    func_80059A04_5A604();
    okayed = 0;
    disable = 1;
    if (mode == 0) {
        multiPlayerGame = 0;
        numberOfPlayers = 1;
        numberOfCameras = 1;
    }
}

u8 frontGetMode(void) {
    return frontEndMode;
}

s32 frontUpdate(Gfx **gfx, Mtx **mtx, Vtx **vtx, void **pol, Gfx **dl) {
    func_80059A98_5A698(dl);
    frontgfx = *gfx;
    frontmtx = *mtx;
    frontvtx = *vtx;
    frontpol = *pol;
    diRcpTrace(frontgfx, (char *) D_800AD370_ADF70, 0x256);
    if (frontInitMultiInstruments()) {
        switch (frontEndMode) {
            case 0:
                break;
            case 2:
                frontRarepage(dl);
                break;
            case 3:
                frontStartScreen(dl);
                break;
            case 4:
                frontOptionsPage(dl);
                break;
            case 17:
                frontMap(dl);
                break;
            case 5:
                frontCharSelect(dl);
                break;
            case 6:
                frontMultiSelect(dl);
                break;
            case 8:
                frontMultiModeSelect(dl);
                break;
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                frontMultiStats(dl);
                break;
            case 23:
                frontCredits(dl);
                break;
            case 24:
                frontKeyboard(dl);
                break;
            case 16:
                frontInstruments(dl);
                break;
        }
    }
    frontMenuFrameDraw();
    frontMenuFrameTick(dl);
    diRcpTrace(frontgfx, (char *) D_800AD380_ADF80, 0x27A);
    *gfx = frontgfx;
    *mtx = frontmtx;
    *vtx = frontvtx;
    *pol = frontpol;
    disable = 0;
    return 0;
}


void frontDemoMessage(Gfx **dl, s32 arg1) {
    s32 x;
    s32 y;

    D_800A5194_A5D94 += arg1;
    if (D_800A5194_A5D94 & 0x10) {
        x = 0xA0;
        y = 0xD0;
        viConvertXY(&x, &y);
        setLanguage(frontGetLanguage());
        fontUseFont(2);
        fontBackground(0, 0, 0, 0);
        fontColour(0, 0, 0, 255, 255);
        fontPrintXY(dl, x + 1, y + 1, front_text[49], 12);
        fontColour(255, 255, 255, 0, 255);
        fontPrintXY(dl, x, y, front_text[49], 12);
    }
}

void frontInstruments(Gfx **dl) {
    s32 count;
    Object **players;

    players = objGetPlayerlist(&count);
    if (multiPlayerGame) {
        switch (multiGameType & 0xF) {
            case 0:
                frontDeathMatchScores(count, players, dl);
                break;
            case 6:
                duckshootDrawTargets(&frontgfx, &frontmtx, &frontvtx, numberOfPlayers);
                break;
            case 4:
                duckshootDrawTargets(&frontgfx, &frontmtx, &frontvtx, numberOfPlayers);
                break;
        }
    } else if (racingInGame) {
        sprintDrawInstruments(&frontgfx, &frontmtx, &frontvtx, numberOfPlayers);
    } else if (count > 0) {
        frontSingleInstruments(players[0], dl);
    }
}

s32 frontUpdateTimer(s32 arg0, s32 limit, s32 delta) {
    s32 ret = 0;

    if (mainGetPauseMode() == 0) {
        currentGameTime += delta;
    }
    if (currentGameTime >= limit) {
        currentGameTime = limit;
        ret = 1;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontPrintNum.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDrawRectangles.s")

void frontDrawRectangle(s32 arg0, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour) {
    FrontRect rect;

    rect.x1 = x1;
    rect.y1 = y1;
    rect.x2 = x2;
    rect.y2 = y2;
    rect.colour = colour;
    frontDrawRectangles(arg0, 1, &rect, 1);
}

void frontFlashScreen(s32 arg0, s32 player, s32 arg2) {
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 width;
    s32 height;

    frontPlayerScreenLimits(player, &x1, &y1, &x2, &y2);
    viGetCurrentSize(&width, &height);
    if (arg0) {
        fxScreenEffect(arg0, otherScreen, width, height, x1, y1, x2, y2, arg2);
    } else {
        fxQueueScreenEffect(otherScreen, width, height, x1, y1, x2, y2, arg2);
    }
}

void frontPlayerScreenLimits(s32 player, s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    s32 mode;
    s32 idx;
    s32 unused;

    mode = viGetVideoMode();
    idx = (((numberOfCameras - 1) << 2) + player) << 2;
    if ((mode & 1) || (numberOfCameras == 2 && frontGet2PlayerSplit())) {
        idx += 0x40;
    }
    *x1 = D_800A508C_A5C8C[idx];
    *y1 = D_800A508C_A5C8C[idx + 1];
    viConvertXY(x1, y1);
    *x2 = D_800A508C_A5C8C[idx + 2];
    *y2 = D_800A508C_A5C8C[idx + 3];
    viConvertXY(x2, y2);
}

void func_80059A04_5A604(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800FF3E8_B1DE8[i] = -1;
        frontJoyDxRepeat[i] = 20;
        D_800FF3E4_B1DE4[i] = 15;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/func_80059A98_5A698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/freeFrontEndList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/freeFrontEndItem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/loadFrontEndList.s")

const char D_800AD370_ADF70[] = "front/front.c";
const char D_800AD380_ADF80[] = "front/front.c";
const char D_800AD390_ADF90[] = "loadFrontEndItem() - Item no %d out of range 0-%d\n";

#pragma GLOBAL_ASM("asm/nonmatchings/menu/loadFrontEndItem.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/setupFrontEndList.s")

void setupFrontEndObject(s32 index) {
    currentobjects[index].unk0 = D_800A51DC_A5DDC[index].unk0;
    currentobjects[index].unk2 = D_800A51DC_A5DDC[index].unk2;
    currentobjects[index].unk4 = D_800A51DC_A5DDC[index].unk4;
    currentobjects[index].unk6 = D_800A51DC_A5DDC[index].unk6;
    currentobjects[index].unkC = D_800A51DC_A5DDC[index].unkC;
    currentobjects[index].unk10 = D_800A51DC_A5DDC[index].unk10;
    currentobjects[index].unk14 = D_800A51DC_A5DDC[index].unk14;
    currentobjects[index].unk8 = D_800A51DC_A5DDC[index].unk8;
    currentobjects[index].unk18 = D_800A51DC_A5DDC[index].unk18;
    currentobjects[index].unk1C = D_800A51DC_A5DDC[index].unk1C;
    currentobjects[index].unk1D = D_800A51DC_A5DDC[index].unk1D;
    currentobjects[index].unk1E = D_800A51DC_A5DDC[index].unk1E;
    currentobjects[index].unk1F = D_800A51DC_A5DDC[index].unk1F;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontDrawObj.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu/frontSetupMultiPickup.s")

s32 frontGameSelected(void) {
    return 0;
}

s32 frontGetWorldLevel(void) {
    return 0;
}

char *frontGetWorldName(s32 world) {
    return front_text[D_800A5918_A6518[world]];
}

s32 frontGetLanguage(void) {
    return D_800FF386_B1D86;
}

void frontSetLanguage(s32 language) {
    D_800FF386_B1D86 = language;
    setLanguage(language);
}

s32 frontGetScreenMode(void) {
    s32 mode = 0;

    if (someResVar.bit30) {
        mode |= 1;
    }
    if (someResVar.bit29) {
        mode |= 2;
    }
    return mode;
}

void frontSetScreenMode(s32 mode) {
    if ((mode & 3) != D_800A51A8_A5DA8) {
        D_800A51A8_A5DA8 = mode & 3;
        if (D_800A51A8_A5DA8 & 1) {
            globalflags.res.bit30 = 1;
        } else {
            globalflags.res.bit30 = 0;
        }
        if (D_800A51A8_A5DA8 & 2) {
            globalflags.res.bit29 = 1;
        } else {
            globalflags.res.bit29 = 0;
        }
    }
}

void frontStoreScreenMode(void) {
    D_800A51A4_A5DA4 = D_800A51A8_A5DA8;
}

u8 frontRecallScreenMode(void) {
    return D_800A51A4_A5DA4;
}

s32 frontGetLevelScreenMode(void) {
    switch (D_800A51A8_A5DA8) {
        case 3:
            return 3;
        case 2:
            return levelGetScreenMode() | 2;
        case 1:
            return 1;
    }
    return levelGetScreenMode();
}

s8 frontGetWideAdjust(void) {
    return widescreenVOffset;
}

void frontSetWideAdjust(s32 offset) {
    viSetWideAdjust(offset);
    widescreenVOffset = viGetWideAdjust();
}

u8 frontGetStereoMode(void) {
    return speakerSetting;
}

void frontSetStereoMode(s32 mode) {
    if (mode < 0) {
        mode = 0;
    }
    if (mode >= 4) {
        mode = 3;
    }
    speakerSetting = mode;
    alSurround_OutputType(D_800A5938_A6538[mode]);
}

u16 frontGetSfxVolume(void) {
    return SFXVolume;
}

void frontSetSfxVolume(s32 volume) {
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 0x100) {
        volume = 0x100;
    }
    SFXVolume = volume;
    gsSndpSetGlobalVolume(volume);
}

u16 frontGetBgmVolume(void) {
    return musicVolume;
}

void frontSetBgmVolume(s32 volume) {
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 0x100) {
        volume = 0x100;
    }
    musicVolume = volume;
    amTuneSetGlobalVolume(volume);
}

s32 frontGet2PlayerSplit(void) {
    s32 split;

    split = someResVar.bit28;
    return split;
}

void frontSet2PlayerSplit(s32 split) {
    globalflags.res.bit28 = split & 1;
}

u8 frontGetTargetControl(s32 mode) {
    return selectedControlModes[mode & 3];
}

void frontSetTargetControl(s32 mode, s32 value) {
    selectedControlModes[mode & 3] = value & 1;
}

#ifdef VERSION_us
void frontCharSelectSetQuitMode(s32 mode) {
    charselquitmode = mode;
}
#endif
