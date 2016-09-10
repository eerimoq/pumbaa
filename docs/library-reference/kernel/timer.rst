`Timer` --- Timer
=================

.. class:: pumbaa.Timer(timeout, event, mask, flags=0)

   Instantiate a timer object from given arguemts. The timer expires
   `timeout` seconds after the timer has been started. When the timer
   expires given `mask` is written to given `event` channel.


   .. method:: start()
    
      Start the timer.
    

   .. method:: stop()
    
      Stop the timer. If the timer is stopped before it has expired it
      will never exipre. This function has no effect if the timer has
      already expired.
    
   .. data:: PERIODIC

      Pass this flag to ``pumbaa.Timer`` for periodic timers.
