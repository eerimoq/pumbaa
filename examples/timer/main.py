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

import event
import timer

TIMEOUT_EVENT = 0x1

EVENT = event.Event()

TIMER = timer.Timer(0.5, EVENT, TIMEOUT_EVENT, timer.PERIODIC)
TIMER.start()

for i in range(10):
    EVENT.read(TIMEOUT_EVENT)
    print("timeout", i)

TIMER.stop()
