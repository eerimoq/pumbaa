Test with webbrowser
====================

Enter ``https://192.168.0.7/index.html`` into the webbrowser. The
fetched webpage should say ``Hello from Pumbaa!``.

Test with curl
==============

Verification of the certificate is turned off because I don't have a
valid certificate in the test.

.. code-block:: text
   > curl --insecure https://192.168.0.7/index.html
   <html><body>
       Hello from Pumbaa!
   </body></html>
   >
