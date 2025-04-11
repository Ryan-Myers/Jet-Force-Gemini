#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"

void squadsPreInit(RomDefHeader *list, s32 listSize);
s32 runlinkDownloadCode(s32);
void *ad_sndp_play(ALBank *arg0, s16 arg1, u16 arg2, u8 arg3, f32 arg4, u8 arg5, void **arg6);
ALLink *func_80085EF0(ALBank *arg0, ALSound *arg1);
u16 amGetSfxCount(void);
s32 scalevol(s32 vol);
OSPiHandle *osCartRomInit(void);
OSPiHandle *osFlashInit(void);
void *mmAlloc(s32 size, u32 colourTag);
void romCopy(u32 romOffset, u32 ramAddress, s32 numBytes);
void mmFree(void *data);
u8 *rzipUncompress(u8 *compressedInput, u8 *decompressedOutput);
s32 rzipUncompressSize(u8 *arg0);
u32 *piRomLoad(u32 assetIndex);
s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
s32 inflate_block(void);
void _blkclr(void *, size_t);
void _bcopy(const void *, void *, size_t);

void mainThread(void *);
void diCpuTraceInit(void);
f32 Cosf(s32);
void camPopModelMtx(Gfx **dlist);
void camRestoreModelMtx(Gfx **dlist);
void camPushModelMtx(Gfx **dList, MatrixS **mtx, ObjectTransform *trans, f32 scale, f32 scaleY);

MemoryPoolSlot *new_memory_pool(MemoryPoolSlot *, s32, s32); // new_memory_pool
s32 allocate_memory_pool_slot(s32 poolIndex, s32 slotIndex, s32 size, s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag);
s32 get_memory_pool_index_containing_address(u8 *address);
void free_memory_pool_slot(s32 poolIndex, s32 slotIndex);
void mmSetDelay(s32 arg0);
s32 *disableInterrupts(void);
void enableInterrupts(s32*);
void *mmAlloc(s32 size, u32 colourTag);
void free_slot_containing_address(u8 *address);
void func_8004B05C(void *dataAddress);
MemoryPoolSlot *allocate_from_memory_pool(s32 poolIndex, s32 size, u32 colourTag);
void runlinkLowMemoryPanic(void);
s32 runlinkIsModuleLoaded(s32 module);
s32 runlinkGetAddressInfo(u32 arg0, s32 *arg1, s32 *arg2, u32 **arg3);
void func_800676F8(void *arg0);
void stop_all_threads_except_main(void);
void func_800677E4(void);
void camlightDelete(void *arg0); //TODO: arg0 typing is incorrect


typedef enum HorizontalAlignmentFlags {
    HORZ_ALIGN_LEFT   = 0,
    HORZ_ALIGN_RIGHT  = 1,
    HORZ_ALIGN_CENTER = 4
} HorizontalAlignmentFlags;

typedef enum VerticalAlignmentFlags {
    VERT_ALIGN_TOP    = 0,
    VERT_ALIGN_BOTTOM = 2,
    VERT_ALIGN_MIDDLE = 8
} VerticalAlignmentFlags;

typedef enum AlignmentFlags {
    ALIGN_TOP_LEFT      = VERT_ALIGN_TOP    | HORZ_ALIGN_LEFT,
    ALIGN_TOP_CENTER    = VERT_ALIGN_TOP    | HORZ_ALIGN_CENTER,
    ALIGN_TOP_RIGHT     = VERT_ALIGN_TOP    | HORZ_ALIGN_RIGHT,
    ALIGN_MIDDLE_LEFT   = VERT_ALIGN_MIDDLE | HORZ_ALIGN_LEFT,
    ALIGN_MIDDLE_CENTER = VERT_ALIGN_MIDDLE | HORZ_ALIGN_CENTER,
    ALIGN_MIDDLE_RIGHT  = VERT_ALIGN_MIDDLE | HORZ_ALIGN_RIGHT,
    ALIGN_BOTTOM_LEFT   = VERT_ALIGN_BOTTOM | HORZ_ALIGN_LEFT,
    ALIGN_BOTTOM_CENTER = VERT_ALIGN_BOTTOM | HORZ_ALIGN_CENTER,
    ALIGN_BOTTOM_RIGHT  = VERT_ALIGN_BOTTOM | HORZ_ALIGN_RIGHT
} AlignmentFlags;

void func_80070518(Gfx **dList, DialogueBoxBackground *box, char *text, AlignmentFlags alignmentFlags);
void fontPrintWindowXY(Gfx **displayList, s32 windowId, s32 xpos, s32 ypos, char *text, AlignmentFlags alignmentFlags);


void camSetScissor(Gfx **dlist);
void fontConvertString(char *inString, char *outString);
s32 fontStringWidth(char *text, s32 font, s32 arg2);
void func_80071A0C(char *input, char *output, s32 number); //parse_string_with_number
void *func_80071B08(u8); //returns cacheline?
void texDPInit(Gfx **);
void func_800660D4(Gfx **dList, u32 ulx, u32 uly, u32 lrx, u32 lry);

void RevealReturnAddresses(void);
void mmInit(void);
void piInit(void);
void rcpInit(OSSched *sc);
void runlinkFreeCode(s32 arg0);
void runlinkInitialise(void);
void rzipInit(void);
void viInit(OSSched *sc);

s32 vsprintf(char *s, const char *format, ...);
s32 func_80066174(Gfx **dList, s32 asciiVal);
void func_800665C8(void);
void func_80066658(void);
void func_8006667C(void);
void sprintfSetSpacingCodes(s32 arg0);
TextureHeader *load_texture(s32 arg0);

void viGetCurrentSize(s32 *arg0, s32 *arg1);
void rcpInitDp(Gfx **dList);
TextureHeader *texLoadTexture(s32 arg0);
s32 func_80065CB4(Gfx **dlist, char *buffer);


void func_80021444(unk800DC950 *arg0, s32 arg1);
s32 *objGetTable(s32 index);
void resetMixCycle(PulsatingLightData *data);
void updateMixCycle(PulsatingLightData *data, s32 timeDelta);
void resetColourCycle(unkResetColourCycle *arg0);
void initColourCycle(unkResetColourCycle *arg0, s32 arg1);
s32 mathRnd(s32, s32);
void texAnimateTexture(TextureHeader *texture, u32 *triangleBatchInfoFlags, s32 *arg2, s32 updateRate);
void setTexMemColour(s32 tagId);
//void func_80057B8C(TextureHeader *tex, u8 *addr); //build_tex_display_list in DKR
void func_80057C50(Gfx **dlist, TextureHeader *tex, s32 arg2, s32 arg3);
void sprSetIA2ColOverride(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5);
void sprClearIA2ColOverride(void);
void sprSetTextureFilter(s32 arg0);

SIDeviceStatus packFileSize(s32 controllerIndex, s32 fileNum, s32 *fileSize);
char *string_to_font_codes(char *inString, char *outString, s32 stringLength);
SIDeviceStatus packOpen(s32 controllerIndex);
s32 packClose(UNUSED s32 controllerIndex);
SIDeviceStatus packOpenFile(s32 controllerIndex, char *fileName, char *fileExt, s32 *fileNumber);
s32 frontGetLanguage(void);
SIDeviceStatus packReadFile(s32 controllerIndex, s32 fileNum, u8 *data, s32 dataLength);
s32 func_8004DDC4(s32 controllerIndex, s32 fileNum);
char *font_codes_to_string(char *inString, char *outString, s32 stringLength);
SIDeviceStatus packWriteFile(s32 controllerIndex, s32 fileNumber, char *fileName, char *fileExt, u8 *dataToWrite, s32 fileSize);
SIDeviceStatus packCopyFile(s32 controllerIndex, s32 fileNumber, s32 secondControllerIndex);
SIDeviceStatus packDeleteFile(s32 controllerIndex, s32 fileNum);
void packDirectoryFree(void);
SIDeviceStatus packFormat(s32 controllerIndex);
SIDeviceStatus packRepair(s32 controllerIndex);
SIDeviceStatus packIsPresent(s32 controllerIndex);
s32 nosMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
SIDeviceStatus packDirectory(s32 controllerIndex, s32 maxNumOfFilesToGet, char **fileNames, char **fileExtensions, u32 *fileSizes, u8 *fileTypes);
void flashROMInit(void);
void packInit(void);
void flashROMWrite(u32 pageNum, u32 *dramAddr);
void flashROMRead(u32 pageNum, u32 *dramAddr);
void rumbleUpdate(void);
void rumbleStop(s32 controllerIndex);
void rumbleKill(void);
void rumbleProcessing(s32 arg0);
void rumbleStart(s32 controllerIndex, s32 arg1, f32 arg2);
void rumbleAlter(s32 controllerIndex, s32 arg1, f32 arg2);
void rumbleMax(s32 controllerIndex, s32 arg1, f32 arg2);
void amStop(void);
void bootCheckStack(void);
void func_80044938(void);
void func_80046070(s32 arg0);
void mainChangeLevel(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void mainInitGame(void);
void mainSetGameFlag(s32 arg0, s32 arg1);
s32 osBootRamTest1_6105(void);
s32 osBootRamTest2_6105(void);
s32 diPrintf(const char *format, ...);
void diPrintfAll(Gfx **dList);
void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfSetXY(u16 x, u16 y);
s32 amAudioMgrGetNextFrameCount(void);
void func_80050670(OSSched *sc);
char *osScGetTaskType(s32 taskID);
void func_800507A4(OSScTask *task);
void mmSlotPrint(void);
void segSetBase(Gfx **dlist, s32 segment, s32 base);
Gfx *func_80050AA4(OSSched *sc, 
    char **retFile, u32 *retUnk0xc, s32 *retUnk0x10,
    char **retFile_2, u32 *retUnk0xc_2, s32 *retUnk0x10_2);
void objUndoPlayerTumble(Object *obj);
void objDoPlayerTumble(Object *this);


Game *mainGetGame(void);
Game *mainGetGameArrayPtr(void);
s32 mainGetNumberOfCameras(void);
s32 packClearGameEprom(s32 saveFileNum, Game *game);
void packEraseEprom(void);
s32 packLoadGameEprom(s32 saveFileNum, Game *game);
s32 packLoadGlobalFlagsEprom(u64 *flags);
s32 packSaveGameEprom(s32 saveFileNum, Game *game);
s32 packSaveGlobalFlagsEprom(u64 *flags);
void rumbleTick(s32 updateRate);
s32 mainResetPressed(void);
s32 osFlashSectorErase(u32 page_num);
s32 osFlashWriteArray(u32 page_num);
s32 osFlashWriteBuffer(OSIoMesg* mb, s32 priority, void* dramAddr, OSMesgQueue* mq);
s32 osFlashReadArray(OSIoMesg* mb, s32 priority, u32 page_num, void* dramAddr, u32 n_pages, OSMesgQueue* mq);
// void diRcpPrintDL(Gfx *, Gfx *, s32);
// u32 *func_800507AC(OSSched *arg0, s32 arg1, Gfx *arg2, OSMesgQueue *arg3, u32 *arg4);
// void diRcpTraceGetInfo(u32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6);


//controller.c
OSMesgQueue *joyMessageQ(void);
s32 joyInit(void);
s32 joyRead(s32 saveDataFlags, s32 updateRate);
void joyResetMap(void);
void joyDisable(s32 player);
void joyEnable(s32 player);
void joyCreateMap(s8 *activePlayers);
u8 joyGetController(s32 player);
u16 joyGetButtons(s32 player);
u16 joyGetPressed(s32 player);
u16 joyGetReleased(s32 player);
s8 joyGetStickX(s32 player);
s8 joyGetAbsX(s32 player);
s8 joyGetStickY(s32 player);
s8 joyGetAbsY(s32 player);
s8 joyClamp(s8 stickMag);
void joySetSecurity(void);
s32 joyCharVal(void);

//lights.c
s32 addObjectLight(Object *, ObjectHeader48 *);
void lightSetupLightSources(Object *obj);
f32 lightDistanceCalc(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
void freeLights();
void lightCreateLightTable(s32 arg0, s32 arg1, s32 arg2, s32* arg3);
void turnLightOff(UnkLight *light);
void turnLightOn(UnkLight *light);
void toggleLight(UnkLight *light);
void changeLightColour(UnkLight *light, u8 red, u8 green, u8 blue);
void changeLightColourCycle(s32 arg0, s32 arg1); //TODO: arg0 typing is incorrect
void changeLightIntensity(UnkLight *light, u8 intensity);
void lightUpdateLights(s32 arg0);
unk800DC950 **lightGetLights(s32 *arg0);
f32 lightDirectionCalc(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
s32 lightKillGlowingLight(void *arg0); //TODO: arg0 typing is incorrect

//diCpu.c
void func_800684F0(s32 x, s32 y, char *s);
void cpuXYPrintf(s32 x, s32 y, const char *format, ...);
void func_8006869C(void);
void func_80067880(OSThread *);

//gameVi.c
s32 viGetVideoMode(void);

//math_util
u16 Arctanf(f32, f32);
void mathMtxXFMF(Matrix mf, float x, float y, float z, float *ox, float *oy, float *oz);

// audio_manager_1050
void amAmbientStop(void);
void amSndSetVolXYZ(SoundMask *soundMask, u8 volume);
void amSndSetXYZ(SoundMask *soundMask, f32 x, f32 y, f32 z);
void func_80003E24(s32 arg0);


#endif
