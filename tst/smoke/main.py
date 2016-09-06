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
import board
import event
import fs
import pin
import timer
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
    help(board)
    print()
    help(event)
    print()
    help(event.Event)
    print()
    help(fs)
    print()
    help(pin)
    print()
    help(pin.Pin)
    print()
    help(timer)
    print()
    help(timer.Timer)
    print()
    help(other)
    print()

    print(os.uname())

    try:
        print('CWD:', os.getcwd())
    except OSError as e:
        print(e)

    os.mkdir('foo')

    print(os.listdir())

    try:
        os.chdir('foo')
    except OSError as e:
        print(e)

    try:
        os.chdir('..')
    except OSError as e:
        print(e)

    try:
        print(os.stat('foo'))
    except OSError as e:
        print(e)

    try:
        os.rename('foo', 'bar')
    except OSError as e:
        print(e)

    try:
        os.remove('bar')
    except OSError as e:
        print(e)

    try:
        os.rmdir('bar')
    except OSError as e:
        print(e)

    assert other.foo() == True

    led = pin.Pin(board.PIN_LED, pin.OUTPUT)
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

    led.set_mode(pin.INPUT)

    try:
        led.set_mode(3)
    except ValueError as e:
        print(e)

    try:
        pin.Pin(-1, pin.OUTPUT)
    except ValueError as e:
        print(e)

    try:
        pin.Pin(300, pin.OUTPUT)
    except ValueError as e:
        print(e)

    try:
        pin.Pin(board.PIN_LED, 10)
    except ValueError as e:
        print(e)

    fs.call("kernel/thrd/list")

    with open("smoke.txt", "w") as fout:
        fout.write("test")

    with open("smoke.txt", "r") as fin:
        print(fin.read())

        
def main():
    testcases = [
        (test_smoke, "test_smoke")
    ]
    harness.run(testcases)

    
if __name__ == '__main__':
    main()
