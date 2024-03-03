@echo off

:: set MSVC_ENV_BAT="D:\ProgramFiles\MicrosoftVisualStudio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
call _local_config.bat

set MSVC_ARCH=x64

call %MSVC_ENV_BAT% %MSVC_ARCH%