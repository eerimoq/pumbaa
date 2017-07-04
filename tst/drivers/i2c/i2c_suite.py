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
from drivers import I2C
import board
import harness
from harness import assert_raises


def test_print():
    print(I2C)
    i2c = I2C(0)
    print(i2c)


def test_write():
    i2c = I2C(0, I2C.BAUDRATE_400KBPS)
    i2c.start()
    i2c.write(0x57, b'1234')


def test_read():
    i2c = I2C(0, I2C.BAUDRATE_1MBPS)
    i2c.start()
    assert i2c.read(0x57, 4) == b'2345'
    buf = bytearray(3)
    assert i2c.read_into(0x57, buf) == 3
    assert buf == bytearray(b"678")
    assert i2c.read_into(0x57, buf, 1) == 1
    assert buf == bytearray(b"978")


def test_scan():
    i2c = I2C(0)
    i2c.start()
    assert i2c.scan() == [0x57]


def test_bad_arguments():
    # Bad device.
    with assert_raises(ValueError, "bad device"):
        I2C(100)

    i2c = I2C(0)
    i2c.start()

    # Bad buffer length in read.
    with assert_raises(ValueError, "bad buffer length"):
        buf = bytearray(1)
        i2c.read_into(0, buf, 2)

    # Bad buffer length in write.
    with assert_raises(ValueError, "bad buffer length"):
        buf = bytearray(1)
        i2c.write(0, buf, 2)


TESTCASES = [
    (test_print, "test_print"),
    (test_write, "test_write"),
    (test_read, "test_read"),
    (test_scan, "test_scan"),
    (test_bad_arguments, "test_bad_arguments")
]
