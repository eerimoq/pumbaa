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
from sync import Event, Queue
from drivers import Exti, Pin
import board
import harness
from harness import assert_raises


def test_print():
    print(Exti)
    event = Event()
    exti = Exti(board.EXTI_D0, Exti.RISING, event, 1)
    print(exti)


def test_falling_edge():
    pin = Pin(board.PIN_D4, Pin.OUTPUT)
    pin.write(1)

    event = Event()

    exti = Exti(board.EXTI_D3, Exti.FALLING, event, 0x1)
    exti.start()

    # Make sure no interrupt has already occured.
    assert event.size() == 0

    # Trigger the interrupt and wait for the event.
    pin.write(0)

    if not 'Linux' in os.uname().machine:
        print("Waiting for the interrupt to occur... ")
        assert event.read(0x1) == 0x1

    exti.stop()


def test_rising_edge_two_pins():
    pin_a = Pin(board.PIN_D4, Pin.OUTPUT)
    pin_b = Pin(board.PIN_D6, Pin.OUTPUT)

    pin_a.write(0)
    pin_b.write(0)

    queue = Queue()

    exti_a = Exti(board.EXTI_D3, Exti.RISING, queue, 'a')
    exti_b = Exti(board.EXTI_D5, Exti.RISING, queue, 'b')

    exti_a.start()
    exti_b.start()

    # Make sure no interrupt has already occured.
    assert queue.size() == 0

    # Trigger interrupts to create the character sequence 'aaba'.
    pin_a.write(1)
    pin_a.write(0)
    pin_a.write(1)
    pin_a.write(0)

    pin_b.write(1)
    pin_b.write(0)

    pin_a.write(1)
    pin_a.write(0)

    if not 'Linux' in os.uname().machine:
        print("Waiting for the character sequence... ")
        assert queue.read(4) == b'aaba'

    exti_a.stop()
    exti_b.stop()


def test_bad_arguments():
    event = Event()
    queue = Queue()

    # Bad device.
    with assert_raises(ValueError, "bad exti device -1"):
        Exti(-1, Exti.BOTH, event, 1)

    # Bad trigger.
    with assert_raises(ValueError, "bad trigger -1"):
        Exti(board.EXTI_D3, -1, event, 1)

    # Bad channel.
    with assert_raises(TypeError, "expected <class 'Event'> or <class 'Queue'>"):
        Exti(board.EXTI_D3, Exti.BOTH, None, 1)

    # Bad event data.
    with assert_raises(TypeError, "can't convert NoneType to int"):
        Exti(board.EXTI_D3, Exti.BOTH, event, None, 1)

    # Bad queue data.
    with assert_raises(TypeError, "object with buffer protocol required"):
        Exti(board.EXTI_D3, Exti.BOTH, queue, None, 1)

    # Bad callback.
    with assert_raises(TypeError, "bad callback"):
        Exti(board.EXTI_D3, Exti.BOTH, event, 1, 1)


TESTCASES = [
    (test_print, "test_print"),
    (test_falling_edge, "test_falling_edge"),
    (test_rising_edge_two_pins, "test_rising_edge_two_pins"),
    (test_bad_arguments, "test_bad_arguments")
]
