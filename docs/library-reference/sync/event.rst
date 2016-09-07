`Event` --- Event channel
=========================

.. class:: pumbaa.Event(event)

   Initialize given event object.


   .. method:: read(mask=0xffffffff)

      Wait for an event to occur and return a mask of all events.


   .. method:: write(mask)

      Write given events to the channel.


   .. method:: size()

      Get the number of events set in the channel.
