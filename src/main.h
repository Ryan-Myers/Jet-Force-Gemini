#ifndef _MAIN_H_
#define _MAIN_H_

#include <PR/ultratypes.h>

#ifdef VERSION_kiosk
// The kiosk doesn't have this function, but it can be used so many times in a single function, 
// so this is a macro to avoid having to ifdef it out multiple times.
#define mainPreNMI()
#else
void mainPreNMI(void);
#endif

u8 mainGetNumberOfPlayers(void);

#endif
