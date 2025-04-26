#include "common.h"
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} ShadowPoly;

#if 0
s32 shadowBoxPolyOverlap(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, ShadowPoly *arg5) {
    f32 temp_f4;
    f32 temp_f5;
    f32 var_f0;
    f32 var_f1;
    f32 var_f2;
    f32 var_f3;
    s32 var_a0;
    s32 var_v0;
    
    var_f0 = arg5->unk0;
    var_f2 = arg5->unk8;
    var_a0 = arg4;
    var_f1 = var_f0;
    var_f3 = var_f2;
    var_v0 = 0;
loop_1:
    var_a0 -= 1;
    if (var_a0 != 0) {
        arg5 += 1;
        temp_f4 = arg5->unk0;
        temp_f5 = arg5->unk8;
        if (temp_f4 < var_f0) {
            var_f0 = temp_f4;
        } else if (!(temp_f4 <= var_f1)) {
            var_f1 = temp_f4;
        }
        if (temp_f5 < var_f2) {
            var_f2 = temp_f5;
        } else if (!(temp_f5 <= var_f3)) {
            var_f3 = temp_f5;
        }
        goto loop_1;
    }
    if ((arg0 <= var_f1) && (arg1 <= var_f3) && !(arg2 < var_f0) && !(arg3 < var_f2)) {
        var_v0 = 1;
    }
    return var_v0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/shadows_214A0/shadowBoxPolyOverlap.s")
#endif

#if 0
void shadowBoundingBox(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, f32 *arg5) {
    f32 temp_f4;
    f32 temp_f5;
    f32 var_f0;
    f32 var_f1;
    f32 var_f2;
    f32 var_f3;
    var_f0 = arg1[0];
    var_f2 = arg1[1];
    var_f1 = var_f0;
    var_f3 = var_f2;
loop_1:
    arg0 -= 1;
    if (arg0 != 0) {
        arg1 += 2;
        temp_f4 = arg1[0];
        temp_f5 = arg1[1];
        if (temp_f4 < var_f0) {
            var_f0 = temp_f4;
        } else if (!(temp_f4 <= var_f1)) {
            var_f1 = temp_f4;
        }
        if (temp_f5 < var_f2) {
            var_f2 = temp_f5;
        } else if (!(temp_f5 <= var_f3)) {
            var_f3 = temp_f5;
        }
        goto loop_1;
    }
    *arg2 = var_f0;
    *arg3 = var_f2;
    *arg4 = var_f1;
    *arg5 = var_f3;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/shadows_214A0/shadowBoundingBox.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/shadows_214A0/shadowYHeight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shadows_214A0/shadowMakeYs.s")
