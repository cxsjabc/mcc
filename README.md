# mcc

#### 介绍
# *nix:
## Ubuntu 22.04 or Ubuntu 22.04 WSL
  1 Gcc (>= 11.2.0)
make: build a mcc compiler.
make clean: clean the mcc compiler.

make test: test the mcc compiler.
make clean_test: clean the test files.

# Windows:
  2.1 MSVC cl.exe (Verified pass on VS2019)
    Precondition: Command Line environment in MSVC.
      Method1: Use nmake.bat.
        nmake or nmake mcc
        nmake clean
            Note: nmake mcc == nmake.bat mcc, nmake.bat will call msvc nmake.exe.
      Method2: (Use make.exe and cl.exe)
        a. Precondition: Change Makefile.win.mk: USE_MSVC = 1
        b.  make -f Makefile.win.mk
            make -f Makefile.win.mk test

            make -f Makefile.win.mk clean
            make -f Makefile.win.mk clean_test

      Note: Not support Visual Stuio C++ 6.0(not support __VA_ARGS__).
  2.2 MinGW gcc
    make -f Makefile.win.mk
    make -f Makefile.win.mk test

    make -f Makefile.win.mk clean
    make -f Makefile.win.mk clean_test

# macOS
  Clang or Gcc.
   - Usually, macOS gcc is a symbol link to clang.
  3.1 Clang (>= Apple clang version 13.0.0 (clang-1300.0.27.3))
    Build command: make

#### 软件架构


#### 安装教程


#### 使用说明


#### 参与贡献


#### 特技
