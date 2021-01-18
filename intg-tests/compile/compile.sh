#!/bin/bash
# Before running compile.sh: please move ld.script compile.sh outputlib.c
# checkfunctions.h test_intg.s to the same directory.

set -v
touch stdoutput.rst
gcc -m32 -c -fno-builtin outputlib.c -o outputlib.o
nasm -f elf test_intg.s -o test_intg.o
gcc -static -m32 -o test_intg test_intg.o outputlib.o -Wl,-Tld.script
./test_intg > stdoutput.rst
gcc -m32 -c -fno-builtin checklib.c -o checklib.o
gcc -static -m32 -o test_intg test_intg.o checklib.o -Wl,-Tld.script
rm *.o
