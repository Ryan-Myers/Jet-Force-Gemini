#ifndef _OVERLAYS_OVERLAY45_H_
#define _OVERLAYS_OVERLAY45_H_

#include <PR/ultratypes.h>
#include <PR/os_message.h>

void osCic6105SendData(u8 *challenge, OSMesgQueue *arg1);
void osCic6105StartGetData(OSMesgQueue *arg0);
void osCic6105GetReadData(u8 *response);

#endif
