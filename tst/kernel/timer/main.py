#
# @file main.py
#
# @section License
# Copyright (C) 2016, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Pumbaa project.
#

import harness
from simba import Timer, Event
from harness import assert_raises


def test_help():
    EVENT = Event()
    TIMER = Timer(1, EVENT, 0x1)
    help(Timer)
    help(TIMER)


def test_single_shot_timer():
    EVENT = Event()
    TIMER = Timer((0, 1000000), EVENT, 0x1)
    print("starting single shot timer")
    TIMER.start()
    EVENT.read(0x1)
    print("timeout")


def test_periodic_timer():
    EVENT = Event()
    TIMER = Timer(0.2, EVENT, 0x1, flags=Timer.PERIODIC)
    print("starting periodic timer")
    TIMER.start()
    for i in range(3):
        EVENT.read(0x1)
        print("timeout", i)
    TIMER.stop()


def test_bad_arguments():
    # Too long tuple.
    with assert_raises(TypeError, "expected tuple of length 2"):
        Timer((1, 0, 0), None, 1)

    # Bad timeout arguement.
    with assert_raises(TypeError, "can't convert NoneType to float"):
        Timer(None, None, 1)

    # Wrong type of second argument.
    with assert_raises(TypeError, "expected <class 'Event'>"):
        Timer(1, None, 1)

    # Too few arguments.
    with assert_raises(TypeError, "'mask' argument required"):
        Timer(1, None)


def main():
    testcases = [
        (test_help, "test_help"),
        (test_single_shot_timer, "test_single_shot_timer"),
        (test_periodic_timer, "test_periodic_timer"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
