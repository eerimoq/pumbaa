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

import socket
import harness
from harness import assert_raises


def test_print():
    print(socket)
    print(socket.socket())


def test_tcp_client():
    client = socket.socket()

    with assert_raises(NotImplementedError):
        client.connect()

    with assert_raises(NotImplementedError):
        client.send()

    with assert_raises(NotImplementedError):
        client.recv()

    with assert_raises(NotImplementedError):
        client.close()


def test_tcp_server():
    listener = socket.socket()

    with assert_raises(NotImplementedError):
        listener.bind()

    with assert_raises(NotImplementedError):
        listener.listen()

    with assert_raises(NotImplementedError):
        listener.close()


def main():
    testcases = [
        (test_print, "test_print"),
        (test_tcp_client, "test_tcp_client"),
        (test_tcp_server, "test_tcp_server")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
