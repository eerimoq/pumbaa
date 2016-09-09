`Dac` --- Digital to analog convertion
======================================

.. class:: pumbaa.Dac(devices, sampling_rate)

   Initialize given DAC object with given list of pin devices
   and sampling rate.


   .. method:: convert(samples)

      Start a synchronous convertion of digital samples to an analog
      signal.


   .. method:: async_convert(samples)

      Start an asynchronous convertion of digital samples to an analog
      signal. Blocks until the samples has been queued in the
      hardware.


   .. method:: async_wait()

      Wait for an ongoing asynchronous convertion to finish.
