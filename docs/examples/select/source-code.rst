.. code-block:: python

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
   from sync import Event, Queue
   from drivers import Exti
   import board
   
   BUTTON_PIN = board.PIN_GPIO0
   UDP_ADDRESS = '192.168.1.103'
   UDP_PORT = 30303
   
   button = Event()
   exti = Exti(BUTTON_PIN, Exti.FALLING, button, 0x1)
   
   queue = Queue()
   
   udp = socket.socket(type=socket.SOCK_DGRAM)
   udp.bind((UDP_ADDRESS, UDP_PORT))
   
   poll = select.poll()
   poll.register(button)
   poll.register(queue)
   poll.register(udp)
   
   queue.write('foo')
   
   while True:
       [(channel, eventmask)] = poll.poll()
   
       if channel is button:
           button.read(0x1)
           print("button")
       elif channel is queue:
           print("queue:", queue.read(3))
       elif channel is udp:
           print("udp:", udp.recv(1024))

