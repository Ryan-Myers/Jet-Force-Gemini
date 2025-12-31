#include "common.h"

const char D_800AC900[] = "Error: Model no. out of range on load. !!\n";
const char D_800AC92C[] = "TEXTURE ERROR!!\n%d,%d\n";
const char D_800AC944[] = "Error: Model table overflow!!\n";
const char D_800AC964[] = "CREATE LOD MODEL :: null model pointer!";
const char D_800AC98C[] = "WARNING :: createModelInstance called with NULL pointer\n";
const char D_800AC9C8[] = "MODELS Error: Tryed to deallocate non-existent model!\n";
const char D_800ACA00[] = "MODELS Error : cannot free NULL model instance pointer!!\n";
const char D_800ACA3C[] = "modLoadAnim: Overflowed AnimTab!\n";
const char D_800ACA60[] = "modFreeAnim : NULL anim!!\n";
const char D_800ACA7C[] = "Anim Error: Tryed to deallocate non-existent anim!!\n";
const char D_800ACAB4[] = "modLoadAnimEvent: Overflowed AnimEventTab!\n";
const char D_800ACAE0[] = "modFreeAnimEvents : NULL anim event table!!\n";
const char D_800ACB10[] = "Anim Event Error: Tryed to deallocate non-existent anim event!\n";

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003B640.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modInitModels.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modLoadModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003BE68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003BED0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003BF58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003C8A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CB50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CCC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003CD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnim.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGenAnimMatrices.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelSetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modelGetModelFlags.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/makeModelGfx.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E100.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/func_8003E13C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modMakeLimbModel.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modLoadAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modFreeAnimEvents.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modGetMakeBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/models/modSetTextureFrame.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/models/modSuspendModelTextures.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/models/modResumeModelTextures.s")
#endif
