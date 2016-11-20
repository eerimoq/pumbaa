TCP Server
==========

About
-----

Create a listening TCP socket waiting for a client to connect. The
server reads one byte at a time from the socket and writes it back to
the client.

Source code
-----------

.. include:: tcp_server/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/tcp_server` folder.

Build and run
-------------

Build and upload the application.

.. code-block:: text

   $ cd examples/tcp_server
   $ make -s BOARD=<board> upload
