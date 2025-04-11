#include "common.h"

const char D_800AC3F0[] = "explosion %d out of range\n";

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionStart.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/objExplosionSetupTriggers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/shockwaveStart.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/shockwaveInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/shockwaveControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/objDrawShockWave.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/expspriteInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/expspriteControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionFlushBlasts.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionAddBlast.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionHitTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/explosionBlastHit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/func_8002A7DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/func_8002A958.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/func_8002AA30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/explosion/func_8002ABA4.s")
