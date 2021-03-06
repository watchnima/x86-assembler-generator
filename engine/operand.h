#ifndef NASM_OPERAND_H
#define NASM_OPERAND_H

struct random_mem_addr {
    int32_t base;           /* base is an index to select a data:
                               0 <= index < SECTION_DATA_NUM */
    int32_t scale;          /* value: 2, 4, 8 */
    int32_t index;
    int32_t disp;
};

bool create_specific_register(enum reg_enum R_reg, operand_seed *opnd_seed, char *buffer);
bool create_control_register(operand_seed *opnd_seed, char *buffer);
bool create_segment_register(operand_seed *opnd_seed, char *buffer);
bool create_unity(operand_seed *opnd_seed, char *buffer);
bool create_sbyteword(operand_seed *opnd_seed, char *buffer);
bool create_sbytedword(operand_seed *opnd_seed, char *buffer);
bool create_gpr_register(operand_seed *opnd_seed, char *buffer);
bool create_immediate(operand_seed *opnd_seed, char *buffer);
bool create_memory(operand_seed *opnd_seed, char *buffer);
bool init_popf(void);
bool init_specific_mem(enum reg_enum R_reg);
bool init_specific_register(enum reg_enum R_reg);
bool init_fpu_dest_register(enum reg_enum R_reg);
bool create_fpu_register(operand_seed *opnd_seed, char *buffer);
bool create_mmx_register(operand_seed *opnd_seed, char *buffer);
bool create_xmm_register(operand_seed *opnd_seed, char *buffer);

char *preappend_mem_size(char *asm_mem, opflags_t opndsize);

#endif
