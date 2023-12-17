@echo off
@echo "nmake.bat %1..."

:: vs2019 environment
:: ^*^* You should configure the path by your computer^'s path ^*^*
set MSVC_ENV_BAT="D:\ProgramFiles\MicrosoftVisualStudio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
:: x64 or x86, default x64
set MSVC_ARCH=x64

set OUT_DIR=out
set PREVIOUS_BUILD=win_arch.txt

mkdir %OUT_DIR%
:: TODO: strange that, it may be executed three times, then VSCMD_VCVARSALL_INIT is ok.
rem echo "VSCMD_VCVARSALL_INIT: %VSCMD_VCVARSALL_INIT%"

cl > nul 2>&1

if %errorlevel% equ 0 (
    echo VCVARS*.bat is executed, ignore
) else (
:need-init
    echo VCVARS*.bat is not executed
    call %MSVC_ENV_BAT% %MSVC_ARCH%
    echo %MSVC_ARCH% > %OUT_DIR%\%PREVIOUS_BUILD%
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
    del %OUT_DIR%\%PREVIOUS_BUILD%
)  else if "%1" == "test" (
    call %WIN_GEN_SRC%

    nmake.exe -f makefiles\Makefile.msvc.mk test
) else if "%1" == "clean_test" (
    nmake.exe -f makefiles\Makefile.msvc.mk clean_test
    del %OUT_DIR%\%PREVIOUS_BUILD%
)