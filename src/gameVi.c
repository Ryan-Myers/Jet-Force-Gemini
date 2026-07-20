#include "common.h"
#include "gameVi.h"

void viChangeMode(s32);
extern s32 *D_800A3900_A4500[4];
extern s32 D_800A3928_A4528;
extern void *D_800FEB60_B1750;
extern OSMesgQueue D_800FEB80_B1770;
extern OSScClient D_800FEC40_B1830;
extern s8 D_800FECA5_B1895;
extern s8 D_800FECA6_B1896;
extern s8 D_800FECA7_B1897;
extern s8 D_800FECA9_B1899;
extern f32 D_800FECC8_B18B8; // gVideoHeightRatio: Height ratio for PAL vs NTSC
extern f32 aspectRatioFloat;
extern s32 viFramesPerSecond;
extern s32 viNoZbufferRealloc;
extern s8 widescreenVOffsetMirror;

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
    osCreateMesgQueue(&D_800FEB80_B1770, &D_800FEB60_B1750, 8);
    osScAddClient(sc, &D_800FEC40_B1830, &D_800FEB80_B1770, 2);
    D_800A3900_A4500[0] = (s32 *) mmAlloc((screenHeight * 0x500) + 0x30, COLOUR_TAG_WHITE);
    widescreenVOffsetMirror = 0;
    D_800FECA5_B1895 = 0;
    D_800FECA6_B1896 = 0;
    D_800FECA7_B1897 = 1;
    viNoZbufferRealloc = 0;
    viChangeMode(0);
    D_800A3928_A4528 = 1;
    D_800FECA9_B1899 = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viChangeMode.s")

#ifdef NON_EQUIVALENT
void viSetTiming(void);

void viReset(void) {
    s32 width;
    s32 height;

    viGetCurrentSize(&width, &height);
    {
        u32 *screen = currentScreen;
        u32 screenSize = (u32) (width * height) >> 1;
        s32 i;
        for (i = screenSize - 1; i >= 0; i--) {
            *screen = 0;
            screen++;
        }
    }
    {
        u32 *screen = otherScreen;
        u32 screenSize = (u32) (width * height) >> 1;
        s32 i;
        for (i = screenSize - 1; i >= 0; i--) {
            *screen = 0;
            screen++;
        }
    }
    osWritebackDCacheAll();
    if ((D_800FF988 != 0) && (D_800FF988 != 4)) {
        if (osTvType == OS_TV_TYPE_PAL) {
            D_800FF988 = 14;
        } else if (osTvType == OS_TV_TYPE_MPAL) {
            D_800FF988 = 13;
        } else {
            D_800FF988 = 12;
        }
        viSetTiming();
    }
    osViBlack(0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viReset.s")
#endif

extern s32 framebufferSize;

void viAllocateZBuffer(s32 arg0, s32 arg1) {
    framebufferSize = (arg0 * arg1 * 2) + 0x30;
    D_800A3900_A4500[3] = (s32 *) mmAlloc(framebufferSize, COLOUR_TAG_WHITE);
    otherZbuf = FBALIGN(D_800A3900_A4500[3]);
}

extern s32 *D_800A390C_A450C; // UNUSED?

void viFreeZBuffer(UNUSED s32 arg0, UNUSED s32 arg1) {
    if (D_800A390C_A450C != NULL) {
        mmFree(D_800A390C_A450C);
        D_800A390C_A450C = NULL;
        otherZbuf = 0;
        framebufferSize = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetTiming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetCurrentSize.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viConvertXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetScaleXY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viFrameRateReset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viFrameSync.s")

s32 viGetVideoMode(void) {
    return D_800FF988 & 3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetWideAdjust.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetWideAdjust.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viSetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viGetTrippleBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viChangeBuffers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viNoClear.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/viDisplayingScreen0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055D44.s")

//get_osViMode
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055DA8.s")

//swap_framebuffer_pointers
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055E68.s")

//osVimode_copy
#pragma GLOBAL_ASM("asm/nonmatchings/gameVi/func_80055F4C.s")
