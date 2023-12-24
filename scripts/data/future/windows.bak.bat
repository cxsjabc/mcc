

mklink /D mcc H:\sources\mcc

In windows nmake, you can add the below redirect commands to dismiss the command's output information:
>nul 2>nul

nmake makefile:
out\src\env.obj: \
./inc/mcc/env.h \