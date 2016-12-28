#!/usr/bin/env python

from __future__ import print_function

import sys
import websocket
from websocket import create_connection

websocket.enableTrace(True)
ws = create_connection("ws://{}:8000/websocket/echo".format(sys.argv[1]))

message = "Hello, World"
print("Sending '{}'.".format(message))
ws.send(message)
print("Received '{}'".format(ws.recv()))

ws.close()
