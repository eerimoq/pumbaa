#!/usr/bin/env bash
#
# This script builds the mpy-cross program for Linux and cross
# compiles for Windows using MinGW.
#
# Must be run on a 64 bit Linux.
#
# Get the Windows tools. Unpack the tar.bz2 archive in the folder
# workspace/mingw32.
#
# $ pwd
# /home/erik/workspace
# $ wget https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds/mingw-w32-bin_x86_64-linux_20131227.tar.bz2/download
# $ mkdir mingw32
# $ cd mingw32
# $ tar xf ../download
#
# Darwin/OSX - Download XCode and run the scripts as described on this
# site:
#
# https://github.com/tpoechtrager/osxcross
#
# $ pwd
# /home/erik/workspace
# $ git clone https://github.com/tpoechtrager/osxcross
# $ cd osxcross
# $ ./tools/gen_sdk_package_darling_dmg.sh <xcode>.dmg
# $ cp *.sdk.tar.xz tarballs
# $ sudo tools/get_dependencies.sh
# $ ./build.sh
#

cd ../micropython/mpy-cross

MPY_CROSS_LINUX_64=mpy-cross-x86_64-linux
MPY_CROSS_LINUX_32=mpy-cross-i686-linux
MPY_CROSS_WINDOWS_32=mpy-cross-i686.exe
MPY_CROSS_DARWIN_64=mpy-cross-x86_64-darwin
MPY_CROSS_DARWIN_32=mpy-cross-i686-darwin

make clean
make PROG=${MPY_CROSS_LINUX_64} LDFLAGS_EXTRA=-static
make clean
make PROG=${MPY_CROSS_LINUX_32} CFLAGS_EXTRA=-m32 LDFLAGS_EXTRA="-m32 -static"
make clean
make PROG=${MPY_CROSS_WINDOWS_32} CC=i686-w64-mingw32-gcc OS=Windows_NT
make clean
make PROG=${MPY_CROSS_DARWIN_64} CC=o64-clang UNAME_S=Darwin STRIP=echo SIZE=echo
make clean
make PROG=${MPY_CROSS_DARWIN_32} CC=o32-clang UNAME_S=Darwin STRIP=echo SIZE=echo CFLAGS_EXTRA=-DMICROPY_NLR_SETJMP

BINS="${MPY_CROSS_LINUX_64} ${MPY_CROSS_LINUX_32} ${MPY_CROSS_WINDOWS_32} ${MPY_CROSS_DARWIN_64} ${MPY_CROSS_DARWIN_32}"

for bin in ${BINS} ; do
    mv ${bin} ${PUMBAA_ROOT}/bin
done
