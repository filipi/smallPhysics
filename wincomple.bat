@echo off
set PATH=%PATH%;C:\Program Files\Dev-Cpp\bin
md MinGW
c:\Program Files\Dev-Cpp\bin\mingw32-make -f  Makefile.win clean
c:\Program Files\Dev-Cpp\bin\mingw32-make -f  Makefile.win

move MinGW\eitsimGL.exe .

rd MinGW
