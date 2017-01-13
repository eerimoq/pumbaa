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


from drivers import Uart


def test_print():
    print(Uart)
    uart = Uart(1)
    print(uart)


def test_write():
    uart = Uart(1)
    uart.start()
    assert uart.write(b'1234') == 4
    assert uart.write(b'5678', 1) == 1
    uart.stop()


def test_read():
    uart = Uart(1)
    uart.start()
    buf = uart.read(1)
    assert len(buf) == 1
    print(buf)
    buf = bytearray(8)
    assert uart.read_into(buf, 1) == 1
    print(buf)
    uart.stop()


TESTCASES = [
    (test_print, "test_print"),
    (test_write, "test_write"),
    (test_read, "test_read")
]
