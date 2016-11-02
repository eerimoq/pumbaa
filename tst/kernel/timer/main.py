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


import harness
from kernel import Timer
from sync import Event
from harness import assert_raises


def test_help():
    event = Event()
    timer = Timer(1, event, 0x1)
    help(Timer)
    help(timer)


def test_single_shot_timer():
    event = Event()
    timer = Timer((0, 1000000), event, 0x1)
    print("starting single shot timer")
    timer.start()
    event.read(0x1)
    print("timeout")


def test_periodic_timer():
    event = Event()
    timer = Timer(0.2, event, 0x1, flags=Timer.PERIODIC)
    print("starting periodic timer")
    timer.start()
    for i in range(3):
        event.read(0x1)
        print("timeout", i)
    timer.stop()


def test_empty():
    Timer(1.0)


def test_bad_arguments():
    # Too long tuple.
    with assert_raises(TypeError, "expected tuple of length 2"):
        Timer((1, 0, 0), None, 1)

    # Bad timeout arguement.
    with assert_raises(TypeError, "can't convert NoneType to float"):
        Timer(None, None, 1)

    # Wrong type of second argument.
    with assert_raises(TypeError, "expected <class 'Event'>"):
        Timer(1, 1, 1)


def main():
    testcases = [
        (test_help, "test_help"),
        (test_single_shot_timer, "test_single_shot_timer"),
        (test_periodic_timer, "test_periodic_timer"),
        (test_empty, "test_empty"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
