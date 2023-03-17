#include "common.h"

/**
 * Return the serial interface message queue.
 */
OSMesgQueue *joyMessageQ(void) {
    return &joyMessageQueue;
}

#ifdef NON_MATCHING
//This will match as soon as I can properly define the data section for this file.
//Having D_800FBBC4 be an extern breaks it.
#define CONTROLLER_MISSING -1
#define CONTROLLER_EXISTS   0
u8 D_800FBBC4[MAXCONTROLLERS];

s32 joyInit(void) {
	s32 i;
    u8 bitpattern;

    osCreateMesgQueue(&joyMessageQueue, &joyMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &joyMessageQueue, joyMessage);
    osContInit(&joyMessageQueue, &bitpattern, &joyStatus);
    osContStartReadData(&joyMessageQueue);
    joyResetMap();

	for (i = 0; i < ARRAY_COUNT(connected); i++) {
		D_800FBBC4[i] = TRUE;
		connected[i] = FALSE;
	}

	numberOfJoypads = 0;

	for (i = 0; i < MAXCONTROLLERS; i++) {
		if ((bitpattern & (CONT_ABSOLUTE << i)) && (!(joyStatus[i].errno & CONT_NO_RESPONSE_ERROR))) {
			connected[i] = TRUE;
			numberOfJoypads++;
		}
	}

	if (connected[0] != 0) {
		return CONTROLLER_EXISTS;
	}

	for (i = 0; i < MAXCONTROLLERS; i++) {
		D_800FBBC4[i] = FALSE;
	}

    return CONTROLLER_MISSING;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyInit.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyRead.s")

#ifdef NON_MATCHING
//REQUIRES the variables to be declared in this file.
/**
 * Set the first 4 player ID's to the controller numbers, so players can input in the menus after boot.
 */
void joyResetMap(void) {
    s32 i;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        D_800FBBC4[i] = i;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyResetMap.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyDisable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyEnable.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyCreateMap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetController.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetButtons.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetPressed.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetReleased.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetStickX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetAbsX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetStickY.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyGetAbsY.s")

#define JOYSTICK_DEADZONE 5 //was 8 in DKR
#define JOYSTICK_MAX_RANGE 65 //was 70 in DKR
s8 joyClamp(s8 stickMag) {
    if (stickMag < JOYSTICK_DEADZONE && stickMag > -JOYSTICK_DEADZONE) {
        return 0;
    }
    if (stickMag > 0) {
        stickMag -= JOYSTICK_DEADZONE;
        if (stickMag > JOYSTICK_MAX_RANGE) {
            stickMag = JOYSTICK_MAX_RANGE;
        }
    } else {
        stickMag += JOYSTICK_DEADZONE;
        if (stickMag < -JOYSTICK_MAX_RANGE) {
            stickMag = -JOYSTICK_MAX_RANGE;
        }
    }
    return stickMag;
}

/**
 * Used when anti-cheat/anti-tamper has failed
 */
void joySetSecurity(void) {
	joySecurity = 0;
}

s32 joyCharVal(void) {
    return 1;
}
