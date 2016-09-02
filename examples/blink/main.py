#!/usr/bin/env python
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
# This file is part of the Pumba project.
#

import sys
import time

import pumbaa.pin as pin
import pumbaa.board as board

def main():
    """Main function of the Python blink application.

    """

    print("Python version:", sys.version)

    # Initialize the LED pin and set it high.
    led = pin.Pin(board.PIN_LED, pin.OUTPUT)
    led.write(1)

    # Toggle the LED state periodically.
    while True:
        time.sleep(0.5)
        led.toggle()


if __name__ == "__main__":
    main()
