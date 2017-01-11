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
import _thread
from sync import Event
import harness
import gc

EVENT = Event()


def thread_main(mask):
    EVENT.write(mask)


def test_print():
    help(_thread)


def test_start():
    mask = 0x1
    _thread.start_new_thread(thread_main, (mask, ))
    assert EVENT.read(mask) == mask


def test_gc():
    if os.uname().machine != "Linux with Linux":
        print('Free memory before gc:', gc.mem_free())
        gc.collect()
        print('Free memory after gc:', gc.mem_free())


TESTCASES = [
    (test_print, "test_print"),
    (test_start, "test_start"),
    (test_gc, "test_gc")
]
