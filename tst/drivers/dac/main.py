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
from drivers import Dac
import board
import harness
from harness import assert_raises


def test_print():
    print(Dac)
    dac = Dac(board.PIN_DAC0)
    print(dac)


def test_output():
    # Single pin.
    dac = Dac(board.PIN_DAC0)
    dac.convert("\x03\xff")

    # List of pins.
    dac = Dac([board.PIN_DAC0, board.PIN_DAC1], 11025)
    dac.convert(bytearray(16 * "\x03\xff"))
    dac.convert(16 * "\x03\xff")
    dac.async_convert(16 * "\x03\xff")
    dac.async_wait()


def test_bad_arguments():
    # Too many devices.
    with assert_raises(ValueError, "too many devices"):
        Dac([0, 1, 2])
        
    # Bad devices type.
    with assert_raises(TypeError, "bad devices"):
        Dac(None)

    # Bad pin.
    with assert_raises(ValueError, "bad pin"):
        Dac(board.PIN_LED)


def main():
    testcases = [
        (test_print, "test_print"),
        (test_output, "test_output"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
