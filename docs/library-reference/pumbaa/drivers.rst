:mod:`drivers` --- Device drivers
=================================

.. module:: drivers
   :synopsis: Device drivers.

This module contains device drivers.

The following classes are defined:

- :class:`class Pin<.Pin>` -- Digital pins
- :class:`class Exti<.Exti>` -- External interrupts
- :class:`class Adc<.Adc>` -- Analog to digital convertion
- :class:`class Dac<.Dac>` -- Digital to analog convertion
- :class:`class Spi<.Spi>` -- Serial peripheral interface
- :class:`class Can<.Can>` -- Controller Area Network
- :class:`class I2CSoft<.I2CSoft>` -- Software I2C
- :class:`class Owi<.Owi>` -- Onewire
- :class:`class Ds18b20<.Ds18b20>` -- DS18B20 temperature
- :class:`class Sd<.Sd>` -- Secure Digital memory
- :class:`class esp_wifi<.esp_wifi>` -- Espressif WiFi

Simba documentation: `drivers`_

----------------------------------------------


.. class:: drivers.Pin(device, mode)

   Create a pin object with given `device` and `mode`. The `device` is
   selected among the pins available in the `board` module. `mode`
   must be either :data:`.INPUT` or :data:`.OUTPUT`.

   .. code-block:: python

      >>> led = Pin(board.PIN_LED, Pin.OUTPUT)
      >>> led.write(1)
      >>> led.toggle()

   Simba documentation: `drivers/pin`_


   .. method:: read()

      Read the current pin value and return it as an integer. Returns
      0 if the pin is low and 1 if the pin is high.


   .. method:: write(value)

      Write the logic level `value` to the pin. `value` must be an
      object that can be converted to an integer. The value is either
      0 or 1, where 0 is low and 1 is high.


   .. method:: toggle()

      Toggle the pin output value (high/low).


   .. method:: set_mode(mode)

      Set the pin mode to given mode `mode`. The mode must be either
      :data:`.INPUT` or :data:`.OUTPUT`.


   .. data:: INPUT

      Input pin mode.


   .. data:: OUTPUT

      Output pin mode.


.. class:: drivers.Exti(device, trigger, channel=None, data=None, callback=None)

   Create an object handling interrupts on given `device`. `trigger`
   may be a combination of :data:`.RISING`, :data:`.FALLING` or
   :data:`.BOTH`. When an interrupt occurs given `callback` is called
   from interrupt context and `data` is written to given event or
   queue channel `channel`.

   Event channel example.

   .. code-block:: python

      >>> event = Event()
      >>> exti = Exti(board.EXTI_D3, Exti.FALLING, event, 0x1)
      >>> exti.start()
      >>> event.read(0x1)        # Wait for an interrupt to occur.
      >>> exti.stop()

   Queue channel example.

   .. code-block:: python

      >>> queue = Queue()
      >>> exti = Exti(board.EXTI_D4, Exti.RISING, queue, b'1')
      >>> exti.start()
      >>> queue.read(1)        # Wait for an interrupt to occur.
      b'1'
      >>> exti.stop()

   Simba documentation: `drivers/exti`_


   .. method:: start()

      Start the interrupt handler.


   .. method:: stop()

      Stop the interrupt handler.


   .. data:: RISING

      Trigger an interrupt on rising edges.


   .. data:: FALLING

      Trigger an interrupt on falling edges.


   .. data:: BOTH

      Trigger an interrupt on both rising and falling edges.


.. class:: drivers.Adc(device, pin_device, reference, sampling_rate)

   Instansiate an Adc object with given `device` and `pin_device`
   devices. `reference` is the voltage reference and `sampling_rate`
   is the sampling frequency.

   Here is an example of how to create a ADC driver object and convert
   an analog signal level to three digital samples with a sampling
   rate of 1 kHz.

   .. code-block:: python

      >>> a0 = Adc(board.PIN_ADC0, board.PIN_A0, Adc.REFERENCE_VCC, 1000)
      >>> a0.convert(3)
      b'\x00\x01\x00\x02\x00\x03'
      >>> array.array('h', a0.convert(3))
      array('h', [1, 2, 3])

   The equivalent asynchronous example.

   .. code-block:: python

      >>> a0 = Adc(board.PIN_ADC0, board.PIN_A0, Adc.REFERENCE_VCC, 1000)
      >>> a0.async_convert(3)
      >>> array.array('h', a0.async_wait())
      array('h', [1, 2, 3])

   Simba documentation: `drivers/adc`_


   .. method:: convert(number_of_samples)

      Start a synchronous convertion of an analog signal to digital
      samples. This is equivalent to :meth:`.async_convert()` +
      :meth:`.async_wait()`, but in a single function call. Returns a
      bytes object where each sample is 2 bytes.


   .. method:: async_convert(number_of_samples)

      Start an asynchronous convertion of analog signal to digital
      samples. Call :meth:`.async_wait()` to wait for the convertion
      to complete.


   .. method:: async_wait()

      Wait for an asynchronous convertion started with
      :meth:`.async_convert()` to complete. Returns a bytes object
      where each sample is 2 bytes.


   .. data:: REFERENCE_VCC

      Use VCC as reference.


.. class:: drivers.Dac(devices, sampling_rate)

   Instansiate a Dac object. `devices` is either a list of DAC pin
   devices or a single DAC pin device. The DAC pin devices can be
   found in the :doc:`board<board>` module, often named ``PIN_DAC0``
   and ``PIN_DAC1``.

   Here is an example of how to create a DAC driver and convert
   digital samples to an analog signal.

   .. code-block:: python

      >>> dac = Dac(board.PIN_DAC0)
      >>> dac.convert(b'\x01\x02\x03\x04')

   Simba documentation: `drivers/dac`_


   .. method:: convert(samples)

      Start a synchronous convertion of digital samples to an analog
      signal. This function returns when all samples have been
      converted.


   .. method:: async_convert(samples)

      Start an asynchronous convertion of digital samples to an analog
      signal. This function only blocks if the hardware is not ready
      to convert more samples. Call :meth:`.async_wait` to wait for an
      asynchronous convertion to finish.


   .. method:: async_wait()

      Wait for an ongoing asynchronous convertion to finish.


.. class:: drivers.Spi(device, slave_select, mode=MODE_MASTER, speed=SPEED_250KBPS, polarity=0, phase=0)

   Create a Spi object. Select the SPI device with `device` and slave
   select pin with `slave_select`. `mode` in one of
   :data:`.MODE_MASTER` and :data:`.MODE_SLAVE`. `speed` is only used
   by the master. `polarity` is the bus idle logic level. `phase`
   controls if sampling are done on falling or rising clock edges.

   Here is an example of how to create a SPI driver and write 4 bytes
   to the slave.

   .. code-block:: python

      >>> spi = Spi(board.SPI_0, board.PIN_D3)
      >>> spi.start()
      >>> spi.select()
      >>> spi.write(b'\x01\x02\x03\x04')
      >>> spi.deselect()
      >>> spi.stop()

   Simba documentation: `drivers/spi`_


   .. method:: start()

      Configures the SPI hardware with the settings of this object.


   .. method:: stop()

      Deconfigures the SPI hardware if given driver currently ownes
      the bus.


   .. method:: take_bus()

      In multi master application the driver must take ownership of
      the SPI bus before performing data transfers. Will re-configure
      the SPI hardware if configured by another driver.


   .. method:: give_bus()

      In multi master application the driver must give ownership of
      the SPI bus to let other masters take it.


   .. method:: select()

      Select the slave by asserting the slave select pin.


   .. method:: deselect()

      Deselect the slave by de-asserting the slave select pin.


   .. method:: transfer(write_buffer[, size])

      Simultaniuos read/write operation over the SPI bus. Writes data
      from `write_buffer` to the bus. The `size` argument can be used to
      transfer fewer bytes than the size of `write_buffer`. Returns
      the read data as a bytes object.

      The number of read and written bytes are always equal for a
      transfer.


   .. method:: transfer_into(read_buffer, write_buffer[, size])

      Same as :meth:`.transfer`, but the read data is written to
      `read_buffer`.


   .. method:: read(size)

      Read `size` bytes from the SPI bus. Returns the read data as a
      bytes object.


   .. method:: read_into(buffer[, size])

      Same as :meth:`.read`, but the read data is written to `buffer`.


   .. method:: write(buffer[, size])

      Write `size` bytes from `buffer` to the SPI bus. Writes all data
      in `buffer` is `size` is not given.


   .. data:: MODE_MASTER

      SPI master mode.


   .. data:: MODE_SLAVE

      SPI slave mode.


   .. data:: SPEED_8MBPS
   .. data:: SPEED_4MBPS
   .. data:: SPEED_2MBPS
   .. data:: SPEED_1MBPS
   .. data:: SPEED_500KBPS
   .. data:: SPEED_250KBPS
   .. data:: SPEED_125KBPS

      SPI bus speed. Only used if the driver is configured as master.


.. class:: drivers.Can(device, speed=SPEED_500KBPS)

   Create a Can object. Select CAN device and speed with `device` and
   `speed`.

   Here is an example of how to create a CAN driver, write a frame and
   then read a frame.

   .. code-block:: python

      >>> can = Can(board.CAN_0)
      >>> can.start()
      >>> can.write(0x123, b'\x01\x02')
      >>> can.read()
      (id=0x32, data=b'\x34\x35\x36', flags=0)
      >>> can.stop()

   Simba documentation: `drivers/can`_


   .. method:: start()

      Starts the CAN device.


   .. method:: stop()

      Stops the CAN device.


   .. method:: read()

      Read a frame from the CAN bus and return it as a named tuple
      with three items; `id`, `data` and `flags`. `id` is the frame id
      as an integer. `flags` contains information about the frame
      format, and possibly additional information in the
      future. `data` is a bytes object of up to 8 bytes of read frame
      data.


   .. method:: write(id, data[, flags])

      Write a frame with given `id` and `data` to the CAN bus. `id` is
      an integer and `data` is a bytes object of up to 8 bytes. Set
      :data:`.FLAGS_EXTENDED_FRAME` in `flags` to write an extended
      frame (29 bits frame id), otherwise a standard frame is written.


   .. data:: SPEED_500KBPS

      CAN bus speed.


   .. data:: FLAGS_EXTENDED_FRAME

      Extended frame flag. A 29 bits frame id will be sent/received.


.. class:: drivers.I2CSoft(scl, sda, baudrate=50000, max_clock_stretching_sleep_us=1000000, clock_stretching_sleep_us=10000)

   Create a I2CSoft object.

   Here is an example of how to create a SD and read the CID.

   .. code-block:: python

      >>> i2c = I2CSoft(board.PIN_D3, board.PIN_D4)
      >>> i2c.start()
      >>> i2c.scan()
      [87, 104]
      >>> i2c.stop()

   Simba documentation: `drivers/i2c_soft`_


   .. method:: start()

      Start the i2c soft driver.


   .. method:: stop()

      Stop the i2c soft driver.


   .. method:: read(address, size)

      Read `size` bytes from slave with address `address`.


   .. method:: read_into(address, buffer[, size])

      Read ``len(buffer)`` bytes from slave with address `address`
      into `buffer`. Give the argument `size` to read fewer bytes than
      ``len(buffer)``.


   .. method:: write(address, buffer[, size])

      Write the buffer `buffer` to slave with address `address`.


   .. method:: scan()

      Scan the bus and return a list of all found slave addresses.


.. class:: drivers.Owi(pin_device)

   Create an Owi object with `pin_device` as the one wire bus pin.

   Here is an example of how to use the Owi class.

   .. code-block:: python

      >>> owi = Owi(board.PIN_D3)
      >>> owi.reset()
      >>> owi.search()
      2
      >>> owi.get_devices()
      [b'12345678', b'abcdefgh']
      >>> owi.read(b'12345678', 3)
      b'\x00\x01\x02'
      >>> owi.write(b'12345678', b'\x00')
      1

   Simba documentation: `drivers/owi`_


   .. method:: reset()

      Send reset on one wire bus.


   .. method:: search()

      Search for devices on the one wire bus. The device id of all
      found devices are stored and returned by :meth:`.get_devices`.


   .. method:: get_devices()

      Returns a list of all devices found in the latest call to
      :meth:`.search`.


   .. method:: read(device_id, size)

      Read `size` bytes from device with id `device_id`.


   .. method:: write(device_id, buffer[, size])

      Write buffer `buffer` to device with id `device_id`. Give `size`
      to write fewer bytes than the buffer size.


.. class:: drivers.Ds18b20(owi)

   Create a Ds18b20 object.

   Here is an example of how to use the Ds18b20 class.

   .. code-block:: python

      >>> owi = Owi(board.PIN_D3)
      >>> owi.search()
      >>> ds18b20 = Ds18b20(owi)
      >>> ds18b20.get_devices()
      [b'(2345678']
      >>> ds18b20.convert()
      >>> ds18b20.get_temperature(b'(2345678')
      20.5

   Simba documentation: `drivers/ds18b20`_


   .. method:: convert()

      Start temperature convertion on all sensors.


   .. method:: get_devices()

      Returns a list of all DS18B20 devices found in the latest call
      to :meth:`.Owi.search`.


   .. method:: get_temperature(device_id)

      Get the temperature for given device identity. Reads the latest
      converted sample in the device with id `device_id`. Call
      :meth:`.convert` before calling this function to get the current
      temperature.


.. class:: drivers.Sd(spi)

   Create a Sd object with given SPI driver.

   Here is an example of how to create a SD and read the CID.

   .. code-block:: python

      >>> sd = Sd(spi)
      >>> sd.start()
      >>> print(sd.read_cid())
      (mid=2, oid=b'TM', pnm=b'SA04G', prv=22, psn=-681299654, mdt=60416, crc=107)
      >>> sd.stop()

   Simba documentation: `drivers/sd`_


   .. method:: start()

      Configures the SD card driver. This resets the SD card and
      performs the initialization sequence.


   .. method:: stop()

      Deconfigures the SD card driver.


   .. method:: read_cid()

      Read card CID register and return it. The CID contains card
      identification information such as Manufacturer ID, Product
      name, Product serial number and Manufacturing date.

      The return value is an object with 7 attributes:

      - mid - manufacturer ID
      - oid - OEM/Application ID
      - pnm - Product name
      - prv - Product revision
      - psn - Product serial number
      - mdt - Manufacturing date
      - crc - CRC7 checksum


   .. method:: read_csd()

      Read card CSD register and return it. The CSD contains that
      provides information regarding access to the card's contents.

      The return value is an object with 29 attributes for version 1
      cards and 24 attributes for version 2 cards:

      - ...


   .. method:: read_block(block)

      Read given block from SD card and returns it as a bytes object.


   .. method:: read_block_into(block, buffer)

      Same as :meth:`.read_block`, but the read data is written to
      `buffer`.


   .. method:: write_block(block, buffer)

      Write `buffer` to given block.


.. class:: drivers.esp_wifi()

   This class is a singleton and can not be instanciated. It
   configures the Espressif WiFi stack.

   An example of how to connect to a WiFi network:

   .. code-block:: python

      >>> esp_wifi.set_op_mode(esp_wifi.OP_MODE_STATION)
      >>> esp_wifi.station_init('ssid', 'password')
      >>> esp_wifi.station_get_status()
      'got-ip'
      >>> esp_wifi.station_get_ip_info()
      (address='192.168.0.5', netmask='255.255.255.0', gateway='192.168.0.1')

   An example of how to setup a SoftAP:

   .. code-block:: python

      >>> esp_wifi.set_op_mode(esp_wifi.OP_MODE_SOFTAP)
      >>> esp_wifi.softap_init('ssid', 'password')
      >>> esp_wifi.softap_get_ip_info()
      (address='192.168.4.1', netmask='255.255.255.0', gateway='192.168.4.1')

   Simba documentation: `drivers/esp_wifi`_


   .. method:: set_op_mode(mode)

      Set the WiFi operating mode to `mode`. `mode` is one of
      :data:`.OP_MODE_STATION`, :data:`.OP_MODE_SOFTAP`,
      :data:`.OP_MODE_STATION_SOFTAP`.


   .. method:: get_op_mode()

      Returns the current WiFi operating mode.


   .. method:: set_phy_mode(mode)

      Set the WiFi physical mode (802.11b/g/n) to one of
      :data:`.PHY_MODE_11B`, :data:`.PHY_MODE_11G` and
      :data:`.PHY_MODE_11N`.


   .. method:: get_phy_mode()

      Returns the physical mode (802.11b/g/n).


   .. method:: softap_init(ssid, password)

      Initialize the WiFi SoftAP interface with given `ssid` and
      `password`.


   .. method:: softap_set_ip_info(info)

      Set the ip address, netmask and gateway of the WiFi SoftAP. The
      info object `info` is a three items tuple of address, netmask
      and gateway strings in IPv4 format.


   .. method:: softap_get_ip_info()

      Returns a three items tuple of the SoftAP ip address, netmask
      and gateway.


   .. method:: softap_get_number_of_connected_stations()

      Returns the number of stations connected to the SoftAP.


   .. method:: softap_get_station_info()

      Returns the information of stations connected to the SoftAP,
      including MAC and IP addresses.


   .. method:: softap_dhcp_server_start()

      Enable the SoftAP DHCP server.


   .. method:: softap_dhcp_server_stop()

      Disable the SoftAP DHCP server. The DHCP server is enabled by
      default.


   .. method:: softap_dhcp_server_status()

      Returns the SoftAP DHCP server status.


   .. method:: station_init(ssid, password[, info])

      Initialize the WiFi station.


   .. method:: station_connect()

      Connect the WiFi station to the Access Point (AP). This function
      returns before a connection has been established. Call
      `station_get_status()` periodically until it retuns ``got-ip``
      to ensure the WiFi station has been assigned an IP the the WiFi
      Access Point DHCP server.


   .. method:: station_disconnect()

      Disconnect the WiFi station from the AP.


   .. method:: station_set_ip_info(info)

      Set the ip address, netmask and gateway of the WiFi station. The
      info object `info` is a three items tuple of address, netmask
      and gateway strings in IPv4 format.


   .. method:: station_get_ip_info()

      Returns the station ip address, netmask and gateway.


   .. method:: station_set_reconnect_policy(policy)

      Set whether the station will reconnect to the AP after
      disconnection. Set `policy` to ``True`` to automatically
      reconnect and ``False`` otherwise.


   .. method:: station_get_reconnect_policy()

      Check whether the station will reconnect to the AP after
      disconnection.


   .. method:: station_get_status()

      Get the connection status of the WiFi station.


   .. method:: station_dhcp_client_start()

      Enable the station DHCP client.


   .. method:: station_dhcp_client_stop()

      Disable the station DHCP client.


   .. method:: station_dhcp_client_status()

      Get the station DHCP client status.


   .. data:: OP_MODE_NULL
   .. data:: OP_MODE_STATION
   .. data:: OP_MODE_SOFTAP
   .. data:: OP_MODE_STATION_SOFTAP

      WiFi operating modes.


   .. data:: PHY_MODE_11B
   .. data:: PHY_MODE_11G
   .. data:: PHY_MODE_11N

      WiFi physical modes.


.. _drivers: http://simba-os.readthedocs.io/en/latest/library-reference/drivers.html
.. _drivers/pin: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/pin.html
.. _drivers/exti: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/exti.html
.. _drivers/adc: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/adc.html
.. _drivers/dac: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/dac.html
.. _drivers/spi: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/spi.html
.. _drivers/can: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/can.html
.. _drivers/sd: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/sd.html
.. _drivers/i2c_soft: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/i2c_soft.html
.. _drivers/owi: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/owi.html
.. _drivers/ds18b20: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/ds18b20.html
.. _drivers/esp_wifi: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/esp_wifi.html
