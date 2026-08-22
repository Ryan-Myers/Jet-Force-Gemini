#include <n_libaudio.h>
#include <ultra64.h>

// Stolen and minimally modified from PD

#define SPEAKERMODE_MONO 1
#define SPEAKERMODE_STEREO 2
#define SPEAKERMODE_HEADPHONE 3
#define SPEAKERMODE_SURROUND 4

SpeakerMode D_80105010_B1750;
u8 D_80105014_B1754[2];
u8 D_80105016_B1756[2];
u8 D_80105018_B1758[4];

void alSurround_ReverbSetup(s32 index, s32 arg1);

void alSurround_OutputType(u8 mode) {
    s32 i;

    D_80105010_B1750.surround = 0;
    D_80105010_B1750.mono = 0;
    D_80105010_B1750.headphone = 0;

    switch (mode) {
        case SPEAKERMODE_MONO:
            D_80105010_B1750.mono = 1;
            break;
        case SPEAKERMODE_HEADPHONE:
            D_80105010_B1750.headphone = 1;
            break;
        case SPEAKERMODE_SURROUND:
            D_80105010_B1750.surround = 1;
            break;
    }

    for (i = 0; i < 2; i++) {
        alSurround_ReverbSetup(i, 0);
    }
}

void alSurround_ReverbSetup(s32 index, s32 arg1) {
    if (arg1 == 0) {
        arg1 = D_80105018_B1758[index];
    }

    D_80105014_B1754[index] = 0;
    D_80105016_B1756[index] = 0;

    switch (arg1) {
        case 2:
            if (D_80105010_B1750.surround) {
                D_80105016_B1756[index] = 1;
            }
            break;
        case 3:
            if (D_80105010_B1750.surround) {
                D_80105014_B1754[index] = 1;
            }
            break;
        case 4:
            if (!D_80105010_B1750.mono) {
                D_80105014_B1754[index] = 1;
            }
            break;
        case 5:
            if (!D_80105010_B1750.mono) {
                D_80105014_B1754[index] = 1;
                D_80105016_B1756[index] = 1;
            }
            break;
    }

    D_80105018_B1758[index] = arg1;
}
