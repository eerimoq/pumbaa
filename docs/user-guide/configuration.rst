Configuration
=============

Standard Library
----------------

The :doc:`../library-reference` is configured at compile time using
defines that starts with ``CONFIG_PUMBAA_``. The default configuration
includes most functionality, as most application wants that. If an
application has special requirements, for example memory constraints,
it has to be configured to remove unnecessaray functionality.

The underlying MicroPython source code can also be configured in
compile time. Often these configuration valiables starts with
``MICROPY_``.

Simba can be configured as described in the `Simba documentation`_.

Search order
^^^^^^^^^^^^

Highest priority first.

Simba build system
""""""""""""""""""

1. Command line as ``CDEFS_EXTRA="<configuration variable>=<value>"``.

2. A file named ``config.h`` in the application root folder.

3. The default configuration file,
   :github-blob:`src/pumbaa_config_default.h`.

PlatformIO
""""""""""

1. The variable ``build_flags`` in ``platformio.ini`` as
   ``build_flags = -D<configuration variable>=<value>``.

2. A file named ``config.h`` in the application source folder ``src``.

3. The default configuration file,
   :github-blob:`src/pumbaa_config_default.h`.

Arduino IDE
"""""""""""

1. A file (also called a `tab`) named ``config.h`` in the sketch.

2. The default configuration file,
   :github-blob:`src/pumbaa_config_default.h`.

Variables
^^^^^^^^^

All configuration variables are listed in
:github-blob:`src/pumbaa_config_default.h`.

.. _Simba documentation: http://simba-os.readthedocs.io/en/latest/user-guide/configuration.html
