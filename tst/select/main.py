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

import select
from simba import Event
import harness
from harness import assert_raises


def test_help():
    poll = select.poll()
    help(select)
    help(poll)


def test_poll():
    poll = select.poll()
    event1 = Event()
    event2 = Event()

    # Register both event channels.
    poll.register(event1)
    poll.register(event2)

    # Timeout waiting for event.
    assert poll.poll(0.01) == []

    # Event write, poll and read.
    event2.write(0x1)
    assert poll.poll() == [(event2, select.POLLIN)]
    assert event2.read(0x1) == 0x1

    # Unregister first event channel.
    poll.unregister(event1)

    # Event write, poll and read.
    event2.write(0x1)
    assert poll.poll() == [(event2, select.POLLIN)]
    assert event2.read(0x1) == 0x1

    # Timeout waiting for event.
    assert poll.poll(0.01) == []


def test_bad_arguments():
    poll = select.poll()

    with assert_raises(TypeError, "channel object required"):
        poll.register(None)

    with assert_raises(OSError):
        poll.unregister(None)


def main():
    testcases = [
        (test_help, "test_help"),
        (test_poll, "test_poll"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
