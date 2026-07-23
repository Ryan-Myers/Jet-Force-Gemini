#include "common.h"

const char D_800AE6C0[] = ">>>> %x: ";
const char D_800AE6CC[] = "     %x: ";
// const char D_800AE6D8[] = "G_SPNOOP";
// const char D_800AE6E4[] = "G_TEXDMA";
// const char D_800AE6F0[] = "G_RESERVED3";
// const char D_800AE6FC[] = "G_RESERVED4";
// const char D_800AE708[] = "G_NOOP";
// const char D_800AE710[] = "gDPNoOp";
// const char D_800AE718[] = "G_SETCIMG";
// const char D_800AE724[] = "G_SETZIMG";
// const char D_800AE730[] = "G_SETTIMG";
// const char D_800AE73C[] = "G_SETCOMBINE";
// const char D_800AE74C[] = "G_SETENVCOLOR";
// const char D_800AE75C[] = "gDPSetEnvColor";
// const char D_800AE76C[] = "G_SETBLENDCOLOR";
// const char D_800AE77C[] = "gDPSetBlendColor";
// const char D_800AE790[] = "G_SETFOGCOLOR";
// const char D_800AE7A0[] = "gDPSetFogColor";
// const char D_800AE7B0[] = "G_SETFILLCOLOR";
// const char D_800AE7C0[] = "gDPSetFillColor";
// const char D_800AE7D0[] = "G_FILLRECT";
// const char D_800AE7DC[] = "G_SETTILE";
// const char D_800AE7E8[] = "G_LOADTILE";
// const char D_800AE7F4[] = "G_LOADBLOCK";
// const char D_800AE800[] = "G_SETTILESIZE";
// const char D_800AE810[] = "G_LOADTLUT";
// const char D_800AE81C[] = "G_RDPSETOTHERMODE";
// const char D_800AE830[] = "G_SETPRIMDEPTH";
// const char D_800AE840[] = "G_SETSCISSOR";
// const char D_800AE850[] = "G_SETCONVERT";
// const char D_800AE860[] = "G_SETKEYR";
// const char D_800AE86C[] = "G_SETKEYGB";
// const char D_800AE878[] = "G_RDPFULLSYNC";
// const char D_800AE888[] = "gDPFullSync";
// const char D_800AE894[] = "G_RDPTILESYNC";
// const char D_800AE8A4[] = "gDPTileSync";
// const char D_800AE8B0[] = "G_RDPPIPESYNC";
// const char D_800AE8C0[] = "gDPPipeSync";
// const char D_800AE8CC[] = "G_RDPLoadSYNC";
// const char D_800AE8DC[] = "gDPLoadSync";
// const char D_800AE8E8[] = "G_TEXRECTFLIP";
// const char D_800AE8F8[] = "G_TEXRECT";
// const char D_800AE904[] = "G_DMAOFFSETS";
// const char D_800AE914[] = "G_CULLDL";
// const char D_800AE920[] = "G_POPMTX";
// const char D_800AE92C[] = "G_TEXTURE";
// const char D_800AE938[] = "G_SETOTHERMODE_H";
// const char D_800AE94C[] = "G_SETOTHERMODE_L";
// const char D_800AE960[] = "G_ENDDL";
// const char D_800AE968[] = "G_SETGEOMETRYMODE";
// const char D_800AE97C[] = "gSPSetGeometryMode";
// const char D_800AE990[] = "G_CLEARGEOMETRYMODE";
// const char D_800AE9A4[] = "gSPClearGeometryMode";
// const char D_800AE9BC[] = "G_LINE3D";
// const char D_800AE9C8[] = "G_PERSPNORMALIZE";
// const char D_800AE9DC[] = "G_RDPHALF_1";
// const char D_800AE9E8[] = "G_RDPHALF_2";
// const char D_800AE9F4[] = "G_RDPHALF_CONT";
// const char D_800AEA04[] = "UNKNOWN COMMAND";
// const char D_800AEA14[] = "%20s: gSPVertex(0x%x, 0x%x, %d, %d);\n";
// const char D_800AEA3C[] = "G_VTX";
// const char D_800AEA44[] = "%20s: gSPPolygon(0x%x, 0x%x, %d, %d);\n";
// const char D_800AEA6C[] = "G_POL";
// const char D_800AEA74[] = "GARBAGE";
// const char D_800AEA7C[] = "NOMTXMULT";
// const char D_800AEA88[] = "MTXMULT";
// const char D_800AEA90[] = "%20s: gSPMatrix2(0x%x, 0x%x, %d, %s, %d);\n";
// const char D_800AEABC[] = "G_MTX";
// const char D_800AEAC4[] = "%20s: gSPLoadTextureOffsets(%d, %08x);\n";
// const char D_800AEAEC[] = "%20s: gSPDPBlock(0x%x, 0x%x, %d);\n";
// const char D_800AEB10[] = "G_DP_BLOCK";
// const char D_800AEB1C[] = "%20s: gSPViewport(0x%x, 0x%x);\n";
// const char D_800AEB3C[] = "G_MOVEMEM";
// const char D_800AEB48[] = "%20s: gSPDisplayList(0x%x, 0x%x);\n";
// const char D_800AEB6C[] = "G_DL";
// const char D_800AEB74[] = "%20s: gSPBranchList(0x%x, 0x%x);\n";
// const char D_800AEB98[] = "G_DL";
// const char D_800AEBA0[] = "%20s: Unknown G_DL command\n";
// const char D_800AEBBC[] = "G_DL";
// const char D_800AEBC4[] = "%20s: %s(0x%08x);\n";
// const char D_800AEBD8[] = "%20s: gDPSetPrimColor(0x%08x, %d, %d, %d, %d, %d, %d);\n";
// const char D_800AEC10[] = "G_SETPRIMCOLOR";
// const char D_800AEC20[] = "%20s: %s(0x%08x, %2d, %2d, %2d, %2d);\n";
// const char D_800AEC48[] = "%20s: gSPSetDMAOffsets(%08x, %08x)\n";
// const char D_800AEC6C[] = "G_MW_SEGMENT";
// const char D_800AEC7C[] = "G_MW_CLIP";
// const char D_800AEC88[] = "G_MW_MTXOFFSET";
// const char D_800AEC98[] = "G_MW_POINTS";
// const char D_800AECA4[] = "G_MW_SPRITEMODE";
// const char D_800AECB4[] = "G_MW_FOG";
// const char D_800AECC0[] = "UNKNOWN INDEX %d";
// const char D_800AECD4[] = "%20s: %s offset = %d, data = 0x%08x\n";
// const char D_800AECFC[] = "G_MOVEWORD";
// const char D_800AED08[] = "%20s";
// const char D_800AED10[] = "%s: 0x%08x  0x%08x\n";
// const char D_800AED24[] = "%20s";
// const char D_800AED2C[] = "%20s: gSetAlphaCompare(%x)\n";
// const char D_800AED48[] = "%20s: gSetDepthSource(%x)\n";
// const char D_800AED64[] = "%20s: gSetRenderMode(";
// const char D_800AED7C[] = "RM_AA_ZB_OPA_SURF";
// const char D_800AED90[] = "RM_RA_ZB_OPA_SURF";
// const char D_800AEDA4[] = "RM_AA_ZB_XLU_SURF";
// const char D_800AEDB8[] = "RM_AA_ZB_OPA_DECAL";
// const char D_800AEDCC[] = "RM_RA_ZB_OPA_DECAL";
// const char D_800AEDE0[] = "RM_AA_ZB_XLU_DECAL";
// const char D_800AEDF4[] = "RM_AA_ZB_OPA_INTER";
// const char D_800AEE08[] = "RM_AA_ZB_XLU_INTER";
// const char D_800AEE1C[] = "RM_AA_ZB_XLU_LINE";
// const char D_800AEE30[] = "RM_AA_ZB_DEC_LINE";
// const char D_800AEE44[] = "RM_AA_ZB_TEX_EDGE";
// const char D_800AEE58[] = "RM_AA_ZB_TEX_INTER";
// const char D_800AEE6C[] = "RM_AA_ZB_SUB_SURF";
// const char D_800AEE80[] = "RM_AA_ZB_PCL_SURF";
// const char D_800AEE94[] = "RM_AA_ZB_OPA_TERR";
// const char D_800AEEA8[] = "RM_AA_ZB_TEX_TERR";
// const char D_800AEEBC[] = "RM_AA_ZB_SUB_TERR";
// const char D_800AEED0[] = "RM_AA_OPA_SURF";
// const char D_800AEEE0[] = "RM_RA_OPA_SURF";
// const char D_800AEEF0[] = "RM_AA_XLU_SURF";
// const char D_800AEF00[] = "RM_AA_XLU_LINE";
// const char D_800AEF10[] = "RM_AA_DEC_LINE";
// const char D_800AEF20[] = "RM_AA_TEX_EDGE";
// const char D_800AEF30[] = "RM_AA_SUB_SURF";
// const char D_800AEF40[] = "RM_AA_PCL_SURF";
// const char D_800AEF50[] = "RM_AA_OPA_TERR";
// const char D_800AEF60[] = "RM_AA_TEX_TERR";
// const char D_800AEF70[] = "RM_AA_SUB_TERR";
// const char D_800AEF80[] = "RM_ZB_OPA_SURF";
// const char D_800AEF90[] = "RM_ZB_XLU_SURF";
// const char D_800AEFA0[] = "RM_ZB_OPA_DECAL";
// const char D_800AEFB0[] = "RM_ZB_XLU_DECAL";
// const char D_800AEFC0[] = "RM_ZB_CLD_SURF";
// const char D_800AEFD0[] = "RM_ZB_OVL_SURF";
// const char D_800AEFE0[] = "RM_ZB_PCL_SURF";
// const char D_800AEFF0[] = "RM_OPA_SURF";
// const char D_800AEFFC[] = "RM_XLU_SURF";
// const char D_800AF008[] = "RM_TEX_EDGE";
// const char D_800AF014[] = "RM_CLD_SURF";
// const char D_800AF020[] = "RM_PCL_SURF";
// const char D_800AF02C[] = "RM_ADD";
// const char D_800AF034[] = "RM_NOOP";
// const char D_800AF03C[] = "RM_VISCVG";
// const char D_800AF048[] = "RM_OPA_CI";
// const char D_800AF054[] = "AA | ";
// const char D_800AF05C[] = "Z_CMP | ";
// const char D_800AF068[] = "Z_UPD | ";
// const char D_800AF074[] = "IM_RD | ";
// const char D_800AF080[] = "CLR_ON_CVG | ";
// const char D_800AF090[] = "CVG_CLAMP | ";
// const char D_800AF0A0[] = "CVG_WRAP | ";
// const char D_800AF0AC[] = "CVG_FULL | ";
// const char D_800AF0B8[] = "CVG_SAVE | ";
// const char D_800AF0C4[] = "OPA | ";
// const char D_800AF0CC[] = "INTER | ";
// const char D_800AF0D8[] = "XLU | ";
// const char D_800AF0E0[] = "DECAL | ";
// const char D_800AF0EC[] = "CVG_X_ALPHA | ";
// const char D_800AF0FC[] = "ALPHA_CVG_SEL | ";
// const char D_800AF110[] = "FORCE_BL | ";
// const char D_800AF11C[] = ");\n";
// const char D_800AF120[] = "%20s: gSetBlender() - UNSUPPORTED\n";
// const char D_800AF144[] = "%20s";
// const char D_800AF14C[] = "%20s: %s(";
// const char D_800AF158[] = "|";
// const char D_800AF15C[] = "G_ZBUFFER";
// const char D_800AF168[] = "G_TEXTURE_ENABLE";
// const char D_800AF17C[] = "G_SHADE";
// const char D_800AF184[] = "G_SHADING_SMOOTH";
// const char D_800AF198[] = "G_CULL_FRONT";
// const char D_800AF1A8[] = "G_CULL_BACK";
// const char D_800AF1B4[] = "G_FOG";
// const char D_800AF1BC[] = "G_LIGHTING";
// const char D_800AF1C8[] = "G_TEXTURE_GEN";
// const char D_800AF1D8[] = "G_TEXTURE_GEN_LINEAR";
// const char D_800AF1F0[] = "G_LOD";
// const char D_800AF1F8[] = ");\n";

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/diRcpPrintDL.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80066F64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80066FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80066FF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067020.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067050.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067080.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067090.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_800670C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_800670F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067120.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067134.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067140.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067154.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067160.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067230.s")

#pragma GLOBAL_ASM("asm/nonmatchings/diRcp/func_80067438.s")
