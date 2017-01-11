#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2016, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Pumbaa project.
#


from drivers import Can
import board
import harness
from harness import assert_raises


def test_print():
    print(Can)
    can = Can(board.CAN_0)
    print(can)


def test_standard_frame():
    can = Can(board.CAN_0, Can.SPEED_500KBPS)
    can.start()
    assert can.write(0x57, b'') is None
    frame = can.read()
    assert frame.id == 0x58
    assert frame.data == b''
    assert frame.flags == 0
    can.stop()


def test_extended_frame():
    can = Can(board.CAN_0, Can.SPEED_500KBPS)
    can.start()
    assert can.write(0x57, b'', Can.FLAGS_EXTENDED_FRAME) is None
    frame = can.read()
    assert frame.id == 0x58
    assert frame.data == b''
    assert frame.flags == Can.FLAGS_EXTENDED_FRAME
    can.stop()


def test_bad_arguments():
    # Bad device.
    with assert_raises(ValueError, "bad device"):
        Can(100, Can.SPEED_500KBPS)

    can = Can(board.CAN_0, Can.SPEED_500KBPS)
    can.start()

    # Bad id.
    with assert_raises(ValueError, "bad frame id"):
        can.write(-3, b'')

    # Bad frame data length.
    with assert_raises(ValueError, "bad frame data length"):
        can.write(1, b'123456789')


TESTCASES = [
    (test_print, "test_print"),
    (test_standard_frame, "test_standard_frame"),
    (test_extended_frame, "test_extended_frame"),
    (test_bad_arguments, "test_bad_arguments")
]
