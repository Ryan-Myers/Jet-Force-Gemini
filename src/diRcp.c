#include "diRcp.h"
#include "common.h"
#include "types.h"

// forward declarations
static s32 diRcpVertex(Gfx *dList);
static s32 diRcpPolygon(Gfx *dList);
static s32 diRcpMatrix(Gfx *dList);
static s32 diRcpTexDma(Gfx *dList, char *dmaCommand);
static s32 diRcpDPBlock(Gfx *dList);
static s32 diRcpViewport(Gfx *dList);
static s32 diRcpDisplayList(Gfx *dList);
static s32 diRcpStrNameMacro(Gfx *dList, char *command, char *macro);
static s32 diRcpPrimColor(Gfx *dList);
static s32 diRcpColor(Gfx *dList, char *command, char *macro);
static s32 diRcpDmaOffsets(Gfx *dList, char *command);
static s32 diRcpMoveWd(Gfx *dList);
static s32 diRcpStrName(Gfx *dList, char *command);
static s32 diRcpOtherMode(Gfx *dList, char *command);
static s32 diRcpGeometryMode(Gfx *dList, char *command, char *macro);

void diRcpPrintDL(Gfx *dList, Gfx *dList2, s32 count) {
    u8 *gListIt;
    s32 i;

    stubbed_printf(">>>> %x: ", dList);
    stubbed_printf("     %x: ", dList2);

    gListIt = (u8 *) dList;
    for (i = 0; i < count; i++) {
        switch (gListIt[0]) {
            case G_VTX:
                gListIt += diRcpVertex((Gfx *) gListIt);
                break;
            case G_POL:
                gListIt += diRcpPolygon((Gfx *) gListIt);
                break;
            case G_SPNOOP:
                gListIt += diRcpStrName((Gfx *) gListIt, "G_SPNOOP");
                break;
            case G_MTX:
                gListIt += diRcpMatrix((Gfx *) gListIt);
                break;
            case G_TEXDMA:
                gListIt += diRcpTexDma((Gfx *) gListIt, "G_TEXDMA");
                break;
            case G_MOVEMEM:
                gListIt += diRcpViewport((Gfx *) gListIt);
                break;
            case G_DL:
                gListIt += diRcpDisplayList((Gfx *) gListIt);
                break;
            case G_DP_BLOCK:
                gListIt += diRcpDPBlock((Gfx *) gListIt);
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

/**
 * Decode a gDma1p macro, which is basically just a Gdma struct.
 * This is used for gSPVertex and gSPMatrix, among others.
 */
static void Decode_gDma1p(Gfx *pkt, s32 *command, s32 *addr, s32 *length, s32 *parameter) {
    *command = (pkt->words.w0 >> 24) & 0xFF;
    *parameter = (pkt->words.w0 >> 16) & 0xFF;
    *length = pkt->words.w0 & 0xFFFF;
    *addr = pkt->words.w1;
}

/**
 * Decode a gImmp21, which is basically just used by the gMoveWd macro.
 */
static void Decode_gMoveWd(Gfx *pkt, s32 *command, s32 *offset, s32 *index, s32 *data) {
    *command = (pkt->words.w0 >> 24) & 0xFF;
    *offset = (pkt->words.w0 >> 8) & 0xFFFF;
    *index = pkt->words.w0 & 0xFF;
    *data = pkt->words.w1;
}

static s32 diRcpVertex(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad[3];

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    stubbed_printf("%20s: gSPVertex(0x%x, 0x%x, %d, %d);\n", "G_VTX", dList, addr, length, command);

    return sizeof(Gfx);
}

static s32 diRcpPolygon(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad[3];

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    stubbed_printf("%20s: gSPPolygon(0x%x, 0x%x, %d, %d);\n", "G_POL", dList, addr, length, command);

    return sizeof(Gfx);
}

static s32 diRcpMatrix(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad[4];
    char *type1 = "GARBAGE";
    char *type2 = "NOMTXMULT";
    char *type3 = "MTXMULT";

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    stubbed_printf("%20s: gSPMatrix2(0x%x, 0x%x, %d, %s, %d);\n", "G_MTX", dList, addr, length, type1, command);
    return sizeof(Gfx);
}

static s32 diRcpTexDma(Gfx *dList, char *dmaCommand) {
    stubbed_printf("%20s: gSPLoadTextureOffsets(%d, %08x);\n", dmaCommand, dList, dList->words.w1);
    return sizeof(Gfx);
}

static s32 diRcpDPBlock(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad;

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    stubbed_printf("%20s: gSPDPBlock(0x%x, 0x%x, %d);\n", "G_DP_BLOCK", dList, addr, length);

    return sizeof(Gfx);
}

static s32 diRcpViewport(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad;

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    stubbed_printf("%20s: gSPViewport(0x%x, 0x%x);\n", "G_MOVEMEM", dList, addr);

    return sizeof(Gfx);
}

static s32 diRcpDisplayList(Gfx *dList) {
    s32 command;
    s32 addr;
    s32 length;
    s32 parameter;
    s32 pad;

    Decode_gDma1p(dList, &command, &addr, &length, &parameter);

    if (parameter == G_DL_PUSH) {
        stubbed_printf("%20s: gSPDisplayList(0x%x, 0x%x);\n", "G_DL", dList, addr);
    } else if (parameter == G_DL_NOPUSH) {
        stubbed_printf("%20s: gSPBranchList(0x%x, 0x%x);\n", "G_DL", dList, addr);
    } else {
        stubbed_printf("%20s: Unknown G_DL command\n", "G_DL");
    }

    return sizeof(Gfx);
}

static s32 diRcpStrNameMacro(Gfx *dList, char *command, char *macro) {
    stubbed_printf("%20s: %s(0x%08x);\n", command, macro, dList);
    return sizeof(Gfx);
}

static s32 diRcpPrimColor(UNUSED Gfx *dList) {
    stubbed_printf("%20s: gDPSetPrimColor(0x%08x, %d, %d, %d, %d, %d, %d);\n", "G_SETPRIMCOLOR", dList,
                   dList->words.w0 >> 8 & 0xFF, dList->words.w0 & 0xFF, (dList->words.w1 >> 24) & 0xFF,
                   (dList->words.w1 >> 16) & 0xFF, (dList->words.w1 >> 8) & 0xFF, dList->words.w1 & 0xFF);

    return sizeof(Gfx);
}

static s32 diRcpColor(Gfx *dList, char *command, char *macro) {
    stubbed_printf("%20s: %s(0x%08x, %2d, %2d, %2d, %2d);\n", command, macro, dList, (dList->words.w1 >> 24) & 0xFF,
                   (dList->words.w1 >> 16) & 0xFF, (dList->words.w1 >> 8) & 0xFF, dList->words.w1 & 0xFF);
    return sizeof(Gfx);
}

static s32 diRcpDmaOffsets(Gfx *dList, char *command) {
    if (dList) {}
    stubbed_printf("%20s: gSPSetDMAOffsets(%08x, %08x)\n", command, dList, dList);
    return sizeof(Gfx);
}

static s32 diRcpMoveWd(Gfx *dList) {
    s32 command;
    s32 index;
    s32 offset;
    s32 data;
    char str[0x2C];

    Decode_gMoveWd(dList, &command, &offset, &index, &data);
    switch (index) {
        case G_MW_SEGMENT:
            stubbed_printf("G_MW_SEGMENT");
            break;
        case G_MW_CLIP:
            stubbed_printf("G_MW_CLIP");
            break;
        case G_MW_MTXOFFSET:
            stubbed_printf("G_MW_MTXOFFSET");
            break;
        case G_MW_POINTS:
            stubbed_printf("G_MW_POINTS");
            break;
        case G_MW_SPRITEMODE:
            stubbed_printf("G_MW_SPRITEMODE");
            break;
        case G_MW_FOG:
            stubbed_printf("G_MW_FOG");
            command = index; // fakematch
            break;
        default:
            sprintf(str, "UNKNOWN INDEX %d", index);
            break;
    }
    if (command && command) {} // fakematch
    stubbed_printf("%20s: %s offset = %d, data = 0x%08x\n", "G_MOVEWORD", dList, offset, data);
    return sizeof(Gfx);
}

static s32 diRcpStrName(UNUSED Gfx *dList, char *command) {
    char print[0x50];
    s32 pad[2];

    sprintf(print, "%20s", command);
    return sizeof(Gfx);
}

static s32 diRcpOtherMode(Gfx *dList, char *command) {
    char sp30[0x50];
    s32 bits;
    s32 shiftValue;
    s32 data;

    shiftValue = (dList->words.w0 >> 8) & 0xFF;
    data = dList->words.w1;

    stubbed_printf("%s: 0x%08x  0x%08x\n", command, dList, data);

    sprintf(sp30, "%20s", command);

    switch (shiftValue) {
        case G_MDSFT_ALPHACOMPARE:
            stubbed_printf("%20s: gSetAlphaCompare(%x)\n", command, data);
            break;
        case G_MDSFT_ZSRCSEL:
            stubbed_printf("%20s: gSetDepthSource(%x)\n", command, data);
            break;
        case G_MDSFT_RENDERMODE:
            data &= 0xFFFF;

            stubbed_printf("%20s: gSetRenderMode(", command);

            if (data == (RM_AA_ZB_OPA_SURF(0))) {
                stubbed_printf("RM_AA_ZB_OPA_SURF");
            } else if (data == (RM_RA_ZB_OPA_SURF(0))) {
                stubbed_printf("RM_RA_ZB_OPA_SURF");
            } else if (data == (RM_AA_ZB_XLU_SURF(0))) {
                stubbed_printf("RM_AA_ZB_XLU_SURF");
            } else if (data == (RM_AA_ZB_OPA_DECAL(0))) {
                stubbed_printf("RM_AA_ZB_OPA_DECAL");
            } else if (data == (RM_RA_ZB_OPA_DECAL(0))) {
                stubbed_printf("RM_RA_ZB_OPA_DECAL");
            } else if (data == (RM_AA_ZB_XLU_DECAL(0))) {
                stubbed_printf("RM_AA_ZB_XLU_DECAL");
            } else if (data == (RM_AA_ZB_OPA_INTER(0))) {
                stubbed_printf("RM_AA_ZB_OPA_INTER");
                // } else if (data == (RM_RA_ZB_OPA_INTER(0))) {
                //     stubbed_printf("RM_RA_ZB_OPA_INTER");
            } else if (data == (RM_AA_ZB_XLU_INTER(0))) {
                stubbed_printf("RM_AA_ZB_XLU_INTER");
            } else if (data == (RM_AA_ZB_XLU_LINE(0))) {
                stubbed_printf("RM_AA_ZB_XLU_LINE");
            } else if (data == (RM_AA_ZB_DEC_LINE(0))) {
                stubbed_printf("RM_AA_ZB_DEC_LINE");
            } else if (data == (RM_AA_ZB_TEX_EDGE(0))) {
                stubbed_printf("RM_AA_ZB_TEX_EDGE");
            } else if (data == (RM_AA_ZB_TEX_INTER(0))) {
                stubbed_printf("RM_AA_ZB_TEX_INTER");
            } else if (data == (RM_AA_ZB_SUB_SURF(0))) {
                stubbed_printf("RM_AA_ZB_SUB_SURF");
            } else if (data == (RM_AA_ZB_PCL_SURF(0))) {
                stubbed_printf("RM_AA_ZB_PCL_SURF");
            } else if (data == (RM_AA_ZB_OPA_TERR(0))) {
                stubbed_printf("RM_AA_ZB_OPA_TERR");
            } else if (data == (RM_AA_ZB_TEX_TERR(0))) {
                stubbed_printf("RM_AA_ZB_TEX_TERR");
            } else if (data == (RM_AA_ZB_SUB_TERR(0))) {
                stubbed_printf("RM_AA_ZB_SUB_TERR");
            } else if (data == (RM_AA_OPA_SURF(0))) {
                stubbed_printf("RM_AA_OPA_SURF");
            } else if (data == (RM_RA_OPA_SURF(0))) {
                stubbed_printf("RM_RA_OPA_SURF");
            } else if (data == (RM_AA_XLU_SURF(0))) {
                stubbed_printf("RM_AA_XLU_SURF");
            } else if (data == (RM_AA_XLU_LINE(0))) {
                stubbed_printf("RM_AA_XLU_LINE");
            } else if (data == (RM_AA_DEC_LINE(0))) {
                stubbed_printf("RM_AA_DEC_LINE");
            } else if (data == (RM_AA_TEX_EDGE(0))) {
                stubbed_printf("RM_AA_TEX_EDGE");
            } else if (data == (RM_AA_SUB_SURF(0))) {
                stubbed_printf("RM_AA_SUB_SURF");
            } else if (data == (RM_AA_PCL_SURF(0))) {
                stubbed_printf("RM_AA_PCL_SURF");
            } else if (data == (RM_AA_OPA_TERR(0))) {
                stubbed_printf("RM_AA_OPA_TERR");
            } else if (data == (RM_AA_TEX_TERR(0))) {
                stubbed_printf("RM_AA_TEX_TERR");
            } else if (data == (RM_AA_SUB_TERR(0))) {
                stubbed_printf("RM_AA_SUB_TERR");
            } else if (data == (RM_ZB_OPA_SURF(0))) {
                stubbed_printf("RM_ZB_OPA_SURF");
            } else if (data == (RM_ZB_XLU_SURF(0))) {
                stubbed_printf("RM_ZB_XLU_SURF");
            } else if (data == (RM_ZB_OPA_DECAL(0))) {
                stubbed_printf("RM_ZB_OPA_DECAL");
            } else if (data == (RM_ZB_XLU_DECAL(0))) {
                stubbed_printf("RM_ZB_XLU_DECAL");
            } else if (data == (RM_ZB_CLD_SURF(0))) {
                stubbed_printf("RM_ZB_CLD_SURF");
            } else if (data == (RM_ZB_OVL_SURF(0))) {
                stubbed_printf("RM_ZB_OVL_SURF");
            } else if (data == (RM_ZB_PCL_SURF(0))) {
                stubbed_printf("RM_ZB_PCL_SURF");
            } else if (data == (RM_OPA_SURF(0))) {
                stubbed_printf("RM_OPA_SURF");
            } else if (data == (RM_XLU_SURF(0))) {
                stubbed_printf("RM_XLU_SURF");
            } else if (data == (RM_TEX_EDGE(0))) {
                stubbed_printf("RM_TEX_EDGE");
            } else if (data == (RM_CLD_SURF(0))) {
                stubbed_printf("RM_CLD_SURF");
            } else if (data == (RM_PCL_SURF(0))) {
                stubbed_printf("RM_PCL_SURF");
            } else if (data == (RM_ADD(0))) {
                stubbed_printf("RM_ADD");
            } else if (data == (RM_NOOP(0))) {
                stubbed_printf("RM_NOOP");
            } else if (data == (RM_VISCVG(0))) {
                stubbed_printf("RM_VISCVG");
            } else if (data == (RM_OPA_CI(0))) {
                stubbed_printf("RM_OPA_CI");
            } else {
                if (data & AA_EN) {
                    stubbed_printf("AA | ");
                }
                if (data & Z_CMP) {
                    stubbed_printf("Z_CMP | ");
                }
                if (data & Z_UPD) {
                    stubbed_printf("Z_UPD | ");
                }
                if (data & IM_RD) {
                    stubbed_printf("IM_RD | ");
                }
                if (data & CLR_ON_CVG) {
                    stubbed_printf("CLR_ON_CVG | ");
                }

                bits = (data >> 8) & 3;
                switch (bits) {
                    case 0:
                        stubbed_printf("CVG_CLAMP | ");
                        break;
                    case 1:
                        stubbed_printf("CVG_WRAP | ");
                        break;
                    case 2:
                        stubbed_printf("CVG_FULL | ");
                        break;
                    case 3:
                        stubbed_printf("CVG_SAVE | ");
                        break;
                }

                bits = (data >> 10) & 3;
                switch (bits) {
                    case 0:
                        stubbed_printf("OPA | ");
                        break;
                    case 1:
                        stubbed_printf("INTER | ");
                        break;
                    case 2:
                        stubbed_printf("XLU | ");
                        break;
                    case 3:
                        stubbed_printf("DECAL | ");
                        break;
                }

                if (data & CVG_X_ALPHA) {
                    stubbed_printf("CVG_X_ALPHA | ");
                }
                if (data & ALPHA_CVG_SEL) {
                    stubbed_printf("ALPHA_CVG_SEL | ");
                }
                if (data & FORCE_BL) {
                    stubbed_printf("FORCE_BL | ");
                }

                stubbed_printf(");\n");
                break;
            }
            break;
        case G_MDSFT_BLENDER:
            stubbed_printf("%20s: gSetBlender() - UNSUPPORTED\n", command);
            break;
    }

    return sizeof(Gfx);
}

static s32 diRcpGeometryMode(Gfx *dList, char *command, char *macro) {
    char s[0x50];
    s32 pad;
    s32 word1;
    s32 bitMask;
    s32 addPipe;

    bitMask = 1;
    addPipe = TRUE;
    word1 = dList->words.w1;
    sprintf(s, "%20s", command);
    do {
        stubbed_printf("%20s: %s(", command, macro);
        if (word1 & bitMask) {
            if (addPipe) {
                stubbed_printf("|");
                addPipe = FALSE;
            }
            switch (bitMask) {
                case G_ZBUFFER:
                    stubbed_printf("G_ZBUFFER");
                    break;
                case G_TEXTURE_ENABLE:
                    stubbed_printf("G_TEXTURE_ENABLE");
                    break;
                case G_SHADE:
                    stubbed_printf("G_SHADE");
                    break;
                case G_SHADING_SMOOTH:
                    stubbed_printf("G_SHADING_SMOOTH");
                    break;
                case G_CULL_FRONT:
                    stubbed_printf("G_CULL_FRONT");
                    break;
                case G_CULL_BACK:
                    stubbed_printf("G_CULL_BACK");
                    break;
                case G_FOG:
                    stubbed_printf("G_FOG");
                    break;
                case G_LIGHTING:
                    stubbed_printf("G_LIGHTING");
                    break;
                case G_TEXTURE_GEN:
                    stubbed_printf("G_TEXTURE_GEN");
                    break;
                case G_TEXTURE_GEN_LINEAR:
                    stubbed_printf("G_TEXTURE_GEN_LINEAR");
                    break;
                case G_LOD:
                    stubbed_printf("G_LOD");
                    break;
            }
        } else {
            stubbed_printf(");\n");
        }
        bitMask <<= 1;
    } while (bitMask <= 0x100000);

    return sizeof(Gfx);
}
