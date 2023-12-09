@echo off
@echo "nmake.bat %1..."

if "%1" == "" (
    :: generate source files
    call scripts\win_gen_src.bat

    :: call msvc nmake.exe
    nmake.exe -f Makefile.win.msvc mcc
) else if "%1" == "mcc" (
    :: generate source files
    call scripts\win_gen_src.bat

    :: call msvc nmake.exe
    nmake.exe -f Makefile.win.msvc mcc
) else if "%1" == "clean" (
    nmake.exe -f Makefile.win.msvc clean
)  else if "%1" == "test" (
    call scripts\win_gen_src.bat

    nmake.exe -f Makefile.win.msvc test
) else if "%1" == "clean_test" (
    nmake.exe -f Makefile.win.msvc clean_test
)