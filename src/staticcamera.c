#include "staticcamera.h"
#include "camera.h"
#include "charControl.h"
#include "common.h"
#include "math/math.h"

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

void func_800427E8_433E8(Camera *cam1, Camera *cam2, s32 exp, s32 smoothed);

// I dunno, there's just so much damned float regalloc.
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

#ifdef NON_EQUIVALENT
void lobbycamPosition(Camera *cam1, Camera *cam2, s32 exp, s32 smoothed) {
    s32 sp88;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp5C;
    f32 sp50;
    f32 sp48;
    f32 sp40;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f10;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f20;
    f32 var_f14_2;
    f32 var_f16;
    f32 var_f18;
    f32 var_f20;
    s16 temp_a0;

    if (gLobbyCam->unk22 < 0x100) {
        func_800427E8_433E8(cam1, cam2, exp, smoothed);
        return;
    }
    sp7C = 1.0f;
    sp78 = 1.0f;
    if (smoothed != 0) {
        sp7C = 1.0f - Powerf(0.9f, exp);
        sp78 = 1.0f - Powerf(0.9375f, exp);
    }
    cam1->trans.position.z = gLobbyCam->unk24;
    cam1->cam_unk_18 = gLobbyCam->unk28;
    cam1->boomLength = gLobbyCam->unk2C;
    lobbycamGetFocalPoint(cam1, &sp6C, &sp68, &sp64);
    sp74 = gLobbyCam->unk0.x;
    sp70 = gLobbyCam->unk0.z;
    if (gLobbyCam->unk1C != 0.0f) {
        sp48 = Sinf(gLobbyCam->unk20);
        temp_f0 = Cosf(gLobbyCam->unk20);
        temp_f16 = sp6C - gLobbyCam->unk0.x;
        temp_f18 = sp64 - gLobbyCam->unk0.z;
        var_f20 = (temp_f16 * temp_f0) - (temp_f18 * sp48);
        temp_f10 = (temp_f16 * sp48) + (temp_f18 * temp_f0);
        if (var_f20 < -gLobbyCam->unk1C) {
            var_f20 = -gLobbyCam->unk1C;
        }
        if (gLobbyCam->unk1C < var_f20) {
            var_f20 = gLobbyCam->unk1C;
        }
        if (temp_f10 < 0.0f) {
            temp_f10 = -temp_f10;
        }
        if (temp_f10 < gLobbyCam->unk14) {
            if (gLobbyCam->unk14 < sqrtf((var_f20 * var_f20) + (temp_f10 * temp_f10))) {
                temp_f0_2 = sqrtf((gLobbyCam->unk14 * gLobbyCam->unk14) - (temp_f10 * temp_f10));
                if (var_f20 < 0.0f) {
                    var_f20 += temp_f0_2;
                } else {
                    var_f20 -= temp_f0_2;
                }
            } else {
                var_f20 = 0.0f;
            }
        }
        sp70 -= var_f20 * sp48;
        sp74 += var_f20 * temp_f0;
    }
    var_f20 = sp74 - sp6C;
    var_f14_2 = sp70 - sp64;
    temp_f12_2 = (var_f20 * var_f20) + (var_f14_2 * var_f14_2);
    if (temp_f12_2 > 0.0f) {
        temp_f12_2 = sqrtf(temp_f12_2);
        var_f20 *= gLobbyCam->unk14 / temp_f12_2;
        var_f14_2 *= gLobbyCam->unk14 / temp_f12_2;
    }
    temp_f16 = sp6C + var_f20;
    sp50 = gLobbyCam->unk18 + sp68;
    temp_f18_2 = sp64 + var_f14_2;
    temp_a0 = Arctanf(gLobbyCam->unk0.x - temp_f16, gLobbyCam->unk0.z - temp_f18_2) - gLobbyCam->unk20;
    if ((temp_a0 != 0x7FFF) && ((temp_a0 < -gLobbyCam->unk22) || (gLobbyCam->unk22 < temp_a0))) {
        temp_f16 = sp74;
        temp_f18_2 = sp70;
    } else if (temp_f12_2 < gLobbyCam->unk14) {
        temp_f16 = sp74;
        temp_f18_2 = sp70;
    }
    cam2->trans.position.x += ((temp_f16 - cam2->trans.position.x) * sp7C);
    cam2->trans.position.y += ((sp50 - cam2->trans.position.y) * sp7C);
    cam2->trans.position.z += ((temp_f18_2 - cam2->trans.position.z) * sp7C);
    temp_f20 = cam2->trans.position.x - sp6C;
    sp5C = cam2->trans.position.y - sp68;
    temp_f14 = cam2->trans.position.z - sp64;
    sp40 = sqrtf((temp_f20 * temp_f20) + (temp_f14 * temp_f14));
    sp88 = 0x8000 - Arctanf(temp_f20, temp_f14);
    Arctanf(sp5C, sp40);
    cam2->trans.rotation.x = dAngle(cam2->trans.rotation.x, sp88, sp78);
    cam2->trans.rotation.y = dAngle(cam2->trans.rotation.y, 0, sp78);
    cam2->trans.rotation.z = dAngle(cam2->trans.rotation.z, 0, sp78);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/staticcamera/lobbycamPosition.s")
#endif

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
