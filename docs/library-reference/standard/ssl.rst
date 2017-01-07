:mod:`ssl` --- TLS/SSL wrapper for socket objects
=================================================

.. module:: ssl
   :synopsis: TLS/SSL wrapper for socket objects.

Wrap sockets in TLS/SSL to encrypt the transport channel.

.. warning:: This module may lead to a false sense of security, as it
             is implemented by a TLS/SSL novice, me. Use with care!

Server side example:

.. code-block:: python

   >>> context = ssl.SSLContext(ssl.PROTOCOL_TLS)
   >>> context.load_cert_chain("server.crt", keyfile="server.key")

   >>> listener_sock = socket.socket()
   >>> listener_sock.bind(('127.0.0.1', 10023))
   >>> listener_sock.listen(5)

   >>> client_sock, _ = listener_sock.accept()
   >>> ssl_client_sock = context.wrap_socket(client_sock, server_side=True)

   >>> ssl_client_sock.recv(5)
   b'hello'
   >>> ssl_client_sock.send(b'goodbye')
   >>> ssl_client_sock.close()
   >>> client_sock.close()

Client side example:

.. code-block:: python

   >>> context = ssl.SSLContext(ssl.PROTOCOL_TLS)
   >>> context.load_verify_locations(cafile="server.crt")

   >>> server_sock = socket.socket()
   >>> server_sock.connect(('127.0.0.1', 10023))
   >>> ssl_server_sock = context.wrap_socket(server_sock)

   >>> ssl_server_sock.send(b'hello')
   >>> ssl_server_sock.recv(7)
   'goodbye'
   >>> ssl_server_sock.close()
   >>> server_sock.close()

----------------------------------------------

.. class:: ssl.SSLContext(protocol=ssl.PROTOCOL_TLS)

   Initialize given SSL context. A SSL context contains settings that
   lives longer than a socket.


   .. method:: load_cert_chain(certfile, keyfile=None)

      Load given certificate chain into the context.


   .. method:: load_verify_locations(cafile)

      Load a set of "certification authority" (CA) certificates used
      to validate other peers’ certificates when `verify_mode` is
      other than :data:`CERT_NONE`.


   .. method:: set_verify_mode(mode)

      Whether to try to verify other peers’ certificates. Set `mode`
      to :data:`CERT_NONE` to skip the verification, and
      :data:`CERT_REQUIRED` to enable verification.

      By default, server side sockets does not verify the client's
      certificate, while client side sockets do verify the server's
      certificate.

      Load CA certificates with `load_verify_location()`.


   .. method:: wrap_socket(sock, server_side=False)

      Wrap a normal TCP socket `sock` in this SSL context.

      Performs the SSL handshake.


   .. data:: CERT_NONE

      Do not verify the peer certificate.


   .. data:: CERT_REQUIRED

      Verify the peer certificate.


.. class:: ssl.SSLSocket

   This is a Python type object that represents the SSL socket.


   .. method:: close()

      Close the SSL socket.


   .. method:: recv(bufsize)

      Receive data from the socket. The return value is a string
      representing the data received. The maximum amount of data to be
      received at once is specified by `bufsize`.


   .. method:: send(string)

      Send data `string` to the socket. The socket must be connected
      to a remote socket. Returns the number of bytes
      sent. Applications are responsible for checking that all data
      has been sent; if only some of the data was transmitted, the
      application needs to attempt delivery of the remaining data.


   .. method:: get_server_hostname()

      Returns the hostname of the server as a string.


   .. method:: cipher()

      Returns the three-tuple with connection cipher information. For
      example ``('TLS-RSA-WITH-AES-256-GCM-SHA384', 'TLSv1.1', -1)``
