#include "common.h"
#include "stdarg.h"
#include "libc/string.h"

#define RENDER_PRINTF_CMD_ARG_BYTE(val) \
    *gDebugPrintBufferEnd = val;                  \
    gDebugPrintBufferEnd++;
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

#define outchar(x)  \
    do {            \
        done++;     \
        (*s++) = x; \
    } while (0)

#define PAD(x)            \
    while (width-- > 0) { \
        outchar(x);       \
    }

#define HAVE_LONGLONG 1

#define PTR void *

/* Cast the next arg, of type ARGTYPE, into CASTTYPE, and put it in VAR.  */
#define castarg(var, argtype, casttype) var = (casttype) va_arg(args, argtype)

/* Get the next arg, of type TYPE, and put it in VAR.  */
#define nextarg(var, type) castarg(var, type, type)

#define BUFSIZ 100

#define isdigit(c) ((c >= '0') && (c <= '9'))

// Returns the total number of characters written.
int vsprintf(char *s, const char *fmt, va_list args) {
    /* Pointer into the format string.  */
    const char *f;

    /* The string describing the size of groups of digits.  */
    const char *grouping;

    /* Number of characters written.  */
    int done = 0;

    f = fmt;
    while (*f != '\0') {
        /* Type modifiers.  */
        char is_short, is_long, is_long_double;

#ifdef HAVE_LONGLONG
        /* We use the `L' modifier for `long long int'.  */
#define is_longlong is_long_double
#else
#define is_longlong 0
#endif

        /* Format spec modifiers.  */
        char space, showsign, left, alt;

        /* Padding character: ' ' or '0'.  */
        char pad;
        /* Width of a field.  */
        int width;

        /* Precision of a field.  */
        int prec;

        /* Decimal integer is negative.  */
        char is_neg;

        /* Current character of the format.  */
        char fc;

        /* Base of a number to be written.  */
        int base;

        /* Integral values to be written.  */
        u64 num;
        s64 signed_num;

        /* String to be written.  */
        const char *str;

        char work[BUFSIZ];

        s32 a1;
        s32 i;
        s32 unused2;
        s32 v1;
        s32 a0;
        s32 digit;

        if (*f != '%') {
            /*   This isn't a format spec, so write everything out until the
                 next one.  To properly handle multibyte characters, we cannot
                 just search for a '%'.  Since multibyte characters are hairy
                 (and dealt with above), if we hit any byte above 127 (only
                 those can start a multibyte character) we just punt back to
                 that code.  */
            while (*f != '%' && *f != '\0') {
                outchar(*f++);
            }
            continue;
        }

        ++f;

        /* Check for "%%".  Note that although the ANSI standard lists
            '%' as a conversion specifier, it says "The complete format
            specification shall be `%%'," so we can avoid all the width
            and precision processing.  */
        if (*f == '%') {
            ++f;
            outchar('%');
            continue;
        }

        /* Check for spec modifiers.  */
        space = showsign = left = alt = 0;
        pad = ' ';
        while (*f == ' ' || *f == '+' || *f == '-' || *f == '#' || *f == '0') {
            switch (*f++) {
                case ' ':
                    /* Output a space in place of a sign, when there is no sign.  */
                    space = 1;
                    break;
                case '+':
                    /* Always output + or - for numbers.  */
                    showsign = 1;
                    break;
                case '-':
                    /* Left-justify things.  */
                    left = 1;
                    break;
                case '#':
                    /* Use the "alternate form":
                    Hex has 0x or 0X, FP always has a decimal point.  */
                    alt = 1;
                    break;
                case '0':
                    /* Pad with 0s.  */
                    pad = '0';
                    break;
            }
        }
        // end of while loop

        if (left) {
            pad = ' ';
        }

        /* Get the field width.  */
        width = 0;
        if (*f == '*') {
            /* The field width is given in an argument.
               A negative field width indicates left justification.  */
            nextarg(width, int);
            if (width < 0) {
                width = -width;
                left = 1;
            }
            ++f;
        } else {
            while (isdigit(*f)) {
                width *= 10;
                width += *f++ - '0';
            }
        }

        /* Get the precision.  */
        /* -1 means none given; 0 means explicit 0.  */
        prec = -1;
        if (*f == '.') {
            ++f;
            if (*f == '*') {
                /* The precision is given in an argument.  */
                nextarg(prec, int);
                /* Avoid idiocy.  */
                if (prec < 0) {
                    prec = -1;
                }
                ++f;
            } else if (isdigit(*f)) {
                prec = 0;
                while (isdigit(*f)) {
                    prec *= 10;
                    prec += *f++ - '0';
                }
            }
        }

        /* Check for type modifiers.  */
        is_short = is_long = is_long_double = 0;
        while (*f == 'h' || *f == 'l' || *f == 'L' || *f == 'Z' || *f == 'q') {
            switch (*f++) {
                case 'h':
                    /* int's are short int's.  */
                    is_short = 1;
                    break;
                case 'l':
#ifdef HAVE_LONGLONG
                    if (is_long) {
                        /* A double `l' is equivalent to an `L'.  */
                        is_longlong = 1;
                    } else {
#endif
                        /* int's are long int's.  */
                        is_long = 1;
                    }
                    break;
                case 'L':
                    /* double's are long double's, and int's are long long int's.  */
                    is_long_double = 1;
                    break;
                case 'Z':
                    /* int's are size_t's.  */
#ifdef HAVE_LONGLONG
                    // assert (sizeof(size_t) <= sizeof(unsigned long long int));
                    // is_longlong = sizeof(size_t) > sizeof(unsigned long int);
#endif
                    is_long = TRUE; // sizeof(size_t) > sizeof(unsigned int);
                    break;
                case 'q':
                    is_longlong = 1;
                    break;
            }
        }

        /* Format specification.  */
        fc = *f++;
        switch (fc) {
            case 'i':
            case 'd':
                /* Decimal integer.  */
                base = 10;
                if (is_longlong) {
                    nextarg(signed_num, s64);
                } else if (is_long) {
                    nextarg(signed_num, long int);
                } else if (!is_short) {
                    castarg(signed_num, int, long int);
                } else {
                    castarg(signed_num, int, short int);
                }

                is_neg = signed_num < 0;
                num = is_neg ? (-signed_num) : signed_num;
                goto number;
            case 'u':
                /* Decimal unsigned integer.  */
                base = 10;
                goto unsigned_number;
            case 'o':
                /* Octal unsigned integer.  */
                base = 8;
                goto unsigned_number;
            case 'X':
                /* Hexadecimal unsigned integer.  */
                base = 16;
                goto unsigned_number;
            case 'x':
                /* Hex with lower-case digits.  */
                base = 16;

            unsigned_number:
                /* Unsigned number of base BASE.  */
                if (is_longlong) {
                    castarg(num, s64, u64);
                } else if (is_long) {
                    castarg(num, long int, unsigned long int);
                } else if (!is_short) {
                    castarg(num, int, unsigned int);
                } else {
                    castarg(num, int, unsigned short int);
                }
                /* ANSI only specifies the `+' and
                   ` ' flags for signed conversions.  */
                is_neg = showsign = space = 0;

            number:
                /* Number of base BASE.  */
                {
                    char *w;
                    char *workend = &work[sizeof(work) - 1];

                    if (D_800A6D40) {
                        outchar(0x84);
                    }
                    if (prec >= 0) {
                        pad = ' ';
                    }
                    /* Supply a default precision if none was given.  */
                    if (prec == -1) {
                        prec = 1;
                    }

                    /* Put the number in WORK.  */
                    w = _itoa(num, workend + 1, base, fc == 'X') - 1;
                    width -= workend - w;
                    prec -= workend - w;

                    if (alt && base == 8 && prec <= 0) {
                        *w-- = '0';
                        width--;
                    }

                    if (prec > 0) {
                        width -= prec;
                        while (prec-- > 0) {
                            *w-- = '0';
                        }
                    }

                    if (alt && base == 16) {
                        width -= 2;
                    }

                    if (is_neg || showsign || space) {
                        width--;
                    }

                    if (!left && pad == ' ') {
                        PAD(' ');
                    }

                    if (is_neg) {
                        outchar('-');
                    } else if (showsign) {
                        outchar('+');
                    } else if (space) {
                        outchar(' ');
                    }

                    if (alt && base == 16) {
                        outchar('0');
                        outchar(fc);
                    }

                    if (!left && pad == '0') {
                        PAD('0');
                    }

                    /* Write the number.  */
                    while (++w <= workend) {
                        outchar(*w);
                    }

                    if (left) {
                        PAD(' ');
                    }
                }
                break;

            case 'e':
            case 'E': {
                s32 showDash;
                s32 unused2;
                f32 f16;
                f32 f02;
                f32 spD0;
                f32 f0;
                s32 exponent;
                s32 unused;

                showDash = FALSE;
                if (D_800A6D40) {
                    outchar(0x84);
                }

                if (prec < 0) {
                    prec = 6;
                }

                if (is_short) {
                    f32 *ptr;
                    castarg(ptr, f32 *, f32 *);
                    spD0 = *ptr;
                } else {
                    f32 *ptr;
                    nextarg(ptr, f32 *);
                    spD0 = *ptr;
                }

                if (*((s8 *) &spD0) < 0) {
                    showDash = TRUE;
                    spD0 = -spD0;
                }

                if (spD0 == 0.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                } else if (spD0 < 1.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                    while (spD0 < f16) {
                        f16 /= 10.0f;
                        exponent--;
                    }
                }

                if (spD0 >= 1.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                    f0 = 10.0f;
                    while (f0 <= spD0) {
                        f16 = f0;
                        f0 *= 10.0f;
                        exponent++;
                    }
                }

                f02 = f16 * 0.5f;

                for (digit = prec; digit > 0; digit--) {
                    f02 /= 10.0f;
                }

                spD0 += f02;
                if (spD0 >= f16 * 10.0f) {
                    f16 = f16 * 10.0f;
                    exponent++;
                }

                a1 = (showDash || showsign || space) + prec + (prec > 0 || alt) + (exponent >= 100) + 5;

                if (!left && pad == ' ') {
                    while (width-- > a1) {
                        outchar(pad);
                    }
                }

                if (showDash) {
                    outchar('-');
                } else if (showsign) {
                    outchar('+');
                } else if (space) {
                    outchar(' ');
                }

                if (!left && pad == '0') {
                    while (width-- > a1) {
                        outchar(pad);
                    }
                }

                digit = '0';
                while (spD0 >= f16) {
                    spD0 -= f16;
                    digit++;
                }
                outchar(digit);
                f16 /= 10.0f;

                if (prec > 0 || alt) {
                    outchar('.');
                }

                while (prec > 0) {
                    digit = '0';
                    while (spD0 >= f16) {
                        spD0 -= f16;
                        digit++;
                    }
                    outchar(digit);
                    f16 /= 10.0f;
                    prec--;
                }

                outchar(fc);

                if (exponent < 0) {
                    exponent = -exponent;
                    outchar('-');
                } else {
                    outchar('+');
                }

                if (exponent >= 100) {
                    outchar('0' + (exponent / 100));
                }

                outchar('0' + ((exponent / 10) % 10));
                outchar('0' + (exponent % 10));

                if (left) {
                    while (width-- > a1) {
                        outchar(' ');
                    }
                }
                break;
            }
            case 'G':
            case 'g':
                break;
            case 'f': {
                f32 f12 = 1.0f;
                f32 f14;
                f32 f2;
                s32 length;
                s32 showDash;
                f32 *ptr;
                s32 i;
                f32 spD0;

                showDash = FALSE;

                if (D_800A6D40) {
                    outchar(0x84);
                }
                if (prec < 0) {
                    prec = 6;
                }

                for (digit = 0; digit < prec; digit++) {
                    f12 /= 10.0f;
                }

                if (is_short) {
                    castarg(ptr, f32 *, f32 *);
                    spD0 = *ptr;
                } else {
                    nextarg(ptr, f32 *);
                    spD0 = *ptr;
                }

                if (spD0 < 0.0f) {
                    showDash = TRUE;
                    spD0 = -spD0;
                }

                spD0 += f12 * 0.5f;

                digit = 1;
                f2 = 1.0f;
                f14 = 10.0f;
                while (spD0 >= f14) {
                    f2 = f14;
                    f14 *= 10.0f;
                    digit++;
                }

                length = (showDash || showsign || space) + (prec > 0 || alt) + digit + prec;
                if (!left && pad == ' ') {
                    while (width-- > length) {
                        outchar(pad);
                    }
                }

                if (showDash) {
                    outchar('-');
                } else if (showsign) {
                    outchar('+');
                } else if (space) {
                    outchar(' ');
                }

                if (!left && pad == '0') {
                    while (width-- > length) {
                        outchar(pad);
                    }
                }

                do {
                    digit = '0';
                    while (spD0 >= f2) {
                        spD0 -= f2;
                        digit++;
                    }
                    f2 /= 10.0f;
                    outchar(digit);
                } while (f2 >= 1.0f);

                if (prec > 0 || alt) {
                    outchar('.');
                }

                while (prec > 0) {
                    digit = '0';
                    while (spD0 >= f2) {
                        spD0 -= f2;
                        digit++;
                    }
                    outchar(digit);
                    f2 /= 10.0f;
                    prec--;
                }

                if (left) {
                    while (width-- > length) {
                        outchar(' ');
                    }
                }
                break;
            }
            case 'c':
                /* Character.  */
                nextarg(num, int);
                if (!left) {
                    while (--width > 0) {
                        outchar(pad);
                    }
                }
                outchar((unsigned char) num);
                if (left) {
                    while (--width > 0) {
                        outchar(' ');
                    }
                }
                break;

            case 's': {
                static const char null[] = "(null)";
                s32 len;

                nextarg(str, char *);
                if (str == NULL) {
                    /* Write "(null)" if there's space.  */
                    if (prec == -1 || prec >= (int) sizeof(null) - 1) {
                        str = null;
                        len = sizeof(null) - 1;
                    } else {
                        str = "";
                        len = 0;
                    }
                } else {
                    len = strlen(str);
                }

                if (prec != -1 && prec < len) {
                    len = prec;
                }
                width -= len;

                if (!left) {
                    PAD(' ');
                }
                while (len-- > 0) {
                    outchar(*str++);
                }

                if (left) {
                    PAD(' ');
                }
            } break;

            case 'p':
                /* Generic pointer.  */
                {
                    PTR ptr;
                    nextarg(ptr, PTR);
                    if (ptr != NULL) {
                        /* If the pointer is not NULL, write it as a %#x spec.  */
                        base = 16;
                        fc = 'x';
                        alt = 1;
                        num = (unsigned long int) ptr;
                        is_neg = 0;
                        goto number;
                    } else {
                        /* Write "(nil)" for a nil pointer.  */
                        static const char nil[] = "(nil)";
                        register const char *p;

                        width -= sizeof(nil) - 1;
                        if (!left) {
                            PAD(' ');
                        }
                        grouping = nil;
                        while (*grouping != '\0') {
                            outchar(*grouping++);
                        }
                        if (left) {
                            PAD(' ');
                        }
                    }
                }
                break;

            case 'n':
                /* Answer the count of characters written.  */
                if (is_longlong) {
                    s64 *p;
                    nextarg(p, s64 *);
                    *p = done;
                } else if (is_long) {
                    long int *p;
                    nextarg(p, long int *);
                    *p = done;
                } else if (!is_short) {
                    int *p;
                    nextarg(p, int *);
                    *p = done;
                } else {
                    short int *p;
                    nextarg(p, short int *);
                    *p = done;
                }
                break;
            default:
                /* Unrecognized format specifier.  */
                break;
        }
        if (D_800A6D40) {
            outchar(0x83);
        }
    }
    *s = '\0';
    return done;
}

void diPrintfInit(void) {
    gTexture[0] = texLoadTexture(0);
    gTexture[1] = texLoadTexture(1);
    gTexture[2] = texLoadTexture(2);
    gDebugPrintBufferEnd = gDebugPrintBufferStart;
}

s32 diPrintf(const char *format, ...) {
    va_list args;
    s32 written;
    va_start(args, format);
    if ((gDebugPrintBufferEnd - gDebugPrintBufferStart) > 0x800) {
        stubbed_printf("*** diPrintf Error *** ---> Out of string space. (Print less text!)\n");
        return -1;
    }
    sprintfSetSpacingCodes(1);
    written = vsprintf(gDebugPrintBufferEnd, format, args);
    sprintfSetSpacingCodes(0);
    if (written > 0) {
        gDebugPrintBufferEnd = &gDebugPrintBufferEnd[written] + 1;
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
    gDebugScreenHeight = height;
    gDebugScreenWidth = width;
    gDPSetScissor((*dList)++, 0, 0, 0, gDebugScreenHeight, gDebugScreenWidth);
    debug_text_bounds();
    gSPDisplayList((*dList)++, dDebugFontSettings);
    buffer = (char *) gDebugPrintBufferStart;
    debug_text_origin();
    gDebugFontTexture = -1;
    gDebugFixedWidthMode = 0;
    D_80101F50 = gDebugTextX;
    D_80101F52 = gDebugTextY;
    while ((s32) buffer != (s32) gDebugPrintBufferEnd) {
        gDebugTextOn = FALSE;
        buffer += debug_text_parse(dList, buffer);
    }
    debug_text_background(dList, D_80101F50, D_80101F52, gDebugTextX, gDebugTextY + 10);
    buffer = (char *) gDebugPrintBufferStart;
    debug_text_origin();
    gDebugFontTexture = -1;
    gDebugFixedWidthMode = 0;
    while ((s32) buffer != (s32) gDebugPrintBufferEnd) {
        gDebugTextOn = TRUE;
        buffer += debug_text_parse(dList, buffer);
    }
    gDebugPrintBufferEnd = gDebugPrintBufferStart;
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

/**
 * Read the current character in the debug text buffer.
 * Also executes commands when they come up.
 */
s32 debug_text_parse(Gfx **dList, char *buffer) {
    char *bufferCopy;
    s32 xOffset;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 bufferValue;

    bufferCopy = buffer;
    bufferValue = *buffer;
    buffer++;
    while (bufferValue != NULL) {
        xOffset = 0;
        switch (bufferValue) {
            case 0x83: // Leave fixed-width mode
                gDebugFixedWidthMode = FALSE;
                break;
            case 0x84: // Enter fixed-width mode
                gDebugFixedWidthMode = TRUE;
                break;
            case 0x81: // Set the text color from the next 4 bytes
                red = buffer[0];
                green = buffer[1];
                blue = buffer[2];
                alpha = buffer[3];
                buffer += 4;
                if (gDebugTextOn) {
                    gDPSetEnvColor((*dList)++, red, green, blue, alpha);
                }
                break;
            case 0x85: // Set the background color from the next 4 bytes
                red = buffer[0];
                green = buffer[1];
                blue = buffer[2];
                alpha = buffer[3];
                buffer += 4;
                if (!gDebugTextOn) {
                    gDPSetPrimColor((*dList)++, 0, 0, red, green, blue, alpha);
                }
                break;
            case 0x82: // Set debug text position from the next 4 bytes
                if (!gDebugTextOn) {
                    debug_text_background(dList, D_80101F50, D_80101F52, gDebugTextX, gDebugTextY + 10);
                }
                gDebugTextX = buffer[0];
                gDebugTextX |= buffer[1] << 8;
                gDebugTextY = buffer[2];
                gDebugTextY |= buffer[3] << 8;
                D_80101F50 = gDebugTextX;
                D_80101F52 = gDebugTextY;
                buffer += 4;
                break;
            case ' ': // Space
                xOffset = 6;
                break;
            case '\n': // Line Feed
                if (!gDebugTextOn) {
                    debug_text_background(dList, D_80101F50, D_80101F52, gDebugTextX, gDebugTextY + 10);
                }
                debug_text_newline();
                D_80101F50 = gDebugTextX;
                D_80101F52 = gDebugTextY;
                break;
            case '\t': // HT - Horizontal Tab
                if (!(gDebugTextX % 32)) {
                    xOffset = 32;
                } else {
                    xOffset = 32 - (gDebugTextX % 32);
                }
                break;
            default:
                xOffset = debug_text_character(dList, bufferValue);
                break;
        }

        if (gDebugFixedWidthMode && bufferValue >= 0x20 && bufferValue < 0x80) {
            xOffset = 7;
        }
        gDebugTextX += xOffset;
        if ((gDebugScreenHeight - 16) < gDebugTextX) {
            if (!gDebugTextOn) {
                debug_text_background(dList, D_80101F50, D_80101F52, gDebugTextX, gDebugTextY + 10);
            }
            debug_text_newline();
            D_80101F50 = gDebugTextX;
            D_80101F52 = gDebugTextY;
        }
        bufferValue = *buffer;
        buffer++;
    }

    return buffer - bufferCopy;
}

void debug_text_background(Gfx **dList, u32 ulx, u32 uly, u32 lrx, u32 lry) {
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
s32 debug_text_character(Gfx **dList, s32 asciiVal) {
    s32 fontCharWidth;
    s32 fontCharU;

    if (asciiVal < 0x40) {
        // Character is a symbol or number and not a letter
        if (gDebugFontTexture != 0) {
            if (gDebugTextOn) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(gTexture[0] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            gDebugFontTexture = 0;
        }
        asciiVal -= 0x21;
    } else if (asciiVal < 0x60) {
        // Character is a upper case letter
        if (gDebugFontTexture != 1) {
            if (gDebugTextOn) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(gTexture[1] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 248, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            gDebugFontTexture = 1;
        }
        asciiVal -= 0x40;
    } else if (asciiVal < 0x80) {
        // Character is a lower case letter
        if (gDebugFontTexture != 2) {
            if (gDebugTextOn) {
                gDPLoadTextureBlockS((*dList)++, OS_PHYSICAL_TO_K0(gTexture[2] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                     0, 2, 2, 0, 0, 0, 0);
            }
            gDebugFontTexture = 2;
        }
        asciiVal -= 0x60;
    }
    fontCharU = gDebugFontCoords[gDebugFontTexture][asciiVal].u;
    fontCharWidth = (gDebugFontCoords[gDebugFontTexture][asciiVal].v - fontCharU) + 1;
    if (gDebugTextOn) {
        gDPSetCombineMode((*dList)++, DKR_CC_UNK14, DKR_CC_UNK14);
        gSPTextureRectangle((*dList)++, (gDebugTextX << 2), (gDebugTextY << 2), ((gDebugTextX + fontCharWidth) << 2),
                            ((gDebugTextY + 10) << 2), 0, (fontCharU << 5), 0, 1024, 1024);
    }
    return fontCharWidth;
}

// debug_text_bounds in DKR
void debug_text_bounds(void) {
    if (gDebugScreenHeight <= 320) {
        gDebugBoundsX1 = 16;
        gDebugBoundsX2 = gDebugScreenHeight - 16;
    } else {
        gDebugBoundsX1 = 32;
        gDebugBoundsX2 = gDebugScreenHeight - 32;
    }
    if (gDebugScreenWidth <= 240) {
        gDebugBoundsY1 = 16;
        gDebugBoundsY2 = gDebugScreenWidth - 16;
    } else {
        gDebugBoundsY1 = 32;
        gDebugBoundsY2 = gDebugScreenWidth - 32;
    }
}

// debug_text_origin in DKR
void debug_text_origin(void) {
    gDebugTextX = gDebugBoundsX1;
    gDebugTextY = gDebugBoundsY1;
}

// debug_text_newline in DKR
void debug_text_newline(void) {
    gDebugTextX = gDebugBoundsX1;
    gDebugTextY += 11;
}
