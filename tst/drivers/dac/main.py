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
    Dac([Board.PIN_DAC0, Board.PIN_DAC1])

def test_bad_arguments():
    # Bad devices.
    try:
        Dac(None)
        assert False
    except ValueError as e:
        assert str(e) == "Bad pin mode 3"


def main():
    testcases = [
        (test_output, "test_output"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
