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

import os
from pumbaa import Board, Pin
import harness


def test_output():
    led = Pin(Board.PIN_LED, Pin.OUTPUT)

    led.write(1)
    value = led.read()
    if os.uname().machine != "Linux with Linux":
        assert value == 1

    led.toggle()
    value = led.read()
    if os.uname().machine != "Linux with Linux":
        assert value == 0

        
def test_input():
    led = Pin(Board.PIN_LED, Pin.INPUT)


def test_bad_arguments():
    try:
        Pin(Board.PIN_LED, 3)
    except ValueError as e:
        print(e)

    led = Pin(Board.PIN_LED, Pin.OUTPUT)

    try:
        led.write(2)
    except ValueError as e:
        print(e)


def main():
    testcases = [
        (test_output, "test_output"),
        (test_input, "test_input"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
