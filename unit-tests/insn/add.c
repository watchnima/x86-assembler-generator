#include "compiler.h"

#include "seed.h"
#include "../ut.h"
#include "generator.h"
#include "ofmt.h"
#include "dfmt.h"
#include "insn-test.h"

static insn_seed ADD_seed =
{
    .opcode = I_ADD
};

static const char fout_head[] = "section .text\n  global _start\n_start:\n";
static const char fout_tail[] = "\n  mov eax,1\n  mov ebx,0\n  int 80h";

bool gen_test_file_ADD(void)
{
    struct output_data data;
    ofmt->init("test_ADD.s");
    dfmt->init("debug_ADD.txt");
    
    generator_init(false);

    data.type = OUTPUT_RAWDATA;
    data.buf = (const char *)fout_head;
    ofmt->output(&data);

    assign_arr5(ADD_seed.opd, REG_GPR|BITS32,IMMEDIATE,0,0,0);

    gsp(&ADD_seed, ofmt);

    data.type = OUTPUT_RAWDATA;
    data.buf = (const char *)fout_tail;
    ofmt->output(&data);
    
    ofmt->cleanup();
    dfmt->cleanup();

    generator_exit();
    return true;
}
