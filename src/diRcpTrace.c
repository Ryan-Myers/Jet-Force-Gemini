#include "common.h"
#include "memory.h"

#ifdef VERSION_kiosk
#define DIRCPTRACE_BUFFER_SIZE 0x960
#define DIRCPTRACE_MAX_ENTRIES 0xC8
#else
#define DIRCPTRACE_BUFFER_SIZE 0x600
#define DIRCPTRACE_MAX_ENTRIES 0x80
#endif

typedef struct {
    Gfx *gdl;   /* +0 */
    char *file; /* +4 */
    s32 line;   /* +8 */
} DLDebugInfo;

DLDebugInfo *D_800A64B0_A70B0[2] = { NULL, NULL };
s32 D_800A64B8_A70B8 = 0;
s32 D_801012A0_B1750[2];
UNUSED u8 D_801012A8_B1758[0x1008];

void diRcpTraceInit(void) {
    D_800A64B0_A70B0[0] = mmAlloc(DIRCPTRACE_BUFFER_SIZE, COLOUR_TAG_CYAN);
    D_800A64B0_A70B0[1] = mmAlloc(DIRCPTRACE_BUFFER_SIZE, COLOUR_TAG_CYAN);
}

void diRcpTraceReset(void) {
    s32 *index = &D_800A64B8_A70B8;

    *index = 1 - *index;
    D_801012A0_B1750[*index] = 0;
}

const char D_800AE670[] = "diRcpTrace: Buffer not allocated!\n";
const char D_800AE694[] = "diRcpTrace: Buffer overflow!\n";

void diRcpTrace(Gfx *gdl, char *file, s32 line) {
    if (D_800A64B0_A70B0[D_800A64B8_A70B8] != NULL) {
        if (D_801012A0_B1750[D_800A64B8_A70B8] < DIRCPTRACE_MAX_ENTRIES) {
            D_800A64B0_A70B0[D_800A64B8_A70B8][D_801012A0_B1750[D_800A64B8_A70B8]].gdl = gdl;
            D_800A64B0_A70B0[D_800A64B8_A70B8][D_801012A0_B1750[D_800A64B8_A70B8]].file = file;
            D_800A64B0_A70B0[D_800A64B8_A70B8][D_801012A0_B1750[D_800A64B8_A70B8]].line = line;
            D_801012A0_B1750[D_800A64B8_A70B8]++;
        }
    }
}

void diRcpTraceGetInfo(Gfx *gdl, char **prevFile, s32 *prevLine, Gfx **prevGdl, char **nextFile, s32 *nextLine,
                       Gfx **nextGdl) {
    s32 i;
    s32 temp_v1;
    Gfx *tempGdl;
    Gfx *bestPrevGdl;
    Gfx *bestNextGdl;
    DLDebugInfo *prevInfo;
    DLDebugInfo *nextInfo;

    temp_v1 = 1 - D_800A64B8_A70B8;
    bestPrevGdl = NULL;
    bestNextGdl = (Gfx *) -1;
    prevInfo = NULL;
    nextInfo = NULL;

    for (i = 0; i < D_801012A0_B1750[temp_v1]; i++) {
        tempGdl = D_800A64B0_A70B0[temp_v1][i].gdl;
        if (gdl >= tempGdl && bestPrevGdl < tempGdl) {
            bestPrevGdl = tempGdl;
            prevInfo = D_800A64B0_A70B0[temp_v1] + i;
        }
        if (gdl < tempGdl && tempGdl < bestNextGdl) {
            bestNextGdl = tempGdl;
            nextInfo = D_800A64B0_A70B0[temp_v1] + i;
        }
    }

    if (prevInfo != NULL) {
        *prevFile = prevInfo->file;
        *prevLine = prevInfo->line;
        *prevGdl = prevInfo->gdl;
    } else {
        *prevFile = NULL;
    }

    if (nextInfo != NULL) {
        *nextFile = nextInfo->file;
        *nextLine = nextInfo->line;
        *nextGdl = nextInfo->gdl;
    } else {
        *nextFile = NULL;
    }
}
