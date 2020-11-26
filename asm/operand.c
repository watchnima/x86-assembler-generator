#include "compiler.h"

#include "nasm.h"
#include "nasmlib.h"
#include "insns.h"
#include "error.h"
#include "seed.h"
#include "gendata.h"
#include "regdis.h"
#include "x86pg.h"
#include "operand.h"

static int imms[14] =
{
  0x0,        0x1,        0x7f,
  0x80,       0x7fff,     0x8000,
  0x03030303, 0x44444444, 0x7fffffff,
  0x80000000, 0x80000001, 0xcccccccc,
  0xf5f5f5f5, 0xffffffff
};

void create_specific_register(char *buffer, enum reg_enum R_reg)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create specific register\n");
#endif
    const char *src;
    src = nasm_reg_names[R_reg - EXPR_REG_START];
    sprintf(buffer, "%s\n", src);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new specific register: %s", buffer);
#endif
}

void create_control_register(char *buffer)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create creg\n");
#endif
    int cregi;
    enum reg_enum creg;
    const char *src;

    bseqiflags_t bseqiflags = bseqi_flags(REG_CREG);
    if (X86PGState.seqMode) {
        cregi = X86PGState.bseqi.indexes[BSEQIFLAG_INDEXPOS(bseqiflags)];
    } else {
        int cregn = BSEQIFLAG_INDEXSIZE(bseqiflags);
        cregi = nasm_random32(cregn);
    }
    creg = nasm_rd_creg[cregi];
    src = nasm_reg_names[creg - EXPR_REG_START];
    sprintf(buffer, "%s\n", src);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new creg: %s", buffer);
#endif
}

void create_segment_register(char *buffer)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create sreg\n");
#endif
    int sregi;
    enum reg_enum sreg;
    const char *src;

    bseqiflags_t bseqiflags = bseqi_flags(REG_SREG);
    if (X86PGState.seqMode) {
        sregi = X86PGState.bseqi.indexes[BSEQIFLAG_INDEXPOS(bseqiflags)];
    } else {
        int sregn = BSEQIFLAG_INDEXSIZE(bseqiflags);
        sregi = nasm_random32(sregn);
    }
    sreg = nasm_rd_sreg[sregi];
    src = nasm_reg_names[sreg - EXPR_REG_START];
    sprintf(buffer, "%s\n", src);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new sreg: %s", buffer);
#endif
}

void create_unity(char *buffer, operand_seed *opnd_seed)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create unity\n");
#endif
    int unity, shiftCount;
    
    if (opnd_seed->opdsize == BITS8) {
        shiftCount = 8;
    } else if (opnd_seed->opdsize == BITS16) {
        shiftCount = 16;
    } else if (opnd_seed->opdsize == BITS32) {
        shiftCount = 32;
    }

    unity = nasm_random32(shiftCount + 1);
    sprintf(buffer, "%d\n", unity);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new unity: %s", buffer);
#endif
}

void create_gpr_register(char *buffer, operand_seed *opnd_seed)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create gpr\n");
#endif
    int gpri;
    enum reg_enum gpr;
    const char *src;

    bseqiflags_t bseqiflags = bseqi_flags(opnd_seed->opndflags);
    if (X86PGState.seqMode) {
        gpri = X86PGState.bseqi.indexes[BSEQIFLAG_INDEXPOS(bseqiflags)];
    } else {
        int gprn = BSEQIFLAG_INDEXSIZE(bseqiflags);
        gpri = nasm_random32(gprn);
    }
    switch (opnd_seed->opdsize) {
        case BITS8:
            gpr = nasm_rd_reg8[gpri];
            break;
        case BITS16:
            gpr = nasm_rd_reg16[gpri];
            break;
        case BITS32:
            gpr = nasm_rd_reg32[gpri];
            break;
    }
    src = nasm_reg_names[gpr - EXPR_REG_START];
    sprintf(buffer, "%s\n", src);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new gpr: %s", buffer);
#endif
}

/* Generate int type immediate.
 * If it's larger than the limmit (8/16-bits imm), the high significant bytes
 * will be wipped away while assembling.
 */
void create_immediate(char *buffer, operand_seed* opnd_seed)
{
#ifdef DEBUG_MODE
    fprintf(stderr, "    try> create immediate\n");
#endif
    int immi, imm;
    
    if (X86PGState.seqMode) {
        bseqiflags_t bseqiflags = bseqi_flags(opnd_seed->opndflags);
        immi = X86PGState.bseqi.indexes[BSEQIFLAG_INDEXPOS(bseqiflags)];
        imm = imms[immi];
    } else {
        long long immn;
        switch (opnd_seed->opdsize) {
            case BITS8:
                immn = 0x100;
                break;
            case BITS16:
                immn = 0x10000;
                break;
            case BITS32:
                immn = 0x100000000;
                break;
            default:
                nasm_fatal("wrong immediate size");
                break;
        }
        imm = (int)nasm_random64(immn);
    }
    sprintf(buffer, "0x%x\n", imm);
#ifdef DEBUG_MODE
    fprintf(stderr, "    done> new immediate: %s", buffer);
#endif
}
