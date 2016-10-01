:mod:`drivers` --- Device drivers
=================================

.. module:: drivers
   :synopsis: Device drivers.

Device drivers.

Simba documentation: `drivers`_

----------------------------------------------

.. class:: drivers.Pin(device, mode)

   Initialize given pin object with given `device` and `mode`. The
   device is selected among the pins available in the `Board`
   class. Mode must be either `INPUT` or `OUTPUT`.

   Simba documentation: `drivers/pin`_

   .. method:: read()

      Read the current pin value and return it as an integer. Returns
      0 if the pin is low and 1 if the pin is high.

   .. method:: write(value)

      Write `value` to the pin. `value` must be an object that can be
      converted to an integer. The value is either 0 or 1, where 0 is
      low and 1 is high.

   .. method:: toggle()

      Toggle the pin output value (high/low).

   .. method:: set_mode(mode)

      Set the pin mode to given mode `mode`. The mode must be either
      ``INPUT`` or ``OUTPUT``.

   .. data:: INPUT

      Input pin mode.

   .. data:: OUTPUT

      Output pin mode.


.. class:: drivers.Exti(device, trigger, event=None, mask=0x1, callback=None)

   Instantiate an object handling interrupts on given
   `device`. `trigger` may be a combination of ``RISING``, ``FALLING``
   or ``BOTH``. When an interrupt occurs given `callback` is called
   from interrupt context and `mask` is written to given event channel
   `event`.

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


.. class:: drivers.Dac(devices, sampling_rate)

   Instansiate a Dac object. `devices` is either a list of DAC pin
   devices or a single DAC pin device. The DAC pin devices can be
   found in the `Board` class, often named `PIN_DAC0` and `PIN_DAC1`.

   Simba documentation: `drivers/dac`_

   .. method:: convert(samples)

      Start a synchronous convertion of digital samples to an analog
      signal. This function returns when all samples have been
      converted.

   .. method:: async_convert(samples)

      Start an asynchronous convertion of digital samples to an analog
      signal. This function only blocks if the hardware is not ready
      to convert more samples. Call `async_wait()` to wait for an
      asynchronous convertion to finish.

   .. method:: async_wait()

      Wait for an ongoing asynchronous convertion to finish.


.. _drivers: http://simba-os.readthedocs.io/en/latest/library-reference/drivers.html
.. _drivers/pin: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/pin.html
.. _drivers/exti: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/exti.html
.. _drivers/dac: http://simba-os.readthedocs.io/en/latest/library-reference/drivers/dac.html
