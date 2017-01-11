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

from drivers import Owi
import board
import harness
from harness import assert_raises
import owi_stub

def test_print():
    print(Owi)
    help(Owi)
    owi = Owi(board.PIN_LED)
    print(owi)


def test_reset():
    owi = Owi(board.PIN_LED)
    assert owi.reset() is None


def test_search():
    owi = Owi(board.PIN_LED)
    assert owi.search() == 4


def test_get_devices():
    devices = [
        b'(2345678',
        b'(2345679',
        b'12345678',
        b'12345678'
    ]
    owi = Owi(board.PIN_LED)
    assert owi.get_devices() == devices


def test_read():
    owi_stub.set_read(b'12')

    owi = Owi(board.PIN_LED)
    assert owi.read(2) == b'12'


def test_write():
    owi_stub.set_write([b'23', b'2'])

    owi = Owi(board.PIN_LED)
    assert owi.write(b'23') == 2
    assert owi.write(b'23', 1) == 1


def test_bad_arguments():
    # Bad pin device.
    with assert_raises(ValueError, "bad pin device"):
        Owi(-1)


TESTCASES = [
    (test_print, "test_print"),
    (test_reset, "test_reset"),
    (test_search, "test_search"),
    (test_get_devices, "test_get_devices"),
    (test_read, "test_read"),
    (test_write, "test_write"),
    (test_bad_arguments, "test_bad_arguments")
]
