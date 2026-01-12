#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/func_overlay_1_00100000_1ED3B20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/dropletFreeLib.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/dropletAdd.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/dropletUpdate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/dropletUpdateAll.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/func_overlay_1_00100678_1ED4198.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/dropletDrawAll.s")

void func_overlay_1_00100000_1ED3B20(void);

#if 0
void dropletFreeLib(void);
void _AutoExit00001(void) {
    //Call dropletFreeLib();
    //dropletFreeLib();
    func_overlay_1_00100000_1ED3B20();
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o1/overlay_1/_AutoExit00001.s")
#endif
