#include "common.h"

UNUSED s32 D_800A39E0 = 0xFFFFFFFF;
u16 joySecurity = 0xFFFF; // Used when anti-cheat/anti-tamper has failed.

OSMesgQueue joyMessageQueue;
OSMesg joyMessageBuf;
OSMesg joyMessage;
OSContStatus joyStatus[MAXCONTROLLERS];
OSContPad sControllerCurrData[MAXCONTROLLERS];
OSContPad sControllerPrevData[MAXCONTROLLERS];
u16 gControllerButtonsPressed[MAXCONTROLLERS];
u16 gControllerButtonsReleased[MAXCONTROLLERS];
u8 sPlayerID[MAXCONTROLLERS];
u8 enabled[MAXCONTROLLERS];
u8 connected[MAXCONTROLLERS];
s32 numberOfJoypads;

OSMesgQueue *joyMessageQ(void) {
    return &joyMessageQueue;
}

#define CONTROLLER_MISSING -1
#define CONTROLLER_EXISTS 0

void joyResetMap(void);

s32 joyInit(void) {
    s32 i;
    u8 bitpattern;

    osCreateMesgQueue(&joyMessageQueue, &joyMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &joyMessageQueue, joyMessage);
    osContInit(&joyMessageQueue, &bitpattern, joyStatus);
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

    // Must be on one line to match
    // clang-format off
    for (i = 0; i < MAXCONTROLLERS; i++) { enabled[i] = FALSE; }
    // clang-format on

    return CONTROLLER_MISSING;
}

#define NUMBER_OF_SAVE_FILES 6

s32 joyRead(s32 saveDataFlags, s32 updateRate) {
    Game *gamePtr;
    OSMesg unusedMsg;
    s32 i;

    if (osRecvMesg(&joyMessageQueue, &unusedMsg, OS_MESG_NOBLOCK) == 0) {
        // Back up old controller data
        for (i = 0; i < MAXCONTROLLERS; i++) {
            sControllerPrevData[i] = sControllerCurrData[i];
        }
        osContGetReadData(sControllerCurrData);
        numberOfJoypads = 0;
        for (i = 0; i < MAXCONTROLLERS; i++) {
            if (sControllerCurrData[i].errno == 0) {
                connected[i] = TRUE;
                numberOfJoypads++;
            } else {
                connected[i] = FALSE;
            }
        }
        if (saveDataFlags != 0) {
            gamePtr = mainGetGame();
            if (saveDataFlags & 0x40000000) {
                packSaveGameEprom((saveDataFlags >> 3) & 7, gamePtr);
            }
            if (saveDataFlags & 0x20000000) {
                packClearGameEprom((saveDataFlags >> 6) & 7, gamePtr);
            }
            if (saveDataFlags & 0x10000000) {
                for (i = 0; i < NUMBER_OF_SAVE_FILES; i++) {
                    packClearGameEprom(i, gamePtr);
                }
            }
            if (saveDataFlags & 0x01000000) {
                packEraseEprom();
            }
            if (saveDataFlags & 0x80000000) {
                packLoadGameEprom(saveDataFlags & 7, gamePtr);
            }
            if (saveDataFlags & 0x8000000) {
                gamePtr = mainGetGameArrayPtr();
                for (i = 0; i < NUMBER_OF_SAVE_FILES; i++) {
                    packLoadGameEprom(i, &gamePtr[i]);
                }
            }
            if ((saveDataFlags & 0x4000000)) {
                packLoadGlobalFlagsEprom(&globalflags);
            }
            if (saveDataFlags & 0x02000000) {
                packSaveGlobalFlagsEprom(&globalflags);
            }
            // Reset all flags
            saveDataFlags = 0;
        }
        rumbleTick(updateRate);
        osContStartReadData(&joyMessageQueue);
    }
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (!enabled[i]) {
            sControllerCurrData[i].button = 0;
            sControllerCurrData[i].stick_x = 0;
            sControllerCurrData[i].stick_y = 0;
        }
        // XOR the diff between the last read of the controller data with the current read to see what buttons have been
        // pushed and released.
        gControllerButtonsPressed[i] =
            ((sControllerCurrData[i].button ^ sControllerPrevData[i].button) & sControllerCurrData[i].button) &
            joySecurity;
        gControllerButtonsReleased[i] =
            ((sControllerCurrData[i].button ^ sControllerPrevData[i].button) & sControllerPrevData[i].button) &
            joySecurity;
    }
    return saveDataFlags;
}

/**
 * Set the first 4 player ID's to the controller numbers, so players can input in the menus after boot.
 */
void joyResetMap(void) {
    s32 i;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        sPlayerID[i] = i;
    }
}

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

#define JOYSTICK_DEADZONE 5   // was 8 in DKR
#define JOYSTICK_MAX_RANGE 65 // was 70 in DKR
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
