#ifndef _GAME_VI_H_
#define _GAME_VI_H_

#include <ultra64.h>

#define REFRESH_50HZ 50
#define REFRESH_60HZ 60

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_HEIGHT_PAL 264
#define HIGH_RES_SCREEN_WIDTH 640
#define HIGH_RES_SCREEN_HEIGHT 480

#define LOW_RES_WIDTH 320.0f
#define LOW_RES_PAL_HEIGHT 264.0f
#define LOW_RES_NTSC_HEIGHT 240.0f
#define LOW_RES_MPAL_HEIGHT LOW_RES_NTSC_HEIGHT

#define PAL_HEIGHT_DIFFERENCE 24

#define ASPECT_RATIO_PAL  (LOW_RES_WIDTH / LOW_RES_PAL_HEIGHT)
#define ASPECT_RATIO_NTSC (LOW_RES_WIDTH / LOW_RES_NTSC_HEIGHT)
#define ASPECT_RATIO_MPAL (LOW_RES_WIDTH / LOW_RES_MPAL_HEIGHT)

#define HEIGHT_RATIO_PAL  (LOW_RES_PAL_HEIGHT / LOW_RES_NTSC_HEIGHT)
#define HEIGHT_RATIO_NTSC (LOW_RES_NTSC_HEIGHT / LOW_RES_NTSC_HEIGHT)
#define HEIGHT_RATIO_MPAL (LOW_RES_MPAL_HEIGHT / LOW_RES_NTSC_HEIGHT)

// Framebuffers require 64 byte alignment.
#define FBALIGN(a) ((s32 *) (((s32) (a) + 0x3F) & ~0x3F))

/**
 * Keeps the value within the range.
 */
#define CLAMP(x, low, high) {       \
    if ((x) < (low)) (x) = (low);   \
    if ((x) > (high)) (x) = (high); \
}

/**
 * Values for the rate game logic will work depending on the framerate. Vanilla DKR will default to LOGIC_30FPS (2)
 */
enum LogicUpdateRates {
    LOGIC_NULL,
    LOGIC_60FPS,
    LOGIC_30FPS,
    LOGIC_20FPS,
    LOGIC_15FPS,
    LOGIC_12FPS,
    LOGIC_10FPS,
    LOGIC_5FPS = 12
};

typedef enum VideoModes {
    VIDEO_MODE_NTSC_LPN,
    VIDEO_MODE_NTSC_LAN,
    VIDEO_MODE_NTSC_HPN,
    VIDEO_MODE_NTSC_HAF,
    VIDEO_MODE_MPAL_LPN,
    VIDEO_MODE_MPAL_LAN,
    VIDEO_MODE_MPAL_HPN,
    VIDEO_MODE_MPAL_HAF,
    VIDEO_MODE_PAL_LPN,
    VIDEO_MODE_PAL_LAN,
    VIDEO_MODE_PAL_HPN,
    VIDEO_MODE_PAL_HAF,
} VideoModes;

// Size: 0x28
typedef struct ResolutionSettings {
    s32 width;          // source framebuffer width
    s32 height;         // source framebuffer height
    s32 displayWidth;   // horizontal active region (xScale reference)
    s32 displayHeight;  // vertical active region  (yScale reference)
    s32 verticalOffset; // vertical offset
    VideoModes videoMode;
    char name[16];
} ResolutionSettings;

void viInit(OSSched *sc);
void viChangeMode(s32 arg0);
void viReset(void);
void viAllocateZBuffer(s32 width, s32 height);
void viFreeZBuffer(s32 width, s32 height);
void viSetTiming(void);
void viGetCurrentSize(s32 *width, s32 *height);
void viConvertXY(s32 *x, s32 *y);
void viGetScaleXY(f32 *_hScale, f32 *_vScale);
void viFrameRateReset(void);
s32 viFrameSync(s32 mesg);
s32 viGetVideoMode(void);
s8 viGetWideAdjust(void);
void viSetWideAdjust(s32 offset);
void viSetTrippleBuffer(s32 arg0);
s8 viGetTrippleBuffer(void);
s32 viChangeBuffers(void);
void viNoClear(void);
s32 viDisplayingScreen0(void);
void func_80055260_55E60(u8 *src, s32 length);
OSViMode *viGetOsViMode(VideoModes videoMode);
void fb_swap(void);
void fb_memcpy(u8 *src, u8 *dest, s32 len);

#endif
