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

# 
# For Arduino Due:
# 
# +---------------+
# |        CAN0TX o----------o----------------+
# |               |          |                |
# |               |   Schottky diode          |
# |               |          |                |
# |        CAN0RX o----------o--------+       |
# |               |                   |       |
# |               |                 R 3k3   R 3k3
# |               |                   |       |
# | D53  - CAN1TX o----------o------- o       |
# |               |          |                |
# |               |   Schottky diode          |
# |               |          |                |
# | DAC0 - CAN1RX o----------o----------------+
# +---------------+
# 

from kernel import sys, thrd, time
from drivers import can
from slib import harness
import board

# Ping-pong can frame ids.
PING_ID = 0x23
PONG_ID = 0x24

CAN0 = None
CAN1 = None


def start(speed):
    """Initialize and start the two CAN controllers, CAN0 and CAN1, with
    given baudrate.
 
    @param[in] speed Baudrate to use.

    """

    global CAN0, CAN1
    CAN0 = can.Can(board.CAN0, speed=speed)
    CAN0.start()

    CAN1 = can.Can(board.CAN1, speed=speed)
    CAN1.start()


def stop():
    """Stop the two CAN controllers, CAN0 and CAN1.

    """

    CAN0.stop()
    CAN1.stop()


def rx_thrd():
    """Thread that receives CAN frames on controller CAN1 and verifies its
    content.
 
    This thread is used by the test `test_max_throughput()`.
    
    """

    frame_id = 0
    frame_count = 0

    thrd.set_name("rx_thrd")

    while True:
        frame = CAN1.read()

        assert frame.id == frame_id
        frame_count += 1

        if frame_count == 10000:
            frame_count = 0
            frame_id = 0
        else:
            frame_id += 1
            frame_id %= 0x800


def test_ping_pong(speed, extended_id):
    """Ping-pong test. Starts the CAN controllers with given speed, sends
    a few CAN frames and then stops the CAN controllers.
    
    @param[in] speed Badrate to use.
    
    @param[in] extended_id Use extendend CAN frame id if true(1),
                           otherwise use standard CAN id.

    """

    start(speed)

    #
    # Ping is transmitted from CAN0 to CAN1.
    #

    # Write ping.
    frame_id = PING_ID << (10 * int(extended_id))
    frame = can.Frame(frame_id, [0xfe], extended_id)
    CAN0.write(frame)

    # Read ping.
    frame = CAN1.read()
    frame.id >>= (10 * int(extended_id))
    assert frame.id == PING_ID
    assert frame.extended_id == extended_id
    assert frame.data == [0xfe]
    
    #
    # Pong is transmitted from can1 to can0.
    #

    # Write pong.
    frame_id = PONG_ID << (10 * int(extended_id))
    frame = can.Frame(frame_id, extended_id=extended_id)
    CAN1.write(frame)

    # Read pong.
    frame = CAN0.read()
    frame.id >>= (10 * int(extended_id))
    assert frame.id == PONG_ID
    assert frame.extended_id == extended_id
    assert frame.data == []

    stop()


def test_ping_pong_250k(_):
    test_ping_pong(can.SPEED_250KBPS, False)


def test_ping_pong_500k(_):
    test_ping_pong(can.SPEED_500KBPS, False)


def test_ping_pong_1000k(_):
    test_ping_pong(can.SPEED_1000KBPS, False)


def test_ping_pong_1000k_extended_id(_):
    test_ping_pong(can.SPEED_1000KBPS, True)


def test_max_throughput(_):
    """Test the maximum throughput on the CAN bus.

    """

    thrd.spawn(rx_thrd, prio=-1)

    start(can.SPEED_1000KBPS)

    frames_per_write = 8
    data_sizes = [0, 4, 8]

    for data_size in data_sizes:
        print("Writing 10000 frames of data size", data_size, ".")

        start_time = time.get()
        frame_id = 0

        # Prepare the array of frames.
        frames = frames_per_write * [can.Frame(-1, data_size * [0xaa])]

        # Write the array of frames to the hardware.
        for _ in range(10000 / frames_per_write):
            for k in range(frames_per_write):
                frames[k].id = frame_id
                frame_id += 1
            CAN0.write(frames)

        stop_time = time.get()

        # Statistics.
        elapsed_time = (stop_time - start_time)
        frames_per_second = (10000 / elapsed_time)
        bits_per_frame = (1 + 11 + 1 + 1 + 1 + 4 + 15 + 1 + 1 + 1 + 7
                          + 8 * data_size)
        bits_per_second = (frames_per_second * bits_per_frame)
        data_bits_per_second = (frames_per_second * 8 * data_size)

        print("elapsed time =", elapsed_time, "s,",
              frames_per_second, "frames/s,",
              bits_per_second, "bits/s,",
              data_bits_per_second, "data bits/s")

    stop()


if __name__ == "__main__":
    sys.start()

    harness.Harness([
        test_ping_pong_250k,
        test_ping_pong_500k,
        test_ping_pong_1000k,
        test_ping_pong_1000k_extended_id,
        test_max_throughput
    ]).run()
