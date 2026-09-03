/* 
    Define a macro for signed multiplication of two registers, storing the result in a destination register
    This is similar to the MUL instruction but is mult instead of multu.
*/
#define MULS(rd, rs, rt) \
    mult   rs, rt; \
    mflo   rd

/* TODO: Figure out how to use LD symbols for this in the hasm files */
#define ASM_CODE_VRAM_START 0x80000450
