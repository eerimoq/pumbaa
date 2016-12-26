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

from inet import HttpServer, WebSocketServer
import kernel
from drivers import esp_wifi


SSID = 'Qvist2'
PASSWORD = 'maxierik'
IP = '192.168.0.7'
PORT = 8000


def on_no_route(client, request):
    print('on_no_route:', client, request)

    return None, HttpServer.RESPONSE_CODE_404_NOT_FOUND


def on_request_index(client, request):
    print('on_request_index:', client, request)

    return "<html><body>Hello from Pumbaa!</body></html>"


def on_request_websocket_echo(client, request):
    print('on_request_websocket_echo:', client, request)

    ws = WebSocketServer(client.socket)
    ws.handshake(request)

    while True:
        message = ws.read()

        if not message:
            break

        ws.write(message)


def main():
    esp_wifi.set_op_mode(esp_wifi.OP_MODE_STATION)
    esp_wifi.station_init(SSID, PASSWORD)
    esp_wifi.station_connect()

    routes = [
        ("/index.html", on_request_index),
        ("/websocket_echo.html", on_request_websocket_echo)
    ]

    http_server = HttpServer(IP, PORT, routes, on_no_route)
    http_server.start()
    kernel.thrd_suspend()


if __name__ == '__main__':
    main()
