#ifndef _DIRCPTRACE_H_
#define _DIRCPTRACE_H_

#include <PR/gbi.h>
#include <PR/ultratypes.h>

void diRcpTraceInit(void);
void diRcpTraceReset(void);
void diRcpTrace(Gfx *gdl, char *file, s32 line);
void diRcpTraceGetInfo(Gfx *gdl, char **prevFile, s32 *prevLine, Gfx **prevGdl, char **nextFile, s32 *nextLine,
                       Gfx **nextGdl);

#endif
