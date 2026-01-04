#include "common.h"

const char D_800AE190[] = "Triangle";
const char D_800AE19C[] = "Rectangle";
const char D_800AE1A8[] = "Sprite";
const char D_800AE1B0[] = "Points";
const char D_800AE1B8[] = "Models";
const char D_800AE1C0[] = "\n\nUnknown trigger type in partInitTrigger %d, Max %d.\n\n";
const char D_800AE1F8[] = "\n\nUnknown trigger type in partInitTrigger %d, Max %d.\n\n";
const char D_800AE230[] = "\n\nUnknown particle type in partInitTriggerPos %d, Max %d.\n\n";
const char D_800AE26C[] = "\n\nUnknown trigger type in partInitTriggerPos %d, Max %d.\n\n";
const char D_800AE2A8[] = "particle warning: Object %s has SP emitter, but matrices not generated\n";
const char D_800AE2F0[] = "allocParticle: Illegal particle type %d\n";
const char D_800AE31C[] = "%s particle buffer full\n";
const char D_800AE338[] = "\n\nParticle of type %d has been freed twice, this is Super Safe, Honest!\n";
const char D_800AE384[] = "freeParticle: Unknown particle type %d\n";
const char D_800AE3AC[] = "%s particle buffer is empty\n";

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partFreeLib.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005DA70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005DC3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005DC7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005DD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005E100.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005E3DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTrigger.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTriggerSPPos.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTriggerPos.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F918.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005FAE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005FD34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005FED8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partObjFreeTriggers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partAdjustScaling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partUpdateTriggers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8006020C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80060400.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_800608EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partModelObjEmitModelPart.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80060ED4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061B50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061CAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8006211C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_800626B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062A4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062BFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063514.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_800637E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063890.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063AB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063C14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063CC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partUpdateParticles.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partDraw.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm_us/nonmatchings/particles/partNullifyCircularParticleParents.s")
#endif
