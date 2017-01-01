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

import os
import time
from inet import HttpServer, HttpServerWebSocket
import kernel
from drivers import esp_wifi


# Configuration.
SSID = 'Qvist2'
PASSWORD = 'maxierik'
IP = '192.168.0.7'
PORT = 80


def on_index(_, request):
    print('on_index:', request)

    return ('<html><body>Hello from Pumbaa!</body></html>', )


def on_websocket_echo(connection, request):
    print('on_websocket_echo:', request)

    ws = HttpServerWebSocket(connection, request)

    while True:
        message = ws.read()
        print('Message:', message)

        if not message:
            break

        ws.write(message)


def on_no_route(_, request):
    print('on_no_route:', request)

    return (request.path + ' not found.',
            HttpServer.RESPONSE_CODE_404_NOT_FOUND,
            HttpServer.CONTENT_TYPE_TEXT_PLAIN)


def main():
    esp_wifi.set_op_mode(esp_wifi.OP_MODE_STATION)
    esp_wifi.station_init(SSID, PASSWORD)
    esp_wifi.station_connect()

    while esp_wifi.station_get_status() != 'got-ip':
        print('Waiting for WiFi connection...')
        time.sleep(2)

    routes = [
        ('/index.html', on_index),
        ('/websocket/echo', on_websocket_echo)
    ]

    http_server = HttpServer(IP, PORT, routes, on_no_route)
    http_server.start()

    while True:
        print(os.system('kernel/thrd/list'))
        time.sleep(5)


if __name__ == '__main__':
    main()
