`Dac` --- Digital to analog convertion
======================================

.. class:: pumbaa.Dac(devices, sampling_rate)

   Instansiate a Dac object. `devices` is either a list of DAC pin
   devices or a single DAC pin device. The DAC pin devices can be
   found in the `Board` class, often named `PIN_DAC0` and `PIN_DAC1`.


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
