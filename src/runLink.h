#ifndef _RUNLINK_H_
#define _RUNLINK_H_

#include <PR/ultratypes.h>

// This function is unique in that it has no specific limit on arguments, 
// and they can change even within the same function call it.
// This empty signature seems to be the way to handle it.
s32 TrapDanglingJump(); 

void runlinkFreeCode(s32 arg0);
void runlinkInitialise(void);
s32 runlinkDownloadCode(s32);
void runlinkLowMemoryPanic(void);
s32 runlinkIsModuleLoaded(s32 module);
s32 runlinkGetAddressInfo(u32 address, s32 *moduleId, s32 *moduleAddress, u32 **arg3);
void runlinkFlushModules(void);

#endif
