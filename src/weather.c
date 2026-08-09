#include "audio.h"
#include "camera.h"
#include "common.h"
#include "functions.h"
#include "fx.h"
#include "math/math.h"
#include "models.h"
#include "textures.h"

typedef enum WeatherType { WEATHER_SNOW, WEATHER_RAIN, WEATHER_UNK } WeatherType;

enum ViewportCount {
    VIEWPORTS_COUNT_1_PLAYER,
    VIEWPORTS_COUNT_2_PLAYERS,
    VIEWPORTS_COUNT_3_PLAYERS,
    VIEWPORTS_COUNT_4_PLAYERS
};

/* Size: 0x2C Bytes */
typedef struct Unk800A5A14 {
    u8 pad0[6];
    s16 unk6;
    u32 pad8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    u8 pad1A[0x28 - 0x1A];
    f32 unk28;
} Unk800A5A14;

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
extern s32 D_801008B4_BB0F4;

// .data
extern SnowGfxData gWeatherPresets[];
extern SnowPosData *gSnowPhysics; // = NULL;
extern SnowGfxData gSnowGfx;      // = { 0, 0 };
// D_800A59D0_A65D0
extern Vertex *gSnowVerts;          // = NULL;
extern s32 gSnowVertCount;          // = 0;
extern Triangle *gSnowTriangles;    // = 0;
extern s16 *gSnowTriIndices;        // = NULL;
extern Vertex *gSnowVertexData[];   // = { 0, 0 };
extern s32 *gWeatherAssetTable;     // = NULL;
extern s8 gWeatherAssetTableLength; // = 0;
extern Unk800A5A14 D_800A5A14_A6614[];
extern f32 D_800A5CF8_A68F8;            // = -3.0f;
extern f32 D_800A5CFC_A68FC;            // = 3.0f;
extern f32 D_800A5D00_A6900;            // = 3.0f;
extern f32 D_800A5D04_A6904;            // = -3.0f;
extern f32 D_800A5D08_A6908;            // = -3.0f;
extern f32 D_800A5D0C_A690C;            // = -3.0f;
extern f32 D_800A5D10_A6910;            // = 3.0f;
extern f32 D_800A5D14_A6914;            // = 3.0f;
extern s32 gWeatherType;                // = WEATHER_SNOW;
extern s32 gLightningFrequency;         // = 0x00010000;
extern s32 D_800A5D20_A6920;            // = 0;
extern s32 D_800A5D24_A6924;            // = 0;
extern s32 gRainOpacity;                // = 0x00010000;
extern s32 D_800A5D2C_A692C;            // = 0;
extern s32 D_800A5D30_A6930;            // = 0x00010000;
extern s32 D_800A5D34_A6934;            // = 0;
extern s32 D_800A5D38_A6938;            // = 0;
extern s32 D_800A5D3C_A693C;            // = 0;
extern s32 D_800A5D40_A6940;            // = 0;
extern TextureSprite *D_800A5D44_A6944; // = NULL;
extern TextureHeader *D_800A5D48_A6948; // = NULL;
extern s32 D_800A5D4C_A694C;            // = 0;
extern SoundMask *D_800A5D50_A6950;     // = NULL;
extern UnkScreenStruct D_800A5D54_A6954;

// forward declarations
void func_8005BD30_5C930(void);          // free_rain_memory in DKR
void func_8005BC44_5C844(s32, s32, s32); // rain_init in DKR
void func_8005CBBC_5D7BC(void);          // snow_init in DKR
void func_8005BDB8_5C9B8(s32, s32, f32); // rain_set in DKR
void func_8005B62C_5C22C(void);          // snow_vertices in DKR
void func_8005B928_5C528(void);          // snow_render in DKR
void func_8005C040_5CC40(s32);           // rain_update in DKR
void func_8005CC0C_5D80C(s32);           // snow_update in DKR

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
    if (gWeatherType != WEATHER_SNOW) {
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
        func_8005BC44_5C844(arg1, arg5 + 1, arg6 + 1);
        return;
    }
    var_v1 = (u8 *) &gWeatherPresets[arg0];
    var_a0 = (s8 *) &gSnowGfx;
    var_a1 = 0x2C;
    while (var_a1--) {
        *var_a0++ = *var_v1++;
    }
    // FAKE
    if (!var_s1_2) {
        ;
    }
    gSnowGfx.pos = (Vec3i *) mmAlloc(gWeatherPresets[arg0].size * sizeof(Vec3i), COLOUR_TAG_PURPLE);
    if (gWeatherPresets[arg0].type == WEATHER_SNOW) {
        func_8005CBBC_5D7BC();
    }
    numOfElements = arg1;
    gSnowDensity = arg1;
    gSnowTriIndices = (s16 *) mmAlloc(arg1 * sizeof(s16), COLOUR_TAG_PURPLE);
    gSnowPhysics = (SnowPosData *) mmAlloc(arg1 * sizeof(SnowPosData), COLOUR_TAG_PURPLE);
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
    gSnowTriangles = (Triangle *) mmAlloc(gSnowTriCount * sizeof(Triangle), COLOUR_TAG_PURPLE);
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
    if (gWeatherType == WEATHER_SNOW) {
        gWeather.intensityStep = (arg3 - gWeather.intensity) / arg5;
        gWeather.opacityStep = (arg4 - gWeather.opacity) / arg5;
        gWeather.intensityTarget = arg3;
        gWeather.opacityTarget = arg4;
        gWeather.shiftTime = arg5;
    } else {
        gWeather.intensity = arg3;
        gWeather.opacity = arg4;
        gWeather.shiftTime = 0;
        func_8005BDB8_5C9B8(arg3 + 1, arg4 + 1, arg5 / 60.0f);
    }
}

void doWeather(Gfx **currDisplayList, Mtx **currHudMat, Vertex **currHudVerts, Triangle **currHudTris, s32 updateRate) {
    gCurrWeatherDisplayList = *currDisplayList;
    gCurrWeatherMatrix = *currHudMat;
    gCurrWeatherVertexList = *currHudVerts;
    gCurrWeatherTriList = *currHudTris;
    gWeatherCamera = camGetPtr();
    gWeatherCameraMatrix = camGetRotationMtx();
    if (gWeatherType != WEATHER_SNOW) {
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

void func_8005B62C_5C22C(void) {
    Vertex *var_s0;
    s16 temp_s5;
    s16 temp_s6;
    s16 temp_f10;
    s16 temp_f18;
    Vec3f posF;
    s16 temp_f6;
    s32 i;
    s32 camX;
    s32 camY;
    s32 camZ;

    temp_s5 = gSnowGfx.vertOffsetW;
    temp_s6 = gSnowGfx.vertOffsetH;
    camX = gWeatherCamera->trans.x_position * 65536.0f;
    camY = gWeatherCamera->trans.y_position * 65536.0f;
    camZ = gWeatherCamera->trans.z_position * 65536.0f;
    gSnowVertCount = 0;
    var_s0 = gSnowVerts;
    for (i = 0; i < gSnowParticleCount; i++) {
        posF.f[0] = (((gSnowPhysics[i].x_position - camX) & gSnowGfx.radiusX) + gSnowGfx.offsetX) * (1.0f / 65536.0f);
        posF.f[1] = (((gSnowPhysics[i].y_position - camY) & gSnowGfx.radiusY) + gSnowGfx.offsetY) * (1.0f / 65536.0f);
        posF.f[2] = (((gSnowPhysics[i].z_position - camZ) & gSnowGfx.radiusZ) + gSnowGfx.offsetZ) * (1.0f / 65536.0f);
        mathMtxFastXFMF(gWeatherCameraMatrix, posF.f, posF.f);
        temp_f6 = posF.f[2];
        if (temp_f6 < gSnowPlane.near && gSnowPlane.current < temp_f6) {
            temp_f10 = posF.f[0];
            temp_f18 = posF.f[1];
            var_s0->x = temp_f10 - temp_s5;
            var_s0->y = temp_f18 + temp_s6;
            var_s0->z = temp_f6;
            var_s0++;
            var_s0->x = temp_f10 + temp_s5;
            var_s0->z = temp_f6;
            var_s0->y = temp_f18 + temp_s6;
            var_s0++;
            var_s0->x = temp_f10 + temp_s5;
            var_s0->y = temp_f18 - temp_s6;
            var_s0->z = temp_f6;
            var_s0++;
            var_s0->x = temp_f10 - temp_s5;
            var_s0->y = temp_f18 - temp_s6;
            var_s0->z = temp_f6;
            var_s0++;
            gSnowVertCount += 4;
            gSnowTriIndices[gSnowVertCount >> 2] = i;
        }
    }
}

void func_8005B928_5C528(void) {
    s32 i;
    Mtx *mtx;
    Vertex *vtx;

    if (gSnowGfx.texture == NULL) {
        return;
    }

    gSnowVertOffset = 4;
    gSnowTriCount = 2;
    if (gSnowVertCount < 4) {
        return;
    }

    i = 0;
    mtx = camGetProjOrgMtx();
    gSPMatrixDKR(gCurrWeatherDisplayList++, OS_K0_TO_PHYSICAL(mtx), G_MTX_DKR_INDEX_0);
    gSPSelectMatrixDKR(gCurrWeatherDisplayList++, G_MTX_DKR_INDEX_0);
    texDPTextureX(&gCurrWeatherDisplayList, gSnowGfx.texture, 2, 0);
    gDPSetPrimColor(gCurrWeatherDisplayList++, 0, 0, 224, 240, 255, 160); // light blue-ish, snow color
    gDPSetEnvColor(gCurrWeatherDisplayList++, 64, 128, 255, 0);           // blue-ish, transparent?
    while (i + gSnowVertOffset < gSnowVertCount) {
        vtx = &gSnowVerts[i];
        gSPVertexJFG(gCurrWeatherDisplayList++, OS_K0_TO_PHYSICAL(vtx), gSnowVertOffset, 0);
        gSPPolygon(gCurrWeatherDisplayList++, OS_K0_TO_PHYSICAL(gSnowTriangles), gSnowTriCount, 1);
        i += gSnowVertOffset;
    }

    vtx = &gSnowVerts[i];
    gSPVertexJFG(gCurrWeatherDisplayList++, OS_K0_TO_PHYSICAL(vtx), (gSnowVertCount - i), 0);
    gSPPolygon(gCurrWeatherDisplayList++, OS_K0_TO_PHYSICAL(gSnowTriangles), ((gSnowVertCount - i) >> 1), 1);
    gDPSetPrimColor(gCurrWeatherDisplayList++, 0, 0, 255, 255, 255, 255); // all white
    gDPSetEnvColor(gCurrWeatherDisplayList++, 255, 255, 255, 255);        // all white
}

void func_8005BC44_5C844(s32 arg0, s32 arg1, s32 arg2) {
    gLightningFrequency = arg1;
    D_800A5D20_A6920 = 0;
    D_800A5D24_A6924 = gLightningFrequency;
    gRainOpacity = arg2;
    D_800A5D2C_A692C = 0;
    D_800A5D30_A6930 = gRainOpacity;
    D_800A5D34_A6934 = 0;
    D_800A5D38_A6938 = 0;
    D_800A5D3C_A693C = 0;
    D_800A5D40_A6940 = 0;
    D_800A5D4C_A694C = 0;
    TrapDanglingJump(arg0, 0x2BC, 0x2BC, 0x2BC, 0x2080E002, 0xA0E0FF04, 0x226);
    D_800A5D44_A6944 = texLoadSprite(0x4A, 0);
    D_800A5D48_A6948 = texLoadTexture(0x1AC);
    gWeatherType = 1;
    D_801008B4_BB0F4 = 0;
}

void func_8005BD30_5C930(void) {
    if (D_800A5D44_A6944 != NULL) {
        texFreeSprite(D_800A5D44_A6944);
        D_800A5D44_A6944 = NULL;
    }

    if (D_800A5D48_A6948 != NULL) {
        texFreeTexture(D_800A5D48_A6948);
        D_800A5D48_A6948 = NULL;
    }

    if (D_800A5D50_A6950 != NULL) {
        amSndStopXYZ(D_800A5D50_A6950);
        D_800A5D50_A6950 = NULL;
    }

    TrapDanglingJump();
    gWeatherType = WEATHER_SNOW;
}

void func_8005BDB8_5C9B8(s32 arg0, s32 arg1, f32 arg2) {
    if (osTvType == OS_TV_TYPE_PAL) {
        D_800A5D34_A6934 = (50.0f * arg2);
    } else {
        D_800A5D34_A6934 = (60.0f * arg2);
    }

    D_800A5D24_A6924 = arg0;
    D_800A5D20_A6920 = (D_800A5D24_A6924 - gLightningFrequency) / D_800A5D34_A6934;
    D_800A5D30_A6930 = arg1;
    D_800A5D2C_A692C = (D_800A5D30_A6930 - gRainOpacity) / D_800A5D34_A6934;
}

void rainSetFog(void) {
    s32 a, b;

    if (gWeatherType != WEATHER_SNOW && camGetMode() == VIEWPORTS_COUNT_1_PLAYER) {
        if (!(levelGetLevel()->unkA2 & 1)) {
            a = ((gLightningFrequency * -38) >> 16) + 1018;
            b = ((gLightningFrequency * -20) >> 16) + 1023;
            trackSetFog(0, a, b, a, 28, 15, 36, 0);
        }
    }
}

f32 rainDensity(void) {
    f32 var_f2;

    var_f2 = (f32) (((gRainOpacity >> 2) * gLightningFrequency) >> 14) / 0x10000;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (var_f2 > 1.0f) {
        var_f2 = 1.0f;
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C040_5CC40.s")

#ifdef NON_EQUIVALENT
extern u8 D_A5CD8[];

// https://decomp.me/scratch/bCbde
s32 hitGetHeights(f32, f32, s32, f32 ***);
Object *objGetPlayerNo(s32);
s32 trackGetHeights(f32, f32, s32, f32 ***);

void func_8005C188_5CD88(s32 arg0) {
    s32 pad;
    Object *temp_v0;
    Unk800A5A14 *var_s2;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    s32 temp_s0;
    s32 temp_t1;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s3;
    s32 var_v0_2;
    s32 pad2;
    f32 **sp88;
    f32 **sp84;

    if (D_800A5D44_A6944 == NULL || D_800A5D48_A6948 == NULL) {
        return;
    }

    temp_t1 = (s32) (((s32) gRainOpacity >> 2) * gLightningFrequency) >> 0xE;
    if (temp_t1 > 0x4000) {
        temp_v0 = objGetPlayerNo(0);
        if (temp_v0 != NULL) {
            D_800A5D40_A6940 -= arg0;
            while (D_800A5D40_A6940 <= 0) {
                var_s2 = D_800A5A14_A6614;
                var_s3 = 0x10;
                var_v0_2 = FALSE;
                while (var_s3 > 0 && var_v0_2 == FALSE) {
                    if (var_s2->unk6 == 0) {
                        var_v0_2 = TRUE;
                    } else {
                        var_s2 += 1;
                    }
                    var_s3--;
                }
                if (var_v0_2 != FALSE) {
                    temp_s0 = mathRnd(0, 0x10000);
                    temp_f20 = mathRnd(30, 300);
                    temp_f22 = (Sinf(temp_s0) * temp_f20) + temp_v0->segment.trans.x_position;
                    temp_f24 = (Cosf(temp_s0) * temp_f20) + temp_v0->segment.trans.z_position;
                    var_s0 = trackGetHeights(temp_f22, temp_f24, 0x400, &sp88);
                    temp_v0_2 = hitGetHeights(temp_f22, temp_f24, 0x400, &sp84);
                    if (var_s0 != 0 || temp_v0_2 != 0) {
                        var_s2->unkC = temp_f22;
                        var_s2->unk14 = temp_f24;
                        var_s2->unk6 = 1;
                        var_s2->unk28 = 0.0f;
                        var_s2->unk18 = mathRnd(0x40, (temp_t1 >> 0xA) + 0x60);
                        if ((var_s0 != 0) && (temp_v0_2 != 0)) {
                            var_s0 = 0;
                        }
                        if (var_s0 != 0) {
                            var_s2->unk10 = **sp88;
                            if (((s8 *) (*sp88))[0x13] == 2) {
                                var_s2->unk6 += 1;
                            }
                        } else {
                            var_s2->unk10 = **sp84;
                        }
                    }
                }
                D_800A5D40_A6940 += 2;
                if (D_800A5D40_A6940 >= 0) {
                    D_800A5D40_A6940 = (D_800A5D40_A6940 - (temp_t1 >> 0xA)) + 0x40;
                    if (D_800A5D40_A6940 < 0) {
                        D_800A5D40_A6940 = 0;
                    }
                }
            }
        }
    }
    gDPSetEnvColor(gCurrWeatherDisplayList++, 0xFF, 0xFF, 0xFF, 0);
    var_s2 = D_800A5A14_A6614;
    for (var_s3 = 0; var_s3 < 16; var_s3++) {
        if (var_s2->unk6 != 0) {
            var_s2->unk28 += arg0 * 0.175f;
            if (var_s2->unk28 < 4.0f) {
                if (var_s2->unk6 == 1) {
                    gDPSetPrimColor(gCurrWeatherDisplayList++, 0, 0, 0xFF, 0xFF, 0xFF, var_s2->unk18);
                    camDoSprite(&gCurrWeatherDisplayList, &gCurrWeatherMatrix, &gCurrWeatherVertexList, var_s2,
                                D_800A5D44_A6944, 0xE, 0);
                } else {
                    texDPTextureX(&gCurrWeatherDisplayList, D_800A5D48_A6948, 0xE, 0);
                    gDPSetPrimColor(gCurrWeatherDisplayList++, 0, 0, 0xC0, 0xE0, 0xFF, 0xFF);
                    gSPVertexJFG(gCurrWeatherDisplayList++, OS_PHYSICAL_TO_K0(gCurrWeatherVertexList), 4, 0);
                    gSPPolygon(gCurrWeatherDisplayList++, D_A5CD8, 2, 1);
                    gCurrWeatherVertexList->x = ((0, D_800A5CF8_A68F8) * var_s2->unk28) + var_s2->unkC;
                    gCurrWeatherVertexList->y = var_s2->unk10;
                    gCurrWeatherVertexList->z = ((0, D_800A5D08_A6908) * var_s2->unk28) + var_s2->unk14;
                    gCurrWeatherVertexList->r = 0xFF;
                    gCurrWeatherVertexList->g = 0xFF;
                    gCurrWeatherVertexList->b = 0xFF;
                    gCurrWeatherVertexList->a = 0xFF;
                    gCurrWeatherVertexList += 1;
                    gCurrWeatherVertexList->x = ((0, D_800A5CFC_A68FC) * var_s2->unk28) + var_s2->unkC;
                    gCurrWeatherVertexList->y = var_s2->unk10;
                    gCurrWeatherVertexList->z = ((0, D_800A5D0C_A690C) * var_s2->unk28) + var_s2->unk14;
                    gCurrWeatherVertexList->r = 0xFF;
                    gCurrWeatherVertexList->g = 0xFF;
                    gCurrWeatherVertexList->b = 0xFF;
                    gCurrWeatherVertexList->a = 0xFF;
                    gCurrWeatherVertexList += 1;
                    gCurrWeatherVertexList->x = ((0, D_800A5D00_A6900) * var_s2->unk28) + var_s2->unkC;
                    gCurrWeatherVertexList->y = var_s2->unk10;
                    gCurrWeatherVertexList->z = ((0, D_800A5D10_A6910) * var_s2->unk28) + var_s2->unk14;
                    gCurrWeatherVertexList->r = 0xFF;
                    gCurrWeatherVertexList->g = 0xFF;
                    gCurrWeatherVertexList->b = 0xFF;
                    gCurrWeatherVertexList->a = 0xFF;
                    gCurrWeatherVertexList += 1;
                    gCurrWeatherVertexList->x = ((0, D_800A5D04_A6904) * var_s2->unk28) + var_s2->unkC;
                    gCurrWeatherVertexList->y = var_s2->unk10;
                    gCurrWeatherVertexList->z = ((0, D_800A5D14_A6914) * var_s2->unk28) + var_s2->unk14;
                    gCurrWeatherVertexList->r = 0xFF;
                    gCurrWeatherVertexList->g = 0xFF;
                    gCurrWeatherVertexList->b = 0xFF;
                    gCurrWeatherVertexList->a = 0xFF;
                    gCurrWeatherVertexList += 1;
                }
            } else {
                var_s2->unk6 = 0;
            }
        }
        var_s2 += 1;
    }

    gDPSetPrimColor(gCurrWeatherDisplayList++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005C188_5CD88.s")
#endif

void func_8005C9B8_5D5B8(s32 arg0) {
    s32 sp1C;

    if (D_800A5D3C_A693C > 0) {
        D_800A5D3C_A693C -= arg0;
        if (D_800A5D3C_A693C <= 0) {
            if (gRainOpacity > 0x8000) {
                if (fxFadeOn() == 0) {
                    setupClearScreen(&D_800A5D54_A6954);
                    D_800A6784_A7384 = 0;
                }
            }
            amSndPlay(6, 0);
            D_800A5D3C_A693C = 0;
        }
    } else if (gLightningFrequency >= 0xC001) {
        if (D_800A5D38_A6938 > 0) {
            D_800A5D38_A6938 -= arg0;
        } else {
            sp1C = (s32) ((gLightningFrequency * 0x258) + 0xFE3E0000) >> 0xE;
            amSndPlay(7, 0);
            D_800A5D3C_A693C = sp1C + 0x3C;
            D_800A5D38_A6938 = mathRnd(0x4B0, 0x5DC) - sp1C;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CAD0_5D6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CBBC_5D7BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/weather/func_8005CC0C_5D80C.s")
