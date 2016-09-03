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

import other
import board
import pin

print('Hello world!')

assert other.foo() == True

try:
    1 / 0
    assert False
except ZeroDivisionError as e:
    print(e)

led = pin.Pin(board.PIN_LED, pin.OUTPUT)
led.write(1)
led.toggle()
print("LED value:", led.read())

help(led)

# open() is not yet implemented.
try:
    with open("smoke.txt", "w") as fout:
        fout.write("test")

    with open("smoke.txt", "r") as fin:
        assert fin.read("test") == "test"

    assert False
except NameError as e:
    print(e)
