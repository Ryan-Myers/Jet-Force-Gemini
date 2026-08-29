/*====================================================================
 * sched.h
 *
 * Synopsis:
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

/**************************************************************************
 *
 *  $Revision: 1.7 $
 *  $Date: 1997/02/11 08:32:02 $
 *  $Source: /exdisk2/cvs/N64OS/Master/cvsmdev2/PR/include/sched.h,v $
 *
 **************************************************************************/

#ifndef __sched__
#define __sched__

#include <ultra64.h>
#include "macros.h"
#include "os_internal_rsp.h"

#define OS_SC_STACKSIZE      0x2000

#define OS_SC_RETRACE_MSG       1
#define OS_SC_DONE_MSG          2
#define OS_SC_RDP_DONE_MSG      3
#define OS_SC_PRE_NMI_MSG       4
#define OS_SC_LAST_MSG          4	/* this should have highest number */
#define OS_SC_MAX_MESGS         8

#define OS_SC_ID_NONE   0
#define OS_SC_ID_AUDIO  1
#define OS_SC_ID_VIDEO  2
#define OS_SC_ID_PRENMI 3

#define OSMESG_SWAP_BUFFER 0
#define MESG_SKIP_BUFFER_SWAP 8

typedef struct {
    short type;
    char  misc[30];
} OSScMsg;

typedef struct OSScTask_s {
    struct OSScTask_s   *next;          /* note: this must be first */
    u32                 state;
    u32			flags;
    void		*framebuffer;	/* used by graphics tasks */

    OSTask              list;
    OSMesgQueue         *msgQ;
    OSMesg              msg;
#ifndef _FINALROM                      /* all #ifdef items should    */
    OSTime              startTime;      /* remain at the end!!, or    */
    OSTime              totalTime;      /* possible conflict if       */
#endif                                  /* FINALROM library used with */
#ifdef RAREDIFFS
    s32                 unk58;
    s32                 unk5C;
    s32                 unk60;
    s32                 unk64;
    s32                 unk68;          /* Added by Rare?             */
    s32                 unk6C;          /* Task ID, used in debug functions in JFG */
#endif
} OSScTask;                             /* non FINALROM code          */

/*
 * OSScTask flags:
 */
#define OS_SC_NEEDS_RDP	        0x0001	/* uses the RDP */
#define OS_SC_NEEDS_RSP	        0x0002  /* uses the RSP */
#define OS_SC_DRAM_DLIST        0x0004  /* SP & DP communicate through DRAM */
#define OS_SC_PARALLEL_TASK     0x0010	/* must be first gfx task on list */
#define OS_SC_LAST_TASK	        0x0020	/* last task in queue for frame */
#define OS_SC_SWAPBUFFER        0x0040	/* swapbuffers when gfx task done */

#define OS_SC_RCP_MASK		0x0003	/* mask for needs bits */
#define OS_SC_TYPE_MASK		0x0007	/* complete type mask */
/*
 * OSScClient:
 *
 * Data structure used by threads that wish to communicate to the
 * scheduling thread
 *
 */
typedef struct SCClient_s {
#ifdef RAREDIFFS
    u8                  id;   /* Client ID, added by Rareware to single out individual scheduler clients */
#endif
    struct SCClient_s   *next;  /* next client in the list      */
    OSMesgQueue         *msgQ;  /* where to send the frame msg  */
} OSScClient;

typedef struct {
    OSScMsg     retraceMsg;
    OSScMsg     prenmiMsg;
    OSMesgQueue interruptQ;
    OSMesg      intBuf[OS_SC_MAX_MESGS];
    OSMesgQueue cmdQ;
    OSMesg      cmdMsgBuf[OS_SC_MAX_MESGS];
    OSThread    thread;    
#ifdef JFGDIFFS
    u8          pad[0x80];
#endif
    OSScClient  *clientList;
    OSScTask    *audioListHead;
    OSScTask    *gfxListHead;
    OSScTask    *audioListTail;
    OSScTask    *gfxListTail;
    OSScTask    *curRSPTask;
    OSScTask    *curRDPTask;
#ifdef RAREDIFFS
   OSScTask    *unkTask;
#endif
    u32         frameCount;
    s32         doAudio;
} OSSched;

void            osCreateScheduler(OSSched *s, void *stack, OSPri priority,
                                  u8 mode, u8 numFields);
#ifdef RAREDIFFS
void            osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ, u8 id);
OSMesgQueue    *osScGetInterruptQ(OSSched *s);
void            func_80079760(OSSched *s);
#else
void            osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ);
#endif
void            osScRemoveClient(OSSched *s, OSScClient *c);
OSMesgQueue     *osScGetCmdQ(OSSched *s);


void func_8004FB30_50730(OSSched *sc);
char *osScGetTaskType(s32 taskID);
void func_8004FC64_50864(OSScTask *task);
Gfx *func_8004FF64_50B64(OSSched *sc, 
    char **retFile, u32 *retUnk0xc, s32 *retUnk0x10,
    char **retFile_2, u32 *retUnk0xc_2, s32 *retUnk0x10_2);
Gfx *func_8004FC6C_5086C(OSSched *, s32, Gfx *, OSMesgQueue *, Gfx *);

extern char D_800AD510[];// = "(Audio task)";
extern char D_800AD520[];// = "(Game task)";
extern char D_800AD52C[];// = "(DI task)";
extern char D_800AD538[];// = "(DI benchmark test)";
extern char D_800AD550[];// = "(Clone task)";
extern char D_800AD560[];// = "(Refract task)";
extern char D_800AD570[];// = "(Blur task)";
extern char D_800AD580[];// = "(Unknown task type %d)";
extern char D_800AD598[];// = "RCP TASK INFO-------------"; //AND MUCH MORE STRING CONTENT
extern char D_800AD7D4[];// = "SP CRASHED, gfx=%x";
extern char D_800AD7E8[];// = "DP CRASHED, gfx=%x";
extern char D_800AD7FC[];// = "** GFX overflow **";
extern char D_800AD810[];// = "** MTX overflow **";
extern char D_800AD824[];// = "** VTX overflow **";
extern char D_800AD838[];// = "** POL overflow **";
extern char D_800AD84C[];// = "Version %s";
extern char D_800AD7D0[];// = "7.2";
extern s32 D_800A38B0_A44B0[]; // = { OSMESG_SWAP_BUFFER, OSMESG_SWAP_BUFFER };
extern s32 D_800A38B8_A44B8[]; //gBootBlackoutMesg[] = { OSMESG_SWAP_BUFFER, MESG_SKIP_BUFFER_SWAP };
extern u8 gGfxOverflowed;
extern u8 gMtxOverflowed;
extern u8 gVtxOverflowed;
extern u8 gPolOverflowed;
extern s32 gCurRSPTaskCounter;
extern s32 gCurRDPTaskCounter;
extern s8 gNextFrameCount;
extern u64 gRetraceCounter64;
extern s32 gCurRSPTaskIsSet;
extern s32 gCurRDPTaskIsSet;
extern s32 *currentScreen;
extern s32 *otherScreen;
extern s32 *otherZbuf;

#endif

