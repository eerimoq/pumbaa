Installation
============

`Pumbaa` uses the `Simba` make based builds system to build
application and upload it to the baord.

The `Simba` development environment can be installed on `Windows
(Cygwin)`_ and `Linux (Ubuntu 14)`_. Just follow the steps below and
you'll be up and running in no time. =)

Windows (Cygwin)
^^^^^^^^^^^^^^^^

Download `Cygwin`_ and select the following packages for installation:

.. code-block:: text

   - gcc-core          (Devel -> gcc-core)
   - make              (Devel -> make)
   - python            (Python -> python)
   - python-setuptools (Python -> python-setuptools)
   - git               (Devel -> git)
   - doxygen           (Devel -> doxygen)

Start `Cygwin` and execute the one-liner below to install `Pumbaa`.

.. code-block:: text

   $ mkdir pumbaa && \
     cd pumbaa && \
     easy_install-2.7 pip && \
     pip install pyserial xpect readchar sphinx breathe && \
     git clone https://github.com/eerimoq/arm-toolchain-windows && \
     git clone https://github.com/eerimoq/pumbaa --recursive

Linux (Ubuntu 14)
^^^^^^^^^^^^^^^^^

Execute the one-liner below to install `Pumbaa`.

.. code-block:: text

   $ mkdir pumbaa && \
     cd pumbaa && \
     sudo apt install ckermit valgrind cppcheck cloc python python-pip doxygen git lcov && \
     sudo apt install bossa-cli gcc-arm-none-eabi && \
     sudo apt install make unrar autoconf automake libtool gcc g++ gperf \
                          flex bison texinfo gawk ncurses-dev libexpat-dev \
                          python-serial sed libtool-bin && \
     sudo pip install pyserial xpect sphinx breathe sphinx_rtd_theme && \
     git clone https://github.com/eerimoq/pumbaa --recursive

.. _Cygwin: https://cygwin.com/setup-x86.exe
