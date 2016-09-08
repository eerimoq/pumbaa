`Pin` --- Digital pin
=====================

.. class:: pumbaa.Pin(device, mode)

   Initialize given pin object with given `device` and `mode`. The
   device is selected among the pins available in the `Board`
   class. Mode must be either `INPUT` or `OUTPUT`.

   
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
      `INPUT` or `OUTPUT`.
