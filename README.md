# mcc

#### 介绍
1 *nix:
make: build a mcc compiler.
make clean: clean the mcc compiler.

make test: test the mcc compiler.
make clean_test: clean the test files.

2 Windows:
  2.1 MSVC cl.exe
    Precondition: Command Line environment in MSVC.
      Method1: Use nmake.bat.
        nmake or nmake mcc
        nmake clean
            Note: nmake mcc == nmake.bat mcc, nmake.bat will call msvc nmake.exe.
      Method2: (Use make.exe and cl.exe)
        a. Precondition: Change Makefile.win: USE_MSVC = 1
        b.  make -f Makefile.win
            make -f Makefile.win test

            make -f Makefile.win clean
            make -f Makefile.win clean_test

  2.2 MinGW gcc
    make -f Makefile.win
    make -f Makefile.win test

    make -f Makefile.win clean
    make -f Makefile.win clean_test

3 macOS
  Clang or Gcc.
  Usually, macOS gcc is a symbol link to clang.
  3.1 Clang (>= Apple clang version 13.0.0 (clang-1300.0.27.3))
    make

#### 软件架构


#### 安装教程


#### 使用说明


#### 参与贡献


#### 特技
