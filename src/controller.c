#include "common.h"

UNUSED s32 D_800A39E0 = 0xFFFFFFFF;
u16 joySecurity = 0xFFFF; //Used when anti-cheat/anti-tamper has failed.

//0xFC750 - 0xFC7CC - 0xFC7D0
//0x800fbb50 - 0x800fbbcc - 0x800FBBD0
// OSMesgQueue joyMessageQueue;
// OSMesg joyMessageBuf;
// OSMesg joyMessage;
// OSContStatus joyStatus[MAXCONTROLLERS];
// OSContPad sControllerCurrData[MAXCONTROLLERS];
// OSContPad sControllerPrevData[MAXCONTROLLERS];
// u16 gControllerButtonsPressed[MAXCONTROLLERS];
// u16 gControllerButtonsReleased[MAXCONTROLLERS];
// u8 sPlayerID[MAXCONTROLLERS];
// u8 enabled[MAXCONTROLLERS];
// u8 connected[MAXCONTROLLERS];
// s32 numberOfJoypads;

/**
 * Return the serial interface message queue.
 */
OSMesgQueue *joyMessageQ(void) {
    return &joyMessageQueue;
}

#ifdef NON_MATCHING
//This will match as soon as I can properly define the data section for this file.
//Having enabled be an extern breaks it.
#define CONTROLLER_MISSING -1
#define CONTROLLER_EXISTS   0
u8 enabled[MAXCONTROLLERS];

s32 joyInit(void) {
	s32 i;
    u8 bitpattern;

    osCreateMesgQueue(&joyMessageQueue, &joyMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &joyMessageQueue, joyMessage);
    osContInit(&joyMessageQueue, &bitpattern, &joyStatus);
    osContStartReadData(&joyMessageQueue);
    joyResetMap();

	for (i = 0; i < ARRAY_COUNT(connected); i++) {
		enabled[i] = TRUE;
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
		enabled[i] = FALSE;
	}

    return CONTROLLER_MISSING;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyInit.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyRead.s")

#ifdef NON_MATCHING
//REQUIRES sPlayerID to be declared in this file.
/**
 * Set the first 4 player ID's to the controller numbers, so players can input in the menus after boot.
 */
void joyResetMap(void) {
    s32 i;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        sPlayerID[i] = i;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/controller/joyResetMap.s")
#endif

void joyDisable(s32 player) {
    enabled[player & 3] = FALSE;
}

void joyEnable(s32 player) {
    enabled[player & 3] = TRUE;
}

void joyCreateMap(s8 *activePlayers) {
    s32 i;
    s32 temp = 0;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (activePlayers[i]) {
            sPlayerID[temp++] = i;
        }
    }
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (!activePlayers[i]) {
            sPlayerID[temp++] = i;
        }
    }
}

u8 joyGetController(s32 player) {
    return sPlayerID[player];
}

u16 joyGetButtons(s32 player) {
    return sControllerCurrData[sPlayerID[player]].button;
}

u16 joyGetPressed(s32 player) {
    return gControllerButtonsPressed[sPlayerID[player]];
}

u16 joyGetReleased(s32 player) {
    return gControllerButtonsReleased[sPlayerID[player]];
}

s8 joyClamp(s8 stickMag);

s8 joyGetStickX(s32 player) {
    return joyClamp(sControllerCurrData[sPlayerID[player]].stick_x);
}

s8 joyGetAbsX(s32 player) {
    return sControllerCurrData[sPlayerID[player]].stick_x;
}

s8 joyGetStickY(s32 player) {
    return joyClamp(sControllerCurrData[sPlayerID[player]].stick_y);
}

s8 joyGetAbsY(s32 player) {
    return sControllerCurrData[sPlayerID[player]].stick_y;
}

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
