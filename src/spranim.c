#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/spranimInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/spranimControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/sprasjiInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/spranimOnceControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/effectboxControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/exitControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/doorControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/doorUnlock.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/doorRegisterOpener.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/doorSwitchControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/skyControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/texscrollControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/rangetriggerControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/objEnsureNotTargetted.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/GetAutoAimTarget.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animobjControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animplayerInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animplayerControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animdroidInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animdroidControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animGirlEventHandler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animBoyEventHandler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animDogEventHandler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/animtriggerControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/hitanimInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/hitanim2Init.s")

#ifdef VERSION_kiosk
#pragma GLOBAL_ASM("asm/nonmatchings/spranim/hitanimsndInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/hitanim2sndInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/hitanimsndControl.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/muzzleFlashInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/severedLimbInit.s")

//CollectedDroneHead is in here, seemingly when the mainSaveGlobalFlags calls are made
#pragma GLOBAL_ASM("asm/nonmatchings/spranim/severedLimbControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/severedLimbDestroy.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/dropletInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/dropletControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/splashInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/splashControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/waterbubbleInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/waterbubbleControl.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/invisibilityInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/spranim/invisibilityControl.s")
