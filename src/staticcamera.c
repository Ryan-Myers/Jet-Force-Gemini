#include "common.h"
#include "math/math.h"
#include "camera.h"
#include "charControl.h"

// Seems to be different from the regular camera struct because it's loading floats in the first few positions.
typedef struct LobbyCamStruct {
    Vec3f unk0;
    f32 unkC;
    u8 pad10[0x4];
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x8];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
} LobbyCamStruct;

LobbyCamStruct *gLobbyCam;
Camera *gStaticCam;

void staticcamFree(void) {
    gStaticCam = NULL;
}

void staticcamAdd(Camera *cam) {
    gStaticCam = cam;
}

void staticcamDelete(Camera *cam) {
    if (cam == gStaticCam) {
        gStaticCam = NULL;
    }
}

LobbyCamStruct *lobbycamInuse(void) {
    return gLobbyCam;
}

Camera *staticcamInuse(void) {
    return gStaticCam;
}

void lobbycamGetFocalPoint(Camera *cam, f32 *arg1, f32 *arg2, f32 *arg3) {
    Vec3f direction;
    direction.x = cam->trans.z_position;
    direction.y = cam->cam_unk_18;
    direction.z = cam->boomLength;
    if (cam->trans.flags != 0) {
        mathOneFloatYPR(&cam->trans.rotation, &direction);
    } else {
        mathOneFloatRPY(&cam->trans.rotation, &direction);
    }
    *arg1 = direction.f[0] + cam->trans.scale;
    *arg2 = direction.f[1] + cam->trans.x_position;
    *arg3 = direction.f[2] + cam->trans.y_position;
}

#ifdef NON_EQUIVALENT
void func_800427E8_433E8(Camera *cam1, Camera *cam2, s32 exp, s32 smoothed) {
    s16 sp8E;
    s16 sp8C;
    f32 focalX;
    f32 focalY;
    f32 focalZ;
    f32 sp68;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f22;
    f32 temp_f24;
    f32 var_f26;
    f32 var_f28;

    var_f26 = 1.0f;
    var_f28 = 1.0f;
    // If smoothed is true, apply smoothing to the camera movement based on the exponent value.
    if (smoothed) {
        var_f26 -= Powerf(0.875f, exp);
        var_f28 -= Powerf(0.9f, exp);
    }
    cam1->trans.position.z = gLobbyCam->unk24;
    cam1->cam_unk_18 = gLobbyCam->unk28;
    cam1->boomLength = gLobbyCam->unk2C;
    lobbycamGetFocalPoint(cam1, &focalX, &focalY, &focalZ);
    temp_f22 = focalX - gLobbyCam->unk0.x;
    temp_f24 = focalY - gLobbyCam->unk0.y;
    temp_f14 = focalZ - gLobbyCam->unk0.z;
    temp_f12 = (temp_f22 * temp_f22) + (temp_f24 * temp_f24) + (temp_f14 * temp_f14);
    if (temp_f12 > 0.0f) {
        temp_f12 = gLobbyCam->unk14 / sqrtf(temp_f12);
    }
    temp_f22 = (((temp_f22 * temp_f12) + focalX) - cam2->trans.position.x);
    temp_f24 = (((temp_f24 * temp_f12) + focalY) - cam2->trans.position.y);
    temp_f14 = (((temp_f14 * temp_f12) + focalZ) - cam2->trans.position.z);
    cam2->trans.position.x += (temp_f22 * var_f26);
    cam2->trans.position.y += ((temp_f24 * var_f26) + gLobbyCam->unk18) * var_f26;
    cam2->trans.position.z += (temp_f14 * var_f26);
    temp_f22 = cam2->trans.position.x - focalX;
    temp_f24 = cam2->trans.position.y - focalY;
    temp_f14 = cam2->trans.position.z - focalZ;
    temp_f12 = sqrtf((temp_f22 * temp_f22) + (temp_f14 * temp_f14));
    sp8E = 0x8000 - Arctanf(temp_f22, temp_f14);
    sp8C = Arctanf(temp_f24, temp_f12);
    cam2->trans.rotation.x = dAngle(cam2->trans.rotation.x, sp8E, var_f28);
    cam2->trans.rotation.y = dAngle(cam2->trans.rotation.y, sp8C, var_f28);
    cam2->trans.rotation.z = dAngle(cam2->trans.rotation.z, 0, var_f28);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/func_800427E8_433E8.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/lobbycamPosition.s")

void staticcamPosition(Camera *cam) {
    if (gStaticCam != NULL) {
        cam->trans.position.x = gStaticCam->trans.position.x;
        cam->trans.position.y = gStaticCam->trans.position.y;
        cam->trans.position.z = gStaticCam->trans.position.z;
        cam->trans.rotation.z = gStaticCam->trans.rotation.z;
        cam->trans.rotation.y = -gStaticCam->trans.rotation.y;
        cam->trans.rotation.x = 0x8000 - gStaticCam->trans.rotation.x;
    }
}
