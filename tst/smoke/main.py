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
import harness
import pumbaa
from pumbaa import Board, Event, Pin, Timer
import other


def test_smoke():
    print('Hello world!')

    print()
    help()
    print()
    help(sys)
    print()
    help(time)
    print()
    help(os)
    print()
    help(pumbaa)
    print()
    help(pumbaa.Board)
    print()
    help(pumbaa.Event)
    print()
    help(pumbaa.Pin)
    print()
    help(pumbaa.Timer)
    print()
    help(other)
    print()

    print(os.uname())

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

    led = Pin(Board.PIN_LED, Pin.OUTPUT)
    time.sleep(0.1)
    led.write(1)
    led.write(False)
    led.write(True)
    time.sleep_ms(1)
    led.toggle()
    time.sleep_us(1)
    print("LED value:", led.read())

    try:
        led.write(2)
    except ValueError as e:
        print(e)

    try:
        led.write(None)
    except Exception as e:
        print(e)

    led.set_mode(Pin.INPUT)

    try:
        led.set_mode(3)
    except ValueError as e:
        print(e)

    try:
        Pin(-1, Pin.OUTPUT)
    except ValueError as e:
        print(e)

    try:
        Pin(300, Pin.OUTPUT)
    except ValueError as e:
        print(e)

    try:
        Pin(Board.PIN_LED, 10)
    except ValueError as e:
        print(e)

    pumbaa.fs_call("kernel/thrd/list")

    with open("smoke.txt", "w") as fout:
        fout.write("test")

    with open("smoke.txt", "r") as fin:
        assert fin.read() == "test"

        
def main():
    testcases = [
        (test_smoke, "test_smoke")
    ]
    harness.run(testcases)

    
if __name__ == '__main__':
    main()
