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

import os
from pumbaa import Board, Dac
import harness


def test_output():
    # Single pin.
    dac = Dac(Board.PIN_DAC0)
    dac.convert("\x03\xff")

    # List of pins.
    dac = Dac([Board.PIN_DAC0, Board.PIN_DAC1], 11025)
    dac.convert(bytearray(16 * "\x03\xff"))
    dac.convert(16 * "\x03\xff")
    dac.async_convert(16 * "\x03\xff")
    dac.async_wait()


def test_bad_arguments():
    # Too many devices.
    try:
        Dac([0, 1, 2])
        assert False
    except ValueError as e:
        assert str(e) == "too many devices"

    # Bad devices type.
    try:
        Dac(None)
        assert False
    except TypeError as e:
        assert str(e) == "bad devices"

    # Bad pin.
    try:
        Dac(Board.PIN_LED)
        assert False
    except ValueError as e:
        assert str(e) == "bad pin"


def main():
    testcases = [
        (test_output, "test_output"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
