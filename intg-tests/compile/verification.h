#ifndef NASM_VERIFICATION_H
#define NASM_VERIFICATION_H

typedef uint32_t float32;
typedef uint64_t float64;

#define MMREG_UNION(n, bits)        \
    union n {                       \
        uint8_t  _b_##n[(bits)/8];  \
        uint16_t _w_##n[(bits)/16]; \
        uint32_t _l_##n[(bits)/32]; \
        uint64_t _q_##n[(bits)/64]; \
        float32  _s_##n[(bits)/32]; \
        float64  _d_##n[(bits)/64]; \
    }

typedef MMREG_UNION(MMXReg, 64)  MMXReg;

typedef struct {
    uint64_t low;
    uint16_t high;
} floatx80;


typedef union {
    floatx80 d __attribute__((aligned(16)));
    MMXReg mmx;
} FPReg;

struct X87LegacyXSaveArea {
    uint16_t fcw;
    uint16_t fsw;
    uint8_t ftw;
    uint8_t reserved0_5;
    uint16_t fpop;
    uint32_t fpip;
    uint16_t fpcs;
    uint16_t reserved0_14;
    uint32_t fpdp;
    uint16_t fpds;
    uint16_t reserved16_6;
    uint32_t mxcsr;
    uint32_t mxcsr_mask;
    FPReg fpregs[8];
    uint8_t xmm_regs[8][16];
    uint8_t reverved[11][16];
    uint8_t available[3][16];
};

typedef uint8_t Reg8;
typedef uint16_t Reg16;
typedef uint32_t Reg32;

struct X86StandardRegisters {
    uint16_t gs;
    uint16_t gs_padding;
    uint16_t fs;
    uint16_t fs_padding;
    uint16_t es;
    uint16_t es_padding;
    uint16_t ds;
    uint16_t ds_padding;
    uint16_t ss;
    uint16_t ss_padding;
    uint16_t cs;
    uint16_t cs_padding;
    uint32_t eflags;
    union {
        uint32_t edi;
        struct {
            uint16_t di;
            uint16_t di_padding;
        };
    };
    union {
        uint32_t esi;
        struct {
            uint16_t si;
            uint16_t si_padding;
        };
    };
    union {
        uint32_t ebp;
        struct {
            uint16_t bp;
            uint16_t bp_padding;
        };
    };
    union {
        uint32_t esp;
        struct {
            uint16_t sp;
            uint16_t sp_padding;
        };
    };
    union {
        uint32_t ebx;
        struct {
            union {
                uint16_t bx;
                struct {
                    uint8_t bl;
                    uint8_t bh;
                };
            };
            uint16_t bx_padding;
        };
    };
    union {
        uint32_t edx;
        struct {
            union {
                uint16_t dx;
                struct {
                    uint8_t dl;
                    uint8_t dh;
                };
            };
            uint16_t dx_padding;
        };
    };
    union {
        uint32_t ecx;
        struct {
            union {
                uint16_t cx;
                struct {
                    uint8_t cl;
                    uint8_t ch;
                };
            };
            uint16_t cx_padding;
        };
    };
    union {
        uint32_t eax;
        struct {
            union {
                uint16_t ax;
                struct {
                    uint8_t al;
                    uint8_t ah;
                };
            };
            uint16_t ax_padding;
        };
    };
};

#endif
