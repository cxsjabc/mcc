@echo off
@echo "nmake.bat %1..."

:: vs2019 environment
:: ^*^* You should configure the path by your computer^'s path ^*^*
set MSVC_ENV_BAT="D:\ProgramFiles\MicrosoftVisualStudio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
:: x64 or x86, default x64
set MSVC_ARCH=x64

if defined __VCVARSALL_HOST_ARCH (
    echo VCVARS*.bat is executed, ignore
) else (
    echo VCVARS*.bat is not executed
    :: TODO: strange that, it may be executed twice or more.
    call %MSVC_ENV_BAT% %MSVC_ARCH%
)

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