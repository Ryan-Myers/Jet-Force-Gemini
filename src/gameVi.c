#include "common.h"
#include "gameVi.h"

void fb_swap(void);
void viChangeMode(s32);
void viSetTiming(void);
extern s32 *D_800A3900_A4500[4];
extern s32 D_800A3928_A4528;
extern void *D_800FEB60_B1750;
extern OSMesgQueue gVideoMesgQueue[8];
extern OSScClient D_800FEC40_B1830;
extern s8 sTripleBuffer;
extern s8 D_800FECA6_B1896;
extern s8 sShouldClearVi;
extern s8 sBlackScreenTimer;
extern f32 D_800FECC8_B18B8; // gVideoHeightRatio: Height ratio for PAL vs NTSC
extern f32 aspectRatioFloat;
extern s32 viFramesPerSecond;
extern s32 viNoZbufferRealloc;
extern s8 widescreenVOffsetMirror;
extern s32 *framebufferPointers[3];
extern u8 D_800A32A0_A3EA0;
extern s32 *extraScreen;
extern s8 framebufferChoice;
extern s8 sTripleBuffer;

void viInit(OSSched *sc) {
    s32 screenHeight;

    if (osTvType == OS_TV_TYPE_PAL) {
        viFramesPerSecond = REFRESH_50HZ;
        aspectRatioFloat = ASPECT_RATIO_PAL;
        screenHeight = SCREEN_HEIGHT_PAL;
        D_800FECC8_B18B8 = HEIGHT_RATIO_PAL;
    } else {
        viFramesPerSecond = REFRESH_60HZ;
        aspectRatioFloat = ASPECT_RATIO_NTSC;
        screenHeight = SCREEN_HEIGHT;
        D_800FECC8_B18B8 = HEIGHT_RATIO_NTSC;
    }
    osCreateMesgQueue(gVideoMesgQueue, &D_800FEB60_B1750, 8);
    osScAddClient(sc, &D_800FEC40_B1830, gVideoMesgQueue, 2);
    D_800A3900_A4500[0] = (s32 *) mmAlloc((screenHeight * 0x500) + 0x30, COLOUR_TAG_WHITE);
    widescreenVOffsetMirror = 0;
    sTripleBuffer = 0;
    D_800FECA6_B1896 = 0;
#ifdef VERSION_kiosk
    sShouldClearVi = FALSE;
    viNoZbufferRealloc = 0;
    viChangeMode(0);
    osViBlack(TRUE);
    sBlackScreenTimer = 0xC;
#else
    sShouldClearVi = TRUE;
    viNoZbufferRealloc = 0;
    viChangeMode(0);
    D_800A3928_A4528 = 1;
    sBlackScreenTimer = 1;
#endif
}

// Size: 0x28
typedef struct ResolutionSettings {
    s32 width;
    s32 height;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u32 videoMode;
    s32 unk18;
    s32 unk1C;
    s32 unk20;    
    s32 unk24;
} ResolutionSettings;
extern ResolutionSettings resolutionSettings[15]; // Size: 0x258
extern s8 D_800FECA8_B1898;
extern f32 hScale;
extern f32 vScale;

void fontSetWindow0(s32 width, s32 height);
void func_80055260_55E60(u8 *src, s32 length);
void viAllocateZBuffer(s32 width, s32 height);
void viFrameRateReset(void);
void viFreeZBuffer(s32 width, s32 height);
void viSetTiming(void);

void viChangeMode(s32 arg0) {
    s32 bufferSize;
    ResolutionSettings *resolution;
    s32 temp_t6;

#ifdef VERSION_kiosk
    D_800FECA8_B1898 = arg0 & 3;
#else
    D_800FECA8_B1898 = arg0 & 3 & ~D_800A3928_A4528;
    D_800A3928_A4528 = 0;
#endif

    if (osTvType == OS_TV_TYPE_MPAL) {
        D_800FECA8_B1898 += 4;
    } else if (osTvType == OS_TV_TYPE_PAL) {
        D_800FECA8_B1898 += 8;
    }
    resolution = &resolutionSettings[D_800FECA8_B1898];
    hScale = resolution->width / LOW_RES_WIDTH;
    vScale = resolution->height / LOW_RES_NTSC_HEIGHT;
    fontSetWindow0(resolution->width, resolution->height);
    if (viNoZbufferRealloc == 0) {
        viFreeZBuffer(resolution->width, resolution->height);
    }
    if (D_800A3900_A4500[1] != NULL) {
        mmFree(D_800A3900_A4500[1]);
        D_800A3900_A4500[1] = NULL;
    }
    if (D_800A3900_A4500[2] != NULL) {
        mmFree(D_800A3900_A4500[2]);
        D_800A3900_A4500[2] = NULL;
    }
    bufferSize = resolution->width * resolution->height * 2;
    framebufferChoice = 0;
    framebufferPointers[0] = FBALIGN(D_800A3900_A4500[0]);
    framebufferPointers[1] = NULL;
    framebufferPointers[2] = NULL;
    temp_t6 = D_800FECA8_B1898 & 3;
    if ((temp_t6 != 2) && (temp_t6 != 3)) {
        if (1){} // Fake
        framebufferPointers[1] = (s32 *) ((u8 *)framebufferPointers[0] + bufferSize);
    } else {
        D_800A3900_A4500[1] = (s32 *) mmAlloc(bufferSize + 0x30, COLOUR_TAG_WHITE);
        framebufferPointers[1] = FBALIGN(D_800A3900_A4500[1]);
    }
    if (D_800FECA6_B1896 != 0) {
        D_800A3900_A4500[2] = (s32 *) mmAlloc(bufferSize + 0x30, COLOUR_TAG_WHITE);;
        framebufferPointers[2] = FBALIGN(D_800A3900_A4500[2]);
    }
    sTripleBuffer = D_800FECA6_B1896;
    if (sShouldClearVi) {
        func_80055260_55E60((u8 *) framebufferPointers[0], bufferSize);
        func_80055260_55E60((u8 *) framebufferPointers[1], bufferSize);
        if (sTripleBuffer) {
            func_80055260_55E60((u8 *) framebufferPointers[2], bufferSize);
        }
    }
    sShouldClearVi = TRUE;
    if (viNoZbufferRealloc == 0) {
        viAllocateZBuffer(resolution->width, resolution->height);
    }
    viFrameRateReset();
    fb_swap();
    viSetTiming();
    viNoZbufferRealloc = 0;
}

#ifdef VERSION_us
void viReset(void) {
    s32 *screen;
    u32 screenSize;
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);
    screen = currentScreen;
    screenSize = (u32) (width * height) >> 1;
    while (screenSize--) {
        *screen++ = 0;
    }
    screen = otherScreen;
    screenSize = (u32) (width * height) >> 1;
    while (screenSize--) {
        *screen++ = 0;
    }
    osWritebackDCacheAll();
    if ((D_800FECA8_B1898 != 0) && (D_800FECA8_B1898 != 4)) {
        if (osTvType == OS_TV_TYPE_PAL) {
            D_800FECA8_B1898 = 14;
        } else if (osTvType == OS_TV_TYPE_MPAL) {
            D_800FECA8_B1898 = 13;
        } else {
            D_800FECA8_B1898 = 12;
        }
        viSetTiming();
    }
    osViBlack(FALSE);
}
#endif

extern s32 framebufferSize;

void viAllocateZBuffer(s32 width, s32 height) {
    framebufferSize = (width * height * 2) + 0x30;
    D_800A3900_A4500[3] = (s32 *) mmAlloc(framebufferSize, COLOUR_TAG_WHITE);
    otherZbuf = FBALIGN(D_800A3900_A4500[3]);
}

extern s32 *D_800A390C_A450C; // UNUSED?

void viFreeZBuffer(UNUSED s32 width, UNUSED s32 height) {
    if (D_800A390C_A450C != NULL) {
        mmFree(D_800A390C_A450C);
        D_800A390C_A450C = NULL;
        otherZbuf = 0;
        framebufferSize = 0;
    }
}

#ifdef NON_EQUIVALENT
OSViMode *viGetOsViMode(u32 videoMode);
void fb_memcpy(u8 *arg0, u8 *arg1, s32 arg2);
extern s32 D_800A3924_A4524; // gSetCustomViMode: Flag to indicate if a custom VI mode has been set
typedef struct Unk_800FEC58_B1848 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Unk_800FEC58_B1848;
extern Unk_800FEC58_B1848 D_800FEC58_B1848; // Size: 0x18
extern s32 D_800FEC70_B1860;


extern OSViMode osViMode_custom;
typedef struct Resbitfield {
    u32 bi31 : 1;
    u32 bit30 : 1;
    u32 rest : 30;
} Resbitfield;
extern Resbitfield someResVar;
extern s8 widescreenVOffsetMirror;

void viSetTiming(void) {
    OSViMode *viMode;
    ResolutionSettings *temp_v1;
    s32 var_a0;
    u32 var_a1;
    s32 i;
    ResolutionSettings *new_var;
    u32 temp_t8;

    temp_v1 = &resolutionSettings[D_800FECA8_B1898];
    viMode = viGetOsViMode(temp_v1->videoMode);
    fb_memcpy((u8 *)viMode, (u8 *)&osViMode_custom, sizeof(OSViMode));
    osViMode_custom.comRegs.width =  temp_v1->width;
    osViMode_custom.comRegs.xScale = ((s32) (temp_v1->width << 9) / (s32) temp_v1->unk8);
    var_a0 = temp_v1->unk10;
    var_a1 = var_a0 << 17;
    if ((D_800FECA8_B1898 & 1)) {
        if (someResVar.bit30 != 0) {
            //if (!temp_t8){}
            var_a0 += widescreenVOffsetMirror;
            var_a1 = var_a0 << 17;
        }
    }
    new_var = temp_v1; // Why does this improve the score so much?
    for (i = 0; i < 2; i++) {
        temp_t8 = osViMode_custom.fldRegs[i].vStart;
        temp_t8 += var_a1;
        osViMode_custom.fldRegs[i].origin = new_var->width * 2;
        osViMode_custom.comRegs.leap = ((s32) (new_var->unk4 << 10) / (s32) new_var->unkC);
        osViMode_custom.fldRegs[i].vStart = temp_t8;
        osViMode_custom.comRegs.hStart = temp_t8 - (((SCREEN_HEIGHT - var_a0) - new_var->unkC) << 1);
    }
    if (D_800A3924_A4524 != 0) {
        osViSetMode(&osViMode_custom);
    }
    osViSetSpecialFeatures(OS_VI_DIVOT_ON | OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF);
    D_800A3924_A4524 = 1;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetTiming.s")
#endif

void viGetCurrentSize(s32 *width, s32 *height) {
    *width = resolutionSettings[D_800FECA8_B1898].width;
    *height = resolutionSettings[D_800FECA8_B1898].height;
}

void viConvertXY(s32 *x, s32 *y) {
    *x = *x * hScale;
    *y = *y * vScale;
}

void viGetScaleXY(f32 *_hScale, f32 *_vScale) {
    *_hScale = hScale;
    *_vScale = vScale;
}

extern u8 D_800FECAB_B189B;
extern u8 gVideoDeltaTime;
extern u8 D_800FECAD_B189D;
extern s32 runInOneFrame;

void viFrameRateReset(void) {
    D_800FECAD_B189D = TRUE;
    D_800FECAB_B189B = 0;
    gVideoDeltaTime = 2;
    runInOneFrame = FALSE;
}

s32 viFrameSync(s32 mesg) {
    u8 tempUpdateRate;

    tempUpdateRate = LOGIC_60FPS;
    if (sBlackScreenTimer) {
        sBlackScreenTimer--;
        if (sBlackScreenTimer == 0) {
            osViBlack(FALSE);
        }
    }
    if (mesg != MESG_SKIP_BUFFER_SWAP) {
        fb_swap();
    }
    while (osRecvMesg(gVideoMesgQueue, NULL, OS_MESG_NOBLOCK) != -1) {
        tempUpdateRate++;
    }

    if (gVideoDeltaTime == 3) {
        if (tempUpdateRate < 3) {
            D_800FECAB_B189B++;
            if (D_800FECAB_B189B > 30) {
                gVideoDeltaTime = 2;
            }
        } else {
            D_800FECAB_B189B = 0;
        }
    } else if ((tempUpdateRate >= 3) && (D_800FECAD_B189D == FALSE)) {
        gVideoDeltaTime = 3;
        D_800FECAB_B189B = 0;
    }
    if (runInOneFrame) {
        gVideoDeltaTime = 1;
    }
    while (tempUpdateRate < gVideoDeltaTime) {
        osRecvMesg(gVideoMesgQueue, NULL, OS_MESG_BLOCK);
        tempUpdateRate++;
    }

    osViSwapBuffer(otherScreen);
    osRecvMesg(gVideoMesgQueue, NULL, OS_MESG_BLOCK);
    D_800FECAD_B189D = FALSE;
    return tempUpdateRate;
}

s32 viGetVideoMode(void) {
    return D_800FECA8_B1898 & 3;
}

s8 viGetWideAdjust(void) {
    return widescreenVOffsetMirror;
}

void viSetWideAdjust(s32 offset) {
    CLAMP(offset, -30, 30);
    widescreenVOffsetMirror = offset;
    viSetTiming();
}

void viSetTrippleBuffer(s32 arg0) {
    D_800FECA6_B1896 = arg0 & 1;
}

s8 viGetTrippleBuffer(void) {
    return sTripleBuffer;
}

s32 viChangeBuffers(void) {
    return sTripleBuffer != D_800FECA6_B1896;
}

void viNoClear(void) {
    sShouldClearVi = FALSE;
}

s32 viDisplayingScreen0(void) {
    if (framebufferPointers[0] == otherScreen) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Fill a region of memory with either 0 or -1, depending on the value of D_800A32A0_A3EA0.
 * If D_800A32A0_A3EA0 is non-zero, fill with 0. Otherwise, fill with -1.
 * The length is specified in bytes, but the function operates on 4-byte chunks.
 */
void func_80055260_55E60(u8 *src, s32 length) {
    s32 *dest;
    s32 fourByteLength;

    dest = (s32 *) src;
    fourByteLength = (length >> 2);
    if (D_800A32A0_A3EA0 != 0) {
        while(fourByteLength--) {
            *dest++ = 0;
        }
    } else {
        while(fourByteLength--) {
            *dest++ = -1;
        }
    }
#ifdef VERSION_us
    osWritebackDCacheAll();
#endif
}

OSViMode *viGetOsViMode(u32 videoMode) {
    switch (videoMode) {
        case 0:
            return &osViModeNtscLpn1;
        case 1:
            return &osViModeNtscLan1;
        case 2:
            return &osViModeNtscHpn1;
        case 3:
            return &osViModeNtscHaf1;
        case 4:
            return &osViModeMpalLpn1;
        case 5:
            return &osViModeMpalLan1;
        case 6:
            return &osViModeMpalHpn1;
        case 7:
            return &osViModeMpalHaf1;
        case 8:
            return &osViModePalLpn1;
        case 9:
            return &osViModePalLan1;
        case 10:
            return &osViModePalHpn1;
        case 11:
            return &osViModePalHaf1;
        default:
            return NULL;
    }
}

//swap_framebuffer_pointers
void fb_swap(void) {
    otherScreen = (s32 *) framebufferPointers[framebufferChoice];
    framebufferChoice++;
    if (sTripleBuffer) {
        if (framebufferChoice >= 3) {
            framebufferChoice = 0;
        }
        extraScreen = (s32 *) framebufferPointers[framebufferChoice];
        if (framebufferChoice < 2) {
            currentScreen = (s32 *) framebufferPointers[framebufferChoice + 1];
        } else {
            currentScreen = (s32 *) framebufferPointers[0];
        }
    } else {
        if (framebufferChoice >= 2) {
            framebufferChoice = 0;
        }
        currentScreen = (s32 *) framebufferPointers[framebufferChoice];
        extraScreen = (s32 *) &currentScreen[0];
    }
}

/**
 * Copy byte-by-byte a region from one address to another.
 */
void fb_memcpy(u8 *src, u8 *dest, s32 len) {
    while (len--) {
        *dest++ = *src++;
    }
}
