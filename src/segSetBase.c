#include "common.h"

/**
 * Set the offset for the given address, so the RSP can get the correct physical address.
 * This is used for the colour and depth buffers.
 */
void segSetBase(Gfx **dlist, s32 segment, s32 base) {
    gSPSegment((*dlist)++, segment, base + K0BASE)
}
