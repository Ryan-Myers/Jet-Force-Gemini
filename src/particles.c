#include "common.h"

const char D_800AD5E0_AE1E0[] = "Triangle";
const char D_800AD5EC_AE1EC[] = "Rectangle";
const char D_800AD5F8_AE1F8[] = "Sprite";
const char D_800AD600_AE200[] = "Points";
const char D_800AD608_AE208[] = "Models";
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

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005CE90_5DA90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005CFD0_5DBD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D19C_5DD9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D1DC_5DDDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D2E8_5DEE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D660_5E260.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005D93C_5E53C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTrigger.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTriggerSPPos.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partInitTriggerPos.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005EE78_5FA78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005EEFC_5FAFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F048_5FC48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F294_5FE94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F438_60038.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partObjFreeTriggers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partAdjustScaling.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partUpdateTriggers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F76C_6036C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005F960_60560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_8005FE4C_60A4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partModelObjEmitModelPart.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80060434_61034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80060EC4_61AC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_800610CC_61CCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061228_61E28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061698_62298.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061C30_62830.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80061FC8_62BC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062178_62D78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062A00_63600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062A90_63690.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062D5C_6395C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062E0C_63A0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80062F0C_63B0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063030_63C30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063190_63D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/func_80063244_63E44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partUpdateParticles.s")

#pragma GLOBAL_ASM("asm/nonmatchings/particles/partDraw.s")

#ifdef VERSION_us
#pragma GLOBAL_ASM("asm/nonmatchings/particles/partNullifyCircularParticleParents.s")
#endif
