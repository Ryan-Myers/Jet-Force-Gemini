#ifndef _CAMLIGHT_H_
#define _CAMLIGHT_H_

#define MAX_CAMLIGHT 50 

typedef struct CamLightStruct_inner2_s {
    char pad[0x2D];
    u8 unk2D;
} CamLightStruct_inner2;

typedef struct CamLightStruct_inner1_s {
    CamLightStruct_inner2 *unk0;
    char pad[0x64];
    Vec3f *unk68;
} CamLightStruct_inner1;

typedef struct CamLightStruct_inner_s {
    /* 0x04 */ char pad0[0xC]; /* maybe part of unk0[3]? */
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ char pad18[0x22]; /* maybe part of unk14[9]? */
    /* 0x3A */ s8 unk3A;
    /* 0x3B */ char pad3B[0x31]; /* maybe part of unk3A[MAX_CAMLIGHT]? */
    /* 0x6C */ CamLightStruct_inner1 **unk6C;
} CamLightStruct_inner; /* size = 0x70 */

typedef struct CamLightStruct_s {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ Vec3f unkC;
    /* 0x18 */ CamLightStruct_inner *unk18;
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
} CamLightStruct; /* size = 0x50 */

typedef struct UnkStruct_Arg1_s {
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


void camlightUpdate(CamLightStruct *arg0);
void camlightUpdate(CamLightStruct *arg0);
void camlightDelete(CamLightStruct *);
CamLightStruct *camlightAdd(s32 arg0, UnkStruct_Arg1 *arg1);

// static functions
void func_80023FA4_24BA4(CamLightStruct *arg0, CamLightStruct *arg1);
void func_80024430_25030(UnkStruct_80024430_25030 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                         f32 arg7, f32 arg8);
                         
#endif
