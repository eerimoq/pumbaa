Getting Started
===============

Installation
------------

There are three build systems available; `PlatformIO`, `Arduino IDE`
and `Simba build system`. The `Simba build system` has more features
than to the other two. It supports executing test suites, generating
code coverage, profiling and more. Still, if you are familiar with
`Arduino IDE` or `PlatformIO`, use that instead since it will be less
troublesome.

|platformio| PlatformIO
^^^^^^^^^^^^^^^^^^^^^^^

Install `Pumbaa` in `PlatformIO`_.

1. Install the `PlatformIO IDE`_.

2. Start the `PlatformIO IDE` and open `PlatformIO` -> `Project
   Examples` and select `pumbaa/blink`.

3. Click on `Upload` (the arrow image) in the top left corner.

4. The built-in LED blinks!

5. Done!

|arduino| Arduino IDE
^^^^^^^^^^^^^^^^^^^^^

Install `Pumbaa` in the `Arduino IDE 1.6.10`_ as a third party board
using the Boards Manager.

1. Open `File` -> `Preferences`.

2. Add these URL:s to `Additional Boards Manager URLs` (click on the
   icon to the right of the text field) and press `OK`.

   .. code-block:: text

      https://raw.githubusercontent.com/eerimoq/pumbaa-releases/master/arduino/sam/package_pumbaa_sam_index.json
      https://raw.githubusercontent.com/eerimoq/pumbaa-releases/master/arduino/esp32/package_pumbaa_esp32_index.json

3. Open `Tools` -> `Board: ...` -> `Boards Manager...` and type
   `pumbaa` in the search box.

4. Click on `Pumbaa by Erik Moqivst version x.y.z` and click
   `Install` and press `Close`.

5. Open `Tools` -> `Board: ...` -> `Boards Manager...` and
   select one of the Pumbaa boards in the list.

6. Open `File` -> `Examples` -> `Pumbaa` -> `blink`.

7. Verify and upload the sketch to your device.

8. The built-in LED blinks!

9. Done!

|simba| Simba build system
^^^^^^^^^^^^^^^^^^^^^^^^^^

The `Pumbaa` development environment can be installed on `Linux
(Ubuntu 14)`.

1. Execute the one-liner below to install `Pumbaa`.

   .. code-block:: text

      $ mkdir pumbaa && \
        cd pumbaa && \
        sudo apt install ckermit valgrind cppcheck cloc python python-pip doxygen git lcov && \
        sudo apt install avrdude gcc-avr binutils-avr gdb-avr avr-libc && \
        sudo apt install bossa-cli gcc-arm-none-eabi && \
        sudo apt install make unrar autoconf automake libtool gcc g++ gperf \
                             flex bison texinfo gawk ncurses-dev libexpat-dev \
                             python-serial sed libtool-bin pmccabe help2man \
                             python-pyelftools unzip && \
        sudo pip install pyserial xpect readchar sphinx breathe sphinx_rtd_theme && \
        (git clone --recursive https://github.com/pfalcon/esp-open-sdk && \
         cd esp-open-sdk && \
         make) && \
        wget https://github.com/eerimoq/simba-releases/raw/master/arduino/esp32/tools/xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        tar xf xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        rm xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        git clone --recursive https://github.com/eerimoq/pumbaa

2. Setup the environment.

   .. code-block:: text

      $ cd pumbaa
      $ source setup.sh

2. Build and upload the blink example to your device.

   .. code-block:: text

      $ cd examples/blink
      $ make -s BOARD=nano32 SERIAL_PORT=<my-serial-port> upload

3. The built-in LED blinks!

4. Done!

.. _PlatformIO IDE: http://platformio.org/platformio-ide

.. _Arduino IDE 1.6.10: https://www.arduino.cc/en/Main/Software

.. |arduino| image:: images/Arduino_Logo.png
             :width: 40 px
.. _arduino: http://arduino.cc

.. |platformio| image:: images/platformio-logo.png
                :width: 40 px
.. _platformio: http://platformio.org

.. |simba| image:: images/Simba_Logo.jpg
                :width: 80 px
.. _simba: http://github.com/eerimoq/simba
