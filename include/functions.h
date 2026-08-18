#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"
#include "libultra_internal.h"
#include "sched.h"
#include "memory.h"
#include "boot.h"
#include "libc/stdarg.h"
#include "enums.h"

#ifdef VERSION_us
#define nosMotorInit osMotorInit
#define nosMotorStart osMotorStart
#define nosMotorStop osMotorStop
#endif

f32 Cosf(s32);


void func_80021434_22034(unk800DC950 *arg0, s32 arg1);
s32 *objGetTable(s32 index);
void resetMixCycle(PulsatingLightData *data);
void updateMixCycle(PulsatingLightData *data, s32 timeDelta);
void resetColourCycle(unkResetColourCycle *arg0);
void initColourCycle(unkResetColourCycle *arg0, s32 arg1);
s32 mathRnd(s32, s32);
void texAnimateTexture(TextureHeader *texture, u32 *triangleBatchInfoFlags, s32 *arg2, s32 updateRate);
void setTexMemColour(s32 tagId);
//void func_800570D8_57CD8(TextureHeader *tex, u8 *addr); //build_tex_display_list in DKR
void func_8005719C_57D9C(Gfx **dlist, TextureHeader *tex, s32 rtile, s32 tmem);
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
s32 func_8004D250_4DE50(s32 controllerIndex, s32 fileNum);
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
#ifdef VERSION_kiosk
void rumbleStop(s32 controllerIndex);
void rumbleKill(void);
#else
void rumbleStop(s32 controllerIndex, s32 arg1);
void rumbleKill(s32 arg0);
#endif
void rumbleProcessing(s32 arg0);
void rumbleStart(s32 controllerIndex, s32 arg1, f32 arg2);
void rumbleAlter(s32 controllerIndex, s32 arg1, f32 arg2);
void rumbleMax(s32 controllerIndex, s32 arg1, f32 arg2);
void amStop(void);
void func_80044FAC_45BAC(void);
void func_800468EC_474EC(s32 arg0);
void mainChangeLevel(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void mainInitGame(void);
void mainSetGameFlag(GameFlags arg0, s32 arg1);
s32 osBootRamTest1_6105(void);
s32 osBootRamTest2_6105(void);
s32 diPrintf(const char *format, ...);
void diPrintfAll(Gfx **dList);
void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfSetXY(u16 x, u16 y);
s32 amAudioMgrGetNextFrameCount(void);
void func_8004FB30_50730(OSSched *sc);
char *osScGetTaskType(s32 taskID);
void func_8004FC64_50864(OSScTask *task);
void segSetBase(Gfx **dlist, s32 segment, s32 base);
Gfx *func_8004FF64_50B64(OSSched *sc, 
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
// u32 *func_8004FC6C_5086C(OSSched *arg0, s32 arg1, Gfx *arg2, OSMesgQueue *arg3, u32 *arg4);
// void diRcpTraceGetInfo(u32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6);
void mainSetDefaultCharacter(MultiCharacter *character, s32 arg1);
void *controlGetWeaponDef(s32);
void mainInitMultiPlayer(void);

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
void freeLights(void);

//diCpu.c
void func_800681D0_68DD0(s32 x, s32 y, char *s);
void cpuXYPrintf(s32 x, s32 y, const char *format, ...);
void func_8006837C_68F7C(void);
void func_80066EB0_67AB0(OSThread *);

//gameVi.c
s32 viGetVideoMode(void);


// level
LevelHeader *levelGetLevel(void);

//camera 
s32 camGetMode(void);

// This function is unique in that it has no specific limit on arguments, 
// and they can change even within the same function call it.
// This empty signature seems to be the way to handle it.
s32 TrapDanglingJump();

s32 objTvTimes(s32 timer);

s32 piRomGetFileSize(u32 assetIndex);

#endif
