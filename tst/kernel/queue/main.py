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

from kernel import sys, thrd, chan, queue
from slib import harness

QUEUES = [queue.Queue(), queue.Queue()]

VALUES = [
    1,
    2,
    3,
    [4, 5, 6, 7],
    [4, 5, 6, 7]
]

def read_write_main():
    for value in VALUES:
        QUEUES[0].write(value)


def poll_main():
    for i in range(8, 12):
        QUEUES[1].write(i)
    QUEUES[0].write(12)


def test_read_write(_):
    thrd.spawn(read_write_main)
    for value in VALUES:
        assert QUEUES[0].read() == value


def test_poll(_):
    thrd.spawn(poll_main)

    chan_list = chan.List(QUEUES)
    i = 8
    for _ in range(5):
        queue = chan_list.poll()

        if queue == QUEUES[1]:
            assert QUEUES[1].read() == i
            i += 1
        elif queue == QUEUES[0]:
            assert QUEUES[0].read() == 12
        else:
            raise

    del chan_list


if __name__ == "__main__":
    sys.start();

    harness.Harness([
        test_read_write,
        test_poll
    ]).run()
