#ifndef _DIPRINT_H_
#define _DIPRINT_H_

#include "stdarg.h"
#include <PR/gu.h>

typedef struct TexFontCoords {
    u8 u, v;
} TexFontCoords;

#define RENDER_PRINTF_CMD_ARG_BYTE(val) *gDebugPrintBufferEnd = val; gDebugPrintBufferEnd++;
#define RENDER_PRINTF_CMD_ARG_SHORT(val) RENDER_PRINTF_CMD_ARG_BYTE(val) RENDER_PRINTF_CMD_ARG_BYTE(val >> 8)

#define RENDER_PRINTF_CMD_END RENDER_PRINTF_CMD_ARG_BYTE(0)

#define RENDER_PRINTF_CMD_SET_COLOR(red, green, blue, alpha) \
    RENDER_PRINTF_CMD_ARG_BYTE(0x81)                         \
    RENDER_PRINTF_CMD_ARG_BYTE(red)                          \
    RENDER_PRINTF_CMD_ARG_BYTE(green)                        \
    RENDER_PRINTF_CMD_ARG_BYTE(blue)                         \
    RENDER_PRINTF_CMD_ARG_BYTE(alpha)                        \
    RENDER_PRINTF_CMD_END

// This is a bit hacky, but it matches.
#define RENDER_PRINTF_CMD_SET_POSITION(x, y) \
    u16 tempX, tempY;                        \
    RENDER_PRINTF_CMD_ARG_BYTE(0x82)         \
    RENDER_PRINTF_CMD_ARG_BYTE(x & 0xFF)     \
    tempX = x >> 8;                          \
    RENDER_PRINTF_CMD_ARG_BYTE(tempX)        \
    RENDER_PRINTF_CMD_ARG_BYTE(y & 0xFF)     \
    tempY = y >> 8;                          \
    RENDER_PRINTF_CMD_ARG_BYTE(tempY)        \
    RENDER_PRINTF_CMD_END
    
#define RENDER_PRINTF_CMD_SET_BACKGROUND_COLOR(red, green, blue, alpha) \
    RENDER_PRINTF_CMD_ARG_BYTE(0x85)                                    \
    RENDER_PRINTF_CMD_ARG_BYTE(red)                                     \
    RENDER_PRINTF_CMD_ARG_BYTE(green)                                   \
    RENDER_PRINTF_CMD_ARG_BYTE(blue)                                    \
    RENDER_PRINTF_CMD_ARG_BYTE(alpha)                                   \
    RENDER_PRINTF_CMD_END


void *memset(void *s, int c, size_t n);
char *_itoa(unsigned long long int n, char *buflim, unsigned int base, int upper_case);
void sprintfSetSpacingCodes(s32 setting);
int vsprintf(char *s, const char *fmt, va_list args);
void diPrintfInit(void);
s32 diPrintf(const char *format, ...);
void diPrintfAll(Gfx **dList);
void diPrintfSetCol(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfSetXY(u16 x, u16 y);
s32 debug_text_parse(Gfx**, char*);
void debug_text_background(Gfx **dList, u32 ulx, u32 uly, u32 lrx, u32 lry);
s32 debug_text_character(Gfx **dList, s32 asciiVal);
void debug_text_bounds(void);
void debug_text_origin(void);
void debug_text_newline(void);

#endif
