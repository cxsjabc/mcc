@echo off

rem This file is used to generate a .c file that contains whether the macro
rem is defined or not.
rem Command example: get_define _MSC_VER int clang
rem  Result: _MSC_VER is 1929
rem The generated source file is:
rem #include <stdio.h>
rem 
rem int main() {
rem #if defined(_MSC_VER)
rem     printf("_MSC_VER is %d\n", _MSC_VER );
rem #else
rem    printf("_MSC_VER is undefined.\n");
rem #endif
rem 
rem     return 0;
rem }
  
setlocal enabledelayedexpansion  
  
set "FILE_NAME=__get_define_0a_b1__.c"  
set "DEFINE_NAME="  
set "DEFINE_TYPE="
set COMPILE=gcc

:mainRoutine  
call :checkArgs %* || exit /b 1  
if "%DEFINE_TYPE%"=="str" (call :genDefinedStr) else if "%DEFINE_TYPE%"=="empty" (call :genDefinedName) else (call :genDefinedInt) 
call :compileAndRun
exit /b 0

:usage  
echo "Usage: %~nx0 <define_name> <define_type>"
exit /b 1

:checkArgs  
if "%~1"=="" (  
    echo Missing arguments.  
    goto usage  
) else if "%~2"=="" (  
    set "DEFINE_NAME=%~1"  
) else if "%~3"=="" (  
    set "DEFINE_NAME=%~1"  
    set "DEFINE_TYPE=%~2"  
) else if "%~4"=="" (  
    set "DEFINE_NAME=%~1"  
    set "DEFINE_TYPE=%~2"
    set "COMPILE=%~3"    
) else (  
    goto usage  
)  

:genDefinedName  
(  
echo #include ^<stdio.h^>
echo.
echo int main(^) ^{
echo #if defined(%DEFINE_NAME%^)
echo     printf("%DEFINE_NAME% is defined\n"^);
echo #else
echo     printf("%DEFINE_NAME% is undefined.\n"^);
echo #endif
echo.
echo     return 0;
echo ^}
) > %FILE_NAME%  
exit /b  
  
:genDefinedInt  
(  
echo #include ^<stdio.h^>
echo.
echo int main(^) ^{
echo #if defined(%DEFINE_NAME%^)
echo     printf("%DEFINE_NAME% is %%d\n", %DEFINE_NAME% ^);
echo #else
echo     printf("%DEFINE_NAME% is undefined.\n"^);
echo #endif
echo.
echo     return 0;
echo ^}
) > %FILE_NAME%  
exit /b  
  
:genDefinedStr  
(  
echo #include ^<stdio.h^>
echo.
echo int main(^) ^{
echo #if defined(%DEFINE_NAME%^)
echo     printf("%DEFINE_NAME% is %%s\n", %DEFINE_NAME%^);
echo #else
echo     printf("%DEFINE_NAME% is undefined.\n"^);
echo #endif
echo.
echo     return 0;
echo ^}
) > %FILE_NAME%
exit /b
  
:compileAndRun
%COMPILE% -o %FILE_NAME%.exe -Werror %FILE_NAME%
echo "Compile result: " %errorlevel%

if %errorlevel% neq 0 (  
    echo Compile failed.  
) else (  
    type %FILE_NAME%  
    %FILE_NAME%.exe 
    echo "Run result: " %errorlevel%
    if %errorlevel% neq 0 (  
        echo Execute failed. 
    )
)  

del %FILE_NAME%.exe > nul 2>&1
del %FILE_NAME% > nul 2>&1  

exit /b 0  

:end