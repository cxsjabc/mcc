@echo off
@echo "nmake.bat %1..."

set WIN_GEN_SRC=scripts\windows\win_gen_src.bat

if "%1" == "" (
    :: generate source files
    call %WIN_GEN_SRC%

    :: call msvc nmake.exe
    nmake.exe -f makefiles\Makefile.msvc.mk mcc
) else if "%1" == "mcc" (
    :: generate source files
    call %WIN_GEN_SRC%

    :: call msvc nmake.exe
    nmake.exe -f makefiles\Makefile.msvc.mk mcc
) else if "%1" == "clean" (
    nmake.exe -f makefiles\Makefile.msvc.mk clean
)  else if "%1" == "test" (
    call %WIN_GEN_SRC%

    nmake.exe -f makefiles\Makefile.msvc.mk test
) else if "%1" == "clean_test" (
    nmake.exe -f makefiles\Makefile.msvc.mk clean_test
)