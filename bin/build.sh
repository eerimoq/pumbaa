#!/usr/bin/env bash
#
# This script builds the mpy-cross program for Linux and cross
# compiles for Windows using MinGW.
#
# Must be run on a 64 bit Linux.
#
# Get the Windows tools. Unpack them in workspace/mingw32
#
# $ wget https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds/mingw-w32-bin_x86_64-linux_20131227.tar.bz2/download
# $ mkdir mingw32
# $ cd mingw32
# $ tar xf ../download
#

export PATH=$PATH:/home/erik/workspace/mingw32/bin

cd ../micropython/mpy-cross

make clean
make PROG=mpy-cross-x86_64-linux LDFLAGS_EXTRA=-static
make clean
make PROG=mpy-cross-i686-linux CFLAGS_EXTRA=-m32 LDFLAGS_EXTRA="-m32 -static"
make clean
make PROG=mpy-cross-i686.exe CC=i686-w64-mingw32-gcc OS=Windows_NT

mv mpy-cross-x86_64-linux mpy-cross-i686-linux mpy-cross-i686.exe ${PUMBAA_ROOT}/bin
