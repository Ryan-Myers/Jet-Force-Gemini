#include "common.h"
#include "math.h"

#pragma GLOBAL_ASM("asm/nonmatchings/lights/freeLights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/setupLights.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80020D94_21994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/addRomdefLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/addObjectLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/turnLightOff.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/turnLightOn.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/toggleLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/changeLightColour.s")

void changeLightColourCycle(s32 arg0, s32 arg1) {
    initColourCycle(arg0 + 0x48, arg1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/changeLightIntensity.s")

void lightUpdateLights(s32 arg0) {
    s32 i;
    for (i = 0; i < D_800A1894_A2494; i++) {
        func_80021444_22044(D_800A1898_A2498[i], arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021444_22044.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/killLight.s")

UNUSED unk800DC950 **lightGetLights(s32 *arg0) {
    *arg0 = D_800A1894_A2494;
    return D_800A1898_A2498;
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightGetStrongestEffect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightUpdateObjects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/func_80021B9C_2279C.s")

#ifdef NON_EQUIVALENT
//Matching, but needs rodata migration for the jump table.
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

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightDirectionCalc.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightObject.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightDefaultObjectLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightSetObjectLight.s")

void lightSetupLightSources(Object *obj) { 
    s32 i;
    for(i = 0; i < obj->segment.header->unk25; i++) { 
        obj->unk74[i] = addObjectLight(obj, &obj->segment.header->unk48[i]);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightSetupFlareSources.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightInitObjectLighting.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightAdjustGlowingLight.s")

#pragma GLOBAL_ASM("asm/nonmatchings/lights/lightKillGlowingLight.s")
