#ifndef _SAVES_H_
#define _SAVES_H_


#include "structs.h"
#include <PR/ultratypes.h>
#include <PR/os_message.h>
#include <PR/os_pfs.h>

#ifdef VERSION_us
#define nosMotorInit osMotorInit
#define nosMotorStart osMotorStart
#define nosMotorStop osMotorStop
#endif

SIDeviceStatus packFileSize(s32 controllerIndex, s32 fileNum, s32 *fileSize);
char *string_to_font_codes(char *inString, char *outString, s32 stringLength);
SIDeviceStatus packOpen(s32 controllerIndex);
s32 packClose(s32 controllerIndex);
SIDeviceStatus packOpenFile(s32 controllerIndex, char *fileName, char *fileExt, s32 *fileNumber);
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
s32 packClearGameEprom(s32 saveFileNum, Game *game);
void packEraseEprom(void);
s32 packLoadGameEprom(s32 saveFileNum, Game *game);
s32 packLoadGlobalFlagsEprom(u64 *flags);
s32 packSaveGameEprom(s32 saveFileNum, Game *game);
s32 packSaveGlobalFlagsEprom(u64 *flags);
void rumbleTick(s32 updateRate);

#endif
