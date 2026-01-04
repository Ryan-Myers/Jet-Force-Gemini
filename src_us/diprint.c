#include "common.h"
#include "stdarg.h"

#define RENDER_PRINTF_CMD_ARG_BYTE(val) \
    *D_800A6D44 = val;                  \
    D_800A6D44++;
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

/**
 * memset(void *s, int c, size_t n)
 *
 * s: start of area to clear
 * c: char to fill with
 * n: size of area to clear
 */
void *memset(void *s, int c, size_t n) {
    u8 *var_v0 = s;

    while (n-- > 0) {
        *var_v0++ = c;
    }
    return s;
}

const char _itoa_lower_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
const char _itoa_upper_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * Standard C Library function that converts integers to strings.
 * Exact match to glibc 1.09 version.
 */
char *_itoa(unsigned long long int n, char *buflim, unsigned int base, int upper_case) {
    /* Base-36 digits for numbers. */
    const char *alphabet = upper_case ? _itoa_upper_digits : _itoa_lower_digits;
    register char *bp = buflim;

    while (n > 0) {
        *(--bp) = alphabet[n % base];
        n /= base;
    }

    return bp;
}

void sprintfSetSpacingCodes(s32 arg0) {
    D_800A6D40 = arg0;
}

UNUSED int sprintf(char *s, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(s, fmt, args);
    va_end(args);
}

const char D_800AE450[] = "";
const char D_800AE454[] = "(null)";
const char D_800AE45C[] = "(nil)";

#pragma GLOBAL_ASM("asm_us/nonmatchings/diprint/vsprintf.s")

void diPrintfInit(void) {
    D_80101F40 = texLoadTexture(0);
    D_80101F44 = texLoadTexture(1);
    D_80101F48 = texLoadTexture(2);
    D_800A6D44 = D_80101640;
}

s32 diPrintf(const char *format, ...) {
    va_list args;
    s32 written;
    va_start(args, format);
    if ((D_800A6D44 - D_80101640) > 0x800) {
        stubbed_printf("*** diPrintf Error *** ---> Out of string space. (Print less text!)\n");
        return -1;
    }
    sprintfSetSpacingCodes(1);
    written = vsprintf(D_800A6D44, format, args);
    sprintfSetSpacingCodes(0);
    if (written > 0) {
        D_800A6D44 = &D_800A6D44[written] + 1;
    }
    return 0;
}

/**
 * At the end of a frame, iterate through the debug text buffer and print it on screen.
 * Soft-clear the buffer afterwards by setting the endpoint to the start point.
 */
void diPrintfAll(Gfx **dList) {
    s32 height;
    s32 width;
    char *buffer;

    rcpInitDp(dList);
    viGetCurrentSize(&height, &width);
    D_80101F70 = height;
    D_80101F72 = width;
    gDPSetScissor((*dList)++, 0, 0, 0, D_80101F70, D_80101F72);
    func_800665C8();
    gSPDisplayList((*dList)++, D_800A6E08);
    buffer = (char *) D_80101640;
    func_80066658();
    D_80101F6C = -1;
    D_80101F54 = 0;
    D_80101F50 = D_80101F4C;
    D_80101F52 = D_80101F4E;
    while ((s32) buffer != (s32) D_800A6D44) {
        D_80101F58 = FALSE;
        buffer += func_80065CB4(dList, buffer);
    }
    func_800660D4(dList, D_80101F50, D_80101F52, D_80101F4C, D_80101F4E + 10);
    buffer = (char *) D_80101640;
    func_80066658();
    D_80101F6C = -1;
    D_80101F54 = 0;
    while ((s32) buffer != (s32) D_800A6D44) {
        D_80101F58 = TRUE;
        buffer += func_80065CB4(dList, buffer);
    }
    D_800A6D44 = D_80101640;
}

/**
 * Set the colour of the current debug text.
 */
UNUSED void diPrintfSetCol(u8 red, u8 green, u8 blue, u8 alpha) {
    RENDER_PRINTF_CMD_SET_COLOR(red, green, blue, alpha)
}

/**
 * Set the background colour of the current debug text.
 */
void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha) {
    RENDER_PRINTF_CMD_SET_BACKGROUND_COLOR(red, green, blue, alpha)
}

/**
 * Sets the character position of further prints to the given coordinates.
 */
void diPrintfSetXY(u16 x, u16 y) {
    RENDER_PRINTF_CMD_SET_POSITION(x, y)
}

// Same as debug_text_parse in DKR
#pragma GLOBAL_ASM("asm_us/nonmatchings/diprint/func_80065CB4.s")

void func_800660D4(Gfx **dList, u32 ulx, u32 uly, u32 lrx, u32 lry) {
    if (!((ulx == lrx) | (uly == lry))) {
        if (ulx >= 2) {
            ulx -= 2;
        }
        lrx += 2;
        gDPSetCombineMode((*dList)++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPFillRectangle((*dList)++, ulx, uly, lrx, lry);
    }
}

// Same as debug_text_character in DKR
// Loads a font texture and returns the width of the character given.
s32 func_80066174(Gfx **dList, s32 asciiVal) {
    s32 fontCharWidth;
    s32 fontCharU;

    if (asciiVal < 0x40) {
        // Character is a symbol or number and not a letter
        if (D_80101F6C != 0) {
            if (D_80101F58) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(D_80101F40 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C = 0;
        }
        asciiVal -= 0x21;
    } else if (asciiVal < 0x60) {
        // Character is a upper case letter
        if (D_80101F6C != 1) {
            if (D_80101F58) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(D_80101F44 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 248, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C = 1;
        }
        asciiVal -= 0x40;
    } else if (asciiVal < 0x80) {
        // Character is a lower case letter
        if (D_80101F6C != 2) {
            if (D_80101F58) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(D_80101F48 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C = 2;
        }
        asciiVal -= 0x60;
    }
    fontCharU = D_800A6D48[D_80101F6C][asciiVal].u;
    fontCharWidth = (D_800A6D48[D_80101F6C][asciiVal].v - fontCharU) + 1;
    if (D_80101F58) {
        gDPSetCombineMode((*dList)++, DKR_CC_UNK14, DKR_CC_UNK14);
        gSPTextureRectangle((*dList)++, (D_80101F4C << 2), (D_80101F4E << 2), ((D_80101F4C + fontCharWidth) << 2),
                            ((D_80101F4E + 10) << 2), 0, (fontCharU << 5), 0, 1024, 1024);
    }
    return fontCharWidth;
}

// debug_text_bounds in DKR
void func_800665C8(void) {
    if (D_80101F70 <= 320) {
        D_80101F5C = 16;
        D_80101F60 = D_80101F70 - 16;
    } else {
        D_80101F5C = 32;
        D_80101F60 = D_80101F70 - 32;
    }
    if (D_80101F72 <= 240) {
        D_80101F64 = 16;
        D_80101F68 = D_80101F72 - 16;
    } else {
        D_80101F64 = 32;
        D_80101F68 = D_80101F72 - 32;
    }
}

// debug_text_origin in DKR
void func_80066658(void) {
    D_80101F4C = D_80101F5C;
    D_80101F4E = D_80101F64;
}

// debug_text_newline in DKR
void func_8006667C(void) {
    D_80101F4C = D_80101F5C;
    D_80101F4E += 11;
}
