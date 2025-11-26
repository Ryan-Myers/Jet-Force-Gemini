#include "common.h"

f32 Cosf(s32);
f32 Sinf(s32);
void guPerspectiveF(float mf[4][4], u16* perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
	float	cot, *scaly;
	int	i, j;

    cot = Cosf((fovy * 91.022f)) / Sinf((fovy * 91.022f));
    scaly = &scale;
	mf[0][0] = (cot / aspect) * (*(&scale));
    mf[0][1] = 0;
    mf[0][2] = 0;
    mf[0][3] = 0;
    mf[1][0] = 0;
	mf[1][1] = cot * (*(&scale));
    mf[1][2] = 0;
    mf[1][3] = 0;
    mf[2][0] = 0;
    mf[2][1] = 0;
	mf[2][2] = ((near + far) / (near - far)) * (*(&scale));
    mf[2][3] = -1;
    mf[3][0] = 0;
    mf[3][1] = 0;
	mf[3][2] = ((2.0f * near * far) / (near - far)) * (*(&scale));
	mf[3][3] = 0;
    
	if (perspNorm != (u16 *) NULL) {
	    if (near+far<=2.0f) {
		*perspNorm = (u16) 0xFFFF;
	    } else  {
		*perspNorm = (s32) ((2.0f*65536.0f)/(near+far));
		if (*perspNorm<=0) 
		    *perspNorm = (u16) 0x0001;
	    }
	}
}
