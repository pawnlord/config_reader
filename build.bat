@echo off
IF not exist bin\ ( mkdir bin )
IF not exist lib\ ( mkdir lib )
gcc -c source\config_reader.c -o bin\cfgreader.o -std=c99
ar rcs lib\libcfgreader.a bin\cfgreader.o