Build system
============

PlatformIO
----------

All Python source files in your project's ``src/`` folder will be
uploaded to the board automatically as frozen modules.

See the `PlatformIO website`_ for more information.

Arduino IDE
-----------

The build system only allows a single Python script file, the Arduino
sketch ``.ino``.

See the `Arduino website`_ for more information.

Simba build system
------------------

The make variable ``PYSRC`` is list of all Python script files.

See the `Simba documentation`_ for more information.

.. _PlatformIO website: http://docs.platformio.org/en/latest/core.html

.. _Arduino website: https://www.arduino.cc

.. _Simba documentation: http://simba-os.readthedocs.io/en/latest/user-guide/build-system.html
