#include "camera.h"
#include "common.h"
#include "models.h"

typedef enum WeatherType { WEATHER_SNOW, WEATHER_RAIN, WEATHER_UNK } WeatherType;

enum ViewportCount {
    VIEWPORTS_COUNT_1_PLAYER,
    VIEWPORTS_COUNT_2_PLAYERS,
    VIEWPORTS_COUNT_3_PLAYERS,
    VIEWPORTS_COUNT_4_PLAYERS
};

/* Size: 0x8 Bytes */
typedef struct {
    /* 0x00 */ s16 near;
    /* 0x02 */ s16 far;
    /* 0x04 */ s32 current;
} SnowLimits;

/* Size: 0x10 Bytes */
typedef struct SnowPosData {
    /* 0x00 */ s32 x_position;
    /* 0x04 */ s32 y_position;
    /* 0x08 */ s32 z_position;
    /* 0x0C */ u8 unused_C;
    /* 0x0D */ u8 unused_D;
    /* 0x0E */ u8 unused_E;
    /* 0x0F */ u8 index;
} SnowPosData;

/* Size: 0x40 Bytes */
typedef struct WeatherData {
    /* 0x00 */ s32 intensity;
    /* 0x04 */ s32 intensityStep;
    /* 0x08 */ s32 intensityTarget;
    /* 0x0C */ s32 velX;
    /* 0x10 */ s32 velXStep;
    /* 0x14 */ s32 velXTarget;
    /* 0x18 */ s32 velY;
    /* 0x1C */ s32 velYStep;
    /* 0x20 */ s32 velYTarget;
    /* 0x24 */ s32 velZ;
    /* 0x28 */ s32 velZStep;
    /* 0x2C */ s32 velZTarget;
    /* 0x30 */ s32 opacity;
    /* 0x34 */ s32 opacityStep;
    /* 0x38 */ s32 opacityTarget;
    /* 0x3C */ s32 shiftTime;
} WeatherData;

/* Size: 0x2C Bytes */
typedef struct SnowGfxData {
    /* 0x00 */ Vec3i *pos;
    /* 0x04 */ s32 size;
    union {
        /* 0x08 */ WeatherType type;
        /* 0x08 */ TextureHeader *texture;
    };
    /* 0x0C */ s32 offsetX;
    /* 0x10 */ s32 offsetY;
    /* 0x14 */ s32 offsetZ;
    /* 0x18 */ s32 radiusX;
    /* 0x1C */ s32 radiusY;
    /* 0x20 */ s32 radiusZ;
    /* 0x24 */ s16 vertOffsetW;
    /* 0x26 */ s16 vertOffsetH;
    /* 0x28 */ s16 vertWidth;
    /* 0x2A */ s16 vertHeight;
} SnowGfxData;

// .bss
extern s32 gSnowDensity;
extern s32 gSnowParticleCount;
extern WeatherData gWeather;
extern SnowLimits gSnowPlane;
extern s32 gSnowVertOffset;
extern s32 gSnowTriCount;
extern s8 gSnowVertexFlip;
extern Gfx *gCurrWeatherDisplayList;
extern Mtx *gCurrWeatherMatrix;
extern Vertex *gCurrWeatherVertexList;
extern Triangle *gCurrWeatherTriList;
extern Camera *gWeatherCamera;
extern Matrix *gWeatherCameraMatrix;

// .data
extern SnowPosData *gSnowPhysics;   // = 0;
extern SnowGfxData gSnowGfx;        // = { 0, 0 };
extern s32 gSnowTriangles;          // = 0;
extern s16 *gSnowTriIndices;        // = NULL;
extern Vertex *gSnowVertexData[];   // = { 0, 0 };
extern s32 *gWeatherAssetTable;     // = 0;
extern s8 gWeatherAssetTableLength; // = 0;
extern SnowGfxData gWeatherPresets[];
extern Vertex *gSnowVerts; // = 0;

// forward declarations
void func_8005BD30_5C930(void);               // free_rain_memory in DKR
void func_8005BC44_5C844(s32, s32, s32, s32); // rain_init in DKR
void func_8005CBBC_5D7BC(void);               // snow_init in DKR
void func_8005BDB8_5C9B8(s32, s32, f32);      // rain_set in DKR
void func_8005B62C_5C22C(void);               // snow_vertices in DKR
void func_8005B928_5C528(void);               // snow_render in DKR
void func_8005C040_5CC40(s32);                // rain_update in DKR
void func_8005CC0C_5D80C(s32);                // snow_update in DKR

void initWeather(void) {
    s32 *temp_v0;

    gSnowGfx.pos = NULL;
    gSnowGfx.size = 0;
    gSnowPhysics = 0;
    gSnowDensity = 0;
    gSnowVertOffset = 6;
    gSnowVertOffset <<= 2;
    gSnowTriCount = gSnowVertOffset >> 1;
    gSnowVertexData[0] = 0;
    gSnowVertexData[1] = 0;
    gSnowTriangles = 0;
    gSnowPlane.near = -1;
    gSnowPlane.far = -0x200;
    if (gWeatherAssetTable == NULL) {
        temp_v0 = (s32 *) piRomLoad(0x1BU);
        gWeatherAssetTableLength = 0;
        gWeatherAssetTable = temp_v0;
        while (gWeatherAssetTable[gWeatherAssetTableLength] != -1) {
            gWeatherAssetTableLength++;
        }
    }

    gSnowVertexFlip = 0;
}

void setWeatherLimits(s16 near, s16 far) {
    if (gSnowPlane.far < gSnowPlane.near) {
        gSnowPlane.near = near;
        gSnowPlane.far = far;
    } else {
        gSnowPlane.near = far;
        gSnowPlane.far = near;
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

    FREE_MEM(gSnowTriangles);
    FREE_MEM(gSnowVertexData[0]);
    FREE_MEM(gSnowVertexData[1]);
    FREE_MEM(gSnowPhysics);
    FREE_MEM(gSnowGfx.pos);
    FREE_TEX(gSnowGfx.texture);
    FREE_MEM(gSnowTriIndices);
    if (D_800A5D18_A6918 != WEATHER_SNOW) {
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
    gWeather.velX = arg2;
    gWeather.velXStep = 0;
    gWeather.velXTarget = arg2;
    gWeather.velY = arg3;
    gWeather.velYStep = 0;
    gWeather.velYTarget = arg3;
    gWeather.velZStep = 0;
    gWeather.intensityStep = 0;
    gWeather.opacityStep = 0;
    gWeather.shiftTime = 0;
    gWeather.velZ = arg4;
    gWeather.velZTarget = arg4;
    gWeather.intensity = arg5;
    gWeather.intensityTarget = arg5;
    gWeather.opacity = arg6;
    gWeather.opacityTarget = arg6;
    if (arg0 > 1) {
        arg0 = 1;
    }
    if (gWeatherPresets[arg0].type == WEATHER_RAIN) {
        func_8005BC44_5C844(arg1, arg5 + 1, arg6 + 1, arg5);
        return;
    }
    var_v1 = &gWeatherPresets[arg0];
    var_a0 = (s8 *) &gSnowGfx;
    var_a1 = 0x2C;
    while (var_a1--) {
        *var_a0++ = *var_v1++;
    }
    // FAKE
    if (!var_s1_2) {
        ;
    }
    gSnowGfx.pos = mmAlloc(gWeatherPresets[arg0].size * sizeof(Vec3i), COLOUR_TAG_PURPLE);
    if (gWeatherPresets[arg0].type == WEATHER_SNOW) {
        func_8005CBBC_5D7BC();
    }
    numOfElements = arg1;
    gSnowDensity = arg1;
    gSnowTriIndices = mmAlloc(arg1 * sizeof(s16), COLOUR_TAG_PURPLE);
    gSnowPhysics = mmAlloc(arg1 * sizeof(SnowPosData), COLOUR_TAG_PURPLE);
    var_s1_2 = gSnowPhysics;
    for (i = 0; i < gSnowDensity; i++) {
        var_s1_2->x_position = mathRnd(0, gSnowGfx.radiusX);
        var_s1_2->y_position = mathRnd(0, gSnowGfx.radiusY);
        var_s1_2->z_position = mathRnd(0, gSnowGfx.radiusZ);
        var_s1_2->unused_C = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->unused_D = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->unused_E = 1 << (mathRnd(0, 0x20) + 5);
        var_s1_2->index = mathRnd(0, gSnowGfx.size - 1);
        var_s1_2++;
    }
    numOfElements *= 4;
    temp_s1 = sizeof(Vertex);
    temp_s1 *= numOfElements;
    gSnowVertexData[0] = (Vertex *) mmAlloc(temp_s1, COLOUR_TAG_PURPLE);
    gSnowVertexData[1] = (Vertex *) mmAlloc(temp_s1, COLOUR_TAG_PURPLE);
    for (j = 0; j < 2; j++) {
        var_a3 = gSnowVertexData[j];
        for (i = 0; i < numOfElements; i++, var_a3++) {
            var_a3->r = 255;
            var_a3->g = 255;
            var_a3->b = 255;
            var_a3->a = 255;
        }
    }
    temp_s1_2 = (gSnowGfx.texture->width << 5) - 1;
    temp_s2 = (gSnowGfx.texture->height << 5) - 1;
    gSnowTriangles = mmAlloc(gSnowTriCount * sizeof(Triangle), COLOUR_TAG_PURPLE);
    var_v1_2 = gSnowTriangles;
    for (i = 0; i < gSnowTriCount; i += 2) {
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
    gSnowVertexFlip = 0;
}

void changeWeather(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if (arg5 <= 0) {
        return;
    }

    if (arg0 == gWeather.velXTarget && arg1 == gWeather.velYTarget && arg2 == gWeather.velZTarget &&
        arg3 == gWeather.intensity && arg4 == gWeather.opacity) {
        return;
    }

    gWeather.velXTarget = arg0;
    gWeather.velYTarget = arg1;
    gWeather.velZTarget = arg2;
    gWeather.velXStep = (arg0 - gWeather.velX) / arg5;
    gWeather.velYStep = (arg1 - gWeather.velY) / arg5;
    gWeather.velZStep = (arg2 - gWeather.velZ) / arg5;
    if (D_800A5D18_A6918 == WEATHER_SNOW) {
        gWeather.intensityStep = (arg3 - gWeather.intensity) / arg5;
        gWeather.opacityStep = (arg4 - gWeather.opacity) / arg5;
        gWeather.intensityTarget = arg3;
        gWeather.opacityTarget = arg4;
        gWeather.shiftTime = arg5;
    } else {
        gWeather.intensity = arg3;
        gWeather.opacity = arg4;
        gWeather.shiftTime = 0;
        func_8005BDB8_5C9B8(arg3 + 1, arg4 + 1, (f32) arg5 / 60.0f);
    }
}

void doWeather(Gfx **currDisplayList, Mtx **currHudMat, Vertex **currHudVerts, Triangle **currHudTris, s32 updateRate) {
    gCurrWeatherDisplayList = *currDisplayList;
    gCurrWeatherMatrix = *currHudMat;
    gCurrWeatherVertexList = *currHudVerts;
    gCurrWeatherTriList = *currHudTris;
    gWeatherCamera = camGetPtr();
    gWeatherCameraMatrix = camGetRotationMtx();
    if (D_800A5D18_A6918 != WEATHER_SNOW) {
        func_8005C040_5CC40(updateRate);
    } else {
        if (gWeather.shiftTime > 0) {
            if (updateRate < gWeather.shiftTime) {
                gWeather.intensity += gWeather.intensityStep * updateRate;
                gWeather.velX += gWeather.velXStep * updateRate;
                gWeather.velY += gWeather.velYStep * updateRate;
                gWeather.velZ += gWeather.velZStep * updateRate;
                gWeather.opacity += gWeather.opacityStep * updateRate;
                gWeather.shiftTime -= updateRate;
            } else {
                gWeather.intensity = gWeather.intensityTarget;
                gWeather.velX = gWeather.velXTarget;
                gWeather.velY = gWeather.velYTarget;
                gWeather.velZ = gWeather.velZTarget;
                gWeather.opacity = gWeather.opacityTarget;
                gWeather.shiftTime = 0;
            }
        }
        gSnowParticleCount = (gSnowDensity * gWeather.intensity) >> 16;
        gSnowPlane.current = (gSnowPlane.near + ((gSnowPlane.far - gSnowPlane.near) * gWeather.opacity)) >> 16;
        func_8005CC0C_5D80C(updateRate);
        if (gSnowParticleCount > 0 && gSnowPlane.current < gSnowPlane.near) {
            gSnowVerts = gSnowVertexData[gSnowVertexFlip];
            func_8005B62C_5C22C();
            func_8005B928_5C528();
            gSnowVertexFlip = 1 - gSnowVertexFlip;
        }
    }
    *currDisplayList = gCurrWeatherDisplayList;
    *currHudMat = gCurrWeatherMatrix;
    *currHudVerts = gCurrWeatherVertexList;
    *currHudTris = gCurrWeatherTriList;
}

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
