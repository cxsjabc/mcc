@echo off

set MCC_EXE=mcc.exe

if exist %MCC_EXE% (
    %MCC_EXE%
) else (
    make && %MCC_EXE%
)