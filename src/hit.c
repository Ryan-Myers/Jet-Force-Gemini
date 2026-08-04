#include "common.h"

#ifdef VERSION_kiosk
const char D_800AFDB0[] = "fragment killed squaddie\n";
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitReset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitFree.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitInitObjectHit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007A018_7AC18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007A324_7AF24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitsCheckLineOfSight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitVectorHitCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPlayerAimCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitVectorCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007CCF0_7D8F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007CF28_7DB28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007D0B4_7DCB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007D168_7DD68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007DE98_7EA98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPlayerKilledPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetHitModels.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitModelInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitModelsTick.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007E5EC_7F1EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/func_8007E794_7F394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitTransformBox.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitMakePolylist.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPolyPlaneTest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPolyEdgeTest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitPolyEdgeBuffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitNearestIntersection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetLedgeCrossed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetAdjacentLedge.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitCrateCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitSquashCheck.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetHeights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetModCompareMask.s")

#pragma GLOBAL_ASM("asm/nonmatchings/hit/hitGetInelasticVelocity.s")
