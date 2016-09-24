#
# @file main.py
#
# @section License
# Copyright (C) 2016, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Pumbaa project.
#

import array
import math
import os
import sys
import time
import micropython
import uio as io
import ubinascii as binascii
import ucollections as collections
import ure as re
import ustruct as struct
import harness
import simba
import other
from harness import assert_raises


def test_smoke():
    """Various tests.

    """

    help()

    objs = [
        array,
        io,
        math,
        os,
        sys,
        time,
        binascii,
        collections,
        re,
        struct,
        simba,
        simba.Board,
        simba.Event,
        simba.Pin,
        simba.Timer,
        other
    ]

    for obj in objs:
        print()
        help(obj)

    try:
        import foo
    except:
        pass
        
    print("dir:", dir())

    print("sys.platform:", sys.platform)
    print("os.uname:", os.uname())
    print("time.time:", time.time())

    time.sleep(0.1)
    time.sleep_ms(1)
    time.sleep_us(1)

    try:
        print('CWD:', os.getcwd())
    except OSError as e:
        print(e)

    try:
        os.mkdir('foo')
    except Exception as e:
        print(e)

    with assert_raises(NotImplementedError):
        os.chdir('foo')

    with assert_raises(NotImplementedError):
        os.chdir('..')

    with assert_raises(NotImplementedError):
        os.rename('foo', 'bar')

    with assert_raises(NotImplementedError):
        os.remove('bar')

    with assert_raises(NotImplementedError):
        os.rmdir('bar')

    assert other.foo() == True

    simba.fs_call("kernel/thrd/list")

    sio = io.StringIO("foo")
    sio.seek(0, 2)
    print("bar", file=sio)
    sio.seek(0)
    assert sio.read().strip() == "foobar"

def main():
    testcases = [
        (test_smoke, "test_smoke")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
