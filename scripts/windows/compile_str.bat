@echo off

rem This file is used to compile a string passed from parameter.
rem Command example: compile_str "sizeof(int)" int
rem  Result: sizeof(int) is 4
rem The generated source file is:
rem #include <stdio.h>
rem 
rem int main() {
rem     printf("sizeof(int) is %lld\n", (unsigned long long)sizeof(int));
rem 
rem     return 0;
rem }
  
setlocal enabledelayedexpansion  
  
set "FILE_NAME=__compile_str_0a_b1__.c"  
set "CODE_STR="  
set "CODE_TYPE="
set COMPILE=gcc
set COMP_FLAG=-Werror

:mainRoutine  
call :checkArgs %* || exit /b 1  
echo yyy
if "%CODE_TYPE%"=="str" (call :genDefinedStr) else (call :genDefinedInt) 
echo xxxx
call :compileAndRun
exit /b 0

:usage  
echo "Usage: %~nx0 <code_str> <code_type>"
exit /b 1

:checkArgs  
if "%~1"=="" (  
    echo Missing arguments.  
    goto usage  
) else if "%~2"=="" (  
    set "CODE_STR=%~1"
    set "CODE_TYPE=int"
) else if "%~3"=="" (  
    set "CODE_STR=%~1"  
    set "CODE_TYPE=%~2"  
) else if "%~4"=="" (  
    set "CODE_STR=%~1"  
    set "CODE_TYPE=%~2"
    set "COMPILE=%~3"    
) else (  
    goto usage  
)  
  
:genDefinedInt  
(  
echo #include ^<stdio.h^>
echo.
echo int main(^) ^{
echo     printf("%CODE_STR% is %%lld\n", (unsigned long long^)(%CODE_STR%^) ^); // TODO: sizeof(int) compile error
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
echo     printf("%CODE_STR% is %%s\n", (%CODE_STR%^) ^);
echo.
echo     return 0;
echo ^}
) > %FILE_NAME%  
exit /b 
  
:compileAndRun
echo "COMP_FLAG: %COMP_FLAG%"
if "%COMPILE%" == "cl" ( 
    echo "Using cl"
    set COMP_FLAG=/WX 
)

echo %COMPILE% -o %FILE_NAME%.exe %COMP_FLAG% %FILE_NAME%
%COMPILE% -o %FILE_NAME%.exe %COMP_FLAG% %FILE_NAME%
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