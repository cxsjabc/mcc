@echo off  
rem setlocal enabledelayedexpansion 

set DEFINE_NAME=_WIN32
set FILE_NAME=__0uc_temp.c

call :genDefinedName

gcc -o %FILE_NAME%.exe -Werror %FILE_NAME%
if errorlevel 1 (  
    echo Compile failed.  
) else (  
    type %FILE_NAME%  
    %FILE_NAME%.exe 
)  

:genDefinedName  
(  
echo #include ^<stdio.h^>
echo.
echo int main(^)
echo ^{
echo #if defined(%DEFINE_NAME%^)
echo     printf("%DEFINE_NAME% is defined\n"^);
echo #endif
echo.
echo     return 0;
echo ^}
) > %FILE_NAME%
exit /b 0