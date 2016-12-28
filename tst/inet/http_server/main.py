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
from inet import HttpServer, HttpWebSocketServer
from harness import assert_raises
import socket_stub

HTTP_SERVER = None


def on_404_not_found(connection, request):
    print('on_404_not_found({}, {})'.format(connection, request))
    text = "The requested page '{}' could not be found.".format(request.path)
    return (text,
            HttpServer.RESPONSE_CODE_404_NOT_FOUND,
            HttpServer.CONTENT_TYPE_TEXT_PLAIN)


def on_index(connection, request):
    print('on_index({}, {})'.format(connection, request))
    return ('Welcome!', )


def on_split(connection, request):
    print('on_split({}, {})'.format(connection, request))
    text = 'Welcome split!'
    connection.response_write((len(text), ))
    connection.socket_write(text)


def on_bad_arguments(connection, request):
    print('on_bad_arguments({}, {})'.format(connection, request))
    return ()


def on_websocket_echo(connection, request):
    print('on_websocket_echo({}, {})'.format(connection, request))
    ws = HttpWebSocketServer(connection, request)
    print(ws)

    # Websocket read() and write().
    message = ws.read()
    ws.write(message)

    # Websocket read_into() and write().
    message = bytearray(16)
    assert ws.read_into(message) == 4
    ws.write(message[:4])


def test_print():
    print(HttpServer)
    print(HttpServer("127.0.0.1", 80, [], on_404_not_found))
    print(HttpWebSocketServer)


def test_start():
    routes = [
        ('/index.html', on_index),
        ('/split.html', on_split),
        ('/websocket/echo', on_websocket_echo),
        ('/bad_arguments.html', on_bad_arguments)
    ]

    global HTTP_SERVER
    HTTP_SERVER = HttpServer("127.0.0.1", 80, routes, on_404_not_found)
    HTTP_SERVER.start()


def test_index():
    # Test data.
    request = "GET /index.html HTTP/1.1\r\n" \
              "User-Agent: TestcaseRequestIndex\r\n" \
              "Connection: keep-alive\r\n" \
              "\r\n"
    response = "HTTP/1.1 200 OK\r\n" \
               "Content-Type: text/html\r\n" \
               "Content-Length: 8\r\n" \
               "\r\n" \
               "Welcome!"

    # Accept.
    socket_stub.accept()

    # Get.
    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response.decode('uft-8') == response
    socket_stub.wait_closed()


def test_split():
    # Test data.
    request = "GET /split.html HTTP/1.1\r\n" \
              "User-Agent: TestcaseRequestIndex\r\n" \
              "Connection: keep-alive\r\n" \
              "\r\n"
    response = "HTTP/1.1 200 OK\r\n" \
               "Content-Type: text/html\r\n" \
               "Content-Length: 14\r\n" \
               "\r\n" \
               "Welcome split!"

    # Accept.
    socket_stub.accept()

    # Get.
    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response.decode('uft-8') == response
    socket_stub.wait_closed()


def test_no_route():
    # Accept.
    socket_stub.accept()

    # Get.
    request = "GET /missing.html HTTP/1.1\r\n" \
              "User-Agent: TestcaseRequestIndex\r\n" \
              "Connection: keep-alive\r\n" \
              "\r\n"
    response = "HTTP/1.1 404 Not Found\r\n" \
               "Content-Type: text/plain\r\n" \
               "Content-Length: 54\r\n" \
               "\r\n" \
               "The requested page '/missing.html' could not be found."
    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response.decode('uft-8') == response
    socket_stub.wait_closed()


def test_bad_arguments():
    # Test data.
    request = "GET /bad_arguments.html HTTP/1.1\r\n" \
              "User-Agent: TestcaseRequestIndex\r\n" \
              "Connection: keep-alive\r\n" \
              "\r\n"

    # Accept.
    socket_stub.accept()

    # Get.
    socket_stub.input(request)
    socket_stub.wait_closed()


def test_websocket_echo():
    # Accept.
    socket_stub.accept()

    # Handshake.
    request = "GET /websocket/echo HTTP/1.1\r\n" \
              "User-Agent: TestcaseRequestIndex\r\n" \
              "Upgrade: WebSocket\r\n" \
              "Connection: Upgrade\r\n" \
              "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n" \
              "Sec-WebSocket-Version: 13\r\n" \
              "\r\n"
    response = "HTTP/1.1 101 Switching Protocols\r\n" \
               "Upgrade: websocket\r\n" \
               "Connection: Upgrade\r\n" \
               "Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\n" \
               "\r\n"
    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response.decode('utf-8') == response

    # Message.
    request = b'\x81\x84\x00\x00\x00\x00123\x00'
    response = b'\x81\x84\x00\x00\x00\x00123\x00'

    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response == response

    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response == response
    socket_stub.wait_closed()


def test_stop():
    HTTP_SERVER.stop()


def main():
    testcases = [
        (test_print, "test_print"),
        (test_start, "test_start"),
        (test_index, "test_index"),
        (test_split, "test_split"),
        (test_no_route, "test_no_route"),
        (test_bad_arguments, "test_bad_arguments"),
        (test_websocket_echo, "test_websocket_echo"),
        (test_stop, "test_stop")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
