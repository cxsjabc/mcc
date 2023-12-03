@echo off
@echo "nmake.bat %1..."

if "%1" == "" (
    :: generate source files
    win_gen_src.bat

    :: call msvc nmake.exe
    nmake.exe -f Makefile.win.msvc mcc
) else if "%1" == "mcc" (
    :: generate source files
    win_gen_src.bat

    :: call msvc nmake.exe
    nmake.exe -f Makefile.win.msvc mcc
) else if "%1" == "clean" (
    nmake.exe -f Makefile.win.msvc clean
)