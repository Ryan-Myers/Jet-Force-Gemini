#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "structs.h"
#include <PR/ultratypes.h>

extern s32 D_800A1000_A1C00; // gMaxLights
extern ObjectLight *D_800A100C_A1C0C;
extern s32 D_800A1004_A1C04;
extern ObjectLight **D_800A1008_A1C08; // gActiveLights

void setupLights(s32 count, s32 arg1, s32 arg2);
void func_80021434_22034(unk800DC950 *arg0, s32 arg1);
s32 addObjectLight(Object *, ObjectHeader48 *);
void lightSetupLightSources(Object *obj);
f32 lightDistanceCalc(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
void freeLights();
void lightCreateLightTable(s32 arg0, s32 arg1, s32 arg2, s32 *arg3);
void turnLightOff(UnkLight *light);
void turnLightOn(UnkLight *light);
void toggleLight(UnkLight *light);
void changeLightColour(UnkLight *light, u8 red, u8 green, u8 blue);
void changeLightColourCycle(s32 arg0, s32 arg1); // TODO: arg0 typing is incorrect
void changeLightIntensity(UnkLight *light, u8 intensity);
void lightUpdateLights(s32 arg0);
unk800DC950 **lightGetLights(s32 *arg0);
f32 lightDirectionCalc(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
s32 lightKillGlowingLight(void *arg0); // TODO: arg0 typing is incorrect
void freeLights(void);

#endif
