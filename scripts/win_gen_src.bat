set TMP_SRC_FILES=scripts\tmp_src_files.txt
set OBJ_POSTFIX=.obj
set BUILD_OUT_FOLDER=out

del %TMP_SRC_FILES%
 
echo OBJS= \>> %TMP_SRC_FILES%
for %%i in (src/*.c) do @echo src\%%~ni%OBJ_POSTFIX% \>> %TMP_SRC_FILES%
echo.>> %TMP_SRC_FILES%

echo TEST_OBJS= \>> %TMP_SRC_FILES%
for %%i in (test/*.c) do @echo test\%%~ni%OBJ_POSTFIX% \>> %TMP_SRC_FILES%
echo.>> %TMP_SRC_FILES%

echo LINK_OBJS= \>> %TMP_SRC_FILES%
for %%i in (src/*.c) do @echo %BUILD_OUT_FOLDER%\src\%%~ni%OBJ_POSTFIX% \>> %TMP_SRC_FILES%
echo.>> %TMP_SRC_FILES%

echo LINK_TEST_OBJS= \>> %TMP_SRC_FILES%
for %%i in (test/*.c) do @echo %BUILD_OUT_FOLDER%\test\%%~ni%OBJ_POSTFIX% \>> %TMP_SRC_FILES%
echo.>> %TMP_SRC_FILES%