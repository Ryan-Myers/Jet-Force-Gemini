#include "camera.h"
#include "common.h"
#include "gameVi.h"
#include "math/math.h"
#include "textures.h"
#include "camlight.h"

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

typedef struct ObjectSegmentCustom {
    /* 0x0000 */ ObjectTransform trans;
    /* 0x0018 */ u8 pad18[0x10];
    /* 0x0028 */ f32 unk28; /* animation value; sprDPset reads it as float bits */
} ObjectSegmentCustom; /* size = 0x2C */

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

s32 D_800A1108_A1D08 = 0;
s32 D_800A110C_A1D0C = 0;
TextureSprite *D_800A1110_A1D10 = NULL;

/*******************************/

/************ .bss ************/

Gfx *sGfxPtr;
Mtx *D_800F5B54_B1754;
Vertex *D_800F5B58_B1758;
CamLightStruct D_800F5B60_B1760[MAX_CAMLIGHT];

// track.h
s32 trackNearestIntersection(s32 arg0, Vec3f *arg1, Vec3f *arg2, void *arg3, s32 arg4, s32 arg5);

// main.h
s32 mainAddZBCheck(s32 arg0, s32 arg1, s32 arg2);
s8 mainGetZBCheck(s32 arg0);

void camlightInit(void) {
    CamLightStruct *var_v0;
    s32 i;

    var_v0 = D_800F5B60_B1760;
    i = MAX_CAMLIGHT;
    while (i--) {
        var_v0->unk6 = 0;
        var_v0->sprite = 0;
        var_v0++;
    }

    D_800A110C_A1D0C = 0;
}

void camlightFlush(void) {
    CamLightStruct *var_s0;
    s32 i;

    var_s0 = D_800F5B60_B1760;
    i = MAX_CAMLIGHT;
    while (i--) {
        camlightDelete(var_s0);
        var_s0++;
    }
}

CamLightStruct *camlightAdd(s32 arg0, UnkStruct_Arg1 *arg1) {
    CamLightStruct *var_a2;

    if (D_800A1108_A1D08 >= MAX_CAMLIGHT) {
        return NULL;
    }
    var_a2 = D_800F5B60_B1760;
    while (var_a2->unk6 != 0) {
        var_a2++;
    }

    D_800A1108_A1D08 += 1;
    var_a2->unk0 = arg1->unk2;
    var_a2->unk2 = arg1->unk4;
    var_a2->unk4 = arg1->unk6;
    var_a2->unk6 = (s8) arg1->unk0;
    if (arg1->unk1F == 0) {
        var_a2->unk7 = 0;
    } else {
        var_a2->unk7 = 0x10;
    }

    var_a2->unk8 = arg1->unk1 * 0.05f;
    var_a2->unk18 = arg0;
    var_a2->unk1C = arg1->unk8;
    var_a2->unk20 = arg1->unkC;
    var_a2->unk24 = arg1->unk10;
    var_a2->unk34 = arg1->unk1E;
    var_a2->unk35 = 0;

    if ((arg0 != 0) && ((var_a2->unk1C != 0.0f) || (var_a2->unk20 != 0.0f) || (var_a2->unk24 != 0.0f))) {
        var_a2->unk35 = 1;
    }

    var_a2->unk36 = 3;
    var_a2->unk37 = -1;
    camlightUpdate(var_a2);
    var_a2->unk38 = arg1->unk14;
    var_a2->unk39 = arg1->unk15;
    var_a2->unk3A = arg1->unk16;
    var_a2->unk3B = arg1->unk17;
    if (arg1->unk18 >= 0) {
        var_a2->sprite = texLoadSprite(arg1->unk18, 0);
    } else {
        var_a2->sprite = 0;
    }
    var_a2->unk40 = arg1->unk1A;
    var_a2->unk44 = 1.0f / var_a2->unk40;
    var_a2->unk48 = arg1->unk1C;
    var_a2->unk4C = 1.0f / var_a2->unk48;
    if (D_800A1108_A1D08 == 1) {
        D_800A1110_A1D10 = texLoadSprite(0x49, 0);
    }
    return var_a2;
}

void camlightDelete(CamLightStruct *arg0) {
    TextureSprite *temp_a0;

    if (arg0 == NULL) {
        return;
    }
    if (arg0->unk6 == 0) {
        return;
    }

    arg0->unk6 = 0;
    temp_a0 = arg0->sprite;
    if (temp_a0 != 0) {
        texFreeSprite((TextureSprite *) temp_a0);
        arg0->sprite = 0;
    }

    D_800A1108_A1D08--;
    if (D_800A1108_A1D08 == 0) {
        if (D_800A1110_A1D10 != NULL) {
            texFreeSprite(D_800A1110_A1D10);
            D_800A1110_A1D10 = NULL;
        }
    }
}

void camlightUpdateAll(void) {
    CamLightStruct *var_s0;
    s32 i;

    if (D_800A1108_A1D08 <= 0) {
        return;
    }

    var_s0 = D_800F5B60_B1760;
    i = MAX_CAMLIGHT;
    while (i--) {
        if (var_s0->unk6 != 0) {
            camlightUpdate(var_s0);
        }
        var_s0++;
    }
}

void camlightUpdate(CamLightStruct *arg0) {
    Vec3f *temp_a2;
    CamLightStruct_inner *temp_s1;
    CamLightStruct_inner1 *temp_v1;

    if (arg0 == NULL) {
        return;
    }

    temp_s1 = arg0->unk18;
    if ((arg0->unk36 & 1) || ((temp_s1 != NULL))) {
        if (temp_s1 == 0) {
            arg0->unkC.x = arg0->unk1C;
            arg0->unkC.y = arg0->unk20;
            arg0->unkC.z = arg0->unk24;
            goto cond;
        }
        if (arg0->unk34 >= 0) {
            temp_v1 = temp_s1->unk6C[temp_s1->unk3A];
            if (temp_v1 != NULL) {
                if (temp_v1->unk0 != NULL) {
                    if (arg0->unk34 < temp_v1->unk0->unk2D) {
                        arg0->unkC.x = temp_v1->unk68[arg0->unk34].x;
                        arg0->unkC.y = temp_v1->unk68[arg0->unk34].y;
                        arg0->unkC.z = temp_v1->unk68[arg0->unk34].z;
                    }
                }
            }
        } else {
            if (arg0->unk35 != 0) {
                pointListRPY(1, (Vec3s *) temp_s1, &arg0->unk1C, &arg0->unkC);
                arg0->unkC.x += temp_s1->unkC;
                arg0->unkC.y += temp_s1->unk10;
                arg0->unkC.z += temp_s1->unk14;
            } else {
                arg0->unkC.x = temp_s1->unkC;
                arg0->unkC.y = temp_s1->unk10;
                arg0->unkC.z = temp_s1->unk14;
            }
        }
    }
cond:
    if ((arg0->unk36 & 2) || (temp_s1 != NULL)) {
        temp_a2 = &arg0->unk28;
        arg0->unk28.f[2] = -1.0f;
        mathOneFloatPY((Vec3s *) arg0, temp_a2);
        if (temp_s1 != 0) {
            pointListRPY(1, (Vec3s *) temp_s1, temp_a2->f, temp_a2->f);
        }
        arg0->unk28.f[0] = -arg0->unk28.f[0];
        arg0->unk28.f[1] = -arg0->unk28.f[1];
        arg0->unk28.f[2] = -arg0->unk28.f[2];
    }
    arg0->unk36 = 0;
}

void func_80023FA4_24BA4(CamLightStruct *arg0, CamLightStruct *arg1) {
    f32 sp2C[11];

    if (trackNearestIntersection(0, &arg0->unkC, &arg1->unkC, sp2C, 0x2404, 0) == 0) {
        arg1->unk7 += 5;
        if (arg1->unk7 >= 0x11) {
            arg1->unk7 = 0x10;
        }
    } else {
        arg1->unk7 -= 5;
        if (arg1->unk7 < 0) {
            arg1->unk7 = 0;
        }
    }
}

void camlightVisibilityCheck(void) {
    Camera *cam;
    CamLightStruct *temp_s0;
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

    if (D_800A1108_A1D08 <= 0) {
        return;
    }

    cam = camGetPtr();
    i = 4;
    index = D_800A110C_A1D0C;
    do {
        temp_s0 = &D_800F5B60_B1760[index++];
        if (index >= MAX_CAMLIGHT) {
            index = 0;
        }
        if (temp_s0->unk6 & 0x20) {
            if (mainGetZBCheck(temp_s0->unk37) == 0) {
                temp_s0->unk7 += 8;
                if (temp_s0->unk7 >= 0x11) {
                    temp_s0->unk7 = 0x10;
                }
            } else {
                temp_s0->unk7 -= 8;
                if (temp_s0->unk7 < 0) {
                    temp_s0->unk7 = 0;
                }
            }
            sp64.f[2] = 256.0f;
            mathOneFloatPY(temp_s0, &sp64);
            sp64.f[0] += cam->trans.position.f[0];
            sp64.f[1] += cam->trans.position.f[1];
            sp64.f[2] += cam->trans.position.f[2];
            if (camProjectPoint(sp64.f[0], sp64.f[1], sp64.f[2], &outX, &outY, TRUE)) {
                temp_s0->unk37 = mainAddZBCheck(outX, outY, 3);
            } else {
                temp_s0->unk37 = -1;
            }
        } else if (temp_s0->unk6 != 0) {
            diffX = temp_s0->unkC.x - cam->trans.position.f[0];
            diffY = temp_s0->unkC.y - cam->trans.position.f[1];
            diffZ = temp_s0->unkC.z - cam->trans.position.f[2];
            ft4 = SQ(temp_s0->unk40);
            if ((SQ(diffX) + SQ(diffY) + SQ(diffZ)) < ft4) {
                if (temp_s0->unk6 & 0x40) {
                    if (i != 0) {
                        func_80023FA4_24BA4((void *) cam, (void *) temp_s0);
                        i--;
                        if (i == 0) {
                            matchFound = index;
                        }
                    }
                } else {
                    temp_s0->unk7 = 0x10;
                }
            } else {
                temp_s0->unk7 = 0;
            }
        }
    } while (index != D_800A110C_A1D0C);

    if (i == 0) {
        D_800A110C_A1D0C = matchFound; // !@bug: matchFound could be uninitialized
    }
}

void func_80024330_24F30(s32 arg0, RGBA *arg1, u8 *arg2, u8 *arg3, u8 *arg4, u8 *arg5) {
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

    *arg2 = var_t0;
    *arg3 = var_v1;
    *arg4 = var_v0;
    *arg5 = var_t4;
}

void func_80024430_25030(UnkStruct_80024430_25030 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                         f32 arg7, f32 arg8) {
    ObjectSegmentCustom sp7C;

    if (arg0 == NULL) {
        return;
    }

    sp7C.trans.rotation.x = 0;
    sp7C.trans.rotation.y = 0;
    sp7C.trans.rotation.z = 0;
    // spA4
    sp7C.unk28 = 0.0f;
    while (arg0->unk4 != 0.0f) {
        sp7C.trans.position.x = (arg0->unk0 * arg4) + arg1;
        sp7C.trans.position.y = (arg0->unk0 * arg5) + arg2;
        sp7C.trans.position.z = (arg0->unk0 * arg6) + arg3;
        sp7C.trans.scale = arg0->unk4 * arg7;
        gDPSetPrimColor(sGfxPtr++, 0, 0, arg0->unk9, arg0->unkA, arg0->unkB, (s32) (arg0->unkC * arg8));
        camDoSprite(&sGfxPtr, &D_800F5B54_B1754, &D_800F5B58_B1758, (ObjectSegment *) &sp7C,
                    (Sprite *) D_800A1110_A1D10, 4, 0);
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
    CamLightStruct *var_s0;
    Matrix *temp_v0;
    f32 temp_fv1;
    f32 var_fs0;
    ObjectSegmentCustom sp17C;
    s8 spB4[50 * 4];
    f32 var_fv0;
    s32 i;
    s32 var_s4;

    if (D_800A1108_A1D08 <= 0) {
        return;
    }

    sGfxPtr = *arg0;
    D_800F5B54_B1754 = *arg1;
    D_800F5B58_B1758 = *arg2;
    cam = camGetPtr();
    temp_v0 = camGetInvProjMtx();
    sp1E0 = -temp_v0[0][2][0];
    sp1DC = -temp_v0[0][2][1];
    sp1D8 = -temp_v0[0][2][2];
    var_s4 = 0;
    var_s0 = D_800F5B60_B1760;
    for (i = 50; i--; var_s0++) {
        if ((var_s0->unk6 == 0) || (var_s0->unk7 <= 0) || (var_s0->unk3B <= 0)) {
            continue;
        }

        if (var_s0->unk6 & 0x20) {
            sp17C.trans.position.f[2] = -1.0f;
            mathOneFloatPY((Vec3s *) var_s0, &sp17C.trans.position);
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
            sp17C.trans.position.f[0] = var_s0->unkC.f[0];
            sp17C.trans.position.f[1] = var_s0->unkC.f[1];
            sp17C.trans.position.f[2] = var_s0->unkC.f[2];
            temp_fs1 = var_s0->unkC.f[0] - cam->trans.position.f[0];
            temp_fs2 = var_s0->unkC.f[1] - cam->trans.position.f[1];
            temp_fs3 = var_s0->unkC.f[2] - cam->trans.position.f[2];
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
            if (var_s0->unk6 & 0x80) {
                var_fs0 = (var_s0->unk28.x * sp1CC.z) + (var_s0->unk28.y * sp1CC.y) + (var_s0->unk28.z * sp1CC.x);
            }
        }
        var_fs0 *= (sp1E0 * sp1CC.z) + (sp1DC * sp1CC.y) + (sp1D8 * sp1CC.x);
        if ((var_fs0 > 0.7071f)) {
            sp17C.trans.rotation.x = 0;
            sp17C.trans.rotation.y = 0;
            sp17C.trans.rotation.z = 0;
            sp17C.unk28 = 0;
            sp1C8 = var_s0->unk7 * 0.0625f;
            var_fs0 *= ((var_fs0 - 0.7071f) * 3.4141345f);
            if (var_ft5 < var_s0->unk48) {
                var_fv0 = var_s0->unk4C * var_ft5;
                if (var_fv0 < 0.5f) {
                    var_fv0 = var_fs0;
                } else {
                    var_fv0 = 2.0f * ((1.0f - var_fv0) * var_fs0);
                }
                spB4[var_s4 * 4 + 0] = var_s0->unk38;
                spB4[var_s4 * 4 + 1] = var_s0->unk39;
                spB4[var_s4 * 4 + 2] = var_s0->unk3A;
                spB4[var_s4 * 4 + 3] = var_s0->unk3B * var_fv0;
                var_s4++;
            }
            if (!(var_s0->unk6 & 0x20)) {
                var_ft5 *= var_s0->unk44;
                if (var_ft5 > 0.5f) {
                    var_ft5 = 2.0f * (1.0f - var_ft5);
                } else {
                    var_ft5 = 1.0f;
                }
            } else {
                var_ft5 = 1.0f;
            }
            if (var_s0->sprite != NULL) {
                sp17C.trans.scale = var_s0->unk8 * var_fs0 * sp1C8;
                sprSetIA2ColOverride(0xFFU, 0xFFU, 0xFFU, var_s0->unk38, var_s0->unk39, var_s0->unk3A);
                camDoSprite(&sGfxPtr, &D_800F5B54_B1754, &D_800F5B58_B1758, &sp17C, (Sprite *) var_s0->sprite, 0x204,
                            (s32) (255.0f * var_fs0 * var_ft5));
                sprClearIA2ColOverride();
            }
            if (var_s0->unk6 & 4) {
                var_s1 = &D_800A1088_A1C88;
            } else {
                if (var_s0->unk6 & 2) {
                    var_s1 = &D_800A1048_A1C48;
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
