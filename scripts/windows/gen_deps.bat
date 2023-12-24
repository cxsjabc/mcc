:: TODO
setlocal enabledelayedexpansion

set BUILD_OUT_FOLDER=out
set OBJ_POSTFIX=.obj
set DEPENDS_POSTFIX=.d

set AWK=awk.exe

if exist %BUILD_OUT_FOLDER%\%1\src.deps.d (
    exit /b 0
)

if "%1" == "src" (
    set DEPS=%BUILD_OUT_FOLDER%\%1\src.deps.d
    echo "DEPS: "%DEPS%
    
    echo.> !DEPS!
    for %%i in (%1/*.c) do (
        set OBJ=%1\%%~ni%OBJ_POSTFIX%
        echo "OBJ: "!OBJ!

        echo %BUILD_OUT_FOLDER%\!OBJ!: \>> !DEPS!
        
        cl.exe -I ./inc /c /showIncludes %1\%%~ni.c /nologo /Fo:NUL | findstr -v "No such file" | findstr "inc\mcc" | %AWK% "{print $3}" | sed '1,$s#$# \\\#' >> !DEPS!
        echo.>> !DEPS!
        echo.>> !DEPS!
    )
)