#ifndef NASM_TMPLT_H
#define NASM_TMPLT_H

#include "wdtree.h"

/********************************************
 * template 
 *      default: seq_statement
 *
 * instruction block statement types
 * 1) seq_statement: select instruction statements sequentially
 * 2) sel_statement: select instruction statements randomly
 * 3) xfr_statement: set instruction statements as destination of loopxx/call control transfer instruction
 * 5) rpt_statement: repeat generating instruction statements
 * 5) elem_statement: repeat generating instruction statements
 *
 * element statement types
 * 1) Iset: instructin set/group
 * 2) Print: print test information
 * 3) Insn: a single instruction
 */

/* element structure
 */
typedef enum elem_type {
    ISET_ELEM,
    PRINT_ELEM,
    INSN_ELEM
} elem_type;

typedef enum print_type {
    X86_STATE, /* x86 register information */
    X87_STATE, /* x87 state information */
    ALL_STATE  /* all information */
} print_type;

typedef struct elem_struct {
    double inip;
    elem_type type;
    union {
        WDTree *wdtree;
        print_type pType;
        char *inst;
    };
} elem_struct;

/* block statement structure
 */
typedef enum blk_type {
    SEQ_BLK,
    SEL_BLK,
    XFR_BLK,
    RPT_BLK,
    TRV_BLK,
    ELEM_BLK
} blk_type;

typedef struct blk_var {
    bool        valid;
    char        *name;
    opflags_t   opndflags;
    char        *asm_var;
} blk_var;

typedef struct blk_struct {
    struct blk_struct *parent;
    blk_type type;
    char *xfrName;
    int times;          /* RPT_BLK, XFR_BLK */
    GArray *vars;
    GArray *blks;
} blk_struct;

/* template
 */
typedef struct tmplt_struct {
    void *blk;
} tmplt_struct;

void init_blk_struct(blk_struct *blk);
void init_blk_var(blk_var *var);
blk_var *blk_search_var(blk_struct *blk, const char *var_name);
void walk_tmplt(void);
void tmplt_clear(tmplt_struct *tmpltm);
void tmplt_free(tmplt_struct *tmpltm);

extern tmplt_struct tmpltm;

#endif