`Event` --- Event channel
=========================

.. class:: pumbaa.Event(event)

   Initialize given event object.


   .. method:: read(mask=0xffffffff)

      Wait for an event to occur and return a mask of all active
      events.


   .. method:: write(mask=0xffffffff)

      Write given event(s) to the channel.


   .. method:: size()

      Get the number of event(s) set in the channel.
