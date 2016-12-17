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


import struct
from drivers import Adc
import board
import harness
from harness import assert_raises

try:
    PIN_A0 = board.PIN_A0
    PIN_A1 = board.PIN_A1
except AttributeError:
    PIN_A0 = board.PIN_GPIO32
    PIN_A1 = board.PIN_GPIO33

def test_print():
    print(Adc)
    help(Adc)
    a0 = Adc(board.ADC_0, PIN_A0, Adc.REFERENCE_VCC, 100)
    print(a0)


def test_convert():
    a0 = Adc(board.ADC_0, PIN_A0, Adc.REFERENCE_VCC, 100)
    a1 = Adc(board.ADC_0, PIN_A1)
    a0_sample = a0.convert(1)
    a1_sample = a1.convert(1)
    assert len(a0_sample) == 2
    assert len(a1_sample) == 2
    print("A0 sample:", a0_sample, struct.unpack('H', a0_sample)[0])
    print("A1 sample:", a1_sample, struct.unpack('H', a1_sample)[0])


def test_async_convert():
    a0 = Adc(board.ADC_0, PIN_A0)
    a0.async_convert(2)
    a0_sample = a0.async_wait()
    assert len(a0_sample) == 4
    print("A0 sample:", a0_sample, struct.unpack('HH', a0_sample))


def test_bad_arguments():
    # Bad device.
    with assert_raises(ValueError, "bad device"):
        Adc(67, 0)

    # Bad pin device.
    with assert_raises(ValueError, "bad pin device"):
        Adc(board.ADC_0, -5)

    # Bad reference.
    with assert_raises(ValueError, "bad reference"):
        Adc(board.ADC_0, PIN_A0, -1)

    # Bad sampling rate.
    with assert_raises(ValueError, "bad sampling rate"):
        Adc(board.ADC_0, PIN_A0, Adc.REFERENCE_VCC, 0)


def main():
    testcases = [
        (test_print, "test_print"),
        (test_convert, "test_convert"),
        (test_async_convert, "test_async_convert"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
