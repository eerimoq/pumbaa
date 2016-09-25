unction:: by   sphinx-quickstart on Sun Dec 20 08:56:50 2015.()

   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

|buildstatus|_
|codecov|_

Welcome to Pumbaa's documentation!
==================================

`Pumbaa` is `Python`_ on top of `Simba`_.

The implementation is a port of `MicroPython`_, designed for embedded
devices with limited amount of RAM and code memory.

Project homepage: https://github.com/eerimoq/pumbaa

.. toctree::
   :maxdepth: 1
   :titlesonly:
   :hidden:

   installation
   user-guide
   developer-guide
   boards
   examples
   library-reference

Features
--------

* `MicroPython 3`_ language.

* :doc:`Python standard library<library-reference/standard-library>` modules.

* :doc:`library-reference/micropython` modules.

* A thin :doc:`C to Python wrapper layer<library-reference/pumbaa/simba>` on
  top of some Simba modules.

See the :doc:`library-reference` for a full list of features.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

.. |buildstatus| image:: https://travis-ci.org/eerimoq/pumbaa.svg
.. _buildstatus: https://travis-ci.org/eerimoq/pumbaa

.. |codecov| image:: https://codecov.io/gh/eerimoq/pumbaa/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/pumbaa

.. _Python: https://www.python.org/
.. _Simba: http://simba-os.readthedocs.io/en/latest/
.. _MicroPython: http://www.micropython.org/
.. _MicroPython 3: http://docs.micropython.org/en/latest/pyboard/reference/index.html
