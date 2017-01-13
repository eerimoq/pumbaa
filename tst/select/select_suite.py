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


import select
import board
from sync import Event, Queue
from drivers import Can, Uart
import harness
from harness import assert_raises


def test_help():
    poll = select.poll()
    help(select)
    help(poll)


def test_register_unregister():
    poll = select.poll()
    queue = Queue()
    event = Event()
    can = Can(board.CAN_0)
    uart = Uart(1)

    poll.register(queue)
    poll.register(event)
    poll.register(can)
    poll.register(uart)

    poll.unregister(queue)
    poll.unregister(event)
    poll.unregister(can)
    poll.unregister(uart)

    with assert_raises(OSError):
        poll.unregister(queue)


def test_poll():
    poll = select.poll()
    queue = Queue()
    event = Event()
    can = Can(board.CAN_0)
    uart = Uart(1)

    # Register both event channels.
    poll.register(queue)
    poll.register(event)
    poll.register(can)
    poll.register(uart)

    # Timeout waiting for event.
    assert poll.poll(0.01) == []

    # Event write, poll and read.
    event.write(0x1)
    assert poll.poll() == [(event, select.POLLIN)]
    assert event.read(0x1) == 0x1

    # Queue write, poll and read.
    queue.write(b'foo')
    assert poll.poll() == [(queue, select.POLLIN)]
    assert queue.read(3) == b'foo'


def test_bad_arguments():
    poll = select.poll()

    with assert_raises(TypeError, "channel object required"):
        poll.register(None)

    with assert_raises(OSError):
        poll.unregister(None)


TESTCASES = [
    (test_help, "test_help"),
    (test_register_unregister, "test_register_unregister"),
    (test_poll, "test_poll"),
    (test_bad_arguments, "test_bad_arguments")
]
