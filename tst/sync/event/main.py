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

from pumbaa import Event
import harness


def test_help():
    EVENT = Event()
    help(Event)
    help(EVENT)


def test_read_write():
    EVENT = Event()

    try:
        EVENT.write()
    except TypeError:
        pass
    else:
        raise

    EVENT.write(0x2)
    assert EVENT.size() == 1
    EVENT.read(0x2)
    EVENT.write(0x2)
    EVENT.read()


def test_bad_arguments():
    EVENT = Event()

    try:
        EVENT.read(None)
    except TypeError:
        pass
    else:
        raise


def main():
    testcases = [
        (test_help, "test_help"),
        (test_read_write, "test_read_write"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
