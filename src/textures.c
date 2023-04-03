#include "common.h"

enum TriangleBatchFlags {
    BATCH_FLAGS_NONE = 0,
    BATCH_FLAGS_BIT0 = (1 << 0),
    BATCH_FLAGS_BIT1 = (1 << 1),
    BATCH_FLAGS_BIT2 = (1 << 2),
    BATCH_FLAGS_BIT3 = (1 << 3),
    BATCH_FLAGS_BIT4 = (1 << 4),
    BATCH_FLAGS_BIT5 = (1 << 5),
    BATCH_FLAGS_BIT6 = (1 << 6),
    BATCH_FLAGS_BIT7 = (1 << 7),
    BATCH_FLAGS_BIT8 = (1 << 8),
    BATCH_FLAGS_BIT9 = (1 << 9),
    BATCH_FLAGS_BIT10 = (1 << 10),
    BATCH_FLAGS_BIT11 = (1 << 11),
    BATCH_FLAGS_BIT12 = (1 << 12),
    BATCH_FLAGS_BIT13 = (1 << 13),
    BATCH_FLAGS_BIT14 = (1 << 14),
    BATCH_FLAGS_BIT15 = (1 << 15),
    BATCH_FLAGS_BIT16 = (1 << 16),
    BATCH_FLAGS_BIT17 = (1 << 17),
    BATCH_FLAGS_BIT18 = (1 << 18),
    BATCH_FLAGS_BIT19 = (1 << 19),
    BATCH_FLAGS_BIT20 = (1 << 20),
    BATCH_FLAGS_BIT21 = (1 << 21),
    BATCH_FLAGS_BIT22 = (1 << 22),
    BATCH_FLAGS_BIT23 = (1 << 23),
    BATCH_FLAGS_BIT24 = (1 << 24),
    BATCH_FLAGS_BIT25 = (1 << 25),
    BATCH_FLAGS_BIT26 = (1 << 26),
    BATCH_FLAGS_BIT27 = (1 << 27),
    BATCH_FLAGS_BIT28 = (1 << 28),
    BATCH_FLAGS_BIT29 = (1 << 29),
    BATCH_FLAGS_BIT30 = (1 << 30),
    BATCH_FLAGS_BIT31 = (1 << 31)
};

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texInitTextures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texDisableModes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texEnableModes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texModelTextureLoad.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texLoadTexture.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texFreeTexture.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texGetTextureNum.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texLoadTextureAddr.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/setTexMemColour.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texFrame.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texDPInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texDPTextureX.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/sprSetTextureFilter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/sprSetIA2ColOverride.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/sprClearIA2ColOverride.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/sprDPset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texLoadSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texFreeSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/func_800577D8_583D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/func_80057B8C_5878C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/func_80057C50_58850.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texAnimateSprite.s")

#pragma GLOBAL_ASM("asm/nonmatchings/textures/texAnimateTexSprite.s")

void texAnimateTexture(TextureHeader *texture, u32 *triangleBatchInfoFlags, s32 *arg2, s32 updateRate) {
    s32 breakVar;
    u16 *frameAdvanceDelay;
    u8 blink;
    s32 arg2Temp = *arg2;
    s32 flags = *triangleBatchInfoFlags;

    if (flags & BATCH_FLAGS_BIT17) {
        blink = blinkMode;
        if (!(flags & BATCH_FLAGS_BIT18)) {
            if (blink == 0) {
                if (mathRnd(0, 1000) > 985) {
                    flags &= ~BATCH_FLAGS_BIT19;
                    flags |= BATCH_FLAGS_BIT18;
                }
            } else if (blink != 2) {
                flags &= ~BATCH_FLAGS_BIT19;
                flags |= BATCH_FLAGS_BIT18;
            }
        } else if (!(flags & BATCH_FLAGS_BIT19)) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
            if (arg2Temp >= texture->numOfTextures) {
                if (blink == 3) {
                    arg2Temp = texture->numOfTextures - 1;
                } else {
                    arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                    if (arg2Temp < 0) {
                        arg2Temp = 0;
                        flags &= ~(BATCH_FLAGS_BIT19 | BATCH_FLAGS_BIT18);
                    } else {
                        flags |= BATCH_FLAGS_BIT19;
                    }
                }
            }
        } else {
            arg2Temp -= texture->frameAdvanceDelay * updateRate;
            if (arg2Temp < 0) {
                arg2Temp = 0;
                flags &= ~(BATCH_FLAGS_BIT19 | BATCH_FLAGS_BIT18);
            }
        }
        blinkMode = 0;
    } else if (flags & BATCH_FLAGS_BIT18) {
        if (!(flags & BATCH_FLAGS_BIT19)) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
        } else {
            //Probably a fake match var, but it works.
            frameAdvanceDelay = &texture->frameAdvanceDelay;
            arg2Temp -= (*frameAdvanceDelay) * updateRate;
        }
        do {
            breakVar = 0;
            if (arg2Temp < 0) {
                arg2Temp = -arg2Temp;
                flags &= ~BATCH_FLAGS_BIT19;
                breakVar = 1;
            }
            if (arg2Temp >= texture->numOfTextures) {
                arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                flags |= BATCH_FLAGS_BIT19;
                breakVar = 1;
            }
        } while (breakVar != 0);
    } else if (!(flags & BATCH_FLAGS_BIT19)) {
        arg2Temp += (texture->frameAdvanceDelay * updateRate);
        if (arg2Temp >= texture->numOfTextures) {
            do {
                arg2Temp -= texture->numOfTextures;
            } while (arg2Temp >= texture->numOfTextures);
        }
    } else {
        arg2Temp = arg2Temp - (texture->frameAdvanceDelay * updateRate);
        while (arg2Temp < 0) {
            arg2Temp += texture->numOfTextures;
        }
    }
    *arg2 = arg2Temp;
    *triangleBatchInfoFlags = flags;
}

void initColourCycle(unkResetColourCycle *arg0, s32 arg1) {
    arg0->unkC = (unkResetColourCycle *) objGetTable(arg1);
    resetColourCycle(arg0);
}

void resetColourCycle(unkResetColourCycle *arg0) {
    unkResetColourCycle *temp_v0;

    temp_v0 = arg0->unkC;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = temp_v0->unk8;
    arg0->unk9 = temp_v0->unk9;
    arg0->unkA = temp_v0->unkA;
    arg0->unkB = temp_v0->unkB;
}

#pragma GLOBAL_ASM("asm/nonmatchings/textures/updateColourCycle.s")

void resetMixCycle(PulsatingLightData *data) {
    s32 i;
    data->currentFrame = 0;
    data->time = 0;
    data->totalTime = 0;
    data->outColorValue = data->frames[0].value;
    for (i = 0; i < data->numberFrames; i++) {
        data->totalTime += data->frames[i].time;
    }
}

void updateMixCycle(PulsatingLightData *data, s32 timeDelta) {
    s32 thisFrameIndex, nextFrameIndex;

    if (data->numberFrames > 1) {
        data->time += timeDelta;
        while (data->time >= data->totalTime) {
            data->time -= data->totalTime;
        }
        while (data->time >= data->frames[data->currentFrame].time) {
            data->time -= data->frames[data->currentFrame].time;
            data->currentFrame++;
            if (data->currentFrame >= data->numberFrames) {
                data->currentFrame = 0;
            }
        }
        thisFrameIndex = data->currentFrame;
        nextFrameIndex = thisFrameIndex + 1;
        if (nextFrameIndex >= data->numberFrames) {
            nextFrameIndex = 0;
        }

        data->outColorValue = data->frames[thisFrameIndex].value +
                              ((data->frames[nextFrameIndex].value * data->time) / data->frames[thisFrameIndex].time);
    }
}
