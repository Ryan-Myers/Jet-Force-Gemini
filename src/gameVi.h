#ifndef _GAME_VI_H_
#define _GAME_VI_H_



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
#define FBALIGN(a) ((u16 *) (((s32) (a) + 0x3F) & ~0x3F))

/**
 * This is an offset with a size of two rows of the framebuffer.
 * It's likely they were trying to offset the framebuffer in order to
 * undo what Libultra does, where in order to sample pixels from the top and
 * bottom of the screen for anti aliasing and dedithering, they shift the framebuffer
 * to hide some visible rows.
 * A small mistake is present though, where they offset by two rows instead of one.
 * This will cause there to be visible noise on the bottom row.
*/
#define VI_OFFSET (SCREEN_WIDTH * sizeof(u16) * 2)

/**
 * The video width is the lower 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_WIDTH(width_and_height) (width_and_height & 0xFFFF)
/**
 * The video width is the higher 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_HEIGHT(width_and_height) (width_and_height >> 16)

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

enum VideoModes {
    VIDEO_MODE_LOWRES_LAN,
    VIDEO_MODE_LOWRES_LPN,
    VIDEO_MODE_MEDRES_LPN,
    VIDEO_MODE_MEDRES_LAN,
    VIDEO_MODE_HIGHRES_HPN,
    VIDEO_MODE_HIGHRES_HAN,
    VIDEO_MODE_HIGHRES_HPF,
    VIDEO_MODE_HIGHRES_HAF,

    VIDEO_MODE_LOWRES_MASK = 0,
    VIDEO_MODE_MIDRES_MASK = 2,
    VIDEO_MODE_HIGHRES_MASK = 4
};

#endif
