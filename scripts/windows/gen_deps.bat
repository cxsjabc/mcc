:: TODO

set BUILD_OUT_FOLDER=out
set OBJ_POSTFIX=.obj
set DEPENDS_POSTFIX=.d

set AWK=awk.exe

if "%1" == "src" (
    echo > %BUILD_OUT_FOLDER%\%1\.src.deps
    echo xxxx
    for %%i in (%1/*.c) do (
        set OBJ=%1\%%~ni%OBJ_POSTFIX%
        echo "OBJ: "%OBJ%
        set DEPS=%OBJ% %DEPENDS_POSTFIX% 
        echo "DEPS: "%DEPS%
        :: echo %BUILD_OUT_FOLDER%\%OBJ%: >> %DEPS%
        :: cl.exe -I ./inc /c /showIncludes %1\%~ni%.c /nologo /Fo:NUL > temp.deps
        :: type temp.deps | findstr "inc\mcc" | %AWK% "{print $3}" >> %DEPS%
        :: echo.>>%DEPS%
    )
    sed -i '1,$s#$#\\\#' %BUILD_OUT_FOLDER%\%1\.src.deps
)