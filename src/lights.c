#include "lights.h"
#include "camlight.h"
#include "common.h"
#include "math/math.h"
#include "memory.h"
#include "textures.h"

extern void *D_800A1010_A1C10;
extern ObjectLightUnk70 *D_800F5B20_F6720;

void freeLights(void) {
    if (D_800A1008_A1C08 != NULL) {
        mmFree(D_800A1008_A1C08);
        D_800A1008_A1C08 = NULL;
        D_800A100C_A1C0C = 0;
    }
    if (D_800F5B20_F6720 != NULL) {
        mmFree(D_800F5B20_F6720);
        D_800F5B20_F6720 = NULL;
    }
    if (D_800A1010_A1C10 != NULL) {
        mmFree(D_800A1010_A1C10);
        D_800A1010_A1C10 = NULL;
    }
    D_800A1004_A1C04 = 0;
    D_800A1000_A1C00 = 0;
}

#ifdef NON_MATCHING
extern s32 D_800A1000_A1C00; // gMaxLights
extern void *D_800A1010_A1C10;

// Reasonably certain this 0x200 is the size of ObjectLightUnk70
#define SIZE_0X200 sizeof(ObjectLightUnk70)

void setupLights(s32 count, UNUSED s32 arg1, UNUSED s32 arg2) {
    s32 i;
    ObjectLight **buffer;

    freeLights();
    D_800A1000_A1C00 = count;
    buffer = mmAlloc(D_800A1000_A1C00 * (sizeof(s32 *) + sizeof(ObjectLight)), COLOUR_TAG_MAGENTA);
    D_800F5B20_F6720 = mmAlloc((D_800A1000_A1C00 * SIZE_0X200) + SIZE_0X200, COLOUR_TAG_MAGENTA);
    D_800A1010_A1C10 = mmAlloc(SIZE_0X200 + 0x40, COLOUR_TAG_MAGENTA);
    D_800A1008_A1C08 = buffer;
    D_800A100C_A1C0C = (ObjectLight *) &buffer[D_800A1000_A1C00];
    for (i = 0; i < D_800A1000_A1C00; i++) {
        D_800A1008_A1C08[i] = &D_800A100C_A1C0C[i];
        D_800A100C_A1C0C[i].unk70 = &D_800F5B20_F6720[(i * SIZE_0X200)];
    }
    lightCreateLightTable(0xFF, 0xFF, 0xFF, D_800F5B20_F6720);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lights/setupLights.s")
#endif

extern f32 D_800AC360; // = 0.3000000119f
void *trackLightAdd(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80020D84_21984(ObjectLight *arg0) {
    if (!(arg0->type & 0x40)) {
        arg0->unk6C = trackLightAdd(arg0->pos.z, arg0->unk1C, arg0->unk20, arg0->unk24 * 1.25f,
                                    arg0->unk24 * 0.3000000119f, (s32) (arg0->unk40 * arg0->unk43) >> 8,
                                    (s32) (arg0->unk41 * arg0->unk43) >> 8, (s32) (arg0->unk42 * arg0->unk43) >> 8);
    } else {
        arg0->unk6C = NULL;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/addRomdefLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/addObjectLight.s")

void turnLightOff(UnkLight *light) {
    light->unk3 &= ~1;
}

void turnLightOn(UnkLight *light) {
    light->unk3 |= 1;
}

void toggleLight(UnkLight *light) {
    light->unk3 ^= 1;
}

void changeLightColour(UnkLight *light, u8 red, u8 green, u8 blue) {
    light->red = red;
    light->green = green;
    light->blue = blue;
    light->unk2 |= 2;
    light->unk54 = 0;
}

void changeLightColourCycle(s32 arg0, s32 arg1) {
    initColourCycle(arg0 + 0x48, arg1);
}

void changeLightIntensity(UnkLight *light, u8 intensity) {
    light->unk43 = intensity;
    light->unk44 = intensity;
    if (light->unk6C != 0) {
        light->unk2 |= 2;
    }
}

void lightUpdateLights(s32 arg0) {
    s32 i;
    for (i = 0; i < D_800A1004_A1C04; i++) {
        func_80021434_22034(D_800A1008_A1C08[i], arg0);
    }
}

// Same as func_80032424 in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021434_22034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/killLight.s")

UNUSED unk800DC950 **lightGetLights(s32 *arg0) {
    *arg0 = D_800A1004_A1C04;
    return D_800A1008_A1C08;
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightGetStrongestEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightUpdateObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021B8C_2278C.s")

f32 lightDistanceCalc(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    f32 temp;

    temp = arg1 * arg2;
    switch (arg3) {
        case 1:
            temp = 1.0f - temp;
            break;
        case 2:
            temp = 1.0f - sqrtf(temp);
            break;
        case 3:
            temp = Cosf(temp * 16384.0f);
            break;
        case 4:
            temp = Cosf(temp * 16384.0f);
            temp *= temp;
            break;
        case 5:
            temp = 1.0f - temp;
            temp *= temp;
            break;
    }
    return arg0 * temp;
}

f32 lightDirectionCalc(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp_f0;
    f32 var_f2;

    if (arg6 > 0.0f) {
        temp_f0 = 1.0f / arg6;
        var_f2 = (arg3 * temp_f0 * arg0) + (arg4 * temp_f0 * arg1) + (arg5 * temp_f0 * arg2);
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
    } else {
        var_f2 = 1.0f;
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightDefaultObjectLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightSetObjectLight.s")

void lightSetupLightSources(Object *obj) {
    s32 i;
    for (i = 0; i < obj->segment.header->unk25; i++) {
        obj->unk74[i] = addObjectLight(obj, &obj->segment.header->unk48[i]);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightSetupFlareSources.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightInitObjectLighting.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightAdjustGlowingLight.s")

s32 lightKillGlowingLight(void *arg0) {
    camlightDelete(arg0);
    return 1;
}
