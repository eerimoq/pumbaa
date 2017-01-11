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

import harness
from harness import assert_raises
import ssl
import socket
import socket_stub


def test_print():
    help(ssl)
    print(ssl)
    context = ssl.SSLContext(ssl.PROTOCOL_TLS)
    print(context)
    help(context)
    server_sock = socket.socket()
    ssl_server_sock = context.wrap_socket(server_sock)
    print(ssl_server_sock)
    help(ssl_server_sock)


def test_client():
    with open('server.crt', 'w') as f:
        f.write('foo')
        
    context = ssl.SSLContext(ssl.PROTOCOL_TLS)
    context.load_verify_locations(cafile="server.crt")
    context.set_verify_mode(ssl.CERT_REQUIRED)

    sock = socket.socket()
    sock.connect(('192.168.0.1', 8080))
    ssl_sock = context.wrap_socket(sock)

    assert ssl_sock.get_server_hostname() == 'test_server'
    assert ssl_sock.cipher() == ('TLS-RSA-WITH-AES-256-GCM-SHA384',
                                 'TLSv1.1',
                                 -1)
    
    assert ssl_sock.send(b'hello') == 5
    assert ssl_sock.recv(7) == b'goodbye'


def test_server():
    with open('server.crt', 'w') as f:
        f.write('bar')
    with open('server.key', 'w') as f:
        f.write('fie')

    socket_stub.set_accept(0)
        
    context = ssl.SSLContext(ssl.PROTOCOL_TLS)
    context.load_cert_chain("server.crt", keyfile="server.key")

    listener_sock = socket.socket()
    listener_sock.bind(('192.168.0.1', 8080))
    listener_sock.listen(5)

    sock, _ = listener_sock.accept()
    ssl_sock = context.wrap_socket(sock, server_side=True)

    assert ssl_sock.get_server_hostname() is None
    assert ssl_sock.cipher() == ('TLS-RSA-WITH-AES-256-GCM-SHA384',
                                 'TLSv1.1',
                                 -1)

    assert ssl_sock.recv(5) == b'hello'
    assert ssl_sock.send(b'goodbye') == 7


TESTCASES = [
    (test_print, "test_print"),
    (test_client, "test_client"),
    (test_server, "test_server")
]
