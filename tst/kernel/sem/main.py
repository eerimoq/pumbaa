#!/usr/bin/env python
#
# @file main.py
# @version 0.2.0
#
# @section License
# Copyright (C) 2014-2016, Erik Moqvist
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
# This file is part of the Pumba project.
#

from kernel import thrd, sem, sys
from slib import harness

SEMS = [sem.Sem(), sem.Sem(0)]

def all_main():
    SEMS[1].put()
    SEMS[0].get()
    SEMS[1].put()
    
    thrd.suspend()


def test_all(_):
    """All tests.

    """

    SEMS[0].get()
    try:
        SEMS[0].get(0.0)
        assert False
    except Exception:
        pass

    # Create two threads with higher priority than this one.
    for _ in range(2):
        thrd.spawn(all_main, prio=-10)

    # Wait until both threads are waiting for sems[0].
    SEMS[1].get()
    SEMS[1].get()

    # Start both threads.
    SEMS[0].put(1)
    SEMS[0].put(1)

    SEMS[1].get()
    SEMS[1].get()


if __name__ == "__main__":
    sys.start();

    harness.Harness([
        test_all
    ]).run()
