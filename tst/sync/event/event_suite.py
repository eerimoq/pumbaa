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


from sync import Event
import harness
from harness import assert_raises


def test_help():
    event = Event()
    help(Event)
    help(event)


def test_read_write():
    event = Event()

    event.write(0x6)
    assert event.size() == 1
    assert event.read(0x2) == 0x2
    assert event.read(0x4) == 0x4


def test_bad_arguments():
    event = Event()

    with assert_raises(TypeError, "can't convert NoneType to int"):
        event.read(None)

    with assert_raises(TypeError, "can't convert NoneType to int"):
        event.write(None)


TESTCASES = [
    (test_help, "test_help"),
    (test_read_write, "test_read_write"),
    (test_bad_arguments, "test_bad_arguments")
]
