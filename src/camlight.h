#ifndef _CAMLIGHT_H_
#define _CAMLIGHT_H_

#include "gameVi.h"
#include "structs.h"
#include "textures.h"
#include "types.h"
#include <PR/gbi.h>
#include <PR/sp.h>
#include <PR/ultratypes.h>

#define MAX_CAMLIGHTS 50

typedef struct CamLight_inner2 {
    /* 0x00 */ char pad[0x2D];
    /* 0x2D */ u8 unk2D;
} CamLight_inner2;

typedef struct CamLight_inner1 {
    /* 0x00 */ CamLight_inner2 *unk0;
    /* 0x04 */ char pad4[0x64];
    /* 0x68 */ Vec3f *unk68;
} CamLight_inner1;

typedef struct CamLight_inner {
    /* 0x00 */ Vec3s unk0;
    /* 0x08 */ char pad8[0x4]; /* maybe part of unk0[3]? */
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ char pad18[0x22]; /* maybe part of unk14[9]? */
    /* 0x3A */ s8 unk3A;
    /* 0x3B */ char pad3B[0x31]; /* maybe part of unk3A[MAX_CAMLIGHTS]? */
    /* 0x6C */ CamLight_inner1 **unk6C;
} CamLight_inner; /* size = 0x70 */

typedef struct CamLight {
    /* 0x00 */ Vec3s unk0;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ Vec3f unkC;
    /* 0x18 */ CamLight_inner *unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ Vec3f unk28;
    /* 0x34 */ s8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ s8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ TextureSprite *sprite;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
} CamLight; /* size = 0x50 */

typedef struct UnkStruct_Arg1 {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ u8 unk1F;
} UnkStruct_Arg1; /* size = 0x20 */

void camlightUpdate(CamLight *arg0);
void camlightUpdate(CamLight *arg0);
void camlightDelete(CamLight *);
CamLight *camlightAdd(CamLight_inner *arg0, UnkStruct_Arg1 *arg1);
void camlightFlush(void);

#endif
