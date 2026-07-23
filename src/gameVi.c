#include "common.h"
#include "gameVi.h"

OSMesg gVideoMesgBuf[8];
OSMesgQueue gVideoMesgQueue[8];
OSScClient gVideoSched;
OSViMode osViMode_custom;
s32 framebufferSize;
s8 framebufferChoice;
s8 sTripleBuffer;
s8 D_800FECA6_B1896;
s8 sShouldClearVi;
s8 D_800FECA8_B1898;
s8 sBlackScreenTimer;
s8 widescreenVOffsetMirror;
u8 D_800FECAB_B189B;
u8 gVideoDeltaTime;
u8 D_800FECAD_B189D;
s32 *currentScreen;
s32 *extraScreen;
s32 *otherScreen;
s32 *otherZbuf;
s32 viFramesPerSecond;
f32 aspectRatioFloat;
f32 heightRatioFloat; // Height ratio for PAL vs NTSC
s32 runInOneFrame;
s32 viNoZbufferRealloc;

s32 *D_800A3900_A4500[4] = { NULL, NULL, NULL, NULL }; // Triple buffer framebuffer pointer and the last element is the Z buffer
s32 *framebufferPointers[3] = { NULL, NULL, NULL };
f32 hScale = 1.0f;
f32 vScale = 1.0f;

#ifdef VERSION_us
s32 sShouldSetCustomViMode = FALSE; // Flag to indicate if a custom VI mode has been set
s32 D_800A3928_A4528 = 1;
#endif

ResolutionSettings resolutionSettings[] = {
    { 320, 240, 320, 240, 0, VIDEO_MODE_NTSC_LPN, "Ntsc LowRes" },
    { 320, 240, 320, 180, 30, VIDEO_MODE_NTSC_LAN, "Ntsc Widescreen" },
    { 448, 336, 320, 240, 0, VIDEO_MODE_NTSC_HAF, "Ntsc MediumRes" },
    { 448, 336, 320, 180, 30, VIDEO_MODE_NTSC_HAF, "Ntsc HiResWide" },
    { 320, 240, 320, 240, 0, VIDEO_MODE_MPAL_LPN, "Mpal LowRes" },
    { 320, 240, 320, 180, 30, VIDEO_MODE_MPAL_LAN, "Mpal Widescreen" },
    { 448, 336, 320, 240, 0, VIDEO_MODE_MPAL_HAF, "Mpal MediumRes" },
    { 448, 336, 320, 180, 30, VIDEO_MODE_MPAL_HAF, "Mpal HiResWide" },
    { 320, 240, 320, 280, -20, VIDEO_MODE_PAL_LPN, "Pal LowRes" },
    { 320, 240, 320, 204, 10, VIDEO_MODE_PAL_LAN, "Pal Widescreen" },
    { 448, 336, 320, 280, -20, VIDEO_MODE_PAL_HAF, "Pal MediumRes" },
    { 448, 336, 320, 204, 10, VIDEO_MODE_PAL_HAF, "Pal HiResWide" },
#ifdef VERSION_us
    { 320, 240, 320, 240, 0, VIDEO_MODE_NTSC_LPN, "Ntsc Reset Mode" },
    { 320, 240, 320, 240, 0, VIDEO_MODE_NTSC_LPN, "Mpal Reset Mode" },
    { 320, 240, 320, 240, 0, VIDEO_MODE_PAL_LPN, "Pal Reset Mode" }
#endif
};

void viInit(OSSched *sc) {
    s32 screenHeight;

    if (osTvType == OS_TV_TYPE_PAL) {
        viFramesPerSecond = REFRESH_50HZ;
        aspectRatioFloat = ASPECT_RATIO_PAL;
        screenHeight = SCREEN_HEIGHT_PAL;
        heightRatioFloat = HEIGHT_RATIO_PAL;
    } else {
        viFramesPerSecond = REFRESH_60HZ;
        aspectRatioFloat = ASPECT_RATIO_NTSC;
        screenHeight = SCREEN_HEIGHT;
        heightRatioFloat = HEIGHT_RATIO_NTSC;
    }
    osCreateMesgQueue(gVideoMesgQueue, gVideoMesgBuf, ARRAY_COUNT(gVideoMesgBuf));
    osScAddClient(sc, &gVideoSched, gVideoMesgQueue, OS_SC_ID_VIDEO);
    D_800A3900_A4500[0] = (s32 *) mmAlloc((screenHeight * 0x500) + 0x30, COLOUR_TAG_WHITE);
    widescreenVOffsetMirror = 0;
    sTripleBuffer = 0;
    D_800FECA6_B1896 = 0;
#ifdef VERSION_kiosk
    sShouldClearVi = FALSE;
    viNoZbufferRealloc = 0;
    viChangeMode(0);
    osViBlack(TRUE);
    sBlackScreenTimer = 12;
#else
    sShouldClearVi = TRUE;
    viNoZbufferRealloc = 0;
    viChangeMode(0);
    D_800A3928_A4528 = 1;
    sBlackScreenTimer = 1;
#endif
}

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

void viAllocateZBuffer(s32 width, s32 height) {
    framebufferSize = (width * height * 2) + 0x30;
    D_800A3900_A4500[3] = (s32 *) mmAlloc(framebufferSize, COLOUR_TAG_WHITE);
    otherZbuf = FBALIGN(D_800A3900_A4500[3]);
}


void viFreeZBuffer(UNUSED s32 width, UNUSED s32 height) {
    if (D_800A3900_A4500[3] != NULL) {
        mmFree(D_800A3900_A4500[3]);
        D_800A3900_A4500[3] = NULL;
        otherZbuf = 0;
        framebufferSize = 0;
    }
}

void viSetTiming(void) {
    OSViMode *viMode;
    s32 verticalOffset;
    s32 i;
    ResolutionSettings *resolution;

    resolution = &resolutionSettings[D_800FECA8_B1898];
    viMode = viGetOsViMode(resolution->videoMode);
    fb_memcpy((u8 *) viMode, (u8 *) &osViMode_custom, sizeof(OSViMode));
    osViMode_custom.comRegs.width = WIDTH(resolution->width);
    osViMode_custom.comRegs.xScale = ((resolution->width << 9) / resolution->displayWidth);
    verticalOffset = resolution->verticalOffset;
    if ((D_800FECA8_B1898 & 1) && (someResVar.bit30)) {
        verticalOffset += widescreenVOffsetMirror;
    }
    for (i = 0; i < ARRAY_COUNT(osViMode_custom.fldRegs); i++) {
        osViMode_custom.fldRegs[i].origin = ORIGIN(resolution->width * 2);
        osViMode_custom.fldRegs[i].yScale = ((resolution->height << 10) / resolution->displayHeight);
        osViMode_custom.fldRegs[i].vStart += (verticalOffset << 17); //((verticalOffset * 2) << 16);
        osViMode_custom.fldRegs[i].vStart -= (((SCREEN_HEIGHT - verticalOffset) - resolution->displayHeight) * 2);
    }
#ifdef VERSION_us
    if (sShouldSetCustomViMode) 
#endif
    {
        osViSetMode(&osViMode_custom);
    }
    osViSetSpecialFeatures(OS_VI_DIVOT_ON | OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF);
#ifdef VERSION_us
    sShouldSetCustomViMode = TRUE;
#endif
}

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

OSViMode *viGetOsViMode(VideoModes videoMode) {
    switch (videoMode) {
        case VIDEO_MODE_NTSC_LPN:
            return &osViModeNtscLpn1;
        case VIDEO_MODE_NTSC_LAN:
            return &osViModeNtscLan1;
        case VIDEO_MODE_NTSC_HPN:
            return &osViModeNtscHpn1;
        case VIDEO_MODE_NTSC_HAF:
            return &osViModeNtscHaf1;
        case VIDEO_MODE_MPAL_LPN:
            return &osViModeMpalLpn1;
        case VIDEO_MODE_MPAL_LAN:
            return &osViModeMpalLan1;
        case VIDEO_MODE_MPAL_HPN:
            return &osViModeMpalHpn1;
        case VIDEO_MODE_MPAL_HAF:
            return &osViModeMpalHaf1;
        case VIDEO_MODE_PAL_LPN:
            return &osViModePalLpn1;
        case VIDEO_MODE_PAL_LAN:
            return &osViModePalLan1;
        case VIDEO_MODE_PAL_HPN:
            return &osViModePalHpn1;
        case VIDEO_MODE_PAL_HAF:
            return &osViModePalHaf1;
        default:
            return NULL;
    }
}

void fb_swap(void) {
    otherScreen = framebufferPointers[framebufferChoice];
    framebufferChoice++;
    if (sTripleBuffer) {
        if (framebufferChoice >= 3) {
            framebufferChoice = 0;
        }
        extraScreen = framebufferPointers[framebufferChoice];
        if (framebufferChoice < 2) {
            currentScreen = framebufferPointers[framebufferChoice + 1];
        } else {
            currentScreen = framebufferPointers[0];
        }
    } else {
        if (framebufferChoice >= 2) {
            framebufferChoice = 0;
        }
        currentScreen = framebufferPointers[framebufferChoice];
        extraScreen = &currentScreen[0];
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
