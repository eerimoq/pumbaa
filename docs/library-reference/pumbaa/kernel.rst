:mod:`kernel` --- Kernel
========================

.. module:: kernel
   :synopsis: Kernel.

The kernel package is the heart in `Simba`. It implements the thread
scheduler.

Simba documentation: `kernel`_

----------------------------------------------
              
.. function:: kernel.sys_lock()

   Take the system lock. Turns off interrupts.


.. function:: kernel.sys_unlock()

   Release the system lock. Turns on interrupts.


.. class:: kernel.Timer(timeout, event=None, mask=0x1, callback=None, flags=0)

   Instantiate a timer object from given arguemts. The timer expires
   `timeout` seconds after the timer has been started. When the timer
   expires given `callback` is called from interrupt context and
   `mask` is written to given `event` channel.

   Simba documentation: `kernel/timer`_

   .. method:: start()

      Start the timer.

   .. method:: stop()

      Stop the timer. If the timer is stopped before it has expired it
      will never exipre. This function has no effect if the timer has
      already expired.

   .. data:: PERIODIC

      Pass this flag to ``kernel.Timer`` for periodic timers.


.. _kernel: http://simba-os.readthedocs.io/en/latest/library-reference/kernel.html
.. _kernel/timer: http://simba-os.readthedocs.io/en/latest/library-reference/kernel/timer.html

