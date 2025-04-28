#ifndef _STRUCTS_H_
#define _STRUCTS_H_

/* Note: Structs are not complete, take them with a grain of salt. */

#include "types.h"
#include "memory.h"
#include "gbi.h"

//Size: 0x538
typedef struct Game {
    u8 pad[0x538];
} Game;

/* Size: 0x230 bytes */
typedef struct epcInfo {
/*  0x00 */ s32 unk0;
/*  0x04 */ s32 unk4;
/*  0x08 */ s32 unk8;
/*  0x0C */ s32 unkC;
/*  0x10 */ s32 unk10;
/*  0x14 */ s32 thread[1];
/*  0x18 */ s32 unk18;
/*  0x1C */ s32 unk1C;
/*  0x20 */ s64 at;
/*  0x28 */ s64 v0;
/*  0x30 */ s64 v1;
/*  0x38 */ s64 a0;
/*  0x40 */ s64 a1;
/*  0x48 */ s64 a2;
/*  0x50 */ s64 a3;
/*  0x58 */ s64 t0;
/*  0x60 */ s64 t1;
/*  0x68 */ s64 t2;
/*  0x70 */ s64 t3;
/*  0x78 */ s64 t4;
/*  0x80 */ s64 t5;
/*  0x88 */ s64 t6;
/*  0x90 */ s64 t7;
/*  0x98 */ s64 s0;
/*  0xA0 */ s64 s1;
/*  0xA8 */ s64 s2;
/*  0xB0 */ s64 s3;
/*  0xB8 */ s64 s4;
/*  0xC0 */ s64 s5;
/*  0xC8 */ s64 s6;
/*  0xD0 */ s64 s7;
/*  0xD8 */ s64 t8;
/*  0xE0 */ s64 t9;
/*  0xE8 */ s64 gp;
/*  0xF0 */ s64 sp;
/*  0xF8 */ s64 s8;
/* 0x100 */ s64 ra;
/* 0x108 */ s32 unk108;
/* 0x10C */ s32 unk10C;
/* 0x110 */ s32 unk110;
/* 0x114 */ s32 unk114;
/* 0x118 */ s32 sr;
/* 0x11C */ s32 epc;
/* 0x120 */ s32 cause;
/* 0x124 */ s32 badvaddr;
/* 0x128 */ s32 unk128;
/* 0x12C */ s32 unk12C;
/* 0x130 */ f32 unk130[3];
/* 0x13C */ u8 pad13C[0x74];
/* 0x1B0 */ u8 pad1B0[0x80];
} epcInfo;

// Stolen from PD
// This hacky structure allows coords to be accessed using
// coord->x, coord->y and coord->z, but also as
// coord->f[0], coord->f[1] and coord->f[2].
// In some places code only matches when using the float array.
typedef struct Vec3f {
  union {
    struct {
      f32 x;
      f32 y;
      f32 z;
    };
    f32 f[3];
  };
} Vec3f;

typedef struct Object_Racer {
  /* 0x000 */ u8 pad00[0x64];
  /* 0x064 */ f32 unk64;
  /* 0x068 */ u8 pad68[0xD8];
  /* 0x140 */ s16 y_rotation_offset;
  /* 0x144 */ s16 z_rotation_offset;
  /* 0x142 */ s16 x_rotation_offset;
  /* 0x146 */ s16 unk146;
} Object_Racer;

typedef struct ObjHeaderParticleEntry {
  /* 0x00 */ s32 upper;
  /* 0x04 */ s32 lower;
} ObjHeaderParticleEntry;

// Size: 0x18 bytes
typedef struct ObjectHeader48 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u16 unk6; //Misc Asset Id?
    union {
        u32 unk8;
        struct {
            u8 unk8A;
            u8 unk9;
            u8 unkA;
            u8 unkB;
        };
    };
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
} ObjectHeader48;

typedef struct ObjectHeader {
             u8 pad0[0x25];
  /* 0x25 */ s8 unk25;
             u8 pad26[0x20];
  /* 0x48 */ ObjectHeader48 *unk48;
} ObjectHeader;

/* Size: 0x018 bytes */
typedef struct ObjectTransform {
  /* 0x0000 */ s16 y_rotation;
  /* 0x0002 */ s16 x_rotation;
  /* 0x0004 */ s16 z_rotation;
  /* 0x0006 */ s16 unk6; // Flags?
  /* 0x0008 */ f32 scale;
  /* 0x000C */ f32 x_position;
  /* 0x0010 */ f32 y_position;
  /* 0x0014 */ f32 z_position;
} ObjectTransform;

typedef struct ObjectSegment {
  /* 0x0000 */ ObjectTransform trans;
  /* 0x0018 */ u8 pad18[0x28];
  /* 0x0048 */ ObjectHeader *header;
} ObjectSegment;

typedef struct Object {
  /* 0x0000 */ ObjectSegment segment;
  /* 0x0046 */ u8 pad46[0x4];
  /* 0x0048 */ s16 behaviorId;
  /* 0x0049 */ u8 pad49[0x1B];
  /* 0x0068 */ Object_Racer *racer; //Object_64 in DKR.
  /* 0x0070 */ u8 pad70[8];
  /* 0x0074 */ u32 *unk74;
} Object;

typedef struct VertexPosition {
/* 0x00 */ s16 x;
/* 0x02 */ s16 y;
/* 0x04 */ s16 z;
} VertexPosition;

typedef struct huft {
  u8 e;                /* number of extra bits or operation */
  u8 b;                /* number of bits in this code or subcode */
  union {
    u16 n;              /* literal, length base, or distance base */
    struct huft *t;     /* pointer to next level of table */
  } v;
} huft;

/* Size: 0x20 bytes */
typedef struct TextureHeader {
    /* 0x00 */ u8 width;
    /* 0x01 */ u8 height;
    /* 0x02 */ u8 format; // Lower 4 bits determines image format.
        // 0 = RGBA32
        // 1 = RGBA16
        // 2 = I8
        // 3 = I4
        // 4 = IA16
        // 5 = IA8
        // 6 = IA4
        // 7 = CI4 (16 colors)
        // 8 = CI8 (64 colors)
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ u8 numberOfInstances; // Always 1 in the ROM.
    /* 0x06 */ s16 flags;
        // 0x04 = Interlaced texture
        // 0x40 = U clamp flag. 0 = Wrap, 1 = Clamp
        // 0x80 = V clamp flag. 0 = Wrap, 1 = Clamp
    /* 0x08 */ s16 ciPaletteOffset;
    /* 0x0A */ s16 numberOfCommands; // initialized in RAM; Number of commands in the texture display list. (Usually 0x07)
    /* 0x0C */ Gfx *cmd; // initialized in RAM; Pointer to texture display list.
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u16 numOfTextures; // For animated textures, static textures are just 0x01. Each texture has it's own header.
    /* 0x14 */ u16 frameAdvanceDelay; // How many frames to delay before moving to the next texture.
    /* 0x16 */ s16 textureSize; // Size in number of bytes, including the header
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 isCompressed;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
} TextureHeader;

/* Size: 8 bytes */
typedef struct FontCharData {
    u8 textureID; // Texture Pointer Index?
    u8 ulx; // Upper Left Corner? Maybe only used when FontData->unk20 is 0 for some reason.
    u8 width; // Font Char Width?
    u8 height; // Font Char Height?
    u8 s; // Upper left coordinate for the texture derived from X?
    u8 t; // Upper left coordinate for the texture derived from Y?
    u8 lrx; // Related to the lower right X Coordinate.
    u8 lry; // Related to the lower right Y Coordinate.
} FontCharData;

/* Size: 0x400 bytes */
typedef struct FontData {
/* 0x000 */ s32 unk0; // Unused
/* 0x004 */ char name[28];
/* 0x020 */ u16 x;
/* 0x022 */ u16 y;
/* 0x024 */ u16 charWidth;
/* 0x026 */ u16 charHeight;
/* 0x028 */ u8 loadedFonts[24];
/* 0x040 */ s16 textureID[32];
/* 0x080 */ TextureHeader *texturePointers[32];
/* 0x100 */ FontCharData letter[96];
} FontData;

/* Size: 0x20 bytes */
//Dialogue Box text linked list (Including background struct)
typedef struct DialogueBox {
/* 0x00 */ u8 unk00; // Unused
/* 0x01 */ u8 textNum; // A number that is drawn with the text, like a balloon door telling you how many more you need.
/* 0x02 */ char *text; // Pointer to the text array
/* 0x06 */ s16 x1; // Left position of the text
/* 0x08 */ s16 y1; // Top portion of the text
/* 0x0A */ s16 x2; // Right portion of the text
/* 0x0C */ s16 y2; // Bottom portion of the text
/* 0x0E */ u8 textColourR;
/* 0x0F */ u8 textColourG;
/* 0x10 */ u8 textColourB;
/* 0x11 */ u8 textColourA;
/* 0x12 */ u8 textBGColourR;
/* 0x13 */ u8 textBGColourG;
/* 0x14 */ u8 textBGColourB;
/* 0x15 */ u8 textBGColourA;
/* 0x16 */ u8 opacity;
/* 0x17 */ u8 font;
/* 0x18 */ struct DialogueBox *nextBox;
} DialogueBox;

/* Size: 0x20 bytes */
typedef struct DialogueTextElement {
/* 0x00 */ u8 unk0;
/* 0x01 */ u8 number;
/* 0x02 */ u8 unk2;
/* 0x03 */ u8 unk3;
/* 0x04 */ char *text; // Pointer to the text array
/* 0x08 */ s16 posX;
/* 0x0A */ s16 posY;
/* 0x0C */ s16 offsetX;
/* 0x0E */ s16 offsetY;
/* 0x10 */ u8 textColourR;
/* 0x11 */ u8 textColourG;
/* 0x12 */ u8 textColourB;
/* 0x13 */ u8 textColourA;
/* 0x14 */ u8 textBGColourR;
/* 0x15 */ u8 textBGColourG;
/* 0x16 */ u8 textBGColourB;
/* 0x17 */ u8 textBGColourA;
/* 0x18 */ u8 opacity;
/* 0x19 */ u8 font;
/* 0x1A */ u16 flags;
/* 0x1C */ DialogueBox *nextBox;
} DialogueTextElement;

/* Size: 0x28 bytes */
// Dialogue Box background
typedef struct DialogueBoxBackground {
  /* 0x00 */ s16 xpos;
  /* 0x02 */ s16 ypos;
  /* 0x04 */ s16 x1;
  /* 0x06 */ s16 y1;
  /* 0x08 */ s16 x2;
  /* 0x0A */ s16 y2;
  /* 0x0C */ s16 width;
  /* 0x0E */ s16 height;
  /* 0x10 */ u8 backgroundColourR; //Ideally should also be an array like the rest, but doesn't currently match.
  /* 0x11 */ u8 backgroundColourG;
  /* 0x12 */ u8 backgroundColourB;
  /* 0x13 */ u8 backgroundColourA;
  //ColourRGBA textColour;
  /* 0x14 */ u8 textColourR;
  /* 0x15 */ u8 textColourG;
  /* 0x16 */ u8 textColourB;
  /* 0x17 */ u8 textColourA;
  /* 0x18 */ u8 textBGColourR;
  /* 0x19 */ u8 textBGColourG;
  /* 0x1A */ u8 textBGColourB;
  /* 0x1B */ u8 textBGColourA;
  /* 0x1C */ u8 opacity;
  /* 0x1D */ u8 font;
  /* 0x1E */ u16 flags;
  /* 0x20 */ s16 textOffsetX; // Functionally Unused
  /* 0x22 */ s16 textOffsetY; // Functionally Unused
  /* 0x24 */ DialogueBox *textBox;
} DialogueBoxBackground;


/* Size: 0x88 bytes */
typedef struct unk800DC950 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s32 unk44;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    s16 unk5A;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    s16 unk70;
    s16 unk72;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    s16 unk7A;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
} unk800DC950;


typedef struct PulsatingLightDataFrame {
    u16 value;
    u16 time;
} PulsatingLightDataFrame;
typedef struct PulsatingLightData {
    u16 numberFrames;
    u16 currentFrame;
    u16 time;
    u16 totalTime;
    s32 outColorValue;
    PulsatingLightDataFrame frames[1];
} PulsatingLightData;

typedef struct UnkLight {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u8 pad4[0x3C];
    /* 0x40 */ u8 red;
    /* 0x41 */ u8 green;
    /* 0x42 */ u8 blue;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ u8 pad48[0xB];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x12];
    /* 0x6C */ s32 unk6C;
} UnkLight;

typedef struct unkResetColourCycle {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    struct unkResetColourCycle *unkC;
} unkResetColourCycle;

/* Size: 0x34 bytes. */
typedef struct ScreenViewport {
    /* 0x00 */ s32 x1;
    /* 0x04 */ s32 y1;
    /* 0x08 */ s32 x2;
    /* 0x0C */ s32 y2;
    /* 0x10 */ s32 posX;
    /* 0x14 */ s32 posY;
    /* 0x18 */ s32 width;
    /* 0x1C */ s32 height;
    /* 0x20 */ s32 scissorX1;
    /* 0x24 */ s32 scissorY1;
    /* 0x28 */ s32 scissorX2;
    /* 0x2C */ s32 scissorY2;
    /* 0x30 */ s32 flags;
} ScreenViewport;

typedef enum {
    CONTROLLER_PAK_GOOD,
    NO_CONTROLLER_PAK,
    CONTROLLER_PAK_INCONSISTENT,
    CONTROLLER_PAK_WITH_BAD_ID,
    CONTROLLER_PAK_FULL,
    CONTROLLER_PAK_CHANGED,
    CONTROLLER_PAK_BAD_DATA, //Moved places in JFG from DKR
    CONTROLLER_PAK_UNK7,
    RUMBLE_PAK // Moved places since DKR
} SIDeviceStatus;

typedef struct RumbleStruct {
    union {
        s16 half;
        u16 half_unsigned;
        struct {
            u16 upper : 4;
            u16 uppermid : 4;
            s8 lower;
        };
        struct {
            u8 state;
            u8 flag;
        };
    } state;
    s16 unk2;
    s16 unk4;
    s16 rumbleTime;
    s16 timer;
} RumbleStruct;

typedef struct RomDefHeader {
    s16 id;
    u8 size;
} RomDefHeader;

/* Size: 0x24 / 36 bytes */
typedef struct SoundMask {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u16 soundId;
    /* 0x0E */ u8 volume;
    /* 0x0F */ u8 pitch;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x14 */ s32 distance;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ struct SoundMask **soundMask;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
} SoundMask;

/* Size: 8 bytes */
typedef struct LevelHeader_70_18 {
    s32 unk0; //0x0000001E
    u8 red; //0xFF
    u8 green; //0x70
    u8 blue; //0x00
    u8 alpha; //0xFF
} LevelHeader_70_18;

/* Unknown size */
typedef struct LevelHeader_70 {
    /* 0x00 */ s32 unk0;  //0x00000004
    /* 0x04 */ s32 unk4;  //0x00000000
    /* 0x08 */ s32 unk8;  //0x00000000
    /* 0x0C */ s32 unkC;  //0x00000000
    /* 0x10 */ u8 red;    //0x72
    /* 0x11 */ u8 green;  //0x75
    /* 0x12 */ u8 blue;   //0x73
    /* 0x13 */ u8 alpha;  //0x20
    /* 0x14 */ u8 red2;   //0xFF
    /* 0x15 */ u8 green2; //0x00
    /* 0x16 */ u8 blue2;  //0x00
    /* 0x17 */ u8 alpha2; //0xFF
    /* 0x18 */ LevelHeader_70_18 unk18[1]; // Actual length depends on unk0
  } LevelHeader_70;

/* Size: 0xC4 bytes */
typedef struct LevelHeader {
    /* 0x00 */ s8 world;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 unk4[4];
    /* 0x08 */ f32 course_height;
    /* 0x0C */ u8 unkC[10];
    /* 0x16 */ u8 unk16[10];
    /* 0x20 */ s8 *AILevelTable;
  
    /* 0x24 */ u8 pad24[6];
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 pad2B[9];
  
    /* 0x34 */ s16 geometry;
    /* 0x36 */ s16 collectables; // Objects such as bananas, balloons, etc.
    /* 0x38 */ s16 skybox;
  
    // Fog related?
    /* 0x3A */ s16 fogNear;
    /* 0x3C */ s16 fogFar;
    /* 0x3E */ s16 fogR;
    /* 0x40 */ s16 fogG;
    /* 0x42 */ s16 fogB;
  
    /* 0x44 */ u8 unk44[0x5];
  
    /* 0x49 */ s8 skyDome;
    /* 0x4A */ s8 playerIndex;
    /* 0x4B */ s8 laps;
    /* 0x4C */ s8 race_type;
    /* 0x4D */ s8 vehicle;
    /* 0x4E */ s8 available_vehicles;
  
    /* 0x4F */ s8 unk4F[3];
  
    /* 0x52 */ u8 music;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u16 instruments;
    /* 0x56 */ u8 unk56; // values between 2 and 8 (except 5 and 7), used to determine waves count?
    /* 0x57 */ u8 unk57; // possible values: 2,4,8,16,20, related to waves
    /* 0x58 */ u8 unk58; // possible values: 1,2,4
    /* 0x59 */ u8 unk59; // always 0?
    /* 0x5A */ s16 unk5A; // values between 512 and 4608
    /* 0x5C */ u8 unk5C; // possible values: 1,2,4
    /* 0x5D */ u8 unk5D; // always 0?
    /* 0x5E */ s16 unk5E; // values between 512 and 4963
    /* 0x60 */ s16 unk60; // possible values: 120, 130, 157, 178, 187
    /* 0x62 */ s16 wavePower; // always 256
    /* 0x64 */ s16 unk64; // Always 153 except in Smokey Castle where it's 0 and the title screen where it's 256 (Some form of secondary power)
    /* 0x66 */ s16 unk66; // values between 908 and 2560
    /* 0x68 */ s16 textureId; // always 62 except in the trophy race where it's 205
    /* 0x6A */ u8 unk6A; // values between 1 and 6
    /* 0x6B */ u8 unk6B; // values between 1 and 6
    /* 0x6C */ s8 unk6C; // values between 0 and 4
    /* 0x6D */ s8 unk6D; // values between 0 and 2 except in Hot Top Volcano where it's -2
    /* 0x6E */ s16 unk6E; // possible values: 3,5
  
      //func_800B8134 Seems to use this struct, and it differs on unk70 only.
    union {
    /* 0x70 */ LevelHeader_70 *unk70[1]; // unknown size, however only size of 1 matches
        struct {
    /* 0x70 */ u8 darkVertexColours; // always 1 except in Hot Top Volcano where it's 0
    /* 0x71 */ u8 unk71; // possible values: 0,1
        };
    };
  
    /* 0x74 */ LevelHeader_70 *unk74[7];
  
    // Weather related?
    /* 0x90 */ s16 weatherEnable; // This affects snow density, but for rain, it simply needs to be nonzero.
    /* 0x92 */ s16 weatherType;
    /* 0x94 */ u8 weatherIntensity;
    /* 0x95 */ u8 weatherOpacity;
    /* 0x96 */ s16 weatherVelX;
    /* 0x98 */ s16 weatherVelY;
    /* 0x9A */ s16 weatherVelZ;
  
    /* 0x9C */ s8 cameraFOV; // Must be a value within [0, 90]
    /* 0x9D */ u8 bgColorRed;
    /* 0x9E */ u8 bgColorGreen;
    /* 0x9F */ u8 bgColorBlue;
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ u8 unkA2;
    /* 0xA3 */ s8 unkA3;
    /* 0xA4 */ TextureHeader *unkA4;
    /* 0xA8 */ s16 unkA8;
    /* 0xAA */ s16 unkAA;
    /* 0xAC */ PulsatingLightData *pulseLightData;
  
    /* 0xB0 */ s16 unkB0;
    /* 0xB2 */ u8 unkB2;
    /* 0xB3 */ u8 voiceLimit;
    /* 0xB4 */ u8 unkB4;
    /* 0xB5 */ u8 unkB5;
    /* 0xB6 */ u8 unkB6;
    /* 0xB7 */ u8 unkB7;
    /* 0xB8 */ s8 bossRaceID;
    /* 0xB9 */ u8 unkB9;
    /* 0xBA */ s16 unkBA;
    /* 0xBC */ u8 unkBC;
    /* 0xBD */ s8 unkBD;
    // Multiplayer gradient background
    /* 0xBE */ u8 BGColourBottomR;
    /* 0xBF */ u8 BGColourBottomG;
    /* 0xC0 */ u8 BGColourBottomB;
    /* 0xC1 */ u8 BGColourTopR;
    /* 0xC2 */ u8 BGColourTopG;
    /* 0xC3 */ u8 BGColourTopB;
} LevelHeader;

/* Unknown size */
typedef struct StackInfo {
    unsigned int var[5];
    unsigned int sp;
} StackInfo;

typedef struct SubMiscAssetObjectHeader24 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
} SubMiscAssetObjectHeader24;

typedef struct MiscAssetObjectHeader24 {
    s32 unk0;
    u8 pad2[16];
    SubMiscAssetObjectHeader24 unk14[1];
} MiscAssetObjectHeader24;

/* Size: 0x200 bytes */
typedef struct ObjectLightUnk70 {
    u8 pad[0x200];
} ObjectLightUnk70;

/* Size: 0x74 bytes */
typedef struct ObjectLight {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 type;
    u8 enabled;
    u8 unk5;
    s16 homeX;
    s16 homeY;
    s16 homeZ;
    Object *owner;
    Vec3f pos;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u16 unk3C;
    u16 unk3E;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    union {
        SubMiscAssetObjectHeader24 *unk44;
        MiscAssetObjectHeader24 *unk44_asset;
    };    
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
    u16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    s16 unk5A;
    f32 radius;
    f32 unk60;
    f32 unk64;
    f32 radiusSquare;
    s32 *unk6C;
    ObjectLightUnk70 *unk70;
} ObjectLight;

#endif
