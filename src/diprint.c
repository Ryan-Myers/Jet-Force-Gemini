#include "common.h"
#include "stdarg.h"

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/memset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/_itoa.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/sprintfSetSpacingCodes.s")

void sprintf(char *s, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(s, format, args);
    va_end(args);
}

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/vsprintf.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/diPrintfInit.s")

s32 diPrintf(const char *format, ...) {
    va_list args;
    s32 written;
    va_start(args, format);
    if ((D_800A6D44_A7944 - D_80101640_102240) > 0x800) {
        return -1;
    }
    sprintfSetSpacingCodes(1);
    written = vsprintf(D_800A6D44_A7944, format, args);
    sprintfSetSpacingCodes(0);
    if (written > 0) {
        D_800A6D44_A7944 = &D_800A6D44_A7944[written] + 1;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/diPrintfAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/diPrintfSetCol.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/diPrintfSetBG.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diprint/diPrintfSetXY.s")

//Same as func_800B653C in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/diprint/func_80065CB4_668B4.s")

void func_800660D4_66CD4(Gfx **dList, u32 ulx, u32 uly, u32 lrx, u32 lry) {
    if (!((ulx == lrx) | (uly == lry))) {
        if (ulx >= 2) {
            ulx -= 2;
        }
        lrx += 2;
        gDPSetCombineMode((*dList)++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPFillRectangle((*dList)++, ulx, uly, lrx, lry);
    }
}

//Same as func_800B69FC in DKR
#ifdef NON_EQUIVALENT
//CALC_DXT needs to return zero in gDPLoadTextureBlock.
//Needs WIDTH adjustment in load texture block
//Loads a font texture and returns the width of the character given.
s32 func_80066174_66D74(Gfx **dList, s32 asciiVal) {
    s32 fontCharWidth;
    s32 fontCharU;

    if (asciiVal < 0x40) {
        //Character is a symbol or number and not a letter
        if (D_80101F6C_102B6C != 0) {
            if (D_80101F58_102B58) {
                gDPLoadTextureBlock((*dList)++, OS_PHYSICAL_TO_K0(D_80101F40_102B40 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11, 0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C_102B6C = 0;
        }
        asciiVal -= 0x21;
    } else if (asciiVal < 0x60) {
        //Character is a upper case letter
        if (D_80101F6C_102B6C != 1) {
            if (D_80101F58_102B58) {
                gDPLoadTextureBlock((*dList)++, OS_PHYSICAL_TO_K0(D_80101F44_102B44 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 248, 11, 0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C_102B6C = 1;
        }
        asciiVal -= 0x40;
    } else if (asciiVal < 0x80) {
        //Character is a lower case letter
        if (D_80101F6C_102B6C != 2) {
            if (D_80101F58_102B58) {
                gDPLoadTextureBlock((*dList)++, OS_PHYSICAL_TO_K0(D_80101F48_102B48 + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11, 0, 2, 2, 0, 0, 0, 0);
            }
            D_80101F6C_102B6C = 2;
        }
        asciiVal -= 0x60;
    }
    fontCharU = D_800A6D48_A7948[D_80101F6C_102B6C][asciiVal].u;
    fontCharWidth = (D_800A6D48_A7948[D_80101F6C_102B6C][asciiVal].v - fontCharU) + 1;
    if (D_80101F58_102B58) {
        gDPSetCombineMode((*dList)++, DKR_CC_UNK12, DKR_CC_UNK12);
        gSPTextureRectangle((*dList)++, (D_80101F4C_102B4C << 2), (D_80101F4E_102B4E << 2), ((D_80101F4C_102B4C + fontCharWidth) << 2), ((D_80101F4E_102B4E + 10) << 2), 0, (fontCharU << 5), 0, 1024, 1024);
    }
    return fontCharWidth;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/diprint/func_80066174_66D74.s")
#endif

void func_800665C8_671C8(void) {
    if (D_80101F70_102B70 <= 320) {
        D_80101F5C_102B5C = 16;
        D_80101F60_102B60 = D_80101F70_102B70 - 16;
    } else {
        D_80101F5C_102B5C = 32;
        D_80101F60_102B60 = D_80101F70_102B70 - 32;
    }
    if (D_80101F72_102B72 <= 240) {
        D_80101F64_102B64 = 16;
        D_80101F68_102B68 = D_80101F72_102B72 - 16;
        return;
    }
    D_80101F64_102B64 = 32;
    D_80101F68_102B68 = D_80101F72_102B72 - 32;
}

void func_80066658_67258(void) {
    D_80101F4C_102B4C = D_80101F5C_102B5C;
    D_80101F4E_102B4E = D_80101F64_102B64;
}

void func_8006667C_6727C(void) {
    D_80101F4C_102B4C = D_80101F5C_102B5C;
    D_80101F4E_102B4E += 11;
}
