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
from drivers import Spi, Sd
import board
import harness
from harness import assert_raises

SD = None


def test_print():
    print(Sd)
    help(Sd)


def test_start():
    global SD
    spi = Spi(board.SPI_0, board.PIN_D4)
    SD = Sd(spi)
    SD.start()


def test_read_cid():
    print(SD.read_cid())


def test_bad_arguments():
    # Bad spi object.
    with assert_raises(TypeError):
        Sd(None)


def main():
    testcases = [
        (test_print, "test_print"),
        (test_start, "test_start"),
        (test_read_cid, "test_read_cid"),
        (test_bad_arguments, "test_bad_arguments")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
