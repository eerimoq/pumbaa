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
from drivers import Spi, Sd
import board
import harness
from harness import assert_raises

SD = None


def test_start():
    global SD
    spi = Spi(board.SPI_0, board.PIN_D4)
    SD = Sd(spi)
    SD.start()


def test_print():
    print(Sd)
    print(SD)
    help(Sd)


def test_read_cid():
    print(SD.read_cid())


def test_read_csd():
    # Version 1.
    csd = SD.read_csd()
    print(csd)
    assert csd.csd_structure == 0

    # Version 2.
    csd = SD.read_csd()
    print(csd)
    assert csd.csd_structure == 1


def test_read_write():
    block = 512 * b'1'
    SD.write_block(0, block)
    assert SD.read_block(0) == block
    buf = bytearray(512)
    SD.read_block_into(0, buf)
    assert buf == block


def test_read_write_fail():
    # The driver fails to read and write data.
    with assert_raises(OSError):
        block = 512 * b'1'
        SD.write_block(0, block)

    with assert_raises(OSError):
        SD.read_block(0)

    with assert_raises(OSError):
        buf = bytearray(512)
        SD.read_block_into(0, buf)


def test_bad_arguments():
    # Bad spi object.
    with assert_raises(TypeError):
        Sd(None)

    # Bad block size.
    with assert_raises(ValueError, "bad buffer length"):
        SD.write_block(0, '')
    with assert_raises(ValueError, "bad buffer length"):
        SD.write_block(0, 1024 * ' ')
    with assert_raises(TypeError):
        SD.read_block_into(0, ' ')

    # Read fails.
    with assert_raises(OSError):
        SD.read_block(0)


def test_stop():
    SD.stop()


def main():
    testcases = [
        (test_start, "test_start"),
        (test_print, "test_print"),
        (test_read_cid, "test_read_cid"),
        (test_read_csd, "test_read_csd"),
        (test_read_write, "test_read_write"),
        (test_read_write_fail, "test_read_write_fail"),
        (test_bad_arguments, "test_bad_arguments"),
        (test_stop, "test_stop")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
