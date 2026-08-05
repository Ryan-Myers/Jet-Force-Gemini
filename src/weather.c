#include "common.h"
#include "models.h"

typedef enum WeatherType { WEATHER_SNOW, WEATHER_RAIN, WEATHER_UNK } WeatherType;

enum ViewportCount {
    VIEWPORTS_COUNT_1_PLAYER,
    VIEWPORTS_COUNT_2_PLAYERS,
    VIEWPORTS_COUNT_3_PLAYERS,
    VIEWPORTS_COUNT_4_PLAYERS
};

typedef struct {
    s16 near;
    s16 far;
    s32 current;
} SnowLimits;

typedef struct SnowPosData {
    s32 x_position;
    s32 y_position;
    s32 z_position;
    u8 unused_C;
    u8 unused_D;
    u8 unused_E;
    u8 index;
} SnowPosData;

typedef struct WeatherData {
    s32 intensity;
    s32 intensityStep;
    s32 intensityTarget;
    s32 velX;
    s32 velXStep;
    s32 velXTarget;
    s32 velY;
    s32 velYStep;
    s32 velYTarget;
    s32 velZ;
    s32 velZStep;
    s32 velZTarget;
    s32 opacity;
    s32 opacityStep;
    s32 opacityTarget;
    s32 shiftTime;
} WeatherData;

/* Size: 0x2C Bytes */
typedef struct SnowGfxData {
    Vec3i *pos;
    s32 size;
    union {
        WeatherType type;
        TextureHeader *texture;
    };
    s32 offsetX;
    s32 offsetY;
    s32 offsetZ;
    s32 radiusX;
    s32 radiusY;
    s32 radiusZ;
    s16 vertOffsetW;
    s16 vertOffsetH;
    s16 vertWidth;
    s16 vertHeight;
} SnowGfxData;

// .bss
extern s32 D_80100840_BB080;
extern SnowLimits D_80100888_BB0C8;
extern s32 D_80100890_BB0D0;
extern s32 D_80100894_BB0D4;
extern s8 D_80100898_BB0D8;
extern WeatherData D_80100848_BB088; // gWeather in DKR

// .data
extern SnowPosData *D_800A59C4_A65C4;  // = 0; // gSnowPhysics in DKR
extern SnowGfxData D_800A59C8_A65C8;   // = { 0, 0 }; // gSnowGfx in DKR
extern s32 D_800A59FC_A65FC;           // = 0; // gSnowTriangles in DKR
extern s16 *D_800A5A00_A6600;          // = NULL; // gSnowTriIndices in DKR
extern Vertex *D_800A5A04_A6604[];     // = { 0, 0 }; // gSnowVertexData in DKR
extern s32 *D_800A5A0C_A660C;          // = 0; // gWeatherAssetTable in DKR
extern s8 D_800A5A10_A6610;            // = 0; // gWeatherAssetTableLength in DKR
extern SnowGfxData D_800A5940_A6540[]; // gWeatherPresets in DKR

void func_8005BD30_5C930(void);               // free_rain_memory in DKR
void func_8005BC44_5C844(s32, s32, s32, s32); // rain_init in DKR
void func_8005CBBC_5D7BC(void);               // snow_init in DKR

void initWeather(void) {
    s32 *temp_v0;

    D_800A59C8_A65C8.pos = NULL;
    D_800A59C8_A65C8.size = 0;
    D_800A59C4_A65C4 = 0;
    D_80100840_BB080 = 0;
    D_80100890_BB0D0 = 6;
    D_80100890_BB0D0 <<= 2;
    D_80100894_BB0D4 = D_80100890_BB0D0 >> 1;
    D_800A5A04_A6604[0] = 0;
    D_800A5A04_A6604[1] = 0;
    D_800A59FC_A65FC = 0;
    D_80100888_BB0C8.near = -1;
    D_80100888_BB0C8.far = -0x200;
    if (D_800A5A0C_A660C == NULL) {
        temp_v0 = (s32 *) piRomLoad(0x1BU);
        D_800A5A10_A6610 = 0;
        D_800A5A0C_A660C = temp_v0;
        while (D_800A5A0C_A660C[D_800A5A10_A6610] != -1) {
            D_800A5A10_A6610++;
        }
    }

    D_80100898_BB0D8 = 0;
}

void setWeatherLimits(s16 near, s16 far) {
    if (D_80100888_BB0C8.far < D_80100888_BB0C8.near) {
        D_80100888_BB0C8.near = near;
        D_80100888_BB0C8.far = far;
    } else {
        D_80100888_BB0C8.near = far;
        D_80100888_BB0C8.far = near;
    }
}

#define FREE_MEM(mem)      \
    tempMem = (s32 *) mem; \
    if (tempMem != NULL) { \
        mmFree(tempMem);   \
        mem = NULL;        \
    }
#define FREE_TEX(tex)            \
    tempTex = tex;               \
    if (tempTex != NULL) {       \
        texFreeTexture(tempTex); \
        tex = NULL;              \
    }

void freeWeather(void) {
    TextureHeader *tempTex;
    s32 *tempMem;

    FREE_MEM(D_800A59FC_A65FC);
    FREE_MEM(D_800A5A04_A6604[0]);
    FREE_MEM(D_800A5A04_A6604[1]);
    FREE_MEM(D_800A59C4_A65C4);
    FREE_MEM(D_800A59C8_A65C8.pos);
    FREE_TEX(D_800A59C8_A65C8.texture);
    FREE_MEM(D_800A5A00_A6600);
    if (D_800A5D18_A6918 != NULL) {
        func_8005BD30_5C930();
    }
}

void setupWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s16 temp_s1_2;
    s16 temp_s2;
    s32 temp_s1;
    u8 var_a1;
    s32 i;
    SnowPosData *var_s1_2;
    Triangle *var_v1_2;
    Vertex *var_a3;
    s32 j;
    s8 *var_a0;
    u8 *var_v1;
    s32 pad;
    s32 numOfElements;

    freeWeather();
    D_80100848_BB088.velX = arg2;
    D_80100848_BB088.velXStep = 0;
    D_80100848_BB088.velXTarget = arg2;
    D_80100848_BB088.velY = arg3;
    D_80100848_BB088.velYStep = 0;
    D_80100848_BB088.velYTarget = arg3;
    D_80100848_BB088.velZStep = 0;
    D_80100848_BB088.intensityStep = 0;
    D_80100848_BB088.opacityStep = 0;
    D_80100848_BB088.shiftTime = 0;
    D_80100848_BB088.velZ = arg4;
    D_80100848_BB088.velZTarget = arg4;
    D_80100848_BB088.intensity = arg5;
    D_80100848_BB088.intensityTarget = arg5;
    D_80100848_BB088.opacity = arg6;
    D_80100848_BB088.opacityTarget = arg6;
    if (arg0 > 1) {
        arg0 = 1;
    }
    if (D_800A5940_A6540[arg0].type == WEATHER_RAIN) {
        func_8005BC44_5C844(arg1, arg5 + 1, arg6 + 1, arg5);
        return;
    }
    var_v1 = &D_800A5940_A6540[arg0];
    var_a0 = (s8 *) &D_800A59C8_A65C8;
    var_a1 = 0x2C;
    while (var_a1--) {
        *var_a0++ = *var_v1++;
    }
    if (!var_s1_2) {
        ;
    }
    D_800A59C8_A65C8.pos = mmAlloc(D_800A5940_A6540[arg0].size * sizeof(Vec3i), COLOUR_TAG_PURPLE);
    if (D_800A5940_A6540[arg0].type == WEATHER_SNOW) {
        func_8005CBBC_5D7BC();
    }
    numOfElements = arg1;
    D_80100840_BB080 = arg1;
    D_800A5A00_A6600 = mmAlloc(arg1 * sizeof(s16), COLOUR_TAG_PURPLE);
    D_800A59C4_A65C4 = mmAlloc(arg1 * sizeof(SnowPosData), COLOUR_TAG_PURPLE);
    var_s1_2 = D_800A59C4_A65C4;
    for (i = 0; i < D_80100840_BB080; i++) {
        var_s1_2->x_position = mathRnd(0, D_800A59C8_A65C8.radiusX);
        var_s1_2->y_position = mathRnd(0, D_800A59C8_A65C8.radiusY);
        var_s1_2->z_position = mathRnd(0, D_800A59C8_A65C8.radiusZ);
        var_s1_2->unused_C = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->unused_D = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->unused_E = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->index = mathRnd(0, D_800A59C8_A65C8.size - 1);
        var_s1_2++;
    }
    numOfElements *= 4;
    temp_s1 = sizeof(Vertex);
    temp_s1 *= numOfElements;
    D_800A5A04_A6604[0] = (Vertex *) mmAlloc(temp_s1, COLOUR_TAG_PURPLE);
    D_800A5A04_A6604[1] = (Vertex *) mmAlloc(temp_s1, COLOUR_TAG_PURPLE);
    for (j = 0; j < 2; j++) {
        var_a3 = D_800A5A04_A6604[j];
        for (i = 0; i < numOfElements; i++, var_a3++) {
            var_a3->r = 255;
            var_a3->g = 255;
            var_a3->b = 255;
            var_a3->a = 255;
        }
    }
    temp_s1_2 = (D_800A59C8_A65C8.texture->width << 5) - 1;
    temp_s2 = (D_800A59C8_A65C8.texture->height << 5) - 1;
    D_800A59FC_A65FC = mmAlloc(D_80100894_BB0D4 * sizeof(Triangle), COLOUR_TAG_PURPLE);
    var_v1_2 = D_800A59FC_A65FC;
    for (i = 0; i < D_80100894_BB0D4; i += 2) {
        var_v1_2[0].flags = 0;
        var_v1_2[0].vi0 = (i << 1) + 3;
        var_v1_2[0].uv0.u = 0;
        var_v1_2[0].uv0.v = temp_s2;
        var_v1_2[0].vi1 = (i << 1) + 1;
        var_v1_2[0].uv1.u = temp_s1_2;
        var_v1_2[0].uv1.v = 0;
        var_v1_2[0].vi2 = (i << 1);
        var_v1_2[0].uv2.u = 0;
        var_v1_2[0].uv2.v = 0;
        var_v1_2[1].flags = 0;
        var_v1_2[1].vi0 = (i << 1) + 3;
        var_v1_2[1].uv0.u = 0;
        var_v1_2[1].uv0.v = temp_s2;
        var_v1_2[1].vi1 = (i << 1) + 2;
        var_v1_2[1].uv1.u = temp_s1_2;
        var_v1_2[1].uv1.v = temp_s2;
        var_v1_2[1].vi2 = (i << 1) + 1;
        var_v1_2[1].uv2.u = temp_s1_2;
        var_v1_2[1].uv2.v = 0;
        var_v1_2 += 2;
    }
    D_80100898_BB0D8 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/changeWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/doWeather.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005B62C_5C22C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005B928_5C528.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005BC44_5C844.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005BD30_5C930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005BDB8_5C9B8.s")

void rainSetFog(void) {
    s32 a, b;

    if (D_800A5D18_A6918 != WEATHER_SNOW && camGetMode() == VIEWPORTS_COUNT_1_PLAYER) {
        if (!(levelGetLevel()->unkA2 & 1)) {
            a = ((D_800A5D1C_A691C * -38) >> 16) + 1018;
            b = ((D_800A5D1C_A691C * -20) >> 16) + 1023;
            trackSetFog(0, a, b, a, 28, 15, 36, 0);
        }
    }
}

f32 rainDensity(void) {
    f32 var_f2;

    var_f2 = (f32) (((D_800A5D28_A6928 >> 2) * D_800A5D1C_A691C) >> 14) / 0x10000;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (var_f2 > 1.0f) {
        var_f2 = 1.0f;
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C040_5CC40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C188_5CD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C9B8_5D5B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CAD0_5D6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CBBC_5D7BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CC0C_5D80C.s")
