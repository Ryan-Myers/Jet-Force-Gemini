#ifndef _SAVES_H_
#define _SAVES_H_

#include "structs.h"
#include <PR/os_cont.h>
#include <PR/os_message.h>
#include <PR/os_pfs.h>
#include <PR/os_pi.h>
#include <PR/ultratypes.h>

#ifdef VERSION_us
#define nosMotorInit osMotorInit
#define nosMotorStart osMotorStart
#define nosMotorStop osMotorStop
#endif

// These are the unique game codes assigned to DKR
#define NDYJ 0x4E44594A // 'NDYJ' ASCII
#define NDYP 0x4E445950 // 'NDYP' ASCII
#define NDYE 0x4E445945 // 'NDYE' ASCII
#define JPN_GAME_CODE NDYJ
#define PAL_GAME_CODE NDYP
#define NTSC_GAME_CODE NDYE
#define CHARFILETYPE 0x43484152
// This is RARE's unique code
#define COMPANY_CODE 0x3459

// The size of 1 sector is 128 pages (16K bytes), and each page of 0~0x7f, 0x80~0xff, 0x100~0x17f... is considered to be
// 1 sector. So basically, flash is written in sectors, and changing a single page needs to read a sector, modify the
// page, and write back the whole sector.
#define SECTOR_SIZE 128
#define SECTOR(sectorNum) ((sectorNum) * SECTOR_SIZE)
#define NUMBER_OF_SECTORS 8

typedef struct FlashSector {
    u8 saveData[0x7E];
    u16 checksum;
} FlashSector;

// The usable space in a flash sector is 126 bytes, and the checksum is 2 bytes
// so the total size is 128 bytes
#define FlashSectorSize (sizeof(FlashSector) - sizeof(u16))

typedef struct unkD_800A3EAC {
    s32 unk0;
    f32 unk4;
} unkD_800A3EAC;
extern unkD_800A3EAC D_800A344C_A404C[3];
extern OSPfs pfs[MAXCONTROLLERS];
extern OSMesgQueue *sControllerMesgQueue;
extern s32 D_800FDDB4_B85F4; // UNUSED
extern u8 gN64FontCodes[];
extern s8 *sPackDirectory;
extern u8 sRumblePaksPresent;
extern u8 D_800A3448_A4048; // sControllerPakPresent?
extern OSMesgQueue flashEventQueue;
extern OSMesg flashEventBuf[1];
extern OSMesg cartEventBuf[1];
extern OSMesgQueue cartEventQueue;
extern OSIoMesg flashMesgReqBlock;
extern u8 D_800A3464_A4064;
extern u8 D_800A346C_A406C;
extern f32 D_800AD4FC;
extern RumbleStruct rumbleStructArray[];
extern f32 D_800AD504;
extern f32 D_800AD500;
extern RumbleStruct D_800FDF5A_B879A[];
extern u8 D_800A3470_A4070;
extern s32 D_800A3474_A4074[];
extern u8 D_800A34AC_A40AC[]; // Looks to be the default state for a new save file, which is copied to the save file if
                              // the checksum doesn't match.

SIDeviceStatus packFileSize(s32 controllerIndex, s32 fileNum, s32 *fileSize);
char *string_to_font_codes(char *inString, char *outString, s32 stringLength);
SIDeviceStatus packOpen(s32 controllerIndex);
s32 packClose(s32 controllerIndex);
SIDeviceStatus packOpenFile(s32 controllerIndex, char *fileName, char *fileExt, s32 *fileNumber);
SIDeviceStatus packReadFile(s32 controllerIndex, s32 fileNum, u8 *data, s32 dataLength);
s32 packGetFileType(s32 controllerIndex, s32 fileNum);
char *font_codes_to_string(char *inString, char *outString, s32 stringLength);
SIDeviceStatus packWriteFile(s32 controllerIndex, s32 fileNumber, char *fileName, char *fileExt, u8 *dataToWrite,
                             s32 fileSize);
SIDeviceStatus packCopyFile(s32 controllerIndex, s32 fileNumber, s32 secondControllerIndex);
SIDeviceStatus packDeleteFile(s32 controllerIndex, s32 fileNum);
void packDirectoryFree(void);
SIDeviceStatus packFormat(s32 controllerIndex);
SIDeviceStatus packRepair(s32 controllerIndex);
SIDeviceStatus packIsPresent(s32 controllerIndex);
s32 nosMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
SIDeviceStatus packDirectory(s32 controllerIndex, s32 maxNumOfFilesToGet, char **fileNames, char **fileExtensions,
                             u32 *fileSizes, u8 *fileTypes);
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
void packClearGameEprom(s32 saveFileNum, Game *game);
void packEraseEprom(void);
s32 packLoadGameEprom(s32 saveFileNum, Game *game);
s32 packLoadGlobalFlagsEprom(FlashSector *save);
s32 packSaveGameEprom(s32 saveFileNum, Game *game);
s32 packSaveGlobalFlagsEprom(FlashSector *save);
void rumbleTick(s32 updateRate);

#endif
