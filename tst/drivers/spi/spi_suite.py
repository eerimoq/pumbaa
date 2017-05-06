#
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2016-2017, Erik Moqvist
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
from drivers import Spi
import board
import harness
from harness import assert_raises


def test_print():
    print(Spi)
    spi = Spi(board.SPI_0, board.PIN_D3)
    print(spi)


def test_data_transfer():
    spi = Spi(board.SPI_0, board.PIN_D3)
    spi.start()
    spi.take_bus()
    spi.select()
    read = spi.transfer(b'foo')
    assert len(read) == 3
    read = spi.transfer(b'foo', 1)
    assert len(read) == 1
    buf = bytearray(3)
    assert spi.transfer_into(buf, b'bar') == 3
    print(buf)
    assert spi.transfer_into(buf, b'bar', 2) == 2
    print(buf)
    read = spi.read(3)
    print(read)
    assert len(read) == 3
    buf = bytearray(3)
    assert spi.read_into(buf) == 3
    assert spi.read_into(buf, 1) == 1
    assert spi.write(b'fie') == 3
    assert spi.write(b'fie', 2) == 2
    spi.deselect()
    spi.give_bus()
    spi.stop()


def test_bad_arguments():
    # Bad device.
    with assert_raises(ValueError, "bad device"):
        Spi(100, 0)

    # Bad slave select.
    with assert_raises(ValueError, "bad slave select"):
        Spi(board.SPI_0, -1)

    # Bad mode.
    with assert_raises(ValueError, "bad mode"):
        Spi(board.SPI_0, board.PIN_D3, -1)

    # Bad polarity.
    with assert_raises(ValueError, "bad polarity"):
        Spi(board.SPI_0, board.PIN_D3, Spi.MODE_MASTER, 1, -1)

    # Bad phase.
    with assert_raises(ValueError, "bad phase"):
        Spi(board.SPI_0, board.PIN_D3, Spi.MODE_MASTER, 1, 1, -1)


TESTCASES = [
    (test_print, "test_print"),
    (test_data_transfer, "test_data_transfer"),
    (test_bad_arguments, "test_bad_arguments")
]
