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
from pumbaa import Timer, Event

def test_help():
    EVENT = Event()
    TIMER = Timer(1, EVENT)
    help(Timer)
    help(TIMER)

def test_single_shot_timer():
    EVENT = Event()
    TIMER = Timer(1, event=EVENT, mask=1, flags=1)
    TIMER = Timer(1, EVENT, -1)
    print("starting single shot timer")
    TIMER.start()
    EVENT.read()
    print("timeout")

def test_periodic_timer():
    EVENT = Event()
    TIMER = Timer(1, EVENT, flags=Timer.PERIODIC)
    print("starting periodic timer")
    TIMER.start()
    for i in range(3):
        EVENT.read()
        print("timeout", i)
    TIMER.stop()

def test_bad_arguments():
    try:
        Timer(1, None)
    except TypeError:
        pass
    else:
        raise

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
