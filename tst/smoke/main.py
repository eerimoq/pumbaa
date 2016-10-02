#
# @file main.py
#
# @section License
# Copyright (C) 2016, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Pumbaa project.
#

import array
import binascii
import cmath
import collections
import hashlib
import io
import json
import math
import os
import random
import struct
import sys
import time
import zlib

import gc
import micropython

import kernel
import sync
import drivers
import filesystems
import board

import harness
import other
from harness import assert_raises


def test_smoke():
    """Various tests.

    """

    help()

    objs = [
        array,
        binascii,
        cmath,
        collections,
        hashlib,
        io,
        json,
        math,
        os,
        random,
        struct,
        sys,
        time,
        zlib,
        gc,
        micropython,
        kernel,
        sync,
        drivers,
        filesystems,
        board,
        sync.Event,
        drivers.Pin,
        drivers.Exti,
        drivers.Dac,
        drivers.Spi,
        kernel.Timer,
        other
    ]

    for obj in objs:
        print()
        help(obj)

    try:
        import foo
    except:
        pass

    print("dir:", dir())

    print("sys.platform:", sys.platform)
    print("os.uname:", os.uname())
    print("time.time:", time.time())
    print("time.localtime():", time.localtime())
    print("time.localtime():", time.localtime(1475271052))
    print('time.localtime().tm_year:', time.localtime().tm_year)

    time.sleep(0.1)
    time.sleep_ms(1)
    time.sleep_us(1)

    try:
        print('CWD:', os.getcwd())
    except OSError as e:
        print(e)

    try:
        os.mkdir('foo')
    except Exception as e:
        print(e)

    with assert_raises(NotImplementedError):
        os.chdir('foo')

    with assert_raises(NotImplementedError):
        os.chdir('..')

    with assert_raises(NotImplementedError):
        os.rename('foo', 'bar')

    with assert_raises(NotImplementedError):
        os.remove('bar')

    with assert_raises(NotImplementedError):
        os.rmdir('bar')

    assert other.foo() == True

    with assert_raises(OSError):
        filesystems.fs_call("bad")

    print(filesystems.fs_call("kernel/thrd/list"))

    sio = io.StringIO("foo")
    sio.seek(0, 2)
    print("bar", file=sio)
    sio.seek(0)
    assert sio.read().strip() == "foobar"

    print(cmath.phase(complex(-1.0, 0.0)))
    z = complex(-1.0, 0.0)
    assert z == z.real + z.imag * 1j
    print(cmath.cos(math.pi))

    ordered_dict = collections.OrderedDict([(1,"a")])
    print(ordered_dict.popitem())

    m = hashlib.sha256()
    m.update(b"Nobody inspects")
    m.update(b" the spammish repetition")
    print(m.digest())

    kernel.sys_lock()
    kernel.sys_unlock()

    kernel.thrd_yield()
    with assert_raises(NotImplementedError):
        kernel.thrd_join(None)
    thrd = kernel.thrd_self()
    with assert_raises(NotImplementedError):
        kernel.thrd_set_name('foo')
    print('thrd_get_name(): ', kernel.thrd_get_name())
    assert kernel.thrd_get_by_name('main') == thrd
    with assert_raises(OSError):
        kernel.thrd_get_by_name('foo')
    kernel.thrd_set_log_mask(thrd, 0xff)
    assert kernel.thrd_get_log_mask() == 0xff
    prio = kernel.thrd_get_prio()
    kernel.thrd_set_prio(thrd, prio + 1)
    assert kernel.thrd_get_prio() == prio + 1
    with assert_raises(NotImplementedError):
        kernel.thrd_set_global_env('foo', 'bar')
    print('thrd_get_global_env(CWD): ', kernel.thrd_get_global_env('CWD'))
    with assert_raises(NotImplementedError):
        kernel.thrd_set_env('foo', 'bar')
    print('thrd_get_env(CWD): ', kernel.thrd_get_env('CWD'))

    spi = drivers.Spi(board.SPI_0, board.PIN_D3, drivers.Spi.MODE_MASTER, 1, 1, 1)
    spi.start()
    spi.take_bus()
    spi.select()
    print(spi.transfer(b'foo'))
    print(spi.transfer(b'foo', 1))
    buf = bytearray(3)
    print(spi.transfer_into(buf, b'bar'))
    print(spi.transfer_into(buf, b'bar', 2))
    print(spi.read(3))
    buf = bytearray(3)
    print(spi.read_into(buf))
    print(spi.read_into(buf, 1))
    spi.write(b'fie')
    spi.write(b'fie', 2)
    spi.deselect()
    spi.give_bus()
    spi.stop()


def main():
    testcases = [
        (test_smoke, "test_smoke")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
