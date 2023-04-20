#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/weather/initWeather.s")

typedef struct {
    s16 unk0;
    s16 unk2;
    //s16 unk4;
    s32 unk4;
} unk80127BF8;
extern unk80127BF8 D_80101568_102168;

void setWeatherLimits(s16 arg0, s16 arg1) {
    if (D_80101568_102168.unk2 < D_80101568_102168.unk0) {
        D_80101568_102168.unk0 = arg0;
        D_80101568_102168.unk2 = arg1;
    } else {
        D_80101568_102168.unk0 = arg1;
        D_80101568_102168.unk2 = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/freeWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/setupWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/changeWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/doWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C0CC_5CCCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C3C8_5CFC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C6E4_5D2E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C7D0_5D3D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C858_5D458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/rainSetFog.s")

extern s32 D_800A66DC_A72DC;
extern s32 D_800A66E8_A72E8;

f32 rainDensity(void) {
    f32 var_f2;

    var_f2 = (f32) (((D_800A66E8_A72E8 >> 2) * D_800A66DC_A72DC) >> 14) / 0x10000;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (var_f2 > 1.0f) {
        var_f2 = 1.0f;
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CAE0_5D6E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CC28_5D828.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D458_5E058.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D568_5E168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D654_5E254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D6A4_5E2A4.s")
