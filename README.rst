About
=====

`Pumba` is `Python` on top of `Simba`.

The implementation is designed for embedded devices with limited
amount of RAM and code memory.

It does not have a runtime compiler, so all `Python` scripts are
compiled into bytecode on the host PC and then uploaded to the
embedded device.

Hardware requirements
=====================

- ? kB of RAM
- ? kB of Flash

Design
======

Everything in `Python` are objects; functions, classes, modules,
lists, integers, etc. It is therefore important to minimize the size
of the object datastructure.

Each `Pumba` thread is a `Simba` thread, and has its own set of
registers and stack.

The `Pumba` modules maps one-to-one to `Simba` modules, with just a
few exceptions.

Supported keywords
------------------

Credits
=======


License
=======

LGPL. See LICENSE.
