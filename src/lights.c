#include "common.h"
#include "math.h"

extern void *D_800A18A0;
extern ObjectLightUnk70 *D_800F65E0;

void freeLights(void) {
    if (D_800A1898 != NULL) {
        mmFree(D_800A1898);
        D_800A1898 = NULL;
        D_800A189C = 0;
    }
    if (D_800F65E0 != NULL) {
        mmFree(D_800F65E0);
        D_800F65E0 = NULL;
    }
    if (D_800A18A0 != NULL) {
        mmFree(D_800A18A0);
        D_800A18A0 = NULL;
    }
    D_800A1894 = 0;
    D_800A1890 = 0;
}

#ifdef NON_MATCHING
extern s32 D_800A1890; // gMaxLights
extern void *D_800A18A0;

// Reasonably certain this 0x200 is the size of ObjectLightUnk70
#define SIZE_0X200 sizeof(ObjectLightUnk70)

void setupLights(s32 count, UNUSED s32 arg1, UNUSED s32 arg2) {
    s32 i;
    ObjectLight **buffer;

    freeLights();
    D_800A1890 = count;
    buffer = mmAlloc(D_800A1890 * (sizeof(s32 *) + sizeof(ObjectLight)), COLOUR_TAG_MAGENTA);
    D_800F65E0 = mmAlloc((D_800A1890 * SIZE_0X200) + SIZE_0X200, COLOUR_TAG_MAGENTA);
    D_800A18A0 = mmAlloc(SIZE_0X200 + 0x40, COLOUR_TAG_MAGENTA);
    D_800A1898 = buffer;
    D_800A189C = (ObjectLight *) &buffer[D_800A1890];
    for (i = 0; i < D_800A1890; i++) {
        D_800A1898[i] = &D_800A189C[i];
        D_800A189C[i].unk70 = &D_800F65E0[(i * SIZE_0X200)];
    }
    lightCreateLightTable(0xFF, 0xFF, 0xFF, D_800F65E0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lights/setupLights.s")
#endif

extern f32 D_800AC360; // = 0.3000000119f
void *trackLightAdd(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80020D94(ObjectLight *arg0) {
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
    for (i = 0; i < D_800A1894; i++) {
        func_80021444(D_800A1898[i], arg0);
    }
}

// Same as func_80032424 in DKR
#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021444.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/killLight.s")

UNUSED unk800DC950 **lightGetLights(s32 *arg0) {
    *arg0 = D_800A1894;
    return D_800A1898;
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightGetStrongestEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightUpdateObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021B9C.s")

#ifdef NON_EQUIVALENT
// Matching, but needs rodata migration for the jump table.
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
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightDistanceCalc.s")
#endif

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
