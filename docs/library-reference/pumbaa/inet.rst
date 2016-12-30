:mod:`inet` --- Internet networking
===================================

.. module:: inet
   :synopsis: Internet networking.

Internet network configuration and protocol implementation.

Simba documentation: `inet`_

----------------------------------------------


.. class:: inet.HttpServer(ip_address, port, routes, on_no_route)

   Create a HTTP server object. The HTTP server opens a socket and
   binds to given `ip_address` and `port`. `routes` is a list of route
   tuples, where each route tuple contains a path and a callback
   function. `on_no_route` is called when a request is received for a
   path that is not found in `routes`.

   Here is an example of a HTTP server with one route,
   ``'/index.html'``. Enter ``http://192.168.0.7:8000/index.html`` in
   your webbrowser to get the index page.

   .. code-block:: python

      >>> from inet import HttpServer
      >>> def on_no_route(_, request):
      >>>     return (request.path + ' not found.',
      >>>             HttpServer.RESPONSE_CODE_404_NOT_FOUND,
      >>>             HttpServer.CONTENT_TYPE_TEXT_PLAIN)

      >>> def on_request_index(_, request):
      >>>     return ('<html><body>Hello from Pumbaa!</body></html>', )

      >>> routes = [('/index.html', on_request_index)]
      >>> http_server = HttpServer('192.168.0.7', 80, routes, on_no_route)
      >>> http_server.start()

   Simba documentation: `inet/http_server`_


   .. method:: start()

      Start the HTTP server.


   .. method:: stop()

      Stop the HTTP server.


.. class:: inet.HttpServerWebSocket(connection, request)

   Create a HTTP server WebSocket object with given `connection` and
   `request`.

   An example of how to use the HTTP server WebSocket class in a HTTP
   server route callback.

   .. code-block:: python

      >>> from inet import HttpServerWebSocket
      >>> def on_websocket_echo(connection, request):
      >>>     ws = HttpServerWebSocket(connection, request)
      >>>     while True:
      >>>         message = ws.read()
      >>>         print('Message:', message)
      >>>         if not message:
      >>>             break
      >>>         ws.write(message)


   .. method:: read()

      Read a message from the remote endpoint.


   .. method:: write(buffer)

      Write `buffer` to the remote endpoint.



.. function:: inet.ping_host_by_ip_address(address, timeout)

   Ping host by IPv4 address `address`. Send an echo request packet to
   the host and wait for the echo reply packet. Only the ICMP header
   is transmitted, no extra payload data is added to the
   packet. Returns the round trip time in milliseconds.

   Raises an `OSError` exception if no response is received within
   `timeout` seconds after the request is sent.

   .. code-block:: python

      >>> inet.ping_host_by_ip_address("192.168.0.5", 2)
      10
      >>> inet.ping_host_by_ip_address("192.168.0.7", 2)
      Traceback (most recent call last):
        File "<stdin>", line 1, in <module>
      OSError:

   Simba documentation: `inet/ping`_


.. _inet: http://simba-os.readthedocs.io/en/latest/library-reference/inet.html
.. _inet/http_server: http://simba-os.readthedocs.io/en/latest/library-reference/inet/http_server.html
.. _inet/ping: http://simba-os.readthedocs.io/en/latest/library-reference/inet/ping.html
