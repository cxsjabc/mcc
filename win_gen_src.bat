 set TMP_SRC_FILES=tmp_src_files.txt
 set OBJ_POSTFIX=.obj
 
 del %TMP_SRC_FILES%
 
 echo OBJS= \>> %TMP_SRC_FILES%
 
 for %%i in (src/*.c) do @echo src/%%~ni%OBJ_POSTFIX% \>> %TMP_SRC_FILES%
 
 echo.>> %TMP_SRC_FILES%