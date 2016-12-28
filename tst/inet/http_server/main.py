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


def on_request_404_not_found(connection, request):
    print('on_request_404_not_found({}, {})'.format(connection, request))
    text = "The requested page '{}' could not be found.".format(request.path)
    return (text,
            HttpServer.RESPONSE_CODE_404_NOT_FOUND,
            HttpServer.CONTENT_TYPE_TEXT_PLAIN)


def on_request_index(connection, request):
    print('on_request_index({}, {})'.format(connection, request))
    return ('Welcome!', )


def on_request_split(connection, request):
    print('on_request_split({}, {})'.format(connection, request))
    text = 'Welcome split!'
    connection.response_write((len(text), ))
    connection.socket_write(text)


def on_request_websocket_echo(connection, request):
    print('on_request_websocket_echo({}, {})'.format(connection, request))
    ws = HttpWebSocketServer(connection, request)
    message = ws.read()
    ws.write(message)


def test_start():
    routes = [
        ('/index.html', on_request_index),
        ('/split.html', on_request_split),
        ('/websocket/echo', on_request_websocket_echo)
    ]

    global HTTP_SERVER
    HTTP_SERVER = HttpServer("127.0.0.1", 80, routes, on_request_404_not_found)
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
               "Connection: Upgrade\r\n" \
               "Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\n" \
               "\r\n"
    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response.decode('utf-8') == response

    # Message.
    request = b'\x81\x84\x00\x00\x00\x00123\x00'
    response = b'\x80\x84\x00\x00\x00\x00123\x00'

    socket_stub.input(request)
    read_response = socket_stub.output(len(response))
    assert read_response == response
    

def test_stop():
    HTTP_SERVER.stop()


def main():
    testcases = [
        (test_start, "test_start"),
        (test_index, "test_index"),
        (test_split, "test_split"),
        (test_no_route, "test_no_route"),
        (test_websocket_echo, "test_websocket_echo"),
        (test_stop, "test_stop")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
