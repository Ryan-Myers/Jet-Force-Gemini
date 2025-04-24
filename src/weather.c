#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/weather/initWeather.s")

typedef struct {
    s16 unk0;
    s16 unk2;
    //s16 unk4;
    s32 unk4;
} unk80127BF8;
extern unk80127BF8 D_80101568;

void setWeatherLimits(s16 arg0, s16 arg1) {
    if (D_80101568.unk2 < D_80101568.unk0) {
        D_80101568.unk0 = arg0;
        D_80101568.unk2 = arg1;
    } else {
        D_80101568.unk0 = arg1;
        D_80101568.unk2 = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/freeWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/setupWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/changeWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/doWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C0CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C3C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C6E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C858.s")

s32 get_current_viewport(void);
void trackSetFog(s32 fogIdx, s16 near, s16 far, u8 red, u8 green, u8 blue);

#pragma GLOBAL_ASM("asm/nonmatchings/weather/rainSetFog.s")

extern s32 D_800A66DC;
extern s32 D_800A66E8;

f32 rainDensity(void) {
    f32 var_f2;

    var_f2 = (f32) (((D_800A66E8 >> 2) * D_800A66DC) >> 14) / 0x10000;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (var_f2 > 1.0f) {
        var_f2 = 1.0f;
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CAE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CC28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D568.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D654.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005D6A4.s")
