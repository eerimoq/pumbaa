`Pin` --- Digital pin
=====================

.. class:: pumbaa.Pin(device, mode)

   Initialize given pin object with given `device` and `mode`.

   
   .. method:: read()

      Read the current pin value.


   .. method:: write(value)

      Write given value `value` to the pin. `value` is either 0 or 1,
      where 0 is low and 1 is high.


   .. method:: toggle()

      Toggle the pin output value (high/low).


   .. method:: pin_set_mode(mode)

      Set the pin mode to given mode `mode`. The mode is one of
      `INPUT` and `OUTPUT`.
