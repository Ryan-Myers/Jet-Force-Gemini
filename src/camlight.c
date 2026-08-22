#include "camlight.h"
#include "camera.h"
#include "common.h"
#include "gameVi.h"
#include "main.h"
#include "math/math.h"
#include "textures.h"
#include "track.h"

typedef struct RGBA {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} RGBA;

typedef struct UnkStruct_80024430_25030_s {
    f32 unk0;
    f32 unk4;
    u8 pad8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    f32 unkC;
} UnkStruct_80024430_25030;

/************ .data ************/

Gfx D_800A1020_A1C20[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_FOG),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE | 0x0000000F,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2),
    gsSPEndDisplayList(),
};

UnkStruct_80024430_25030 D_800A1048_A1C48[4] = {
    { -40.0f, 0.75f, 0, 0x80, 0x80, 0xFF, 144.0f },
    { -80.0f, 1.25f, 0, 0x80, 0xFF, 0x80, 144.0f },
    { -120.0f, 0.75f, 0, 0xFF, 0x80, 0x80, 144.0f },
    { 0.0f, 0.0f, 0, 0, 0, 0, 0.0f },
};

UnkStruct_80024430_25030 D_800A1088_A1C88[8] = {
    { -32.0f, 0.75f, 0, 0xFF, 0x80, 0xFF, 144.0f },  { -64.0f, 1.25f, 0, 0x80, 0x80, 0xFF, 144.0f },
    { -96.0f, 0.75f, 0, 0x80, 0xFF, 0x80, 144.0f },  { -128.0f, 1.25f, 0, 0xFF, 0xFF, 0x80, 144.0f },
    { -160.0f, 0.75f, 0, 0xFF, 0xC0, 0x80, 144.0f }, { -192.0f, 1.1f, 0, 0xFF, 0x80, 0x80, 144.0f },
    { -224.0f, 0.6f, 0, 0xFF, 0xE0, 0xE0, 144.0f },  { 0.0f, 0.0f, 0, 0, 0, 0, 0.0f },
};

s32 sCurrentCamLightsCount = 0;
s32 D_800A110C_A1D0C = 0;
TextureSprite *D_800A1110_A1D10 = NULL;

/*******************************/

/************ .bss ************/

Gfx *sGfxPtr;
Mtx *D_800F5B54_B1754;
Vertex *D_800F5B58_B1758;
CamLight sCamLights[MAX_CAMLIGHTS];

// forward declarations
void func_80024430_25030(UnkStruct_80024430_25030 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                         f32 arg7, f32 arg8);
void func_80023FA4_24BA4(CamLight *arg0, CamLight *arg1);

void camlightInit(void) {
    CamLight *camLight;
    s32 i;

    camLight = sCamLights;
    i = MAX_CAMLIGHTS;
    while (i--) {
        camLight->unk6 = 0;
        camLight->sprite = 0;
        camLight++;
    }

    D_800A110C_A1D0C = 0;
}

void camlightFlush(void) {
    CamLight *camLight;
    s32 i;

    camLight = sCamLights;
    i = MAX_CAMLIGHTS;
    while (i--) {
        camlightDelete(camLight);
        camLight++;
    }
}

CamLight *camlightAdd(CamLight_inner *arg0, UnkStruct_Arg1 *arg1) {
    CamLight *camLight;

    if (sCurrentCamLightsCount >= MAX_CAMLIGHTS) {
        return NULL;
    }
    camLight = sCamLights;
    while (camLight->unk6 != 0) {
        camLight++;
    }

    sCurrentCamLightsCount++;
    camLight->unk0.x = arg1->unk2;
    camLight->unk0.y = arg1->unk4;
    camLight->unk0.z = arg1->unk6;
    camLight->unk6 = (s8) arg1->unk0;
    if (arg1->unk1F == 0) {
        camLight->unk7 = 0;
    } else {
        camLight->unk7 = 0x10;
    }

    camLight->unk8 = arg1->unk1 * 0.05f;
    camLight->unk18 = arg0;
    camLight->unk1C = arg1->unk8;
    camLight->unk20 = arg1->unkC;
    camLight->unk24 = arg1->unk10;
    camLight->unk34 = arg1->unk1E;
    camLight->unk35 = 0;

    if ((arg0 != 0) && ((camLight->unk1C != 0.0f) || (camLight->unk20 != 0.0f) || (camLight->unk24 != 0.0f))) {
        camLight->unk35 = 1;
    }

    camLight->unk36 = 3;
    camLight->unk37 = -1;
    camlightUpdate(camLight);
    camLight->unk38 = arg1->unk14;
    camLight->unk39 = arg1->unk15;
    camLight->unk3A = arg1->unk16;
    camLight->unk3B = arg1->unk17;
    if (arg1->unk18 >= 0) {
        camLight->sprite = texLoadSprite(arg1->unk18, 0);
    } else {
        camLight->sprite = 0;
    }
    camLight->unk40 = arg1->unk1A;
    camLight->unk44 = 1.0f / camLight->unk40;
    camLight->unk48 = arg1->unk1C;
    camLight->unk4C = 1.0f / camLight->unk48;
    if (sCurrentCamLightsCount == 1) {
#ifdef VERSION_kiosk
        D_800A1110_A1D10 = texLoadSprite(72, 0);
#else
        D_800A1110_A1D10 = texLoadSprite(73, 0);
#endif
    }
    return camLight;
}

void camlightDelete(CamLight *camLight) {
    TextureSprite *sprite;

    if (camLight == NULL) {
        return;
    }
    if (camLight->unk6 == 0) {
        return;
    }

    camLight->unk6 = 0;
    sprite = camLight->sprite;
    if (sprite != 0) {
        texFreeSprite(sprite);
        camLight->sprite = 0;
    }

    sCurrentCamLightsCount--;
    if (sCurrentCamLightsCount == 0) {
        if (D_800A1110_A1D10 != NULL) {
            texFreeSprite(D_800A1110_A1D10);
            D_800A1110_A1D10 = NULL;
        }
    }
}

void camlightUpdateAll(void) {
    CamLight *camLight;
    s32 i;

    if (sCurrentCamLightsCount <= 0) {
        return;
    }

    camLight = sCamLights;
    i = MAX_CAMLIGHTS;
    while (i--) {
        if (camLight->unk6 != 0) {
            camlightUpdate(camLight);
        }
        camLight++;
    }
}

void camlightUpdate(CamLight *camLight) {
    Vec3f *temp_a2;
    CamLight_inner *temp_s1;
    CamLight_inner1 *temp_v1;

    if (camLight == NULL) {
        return;
    }

    temp_s1 = camLight->unk18;
    if ((camLight->unk36 & 1) || ((temp_s1 != NULL))) {
        if (temp_s1 == 0) {
            camLight->unkC.x = camLight->unk1C;
            camLight->unkC.y = camLight->unk20;
            camLight->unkC.z = camLight->unk24;
            goto cond;
        }
        if (camLight->unk34 >= 0) {
            temp_v1 = temp_s1->unk6C[temp_s1->unk3A];
            if (temp_v1 != NULL) {
                if (temp_v1->unk0 != NULL) {
                    if (camLight->unk34 < temp_v1->unk0->unk2D) {
                        camLight->unkC.x = temp_v1->unk68[camLight->unk34].x;
                        camLight->unkC.y = temp_v1->unk68[camLight->unk34].y;
                        camLight->unkC.z = temp_v1->unk68[camLight->unk34].z;
                    }
                }
            }
        } else {
            if (camLight->unk35 != 0) {
                pointListRPY(1, (Vec3s *) temp_s1, &camLight->unk1C, camLight->unkC.f);
                camLight->unkC.x += temp_s1->unkC;
                camLight->unkC.y += temp_s1->unk10;
                camLight->unkC.z += temp_s1->unk14;
            } else {
                camLight->unkC.x = temp_s1->unkC;
                camLight->unkC.y = temp_s1->unk10;
                camLight->unkC.z = temp_s1->unk14;
            }
        }
    }
cond:
    if ((camLight->unk36 & 2) || (temp_s1 != NULL)) {
        temp_a2 = &camLight->unk28;
        camLight->unk28.f[2] = -1.0f;
        mathOneFloatPY(&camLight->unk0, temp_a2);
        if (temp_s1 != 0) {
            pointListRPY(1, (Vec3s *) temp_s1, temp_a2->f, temp_a2->f);
        }
        camLight->unk28.f[0] = -camLight->unk28.f[0];
        camLight->unk28.f[1] = -camLight->unk28.f[1];
        camLight->unk28.f[2] = -camLight->unk28.f[2];
    }
    camLight->unk36 = 0;
}

void func_80023FA4_24BA4(CamLight *cl1, CamLight *cl2) {
    f32 sp2C[11];

    if (trackNearestIntersection(0, &cl1->unkC, &cl2->unkC, sp2C, 0x2404, 0) == 0) {
        cl2->unk7 += 5;
        if (cl2->unk7 >= 0x11) {
            cl2->unk7 = 0x10;
        }
    } else {
        cl2->unk7 -= 5;
        if (cl2->unk7 < 0) {
            cl2->unk7 = 0;
        }
    }
}

void camlightVisibilityCheck(void) {
    Camera *cam;
    CamLight *camLight;
    s32 matchFound;
    f32 outX;
    f32 outY;
    f32 diffX;
    f32 diffZ;
    f32 diffY;
    s32 index;
    s32 i;
    Vec3f sp64;
    s32 pad50;
    f32 ft4;

    if (sCurrentCamLightsCount <= 0) {
        return;
    }

    cam = camGetPtr();
    i = 4;
    index = D_800A110C_A1D0C;
    do {
        camLight = &sCamLights[index++];
        if (index >= MAX_CAMLIGHTS) {
            index = 0;
        }
        if (camLight->unk6 & 0x20) {
            if (mainGetZBCheck(camLight->unk37) == 0) {
                camLight->unk7 += 8;
                if (camLight->unk7 >= 0x11) {
                    camLight->unk7 = 0x10;
                }
            } else {
                camLight->unk7 -= 8;
                if (camLight->unk7 < 0) {
                    camLight->unk7 = 0;
                }
            }
            sp64.f[2] = 256.0f;
            mathOneFloatPY(&camLight->unk0, &sp64);
            sp64.f[0] += cam->trans.position.f[0];
            sp64.f[1] += cam->trans.position.f[1];
            sp64.f[2] += cam->trans.position.f[2];
            if (camProjectPoint(sp64.f[0], sp64.f[1], sp64.f[2], &outX, &outY, TRUE)) {
                camLight->unk37 = mainAddZBCheck(outX, outY, 3);
            } else {
                camLight->unk37 = -1;
            }
        } else if (camLight->unk6 != 0) {
            diffX = camLight->unkC.x - cam->trans.position.f[0];
            diffY = camLight->unkC.y - cam->trans.position.f[1];
            diffZ = camLight->unkC.z - cam->trans.position.f[2];
            ft4 = SQ(camLight->unk40);
            if ((SQ(diffX) + SQ(diffY) + SQ(diffZ)) < ft4) {
                if (camLight->unk6 & 0x40) {
                    if (i != 0) {
                        func_80023FA4_24BA4((void *) cam, (void *) camLight);
                        i--;
                        if (i == 0) {
                            matchFound = index;
                        }
                    }
                } else {
                    camLight->unk7 = 0x10;
                }
            } else {
                camLight->unk7 = 0;
            }
        }
    } while (index != D_800A110C_A1D0C);

    if (i == 0) {
        D_800A110C_A1D0C = matchFound; // !@bug: matchFound could be uninitialized
    }
}

void func_80024330_24F30(s32 arg0, RGBA *arg1, u8 *r, u8 *g, u8 *b, u8 *a) {
    s32 temp_lo;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_t0;
    s32 var_v0;
    s32 var_v1;

    var_v0 = 0;
    var_v1 = 0;
    var_t0 = 0;
    var_t1 = 0;
    var_t2 = 0;
    var_t3 = 0;
    var_t4 = 0;

    while (arg0--) {
        temp_lo = arg1->r * arg1->a;
        if (var_t3 < temp_lo) {
            var_t3 = temp_lo;
            var_t0 = arg1->r;
        }
        temp_lo = arg1->g * arg1->a;
        if (var_t2 < temp_lo) {
            var_t2 = temp_lo;
            var_v1 = arg1->g;
        }
        temp_lo = arg1->b * arg1->a;
        if (var_t1 < temp_lo) {
            var_t1 = temp_lo;
            var_v0 = arg1->b;
        }

        if (var_t4 < arg1->a) {
            var_t4 = arg1->a;
        }
        arg1++;
    }

    *r = var_t0;
    *g = var_v1;
    *b = var_v0;
    *a = var_t4;
}

void func_80024430_25030(UnkStruct_80024430_25030 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                         f32 arg7, f32 arg8) {
    ObjectSegment1 sp7C;

    if (arg0 == NULL) {
        return;
    }

    sp7C.trans.rotation.x = 0;
    sp7C.trans.rotation.y = 0;
    sp7C.trans.rotation.z = 0;
    sp7C.unk28 = 0.0f;
    while (arg0->unk4 != 0.0f) {
        sp7C.trans.position.x = (arg0->unk0 * arg4) + arg1;
        sp7C.trans.position.y = (arg0->unk0 * arg5) + arg2;
        sp7C.trans.position.z = (arg0->unk0 * arg6) + arg3;
        sp7C.trans.scale = arg0->unk4 * arg7;
        gDPSetPrimColor(sGfxPtr++, 0, 0, arg0->unk9, arg0->unkA, arg0->unkB, (s32) (arg0->unkC * arg8));
        camDoSprite(&sGfxPtr, &D_800F5B54_B1754, &D_800F5B58_B1758, &sp7C, (Sprite *) D_800A1110_A1D10, 4, 0);
        arg0++;
    }
}

void camlightDraw(Gfx **arg0, Mtx **arg1, Vertex **arg2) {
    Camera *cam;
    UnkStruct_80024430_25030 *var_s1;
    s32 sp1EC;
    s32 sp1E8;
    u8 sp1E7;
    u8 sp1E6;
    u8 sp1E5;
    u8 sp1E4;
    f32 sp1E0;
    f32 sp1DC;
    f32 sp1D8;
    Vec3f sp1CC;
    f32 sp1C8;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs3; // sp1BC
    f32 var_ft5;  // sp1B8
    CamLight *camLight;
    Matrix *mtx;
    f32 temp_fv1;
    f32 var_fs0;
    ObjectSegment1 sp17C;
    s8 spB4[50 * 4];
    f32 var_fv0;
    s32 i;
    s32 var_s4;

    if (sCurrentCamLightsCount <= 0) {
        return;
    }

    sGfxPtr = *arg0;
    D_800F5B54_B1754 = *arg1;
    D_800F5B58_B1758 = *arg2;
    cam = camGetPtr();
    mtx = camGetInvProjMtx();
    sp1E0 = -mtx[0][2][0];
    sp1DC = -mtx[0][2][1];
    sp1D8 = -mtx[0][2][2];
    var_s4 = 0;
    camLight = sCamLights;

    for (i = MAX_CAMLIGHTS; i--; camLight++) {
        if ((camLight->unk6 == 0) || (camLight->unk7 <= 0) || (camLight->unk3B <= 0)) {
            continue;
        }

        if (camLight->unk6 & 0x20) {
            sp17C.trans.position.f[2] = -1.0f;
            mathOneFloatPY(&camLight->unk0, &sp17C.trans.position);
            sp1CC.z = -sp17C.trans.position.f[0];
            sp1CC.y = -sp17C.trans.position.f[1];
            sp1CC.x = -sp17C.trans.position.f[2];
            sp17C.trans.position.f[0] *= -256.0f;
            sp17C.trans.position.f[1] *= -256.0f;
            sp17C.trans.position.f[2] *= -256.0f;
            sp17C.trans.position.f[0] += cam->trans.position.f[0];
            sp17C.trans.position.f[1] += cam->trans.position.f[1];
            sp17C.trans.position.f[2] += cam->trans.position.f[2];
            var_fs0 = 1;
        } else {
            sp17C.trans.position.f[0] = camLight->unkC.f[0];
            sp17C.trans.position.f[1] = camLight->unkC.f[1];
            sp17C.trans.position.f[2] = camLight->unkC.f[2];
            temp_fs1 = camLight->unkC.f[0] - cam->trans.position.f[0];
            temp_fs2 = camLight->unkC.f[1] - cam->trans.position.f[1];
            temp_fs3 = camLight->unkC.f[2] - cam->trans.position.f[2];
            var_ft5 = SQ(temp_fs1) + SQ(temp_fs2) + SQ(temp_fs3);
            var_fs0 = 1.0f;
            if (var_ft5 > 0) {
                var_ft5 = sqrtf(var_ft5);
                var_fs0 /= var_ft5;
            }

            sp1CC.z = temp_fs1 * var_fs0;
            sp1CC.y = temp_fs2 * var_fs0;
            sp1CC.x = temp_fs3 * var_fs0;
            var_fs0 = 1.0f;
            if (camLight->unk6 & 0x80) {
                var_fs0 = (camLight->unk28.x * sp1CC.z) + (camLight->unk28.y * sp1CC.y) + (camLight->unk28.z * sp1CC.x);
            }
        }
        var_fs0 *= (sp1E0 * sp1CC.z) + (sp1DC * sp1CC.y) + (sp1D8 * sp1CC.x);
        if ((var_fs0 > 0.7071f)) {
            sp17C.trans.rotation.x = 0;
            sp17C.trans.rotation.y = 0;
            sp17C.trans.rotation.z = 0;
            sp17C.unk28 = 0;
            sp1C8 = camLight->unk7 * 0.0625f;
            var_fs0 *= ((var_fs0 - 0.7071f) * 3.4141345f);
            if (var_ft5 < camLight->unk48) {
                var_fv0 = camLight->unk4C * var_ft5;
                if (var_fv0 < 0.5f) {
                    var_fv0 = var_fs0;
                } else {
                    var_fv0 = 2.0f * ((1.0f - var_fv0) * var_fs0);
                }
                spB4[var_s4 * 4 + 0] = camLight->unk38;
                spB4[var_s4 * 4 + 1] = camLight->unk39;
                spB4[var_s4 * 4 + 2] = camLight->unk3A;
                spB4[var_s4 * 4 + 3] = camLight->unk3B * var_fv0;
                var_s4++;
            }
            if (!(camLight->unk6 & 0x20)) {
                var_ft5 *= camLight->unk44;
                if (var_ft5 > 0.5f) {
                    var_ft5 = 2.0f * (1.0f - var_ft5);
                } else {
                    var_ft5 = 1.0f;
                }
            } else {
                var_ft5 = 1.0f;
            }
            if (camLight->sprite != NULL) {
                sp17C.trans.scale = camLight->unk8 * var_fs0 * sp1C8;
                sprSetIA2ColOverride(0xFFU, 0xFFU, 0xFFU, camLight->unk38, camLight->unk39, camLight->unk3A);
                camDoSprite(&sGfxPtr, &D_800F5B54_B1754, &D_800F5B58_B1758, &sp17C, (Sprite *) camLight->sprite, 0x204,
                            (s32) (255.0f * var_fs0 * var_ft5));
                sprClearIA2ColOverride();
            }
            if (camLight->unk6 & 4) {
                var_s1 = D_800A1088_A1C88;
            } else {
                if (camLight->unk6 & 2) {
                    var_s1 = D_800A1048_A1C48;
                } else {
                    var_s1 = NULL;
                }
            }
            if ((var_fs0 > 0.7795f)) {
                if (var_s1 != NULL) {
                    var_fv0 = SQ((var_fs0 - 0.7795f) * 4.535147f);
                    var_fs0 *= var_fs0;
                    temp_fv1 = ((sp1E0 * sp1CC.z) + (sp1DC * sp1CC.y) + (sp1D8 * sp1CC.x)) * 2 /*.0f*/;
                    temp_fs1 = (temp_fv1 * sp1E0) - sp1CC.z;
                    temp_fs2 = (temp_fv1 * sp1DC) - sp1CC.y;
                    temp_fs3 = (temp_fv1 * sp1D8) - sp1CC.x;
                    func_80024430_25030(var_s1, sp17C.trans.position.f[0], sp17C.trans.position.f[1],
                                        sp17C.trans.position.f[2], temp_fs1, temp_fs2, temp_fs3, var_fs0,
                                        var_fv0 * sp1C8 * var_ft5);

                    temp_fv1 = ((temp_fs1 * sp1CC.z) + (temp_fs2 * sp1CC.y) + (temp_fs3 * sp1CC.x)) * 2;
                    temp_fs1 = (temp_fv1 * sp1CC.z) - temp_fs1;
                    temp_fs2 = (temp_fv1 * sp1CC.y) - temp_fs2;
                    temp_fs3 = (temp_fv1 * sp1CC.x) - temp_fs3;
                    func_80024430_25030(var_s1, sp17C.trans.position.f[0], sp17C.trans.position.f[1],
                                        sp17C.trans.position.f[2], temp_fs1, temp_fs2, temp_fs3, var_fs0,
                                        var_fv0 * sp1C8 * var_ft5);
                }
            }
        }
    }

    if (var_s4 > 0) {
        viGetCurrentSize(&sp1EC, &sp1E8);
        func_80024330_24F30(var_s4, (RGBA *) spB4, &sp1E7, &sp1E6, &sp1E5, &sp1E4);
        gSPDisplayList(sGfxPtr++, D_800A1020_A1C20);
        gDPSetPrimColor(sGfxPtr++, 0, 0, sp1E7, sp1E6, sp1E5, sp1E4);
        gDPFillRectangle(sGfxPtr++, 0, 0, sp1EC, sp1E8);
        gDPPipeSync(sGfxPtr++);
    }
    texDPInit(&sGfxPtr);
    *arg0 = sGfxPtr;
    *arg1 = D_800F5B54_B1754;
    *arg2 = D_800F5B58_B1758;
}
