Configuration
=============

Standard Library
----------------

The :doc:`../library-reference` is configured at compile time using
defines named ``CONFIG_*``. The default configuration includes most
functionality, as most application wants that. If an application has
special requirements, for example memory constraints, it has to be
configured to remove unnecessaray functionality.

Simba can be configured as described in the `Simba documentation`_.

Search order
^^^^^^^^^^^^

Highest priority first.

1. Command line as ``CDEFS_EXTRA="<configuration variable>=<value>"``.

2. A file named ``config.h`` in the application root folder. Always
   add ``#include "simba_config.h"`` at the end of this file to
   include the default Simba configuration as defined by Pumbaa.

3. The default configuration file, :github-blob:`src/pumbaa_config_default.h`.

Variables
^^^^^^^^^

All configuration variables are listed below. Their default values are
defined in :github-blob:`src/pumbaa_config_default.h`.

.. _Simba documentation: http://simba-os.readthedocs.io/en/latest/user-guide/configuration.html
