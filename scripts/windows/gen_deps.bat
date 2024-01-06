:: TODO
setlocal enabledelayedexpansion

set BUILD_OUT_FOLDER=out
set OBJ_POSTFIX=.obj
set DEPENDS_POSTFIX=.win

set AWK=awk.exe

:loop
if "%1" neq "" (
    if exist %BUILD_OUT_FOLDER%\%1\%1.deps.win (
        goto :continue
    )

    set DEPS=%BUILD_OUT_FOLDER%\%1\%1.deps.win
    rem echo "DEPS: "!DEPS!
    
    echo.> !DEPS!
    for %%i in (%1/*.c) do (
        set OBJ=%1\%%~ni%OBJ_POSTFIX%
        rem echo "OBJ: "!OBJ!

        echo %BUILD_OUT_FOLDER%\!OBJ!: \>> !DEPS!
        cl.exe -I ./inc /c /showIncludes %1\%%~ni.c /nologo /Fo:NUL | findstr -v "No such file" | findstr "inc\mcc" | %AWK% "{print $3}" | sed '1,$s#$# \\\#' >> !DEPS!
        echo.>> !DEPS!
        echo.>> !DEPS!
    )
) else (
    exit /b 0
)
:continue
shift
goto loop