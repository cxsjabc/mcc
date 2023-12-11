@echo off

set MCC_TEST_EXE=mcc_test.exe

if exist %MCC_TEST_EXE% (
    %MCC_TEST_EXE%
) else (
    make test && %MCC_TEST_EXE%
)