#include "common.h"
#include "types.h"

// older G_DMAOFFSETS overlaps with G_TRI1
#define G_DMAOFFSETS (G_IMMFIRST - 0)

// needed for RM_AA_ZB_OPA_SURF
#define GBL_c0(m1a, m1b, m2a, m2b) 0

const char D_800AE6C0[] = ">>>> %x: ";
const char D_800AE6CC[] = "     %x: ";

// forward declarations
static s32 diRcpVertex(Gfx *dList);
static s32 diRcpReserved1(Gfx *dList);
static s32 diRcpMatrix(Gfx *dList);
static s32 diRcpReserved0(Gfx *dList, char *arg1);
static s32 diRcpReserved2(Gfx *dList);
static s32 diRcpMoveMem(Gfx *dList);
static s32 diRcpDisplayList(Gfx *dList);
static s32 diRcpStrNameMacro(Gfx *dList, char *arg1, char *arg2);
static s32 diRcpPrimColor(Gfx *dList);
static s32 diRcpColor(Gfx *dList, char *arg1, char *arg2);
static s32 diRcpDmaOffsets(Gfx *dList, char *arg1);
static s32 diRcpMoveWd(Gfx *dList);
static s32 diRcpStrName(Gfx *dList, char *arg1);
static s32 diRcpOtherMode(Gfx *dList, char *arg1);
static s32 diRcpGeometryMode(Gfx *dList, char *str, char *arg2);

void diRcpPrintDL(Gfx *dList, UNUSED s32 arg1, s32 count) {
    u8 *gListIt;
    s32 i;
    gListIt = (u8 *) dList;
    for (i = 0; i < count; i++) {
        switch (gListIt[0]) {
            case G_VTX:
                gListIt += diRcpVertex((Gfx *) gListIt);
                break;
            case G_RESERVED1:
                gListIt += diRcpReserved1((Gfx *) gListIt);
                break;
            case G_SPNOOP:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SPNOOP");
                break;
            case G_MTX:
                gListIt += diRcpMatrix((Gfx *) gListIt);
                break;
            case G_RESERVED0:
                gListIt += diRcpReserved0((Gfx *) gListIt, "G_TEXDMA");
                break;
            case G_MOVEMEM:
                gListIt += diRcpMoveMem((Gfx *) gListIt);
                break;
            case G_DL:
                gListIt += diRcpDisplayList((Gfx *) gListIt);
                break;
            case G_RESERVED2:
                gListIt += diRcpReserved2((Gfx *) gListIt);
                break;
            case G_RESERVED3:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_RESERVED3");
                break;
            case G_RESERVED4:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_RESERVED4");
                break;
            case G_NOOP:
                gListIt += diRcpStrNameMacro((Gfx *) gListIt, "G_NOOP", "gDPNoOp");
                break;
            case G_SETCIMG:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETCIMG");
                break;
            case G_SETZIMG:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETZIMG");
                break;
            case G_SETTIMG:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETTIMG");
                break;
            case G_SETCOMBINE:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETCOMBINE");
                break;
            case G_SETENVCOLOR:
                gListIt += diRcpColor((Gfx *) gListIt, "G_SETENVCOLOR", "gDPSetEnvColor");
                break;
            case G_SETPRIMCOLOR:
                gListIt += diRcpPrimColor((Gfx *) gListIt);
                break;
            case G_SETBLENDCOLOR:
                gListIt += diRcpColor((Gfx *) gListIt, "G_SETBLENDCOLOR", "gDPSetBlendColor");
                break;
            case G_SETFOGCOLOR:
                gListIt += diRcpColor((Gfx *) gListIt, "G_SETFOGCOLOR", "gDPSetFogColor");
                break;
            case G_SETFILLCOLOR:
                gListIt += diRcpColor((Gfx *) gListIt, "G_SETFILLCOLOR", "gDPSetFillColor");
                break;
            case G_FILLRECT:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_FILLRECT");
                break;
            case G_SETTILE:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETTILE");
                break;
            case G_LOADTILE:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_LOADTILE");
                break;
            case G_LOADBLOCK:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_LOADBLOCK");
                break;
            case G_SETTILESIZE:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETTILESIZE");
                break;
            case G_LOADTLUT:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_LOADTLUT");
                break;
            case G_RDPSETOTHERMODE:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_RDPSETOTHERMODE");
                break;
            case G_SETPRIMDEPTH:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETPRIMDEPTH");
                break;
            case G_SETSCISSOR:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETSCISSOR");
                break;
            case G_SETCONVERT:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETCONVERT");
                break;
            case G_SETKEYR:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETKEYR");
                break;
            case G_SETKEYGB:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SETKEYGB");
                break;
            case G_RDPFULLSYNC:
                gListIt += diRcpStrNameMacro((Gfx *) gListIt, "G_RDPFULLSYNC", "gDPFullSync");
                break;
            case G_RDPTILESYNC:
                gListIt += diRcpStrNameMacro((Gfx *) gListIt, "G_RDPTILESYNC", "gDPTileSync");
                break;
            case G_RDPPIPESYNC:
                gListIt += diRcpStrNameMacro((Gfx *) gListIt, "G_RDPPIPESYNC", "gDPPipeSync");
                break;
            case G_RDPLOADSYNC:
                gListIt += diRcpStrNameMacro((Gfx *) gListIt, "G_RDPLoadSYNC", "gDPLoadSync");
                break;
            case G_TEXRECTFLIP:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_TEXRECTFLIP");
                break;
            case G_TEXRECT:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_TEXRECT");
                break;
            case 0xF1: // Right between G_SETTILESIZE and G_LOADTLUT. Custom command for JFG, not in DKR?
            default:
                switch ((s8) gListIt[0]) {
                    case G_DMAOFFSETS:
                        gListIt += diRcpDmaOffsets((Gfx *) gListIt, "G_DMAOFFSETS");
                        break;
                    case G_CULLDL:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_CULLDL");
                        break;
                    case G_POPMTX:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_POPMTX");
                        break;
                    case G_MOVEWORD:
                        gListIt += diRcpMoveWd((Gfx *) gListIt);
                        break;
                    case G_TEXTURE:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_TEXTURE");
                        break;
                    case G_SETOTHERMODE_H:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_SETOTHERMODE_H");
                        break;
                    case G_SETOTHERMODE_L:
                        gListIt += diRcpOtherMode((Gfx *) gListIt, "G_SETOTHERMODE_L");
                        break;
                    case G_ENDDL:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_ENDDL");
                        break;
                    case G_SETGEOMETRYMODE:
                        gListIt += diRcpGeometryMode((Gfx *) gListIt, "G_SETGEOMETRYMODE", "gSPSetGeometryMode");
                        break;
                    case G_CLEARGEOMETRYMODE:
                        gListIt += diRcpGeometryMode((Gfx *) gListIt, "G_CLEARGEOMETRYMODE", "gSPClearGeometryMode");
                        break;
                    case G_LINE3D:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_LINE3D");
                        break;
                    case G_PERSPNORMALIZE:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_PERSPNORMALIZE");
                        break;
                    case G_RDPHALF_1:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_RDPHALF_1");
                        break;
                    case G_RDPHALF_2:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_RDPHALF_2");
                        break;
                    case G_RDPHALF_CONT:
                        gListIt += diRcpStrName((Gfx *) gListIt, "G_RDPHALF_CONT");
                        break;
                    default:
                        gListIt += diRcpStrName((Gfx *) gListIt, "UNKNOWN COMMAND");
                        break;
                }
                break;
        }
    }
}

const char D_800AEA14[] = "%20s: gSPVertex(0x%x, 0x%x, %d, %d);\n";
const char D_800AEA3C[] = "G_VTX";
const char D_800AEA44[] = "%20s: gSPPolygon(0x%x, 0x%x, %d, %d);\n";
const char D_800AEA6C[] = "G_POL";
const char D_800AEA74[] = "GARBAGE";
const char D_800AEA7C[] = "NOMTXMULT";
const char D_800AEA88[] = "MTXMULT";
const char D_800AEA90[] = "%20s: gSPMatrix2(0x%x, 0x%x, %d, %s, %d);\n";
const char D_800AEABC[] = "G_MTX";
const char D_800AEAC4[] = "%20s: gSPLoadTextureOffsets(%d, %08x);\n";
const char D_800AEAEC[] = "%20s: gSPDPBlock(0x%x, 0x%x, %d);\n";
const char D_800AEB10[] = "G_DP_BLOCK";
const char D_800AEB1C[] = "%20s: gSPViewport(0x%x, 0x%x);\n";
const char D_800AEB3C[] = "G_MOVEMEM";
const char D_800AEB48[] = "%20s: gSPDisplayList(0x%x, 0x%x);\n";
const char D_800AEB6C[] = "G_DL";
const char D_800AEB74[] = "%20s: gSPBranchList(0x%x, 0x%x);\n";
const char D_800AEB98[] = "G_DL";
const char D_800AEBA0[] = "%20s: Unknown G_DL command\n";
const char D_800AEBBC[] = "G_DL";
const char D_800AEBC4[] = "%20s: %s(0x%08x);\n";
const char D_800AEBD8[] = "%20s: gDPSetPrimColor(0x%08x, %d, %d, %d, %d, %d, %d);\n";
const char D_800AEC10[] = "G_SETPRIMCOLOR";
const char D_800AEC20[] = "%20s: %s(0x%08x, %2d, %2d, %2d, %2d);\n";
const char D_800AEC48[] = "%20s: gSPSetDMAOffsets(%08x, %08x)\n";
const char D_800AEC6C[] = "G_MW_SEGMENT";
const char D_800AEC7C[] = "G_MW_CLIP";
const char D_800AEC88[] = "G_MW_MTXOFFSET";
const char D_800AEC98[] = "G_MW_POINTS";
const char D_800AECA4[] = "G_MW_SPRITEMODE";
const char D_800AECB4[] = "G_MW_FOG";

static void func_80066594_67194(Gfx *dList, s32 *w0_24_31, s32 *w0_16_23, s32 *w0_0_15, s32 *w1) {
    *w0_24_31 = (dList->words.w0 >> 0x18) & 0xFF;
    *w1 = (dList->words.w0 >> 0x10) & 0xFF;
    *w0_0_15 = dList->words.w0 & 0xFFFF;
    *w0_16_23 = dList->words.w1;
}

static void func_800665D8_671D8(Gfx *dList, s32 *w0_24_31, s32 *w0_8_23, s32 *w0_0_7, s32 *w1) {
    *w0_24_31 = (dList->words.w0 >> 0x18) & 0xFF;
    *w0_8_23 = (dList->words.w0 >> 8) & 0xFFFF;
    *w0_0_7 = dList->words.w0 & 0xFF;
    *w1 = dList->words.w1;
}

static s32 diRcpVertex(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[4];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpReserved1(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[4];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpMatrix(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[8];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpReserved0(Gfx *dList, char *arg1) {
    return 8;
}

static s32 diRcpReserved2(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[2];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpMoveMem(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[2];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpDisplayList(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_16_23;
    s32 w0_0_15;
    s32 w1;
    s32 pad[2];

    func_80066594_67194(dList, &w0_24_31, &w0_16_23, &w0_0_15, &w1);
    return 8;
}

static s32 diRcpStrNameMacro(UNUSED Gfx *dList, UNUSED char *arg1, UNUSED char *arg2) {
    return 8;
}

static s32 diRcpPrimColor(UNUSED Gfx *dList) {
    return 8;
}

static s32 diRcpColor(UNUSED Gfx *dList, UNUSED char *arg1, UNUSED char *arg2) {
    return 8;
}

static s32 diRcpDmaOffsets(Gfx *dList, UNUSED char *arg1) {
    if (dList) {}
    return 8;
}

static s32 diRcpMoveWd(Gfx *dList) {
    s32 w0_24_31;
    s32 w0_0_7;
    s32 w0_8_23;
    s32 w1;
    char str[0x2C];

    func_800665D8_671D8(dList, &w0_24_31, &w0_8_23, &w0_0_7, &w1);
    switch (w0_0_7) {
        case 6:
            break;
        case 4:
            break;
        case 10:
            break;
        case 12:
            break;
        case 2:
            break;
        case 8:
            w0_24_31 = w0_0_7; // fakematch
            break;
        default:
            sprintf(str, "UNKNOWN INDEX %d", w0_0_7);
            break;
    }
    if (w0_24_31 && w0_24_31) {} // fakematch
    return 8;
}

const char D_800AECD4[] = "%20s: %s offset = %d, data = 0x%08x\n";
const char D_800AECFC[] = "G_MOVEWORD";

static s32 diRcpStrName(UNUSED Gfx *dList, char *arg1) {
    char sp20[0x50];
    s32 pad[2];

    sprintf(sp20, "%20s", arg1);
    return 8;
}

const char D_800AED10[] = "%s: 0x%08x  0x%08x\n";

static s32 diRcpOtherMode(Gfx *dList, char *arg1) {
    char sp30[0x50];
    s32 bits;
    s32 w0_8_15;
    s32 w1Lower16;

    w0_8_15 = (dList->words.w0 >> 8) & 0xFF;
    w1Lower16 = dList->words.w1;
    sprintf(sp30, "%20s", arg1);
    switch (w0_8_15) {
        case G_MDSFT_RENDERMODE:
            w1Lower16 &= 0xFFFF;
            if (w1Lower16 != (RM_AA_ZB_OPA_SURF(0)) && w1Lower16 != (RM_RA_ZB_OPA_SURF(0)) &&
                w1Lower16 != (RM_AA_ZB_XLU_SURF(0)) && w1Lower16 != (RM_AA_ZB_OPA_DECAL(0)) &&
                w1Lower16 != (RM_RA_ZB_OPA_DECAL(0)) && w1Lower16 != (RM_AA_ZB_XLU_DECAL(0)) &&
                w1Lower16 != (RM_AA_ZB_OPA_INTER(0)) &&
                // missing check for RM_RA_ZB_OPA_INTER
                w1Lower16 != (RM_AA_ZB_XLU_INTER(0)) && w1Lower16 != (RM_AA_ZB_XLU_LINE(0)) &&
                w1Lower16 != (RM_AA_ZB_DEC_LINE(0)) && w1Lower16 != (RM_AA_ZB_TEX_EDGE(0)) &&
                w1Lower16 != (RM_AA_ZB_TEX_INTER(0)) && w1Lower16 != (RM_AA_ZB_SUB_SURF(0)) &&
                w1Lower16 != (RM_AA_ZB_PCL_SURF(0)) && w1Lower16 != (RM_AA_ZB_OPA_TERR(0)) &&
                w1Lower16 != (RM_AA_ZB_TEX_TERR(0)) && w1Lower16 != (RM_AA_ZB_SUB_TERR(0)) &&
                w1Lower16 != (RM_AA_OPA_SURF(0)) && w1Lower16 != (RM_RA_OPA_SURF(0)) &&
                w1Lower16 != (RM_AA_XLU_SURF(0)) && w1Lower16 != (RM_AA_XLU_LINE(0)) &&
                w1Lower16 != (RM_AA_DEC_LINE(0)) && w1Lower16 != (RM_AA_TEX_EDGE(0)) &&
                w1Lower16 != (RM_AA_SUB_SURF(0)) && w1Lower16 != (RM_AA_PCL_SURF(0)) &&
                w1Lower16 != (RM_AA_OPA_TERR(0)) && w1Lower16 != (RM_AA_TEX_TERR(0)) &&
                w1Lower16 != (RM_AA_SUB_TERR(0)) && w1Lower16 != (RM_ZB_OPA_SURF(0)) &&
                w1Lower16 != (RM_ZB_XLU_SURF(0)) && w1Lower16 != (RM_ZB_OPA_DECAL(0)) &&
                w1Lower16 != (RM_ZB_XLU_DECAL(0)) && w1Lower16 != (RM_ZB_CLD_SURF(0)) &&
                w1Lower16 != (RM_ZB_OVL_SURF(0)) && w1Lower16 != (RM_ZB_PCL_SURF(0)) && w1Lower16 != (RM_OPA_SURF(0)) &&
                w1Lower16 != (RM_XLU_SURF(0)) && w1Lower16 != (RM_TEX_EDGE(0)) && w1Lower16 != (RM_CLD_SURF(0)) &&
                w1Lower16 != (RM_PCL_SURF(0)) && w1Lower16 != (RM_ADD(0)) && w1Lower16 != (RM_NOOP(0)) &&
                w1Lower16 != (RM_VISCVG(0)) && w1Lower16 != (RM_OPA_CI(0))) {
                bits = (w1Lower16 >> 8) & 3;
                switch (bits) {
                    case 0:
                    case 2:
                        break;
                    case 1:
                    case 3:
                        break;
                }
                bits = (w1Lower16 >> 10) & 3;
                switch (bits) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        break;
                }
                break;
            }
            break;
        case G_MDSFT_ALPHACOMPARE:
        case G_MDSFT_ZSRCSEL:
        case G_MDSFT_BLENDER:
            break;
    }

    return 8;
}

const char D_800AED2C[] = "%20s: gSetAlphaCompare(%x)\n";
const char D_800AED48[] = "%20s: gSetDepthSource(%x)\n";
const char D_800AED64[] = "%20s: gSetRenderMode(";
const char D_800AED7C[] = "RM_AA_ZB_OPA_SURF";
const char D_800AED90[] = "RM_RA_ZB_OPA_SURF";
const char D_800AEDA4[] = "RM_AA_ZB_XLU_SURF";
const char D_800AEDB8[] = "RM_AA_ZB_OPA_DECAL";
const char D_800AEDCC[] = "RM_RA_ZB_OPA_DECAL";
const char D_800AEDE0[] = "RM_AA_ZB_XLU_DECAL";
const char D_800AEDF4[] = "RM_AA_ZB_OPA_INTER";
const char D_800AEE08[] = "RM_AA_ZB_XLU_INTER";
const char D_800AEE1C[] = "RM_AA_ZB_XLU_LINE";
const char D_800AEE30[] = "RM_AA_ZB_DEC_LINE";
const char D_800AEE44[] = "RM_AA_ZB_TEX_EDGE";
const char D_800AEE58[] = "RM_AA_ZB_TEX_INTER";
const char D_800AEE6C[] = "RM_AA_ZB_SUB_SURF";
const char D_800AEE80[] = "RM_AA_ZB_PCL_SURF";
const char D_800AEE94[] = "RM_AA_ZB_OPA_TERR";
const char D_800AEEA8[] = "RM_AA_ZB_TEX_TERR";
const char D_800AEEBC[] = "RM_AA_ZB_SUB_TERR";
const char D_800AEED0[] = "RM_AA_OPA_SURF";
const char D_800AEEE0[] = "RM_RA_OPA_SURF";
const char D_800AEEF0[] = "RM_AA_XLU_SURF";
const char D_800AEF00[] = "RM_AA_XLU_LINE";
const char D_800AEF10[] = "RM_AA_DEC_LINE";
const char D_800AEF20[] = "RM_AA_TEX_EDGE";
const char D_800AEF30[] = "RM_AA_SUB_SURF";
const char D_800AEF40[] = "RM_AA_PCL_SURF";
const char D_800AEF50[] = "RM_AA_OPA_TERR";
const char D_800AEF60[] = "RM_AA_TEX_TERR";
const char D_800AEF70[] = "RM_AA_SUB_TERR";
const char D_800AEF80[] = "RM_ZB_OPA_SURF";
const char D_800AEF90[] = "RM_ZB_XLU_SURF";
const char D_800AEFA0[] = "RM_ZB_OPA_DECAL";
const char D_800AEFB0[] = "RM_ZB_XLU_DECAL";
const char D_800AEFC0[] = "RM_ZB_CLD_SURF";
const char D_800AEFD0[] = "RM_ZB_OVL_SURF";
const char D_800AEFE0[] = "RM_ZB_PCL_SURF";
const char D_800AEFF0[] = "RM_OPA_SURF";
const char D_800AEFFC[] = "RM_XLU_SURF";
const char D_800AF008[] = "RM_TEX_EDGE";
const char D_800AF014[] = "RM_CLD_SURF";
const char D_800AF020[] = "RM_PCL_SURF";
const char D_800AF02C[] = "RM_ADD";
const char D_800AF034[] = "RM_NOOP";
const char D_800AF03C[] = "RM_VISCVG";
const char D_800AF048[] = "RM_OPA_CI";
const char D_800AF054[] = "AA | ";
const char D_800AF05C[] = "Z_CMP | ";
const char D_800AF068[] = "Z_UPD | ";
const char D_800AF074[] = "IM_RD | ";
const char D_800AF080[] = "CLR_ON_CVG | ";
const char D_800AF090[] = "CVG_CLAMP | ";
const char D_800AF0A0[] = "CVG_WRAP | ";
const char D_800AF0AC[] = "CVG_FULL | ";
const char D_800AF0B8[] = "CVG_SAVE | ";
const char D_800AF0C4[] = "OPA | ";
const char D_800AF0CC[] = "INTER | ";
const char D_800AF0D8[] = "XLU | ";
const char D_800AF0E0[] = "DECAL | ";
const char D_800AF0EC[] = "CVG_X_ALPHA | ";
const char D_800AF0FC[] = "ALPHA_CVG_SEL | ";
const char D_800AF110[] = "FORCE_BL | ";
const char D_800AF11C[] = ");\n";
const char D_800AF120[] = "%20s: gSetBlender() - UNSUPPORTED\n";

static s32 diRcpGeometryMode(Gfx *dList, char *str, UNUSED char *arg2) {
    char sp38[0x50];
    s32 pad;
    s32 word1;
    s32 bitMask;
    s32 sp28;

    bitMask = 1;
    sp28 = TRUE;
    word1 = dList->words.w1;
    sprintf(sp38, "%20s", str);
    do {
        stubbed_printf(sp38, "%20s: %s(");
        stubbed_printf(sp38, "|");
        if (word1 & bitMask) {
            if (sp28) {
                sp28 = FALSE;
            }
            switch (bitMask) {
                case G_ZBUFFER:
                    stubbed_printf(sp38, "G_ZBUFFER");
                    break;
                case G_TEXTURE_ENABLE:
                    stubbed_printf(sp38, "G_TEXTURE_ENABLE");
                    break;
                case G_SHADE:
                    stubbed_printf(sp38, "G_SHADE");
                    break;
                case G_SHADING_SMOOTH:
                    stubbed_printf(sp38, "G_SHADING_SMOOTH");
                    break;
                case G_CULL_FRONT:
                    stubbed_printf(sp38, "G_CULL_FRONT");
                    break;
                case G_CULL_BACK:
                    stubbed_printf(sp38, "G_CULL_BACK");
                    break;
                case G_FOG:
                    stubbed_printf(sp38, "G_FOG");
                    break;
                case G_LIGHTING:
                    stubbed_printf(sp38, "G_LIGHTING");
                    break;
                case G_TEXTURE_GEN:
                    stubbed_printf(sp38, "G_TEXTURE_GEN");
                    break;
                case G_TEXTURE_GEN_LINEAR:
                    stubbed_printf(sp38, "G_TEXTURE_GEN_LINEAR");
                    break;
                case G_LOD:
                    stubbed_printf(sp38, "G_LOD");
                    break;
            }
        } else {
        }
        stubbed_printf(sp38, ");\n");
        bitMask <<= 1;
    } while (bitMask <= 0x100000);
    return 8;
}
