#include "common.h"

#ifdef VERSION_kiosk
const char D_800AFDB0[] = "fragment killed squaddie\n";
#endif

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitReset.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitFree.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitInitObjectHit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007A638.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007A944.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitUpdate.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitsCheckLineOfSight.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitVectorHitCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPlayerAimCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitVectorCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007D1E8.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007D420.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007D5AC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007D660.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007E2F0.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPlayerKilledPlayer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetHitModels.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitModelInit.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitModelsTick.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007EA44.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/func_8007EBEC.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitTransformBox.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitMakePolylist.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPolyPlaneTest.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPolyEdgeTest.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitPolyEdgeBuffer.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitNearestIntersection.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetLedgeCrossed.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetAdjacentLedge.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitCrateCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitSquashCheck.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetHeights.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetModCompareMask.s")

#pragma GLOBAL_ASM("asm_us/nonmatchings/hit/hitGetInelasticVelocity.s")
