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

import sys
import select
import socket
from sync import Event, Queue
from drivers import Exti
import board

BUTTON_PIN = board.PIN_GPIO0
UDP_ADDRESS = '192.168.1.103'
UDP_PORT = 30303

button = Event()
exti = Exti(BUTTON_PIN, button, 0x1)

udp = socket.socket(type=socket.SOCK_DGRAM)
udp.bind((UDP_ADDRESS, UDP_PORT))

poll = select.poll()
poll.register(button)
poll.register(sys.stdin)
poll.register(udp)

while True:
    channels = poll.poll()

    for channel, evetmask in channels:
        if channel is button:
            button.read(0x1)
            print("button")
        elif channel is sys.stdin:
            print("sys.stdin:", sys.stdin.read(1))
        elif channel is udp:
            print("udp:", udp.recv(1024))
