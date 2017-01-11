#
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2016, Erik Moqvist
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Pumbaa project.
#


import os
from drivers import Pin
import board
import harness
from harness import assert_raises


def test_output():
    led = Pin(board.PIN_LED, Pin.OUTPUT)
    print(led)

    led.write(1)
    value = led.read()
    if os.uname().machine != "Linux with Linux":
        assert value == 1

    led.toggle()
    value = led.read()
    if os.uname().machine != "Linux with Linux":
        assert value == 0

        
def test_input():
    led = Pin(board.PIN_LED, Pin.INPUT)
    print(led)

def test_set_mode():
    led = Pin(board.PIN_LED, Pin.INPUT)
    led.set_mode(Pin.OUTPUT)

    with assert_raises(ValueError, "bad pin mode 6"):
        led.set_mode(6)


def test_bad_arguments():
    # Bad mode.
    with assert_raises(ValueError, "bad pin mode 3"):
        Pin(board.PIN_LED, 3)

    # bad device.
    with assert_raises(ValueError, "bad pin device -1"):
        Pin(-1, Pin.OUTPUT)

    with assert_raises(ValueError, "bad pin device 500"):
        Pin(500, Pin.OUTPUT)

    led = Pin(board.PIN_LED, Pin.OUTPUT)

    with assert_raises(ValueError, "bad pin value 2"):
        led.write(2)


TESTCASES = [
    (test_output, "test_output"),
    (test_input, "test_input"),
    (test_set_mode, "test_set_mode"),
    (test_bad_arguments, "test_bad_arguments")
]
