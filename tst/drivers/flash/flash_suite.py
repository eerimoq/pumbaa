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
from drivers import Flash
import harness
from harness import assert_raises


def test_print():
    print(Flash)
    flash = Flash(0)
    print(flash)


def test_erase_read_write_read():
    address = 0
    flash = Flash(0)
    flash.erase(address, 4096)
    assert flash.read(address, 1) == b'\xff'
    assert flash.write(address, b'\x12') == 1
    assert flash.read(address, 1) == b'\x12'


TESTCASES = [
    (test_print, "test_print"),
    (test_erase_read_write_read, "test_erase_read_write_read")
]
