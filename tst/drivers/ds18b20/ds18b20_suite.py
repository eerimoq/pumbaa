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
from drivers import Owi, Ds18b20
import board
import harness
from harness import assert_raises


def test_print():
    print(Ds18b20)
    help(Ds18b20)
    ds18b20 = Ds18b20(Owi(board.PIN_LED))
    print(ds18b20)


def test_convert():
    ds18b20 = Ds18b20(Owi(board.PIN_LED))
    assert ds18b20.convert() == None


def test_get_devices():
    ds18b20 = Ds18b20(Owi(board.PIN_LED))
    assert ds18b20.get_devices() == [b'\x282345678', b'\x282345679']


def test_get_temperature():
    ds18b20 = Ds18b20(Owi(board.PIN_LED))
    assert ds18b20.get_temperature(b'\x282345678') == 22.0


def test_bad_arguments():
    # Owi object expected.
    with assert_raises(TypeError, "Owi object expected"):
        Ds18b20(None)


TESTCASES = [
    (test_print, "test_print"),
    (test_convert, "test_convert"),
    (test_get_devices, "test_get_devices"),
    (test_get_temperature, "test_get_temperature"),
    (test_bad_arguments, "test_bad_arguments")
]
