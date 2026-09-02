#ifndef _MIPS_H_
#define _MIPS_H_

/**
 * MIPS instruction word layout (big-endian).
 * Used for runtime instruction patching during relocation.
 */
typedef union MipsInstruction {
    unsigned int word;
    struct {
        unsigned int opcode : 6;  // Instruction opcode (bits 26-31)
        unsigned int target : 26; // J/JAL target address >> 2 (bits 0-25)
    } jump;
    struct {
        unsigned short upper;     // rs/rt/opcode (bits 16-31)
        unsigned short immediate; // immediate value (bits 0-15)
    } itype;
    // MIPS instruction format for shift instructions
    struct {
        unsigned int opcode : 6;
        unsigned int sourceRegister : 5;
        unsigned int targetRegister : 5;
        unsigned int destinationRegister : 5;
        unsigned int shiftAmount : 5;
        unsigned int function : 6;
    } shiftEncoding;
    // MIPS instruction format for addiu instruction
    struct {
        unsigned int opcode : 6;
        unsigned int sourceRegister : 5;
        unsigned int targetRegister : 5;
        unsigned int immediate : 16; // Value to add to the source register and store in the target register
    } addiu;
    unsigned char bytes[4];
} MipsInstruction;

#define ADDRESS_CANARY 0x666

#define ADDIU_OPCODE 0x9
#define OR_OPCODE 0x25
#define SLL_OPCODE 0

// Register that stores the return address of a function that has been called but not yet returned from. This is used to detect when a function has been called and not yet returned, which can be useful for debugging and profiling.
#define RA_REG 31
#define ZERO_REG 0

// Relocation types (patchOperation values - destination type)
#define RELOC_PATCH_WORD 2   // R_MIPS_32: Full 32-bit word
#define RELOC_PATCH_JAL 4    // R_MIPS_26: JAL target
#define RELOC_PATCH_HI16 5   // Upper 16 bits (LUI)
#define RELOC_PATCH_LO16 6   // Lower 16 bits (ADDIU/LW/SW immediate)

// Relocation types (relocType values - source type)  
#define RELOC_TYPE_EXTERNAL 0  // Reference to external symbol (via overlayRomTable)
#define RELOC_TYPE_LOCAL 1     // Local offset within overlay
#define RELOC_TYPE_JUMP 2      // Jump target (relative)
#define RELOC_TYPE_DATA 3      // Data section relocation

#endif
