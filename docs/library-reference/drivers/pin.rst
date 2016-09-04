:mod:`pin` --- Digital pins
===========================

.. module:: pin
   :synopsis: Digital pins.

----------------------------------------------

.. class:: pin.Pin(pin, mode)

   Initialize given pin object with given `device` and `mode`.

   
   .. method:: read()

      Read the current pin value.


   .. method:: write(value)

      Write the value `value` to the pin.


   .. method:: toggle()

      Toggle the pin output value (high/low).


   .. method:: pin_set_mode(mode)

      Set the pin mode to `mode`.
