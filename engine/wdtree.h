#ifndef NASM_WDTREE_H
#define NASM_WDTREE_H

#include "hashtbl.h"

enum const_type {
    CONST_IMM8,
    CONST_IMM16,
    CONST_IMM32,
    CONST_IMM64,
    CONST_FLOAT,
    CONST_BCD,
    CONST_UNITY,
    CONST_ASM_OP
};

struct const_node {
    enum const_type type;
    union {
        uint8_t imm8;
        uint16_t imm16;
        uint32_t imm32;
        uint64_t imm64;
        uint32_t bcd[3];
        uint8_t unity;
        char *asm_op;
    };
    struct {
        float float32;
        double float64;
        long double float80;
    };
};

/* weight decision tree
 */
struct wd_node {
    bool isleaf;
    int size;
    GArray *weights;
    GArray *sub_nodes;
    GArray *const_nodes;
};

struct wd_root {
    struct wd_node *wd_node;
};

struct wd_root *wdtree_create(void);
struct wd_node *wdtree_node_create(void);
void wdtree_node_clear(struct wd_node *tree);
void wdtrees_free_all(void);
struct const_node *wdtree_select_leaf_node(struct wd_root *tree);

extern struct hash_table hash_wdtrees;

#endif
