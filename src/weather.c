#include "common.h"

typedef enum WeatherType { WEATHER_SNOW, WEATHER_RAIN, WEATHER_UNK } WeatherType;
enum ViewportCount {
    VIEWPORTS_COUNT_1_PLAYER,
    VIEWPORTS_COUNT_2_PLAYERS,
    VIEWPORTS_COUNT_3_PLAYERS,
    VIEWPORTS_COUNT_4_PLAYERS
};

#pragma GLOBAL_ASM("asm/nonmatchings/weather/initWeather.s")

typedef struct {
    s16 unk0;
    s16 unk2;
    // s16 unk4;
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

void rainSetFog(void) {
    s32 a, b;

    if (D_800A66D8 != WEATHER_SNOW && camGetMode() == VIEWPORTS_COUNT_1_PLAYER) {
        if (!(levelGetLevel()->unkA2 & 1)) {
            a = ((D_800A66DC * -38) >> 16) + 1018;
            b = ((D_800A66DC * -20) >> 16) + 1023;
            trackSetFog(0, a, b, a, 28, 15, 36, 0);
        }
    }
}

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
