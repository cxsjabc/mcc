#!/bin/bash

make -f Makefile clean_test clean
rm -rf out
rm mcc mcc_test m t
rm mcc.exe mcc_test.exe m.exe t.exe

rm mcc.ilk mcc.pdb mcc_test.ilk mcc_test.pdb mcc_test.exe.stackdump
rm m.ilk m.pdb t.ilk t.pdb m.exe.stackdump t.exe.stackdump
