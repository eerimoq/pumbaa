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


import select
import socket
from harness import assert_raises
import socket_stub


def test_print():
    print(socket)


def test_tcp_client():
    sock = socket.socket()
    sock.connect(("192.168.0.1", 8080))
    assert sock.send(b'send()') == 6
    assert sock.sendall(b'send()') == 6
    assert sock.recv(6) == b'recv()'
    sock.close()


def test_tcp_server():
    socket_stub.set_accept(0)
    
    listener = socket.socket()
    listener.bind(("192.168.0.1", 8080))
    listener.listen(1)
    client, fromaddr = listener.accept()
    assert client is not None
    assert fromaddr is None
    listener.close()


def test_udp():
    # Prepare the socket stub.
    socket_stub.set_sendto([
        b'123',
        (b'456', -1),
        (b'789', 0),
    ])
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    buf, fromaddr = sock.recvfrom(1024)
    assert buf == b'recvfrom()'
    assert fromaddr == (b'192.168.0.1', 8080)
    assert sock.sendto(b'123', fromaddr) == 3
    assert sock.sendto(b'456', fromaddr) == 0
    assert sock.sendto(b'789', fromaddr) == 0


def test_select():
    poll = select.poll()
    tcp = socket.socket()

    # Register both event channels.
    poll.register(tcp)

    # Timeout waiting for data on the socket.
    assert poll.poll(0.01) == []

    tcp.close()


def test_errors():
    # Failed accept.
    socket_stub.set_accept(-1)

    listener = socket.socket()
    listener.bind(("192.168.0.1", 8080))
    listener.listen(1)

    with assert_raises(OSError, 'socket accept failed'):
        listener.accept()

    listener.close()

    # Failed bind and close.
    socket_stub.set_bind(-1)
    socket_stub.set_close(-1)

    listener = socket.socket()

    with assert_raises(OSError, 'socket bind failed'):
        listener.bind(("192.168.0.1", 8080))

    with assert_raises(OSError, 'socket close failed'):
        listener.close()

    # Failed listen.
    socket_stub.set_listen(-1)
    
    listener = socket.socket()
    listener.bind(("192.168.0.1", 8080))

    with assert_raises(OSError, 'socket listen failed'):
        listener.listen(1)

    listener.close()

    # Failed connect.
    socket_stub.set_connect(-1)
    
    sock = socket.socket()

    with assert_raises(OSError, 'socket connect failed'):
        sock.connect(("192.168.0.1", 8080))

    sock.close()

    # Failed send and recv.
    socket_stub.set_send([-1, 0])
    socket_stub.set_recv([-1, 0])
    
    sock = socket.socket()
    sock.connect(("192.168.0.1", 8080))
    assert sock.send(b'bar') == 0
    assert sock.recv(5) == b''
    assert sock.send(b'bar') == 0
    assert sock.recv(5) == b''
    sock.close()

    
def test_bad_arguments():
    # Bad socket family.
    with assert_raises(OSError):
        socket.socket(-1)

    # Bad socket type.
    with assert_raises(OSError):
        socket.socket(socket.AF_INET, -1)


TESTCASES = [
    (test_print, "test_print"),
    (test_tcp_client, "test_tcp_client"),
    (test_tcp_server, "test_tcp_server"),
    (test_udp, "test_udp"),
    (test_select, "test_select"),
    (test_errors, "test_errors"),
    (test_bad_arguments, "test_bad_arguments")
]
