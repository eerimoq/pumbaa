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

import sys
import time
import os
import ucollections as collections
import ubinascii as binascii
import ustruct as struct
import ure as re
import harness
import pumbaa
from pumbaa import Board, Event, Pin, Timer
import other


def test_smoke():
    """Various tests.

    """
    
    help()

    objs = [
        sys,
        time,
        os,
        collections,
        binascii,
        struct,
        re,
        pumbaa,
        pumbaa.Board,
        pumbaa.Event,
        pumbaa.Pin,
        pumbaa.Timer,
        other
    ]

    for obj in objs:
        print()
        help(obj)

    print("sys.platform:", sys.platform)
    print("os.uname:", os.uname())
    print("time.time:", time.time())
    
    try:
        print('CWD:', os.getcwd())
    except OSError as e:
        print(e)

    try:
        os.mkdir('foo')
    except Exception as e:
        print(e)

    try:
        os.listdir()
    except Exception as e:
        print(e)

    try:
        os.chdir('foo')
    except NotImplementedError as e:
        print(e)

    try:
        os.chdir('..')
    except NotImplementedError as e:
        print(e)

    try:
        print(os.stat('foo'))
    except OSError as e:
        print(e)

    try:
        os.rename('foo', 'bar')
    except NotImplementedError as e:
        print(e)

    try:
        os.remove('bar')
    except NotImplementedError as e:
        print(e)

    try:
        os.rmdir('bar')
    except NotImplementedError as e:
        print(e)

    assert other.foo() == True

    pumbaa.fs_call("kernel/thrd/list")

        
def main():
    testcases = [
        (test_smoke, "test_smoke")
    ]
    harness.run(testcases)

    
if __name__ == '__main__':
    main()
