#include "menu.h"
#include "audio.h"
#include "common.h"
#include "font.h"
#include "gameVi.h"
#include "gsSnd.h"
#include "models.h"
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
#include "textures.h"

extern s32 currentGameTime;
s8 mainGetPauseMode(void);
/* menu.c's own .bss: exactly the 16 bytes at 0x800FF1B0..BF.  playChoice is
   per-player and must be DEFINED here rather than extern -- a defined object is
   addressed section-relative, so IDO shares one `lui $at` across all four
   stores in initFront(), which is what the ROM does.  playStatus and
   gameoverFade come along because IDO gives .bss 16-byte alignment AND rounds
   its size to 16, so the block has to start and end on a 16-byte boundary. */
s32 playStatus;
u8 playChoice[4];
u8 gameoverFade[8];

void *mmAlloc(s32 size, u32 colourTag);
u32 *piRomLoad(u32 assetIndex);
/* NOTE: pi.h declares this second parameter as u32.  It is void * here on
   purpose: with a u32 parameter the (u32) cast on front_text becomes a separate
   value and IDO emits `move a1,v1` instead of loading straight into a1. */
s32 piRomLoadSection(u32 assetIndex, void *address, s32 assetOffset, s32 size);

Object *objSetupObject(StaticInstanceSpawn *spawn, s32 arg1);

void diRcpTrace(Gfx *gdl, char *file, s32 line);
extern const char D_800AD370_ADF70[];
extern const char D_800AD380_ADF80[];
void sprintInitInstruments(void);
void sparkUpdate(void);
Object **objGetPlayerlist(s32 *count);
void duckshootDrawTargets(Gfx **gfx, Mtx **mtx, Vtx **vtx, s32 players);
void sprintDrawInstruments(Gfx **gfx, Mtx **mtx, Vtx **vtx, s32 players);
void fxScreenEffect(s32 arg0, s32 *screen, s32 width, s32 height, s32 x1, s32 y1, s32 x2, s32 y2, s32 arg8);
void fxQueueScreenEffect(s32 *screen, s32 width, s32 height, s32 x1, s32 y1, s32 x2, s32 y2, s32 arg7);

TextureHeader *texFrame(TextureHeader *texHead, s32 offset);
ModelInstance_JFG *modLoadModel(s32 id, s32 arg1);
void objPrintObject(Gfx **gfx, Mtx **mtx, Vtx **vtx, Object *obj);
void camDo2DSprite(Gfx **gfx, Mtx **mtx, Vtx **vtx, ObjectSegment1 *seg, void *tex, s32 a5, s32 a6);
void camPushModelMtx(Gfx **dList, Mtx **mtx, ObjectTransform *trans, f32 scale, f32 scaleY);
void camPopModelMtx(Gfx **dlist);
void objFreeObject(Object *obj);
void objDoFrees(void);
/* joy.h is not included here; this TU sees the joystick getters as returning
   int, which is what the ROM's code assumes (no narrowing after the calls). */
s32 joyGetButtons(s32 player);
s32 joyGetStickX(s32 player);
s32 joyGetStickY(s32 player);
extern u8 numberOfCameras;
s32 levelGetScreenMode(void);

void setLanguage(s32 language) {
    s32 idx;
    s32 i;
    s32 size;

    if (D_800A51AC_A5DAC == NULL) {
        D_800A51AC_A5DAC = piRomLoad(7);
    }
    switch (language) {
        default:
            idx = 1;
            break;
        case 4:
            idx = 5;
            break;
        case 3:
            idx = 4;
            break;
        case 2:
            idx = 3;
            break;
        case 1:
            idx = 2;
            break;
    }
    size = D_800A51AC_A5DAC[idx + 1];
    idx = D_800A51AC_A5DAC[idx];
    size -= idx;
    if (front_text != NULL) {
        piRomLoadSection(6, front_text, idx, size);
        for (i = 0; i < (s32) D_800A51AC_A5DAC[0]; i++) {
            if ((s32) front_text[i] == -1) {
                front_text[i] = NULL;
            } else {
                front_text[i] = (char *) ((s32) front_text[i] + (s32) front_text);
            }
        }
    }
}

void initFront(void) {
    s32 i;

    D_800FF3A8_B1938[0] = mmAlloc(0xFA8, -1);
    for (i = 0; i < 3; i++) {
        D_800FF3A8_B1938[i] = D_800FF3A8_B1938[i - 1] + 0x538;
    }
    front_text = mmAlloc(0x3000, -1);
    setLanguage(0);
    i = 0;
    while (i < 180) {
        frontendptrs[i] = NULL;
        i++;
    }
    D_800A51A0_A5DA0 = 0;
    D_800A51D0_A5DD0 = piRomLoad(0x1A);
    D_800A51D4_A5DD4 = 0;
    while (D_800A51D0_A5DD0[D_800A51D4_A5DD4] != -1) {
        D_800A51D4_A5DD4++;
    }
    D_800A51D8_A5DD8 = 0;
    for (i = 0; i < D_800A51D4_A5DD4; i++) {
        D_800FF6C8_B1C58[i] = 0;
    }
    i = 0;
    while (i < 4) {
        playChoice[i++] = 2;
    }
}

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

s32 frontUpdate(Gfx **gfx, Mtx **mtx, Vtx **vtx, void **pol, s32 updateRate) {
    func_80059A98_5A698(updateRate);
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
                frontRarepage(updateRate);
                break;
            case 3:
                frontStartScreen(updateRate);
                break;
            case 4:
                frontOptionsPage(updateRate);
                break;
            case 17:
                frontMap(updateRate);
                break;
            case 5:
                frontCharSelect(updateRate);
                break;
            case 6:
                frontMultiSelect(updateRate);
                break;
            case 8:
                frontMultiModeSelect(updateRate);
                break;
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                frontMultiStats(updateRate);
                break;
            case 23:
                frontCredits(updateRate);
                break;
            case 24:
                frontKeyboard(updateRate);
                break;
            case 16:
                frontInstruments(updateRate);
                break;
        }
    }
    frontMenuFrameDraw();
    frontMenuFrameTick(updateRate);
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

void frontInstruments(s32 updateRate) {
    s32 count;
    Object **players;

    players = objGetPlayerlist(&count);
    if (multiPlayerGame) {
        switch (multiGameType & 0xF) {
            case 0:
                frontDeathMatchScores(count, players, updateRate);
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
        frontSingleInstruments(players[0], updateRate);
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

void frontPrintNum(s32 number, s32 x, s32 y, s32 fontId, s32 minDigits, u32 colour1, u32 colour2) {
    TextureHeader *tex;
    s32 xl;
    Gfx *cmd;
    s32 xh;
    s32 yh;
    s32 nCmds;
    s32 sBase;
    s32 advance;
    s32 charW;
    s32 digits;
    s32 pad;
    s32 width;
    s32 height;
    s32 sVal;

    viGetCurrentSize(&width, &height);
    x += (u32) width >> 1;
    y = ((u32) height >> 1) - y;
    tex = frontendptrs[fontId];
    pad = fontId;
    if (tex == NULL) {
        return;
    }
    if (pad != 8) {
        if (pad == 9) {
            charW = 8;
            advance = 8;
        }
    } else {
        charW = 11;
        advance = 12;
    }
    gSPDisplayList(frontgfx++, D_800A58A0_A64A0);
    gDkrDmaDisplayList(frontgfx++, D_A58E8, 2);
    xl = x << 2;
    sBase = (tex->height - 1) << 5;
    xh = (charW << 2) + xl;
    yh = (tex->height << 2) + (y << 2);
    cmd = tex->cmd;
    frontgfx->words.w0 = cmd->words.w0;
    frontgfx->words.w1 = (s32) texFrame(tex, 0) + 0x80000000;
    frontgfx++;
    cmd++;
    {
        Gfx *_g = frontgfx++;
        nCmds = tex->numberOfCommands - 1;
        _g->words.w0 = (_SHIFTL(G_DMADL, 24, 8) | _SHIFTL(nCmds, 16, 8) | _SHIFTL((nCmds * 8), 0, 16));
        _g->words.w1 = (unsigned int) ((s32) cmd + 0x80000000);
    }
    gDPSetPrimColorRGBA(frontgfx++, colour1);
    digits = 0;
    while (number != 0) {
        sVal = ((number - 1) % 10) * (charW << 5);
        xh -= advance << 2;
        xl -= advance << 2;
        gSPTextureRectangle(frontgfx++, xl, y << 2, xh, yh, 0, sVal, sBase, 0x400, -0x400);
        digits++;
        number /= 10;
    }
    gDPSetPrimColorRGBA(frontgfx++, colour2);
    digits = minDigits - digits;
    while (digits != 0) {
        xh -= advance << 2;
        xl -= advance << 2;
        gSPTextureRectangle(frontgfx++, xl, y << 2, xh, yh, 0, 9 * (charW << 5), sBase, 0x400, -0x400);
        number /= 10;
        digits--;
    }
}

/* IDO will copy-propagate the comparison's register into the assignment if the
   two live in one `if` statement, emitting `move` where the ROM re-reads the
   bound from the stack.  Splitting them with the usual do/while(0) + break
   keeps them separate statements and restores the `lw`. */
#define FRONT_CLAMP_MAX(v, m) \
    do {                      \
        if ((v) <= (m)) {     \
            break;            \
        }                     \
        (v) = (m);            \
    } while (0)

void frontDrawRectangles(Gfx **dList, s32 count, FrontRect *rects, s32 arg3) {
    /* These five must be declared ahead of width/height: they supply the 20
       bytes that put width at sp+0x58 and height at sp+0x54. */
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    u32 colour;
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);
    gSPDisplayList((*dList)++, D_800A58F8_A64F8);
    if (arg3) {
        gDPSetOtherMode((*dList)++, 0x082C0F, 0x00504340);
    } else {
        gDPSetOtherMode((*dList)++, 0x082C0F, 0x00504240);
    }
    colour = rects->colour;
    gDPSetPrimColorRGBA((*dList)++, colour);
    while (count--) {
        x2 = rects->x2;
        y2 = rects->y2;
        if (x2 >= 0 && y2 >= 0) {
            x1 = rects->x1;
            y1 = rects->y1;
            if (x1 < width && y1 < height) {
                if (x1 < 0) {
                    x1 = 0;
                }
                if (y1 < 0) {
                    y1 = 0;
                }
                FRONT_CLAMP_MAX(x2, width);
                FRONT_CLAMP_MAX(y2, height);
                if (colour != rects->colour) {
                    colour = rects->colour;
                    gDPPipeSync((*dList)++);
                    gDPSetPrimColorRGBA((*dList)++, colour);
                }
                gDPFillRectangle((*dList)++, x1, y1, x2, y2);
            }
        }
        rects++;
    }
    texDPInit(dList);
    gDPSetPrimColorRGBA((*dList)++, -1);
}

void frontDrawRectangle(s32 arg0, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour) {
    FrontRect rect;

    rect.x1 = x1;
    rect.y1 = y1;
    rect.x2 = x2;
    rect.y2 = y2;
    rect.colour = colour;
    frontDrawRectangles((Gfx **) arg0, 1, &rect, 1);
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
        D_800FF3E8_B1978[i] = -1;
        frontJoyDxRepeat[i] = 20;
        D_800FF3E4_B1974[i] = 15;
    }
}

void func_80059A98_5A698(s32 updateRate) {
    s32 i;

    for (i = 0; i < 4; i++) {
        frontJoyHeld[i] = joyGetButtons(i);
        frontJoyPressed[i] = frontJoyHeld[i] & ~D_800FF3E8_B1978[i];
        D_800FF3E8_B1978[i] = frontJoyHeld[i];

        frontJoyDx[i] = joyGetStickX(i);
        if (frontJoyDx[i] < -0x23) {
            if (frontJoyDxRepeat[i] < 0) {
                frontJoyDxRepeat[i] += updateRate;
                if (frontJoyDxRepeat[i] >= 0) {
                    frontJoyDxRepeat[i] = -0xF;
                } else {
                    frontJoyDx[i] = 0;
                }
            } else {
                frontJoyDxRepeat[i] = -0x14;
            }
        } else if (frontJoyDx[i] >= 0x24) {
            if (frontJoyDxRepeat[i] > 0) {
                frontJoyDxRepeat[i] -= updateRate;
                if (frontJoyDxRepeat[i] <= 0) {
                    frontJoyDxRepeat[i] = 0xF;
                } else {
                    frontJoyDx[i] = 0;
                }
            } else {
                frontJoyDxRepeat[i] = 0x14;
            }
        } else {
            frontJoyDx[i] = 0;
            frontJoyDxRepeat[i] = 0;
        }

        frontJoyDy[i] = joyGetStickY(i);
        if (frontJoyDy[i] < -0x23) {
            if (D_800FF3E4_B1974[i] < 0) {
                D_800FF3E4_B1974[i] += updateRate;
                if (D_800FF3E4_B1974[i] >= 0) {
                    D_800FF3E4_B1974[i] = -0xF;
                } else {
                    frontJoyDy[i] = 0;
                }
            } else {
                D_800FF3E4_B1974[i] = -0x14;
            }
        } else if (frontJoyDy[i] >= 0x24) {
            if (D_800FF3E4_B1974[i] > 0) {
                D_800FF3E4_B1974[i] -= updateRate;
                if (D_800FF3E4_B1974[i] <= 0) {
                    D_800FF3E4_B1974[i] = 0xF;
                } else {
                    frontJoyDy[i] = 0;
                }
            } else {
                D_800FF3E4_B1974[i] = 0x14;
            }
        } else {
            frontJoyDy[i] = 0;
            D_800FF3E4_B1974[i] = 0;
        }
    }
}

void freeFrontEndList(s16 *list) {
    s16 *p;
    s32 item;

    if (*list == -1) {
        return;
    }
    p = list;
    do {
        item = *p;
        p++;
        freeFrontEndItem(item);
    } while (*p != -1);
}

void freeFrontEndItem(s32 item) {
    if (D_800FF6C8_B1C58[item] != 0) {
        if (frontendptrs[item] != NULL) {
            if ((D_800A51D0_A5DD0[item] & 0xC000) == 0xC000 && frontendptrs[item] != NULL) {
                texFreeTexture((void *) (u32) frontendptrs[item]);
            } else if (D_800A51D0_A5DD0[item] & 0x8000) {
                texFreeSprite((void *) (u32) frontendptrs[item]);
            } else if (D_800A51D0_A5DD0[item] & 0x4000) {
                objFreeObject((void *) (u32) frontendptrs[item]);
            } else {
                modFreeModel((void *) (u32) frontendptrs[item]);
            }
        }
        frontendptrs[item] = NULL;
        D_800FF6C8_B1C58[item] = 0;
        D_800A51D8_A5DD8--;
        objDoFrees();
    }
}

void loadFrontEndList(s16 *list) {
    s16 *p;
    s32 item;

    if (*list == -1) {
        return;
    }
    p = list;
    do {
        item = *p;
        p++;
        loadFrontEndItem(item);
    } while (*p != -1);
}

const char D_800AD370_ADF70[] = "front/front.c";
const char D_800AD380_ADF80[] = "front/front.c";
const char D_800AD390_ADF90[] = "loadFrontEndItem() - Item no %d out of range 0-%d\n";

void loadFrontEndItem(s32 item) {
    FrontEndSpawn spawn;
    Object *obj;
    Object_Racer *racer;

    if (D_800FF6C8_B1C58[item] == 0) {
        if ((D_800A51D0_A5DD0[item] & 0xC000) == 0xC000) {
            frontendptrs[item] = texLoadTexture(D_800A51D0_A5DD0[item] & 0x3FFF);
        } else if (D_800A51D0_A5DD0[item] & 0x8000) {
            frontendptrs[item] = texLoadSprite(D_800A51D0_A5DD0[item] & 0x3FFF, 0);
        } else if (D_800A51D0_A5DD0[item] & 0x4000) {
            spawn.objectId = D_800A51D0_A5DD0[item] & 0x3FFF;
            spawn.unk2 = 10;
            spawn.unk4 = 0;
            spawn.unk6 = 0;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.unkB = 0x40;
            spawn.unkA = 0;
            obj = objSetupObject((StaticInstanceSpawn *) &spawn, 0);
            if (obj->segment.header->unk1F > 0) {
                racer = obj->unk6C[0];
                racer->unkA = 2;
            }
            frontendptrs[item] = obj;
        } else {
            frontendptrs[item] = modLoadModel(D_800A51D0_A5DD0[item] & 0x3FFF, 0);
        }
        D_800FF6C8_B1C58[item] = 1;
        D_800A51D8_A5DD8++;
    }
}

void setupFrontEndList(s16 *list) {
    s16 *p;
    s32 item;

    if (*list == -1) {
        return;
    }
    p = list;
    do {
        item = *p;
        p++;
        setupFrontEndObject(item);
    } while (*p != -1);
}

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

void frontDrawObj(s32 index) {
    FrontEndObject *o;
    Gfx *g1;
    ModelInstance_JFG *mod;
    Gfx *g2;
    ObjectSegment1 seg;
    void *tex;
    Object *obj;
    s32 type;
    Gfx *g3;
    o = &currentobjects[index];
    if (frontendptrs[o->unk6] == NULL) {
        return;
    }
    if ((D_800A51D0_A5DD0[o->unk6] & 0xC000) == 0xC000) {
        return;
    }
    seg.trans.rotation.x = o->unk0;
    seg.trans.rotation.y = o->unk2;
    seg.trans.rotation.z = o->unk4;
    seg.trans.position.x = o->unkC;
    seg.trans.position.y = o->unk10;
    seg.trans.position.z = o->unk14;
    seg.trans.scale = o->unk8;
    type = D_800A51D0_A5DD0[o->unk6];
    if (type & 0x4000) {
        obj = frontendptrs[o->unk6];
        obj->segment.trans.rotation.x = o->unk0;
        obj->segment.trans.rotation.y = o->unk2;
        obj->segment.trans.rotation.z = o->unk4;
        obj->segment.trans.position.x = o->unkC;
        obj->segment.trans.position.y = o->unk10;
        obj->segment.trans.position.z = o->unk14;
        obj->segment.trans.scale = o->unk8;
        obj->segment.unk39 = objtrans;
        objPrintObject(&frontgfx, &frontmtx, &frontvtx, obj);
        return;
    }
    if (type & 0x8000) {
        seg.unk28 = o->unk18;
        tex = frontendptrs[o->unk6];
        gDPPipeSync(frontgfx++);
        gDPSetPrimColor(frontgfx++, 0, 0, frR, D_800A51B8_A5DB8, frB, objtrans);
        gDPSetEnvColorRGBA(frontgfx++, -0x100);
        camDo2DSprite(&frontgfx, &frontmtx, &frontvtx, &seg, tex, D_800A51C4_A5DC4, objtrans);
        gDPPipeSync(frontgfx++);
        gDPSetPrimColorRGBA(frontgfx++, -1);
        return;
    }
    gDPPipeSync(frontgfx++);
    if (objtrans < 0xFF) {
        gDPSetPrimColorRGBA(frontgfx++, (objtrans & 0xFF) | -0x100);
    } else {
        gDPSetPrimColorRGBA(frontgfx++, -1);
    }
    gDPSetEnvColorRGBA(frontgfx++, -0x100);
    seg.unk28 = o->unk18 * 0.0625f;
    mod = frontendptrs[o->unk6];
    if (mod->objModel->unk4E == 0) {
        camPushModelMtx(&frontgfx, &frontmtx, &seg.trans, 1.0f, 0.0f);
        g1 = frontgfx++;
        g1->words.w0 = ((((s32 *) mod->unk10)[mod->unkB] + 0x80000000) & 0xFFFFFF) | 0xBF000000;
        g1->words.w1 = (s32) mod->unk4 + 0x80000000;
        g2 = frontgfx++;
        g2->words.w0 = 0x06000000;
        g2->words.w1 = (s32) mod->objModel->unk74 + 0x80000000;
        g3 = frontgfx++;
        g3->words.w1 = 0;
        g3->words.w0 = 0xBF000000;
        camPopModelMtx(&frontgfx);
    }
    if (objtrans < 0xFF) {
        gDPPipeSync(frontgfx++);
        gDPSetPrimColorRGBA(frontgfx++, -1);
    }
}

typedef struct MultiPickupHdr {
    /* 0x0 */ s16 objectId;
    /* 0x2 */ s8 unk2;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
} MultiPickupHdr;

typedef struct MultiPickup {
    union {
        /* 0x0 */ MultiPickupHdr hdr;
        /* 0x0 */ u8 bytes[0xA];
    };
    /* 0xA */ s8 unkA;
    /* 0xB */ s8 unkB;
} MultiPickup;

typedef struct PickupInfo {
    /* 0x0 */ s16 objectId;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
} PickupInfo;

typedef struct DeathMatchEntry {
    /* 0x0 */ s32 level;
    /* 0x4 */ PickupInfo *lists[3];
} DeathMatchEntry;

DeathMatchEntry *getDeathMatchObjectTable(void);
s32 mainGetCurrentLevel(void);

void frontSetupMultiPickup(MultiPickup *arg0) {
    /* The original had more locals than survive here; `unused0`/`unused1`/
       `unused2` stand in for them.  Only three homes are load bearing: `i` at
       sp+0x78, `spawn` at sp+0x50 and `table` at sp+0x28 (i and table are the
       two variables the call to mainGetCurrentLevel spills), and the declared
       local block must total 0x58 bytes so the frame comes out 0x80. */
    s32 unused0;
    s32 i;
    s32 unused1[7];
    MultiPickup spawn;
    PickupInfo *info;
    Object *obj;
    u8 *src;
    u8 *dst;
    s32 level;
    s32 size;
    s32 n;
    s32 unused2[2];
    DeathMatchEntry *table;

    table = NULL;
    if ((multiGameType & 0xF) == 0) {
        table = getDeathMatchObjectTable();
    }
    if (table == NULL) {
        return;
    }
    i = 0;
    level = mainGetCurrentLevel();
    while (table[i].level >= 0 && table[i].level != level) {
        i++;
    }
    if (table[i].level != level) {
        return;
    }
    info = table[i].lists[multiObjectList];
    switch (info[arg0->unkA].objectId) {
        case 0xCF:
        case 0xE7:
        case 0xE8:
            spawn.unkB = info[arg0->unkA].unk2;
            spawn.unkA = info[arg0->unkA].unk4;
            size = 0x10;
            break;
        case 0xA9:
        case 0xE9:
            spawn.unkA = info[arg0->unkA].unk2;
            spawn.unkB = info[arg0->unkA].unk4;
            size = 0xC;
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF:
        case 0x100:
        case 0x101:
        case 0x102:
        case 0x103:
        case 0x104:
        case 0x105:
        case 0x123:
        case 0x124:
        case 0x125:
            spawn.unkA = 0;
            spawn.unkB = info[arg0->unkA].unk2;
            size = 0xC;
            break;
        case 0x12A:
            spawn.unkA = info[arg0->unkA].unk2;
            spawn.unkB = info[arg0->unkA].unk4;
            size = 0xC;
            break;
        case 0x33C:
            spawn.unkA = info[arg0->unkA].unk2;
            spawn.unkB = info[arg0->unkA].unk4;
            size = 0xC;
            break;
        default:
            return;
    }
    src = (u8 *) arg0;
    dst = (u8 *) &spawn;
    n = 9;
    do {
        *dst++ = *src++;
    } while (n--);
    spawn.hdr.objectId = info[arg0->unkA].objectId;
    spawn.hdr.unk2 = size;
    obj = objSetupObject((StaticInstanceSpawn *) &spawn, 1);
    if (obj != NULL) {
        obj->segment.unk3C = 0;
    }
}

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
    return D_800FF386_B1916;
}

void frontSetLanguage(s32 language) {
    D_800FF386_B1916 = language;
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
