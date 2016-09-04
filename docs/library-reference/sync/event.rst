:mod:`event` --- Event channel
==============================

.. module:: event
   :synopsis: Event channel.

----------------------------------------------

.. class:: event.Event(event)

   Initialize given event object.


   .. method:: read(mask)

      Wait for an event to occur and return a mask of all events.


   .. method:: write(mask)

      Write given events to the channel.


   .. method:: size()

      Get the number of events set in the channel.
