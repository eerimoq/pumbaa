#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2016, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
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
import socket
import struct
import sys
import time
import zlib

import gc
import micropython

import kernel
import sync
import drivers
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
        socket,
        struct,
        sys,
        time,
        zlib,
        gc,
        micropython,
        kernel,
        sync,
        drivers,
        board,
        sync.Event,
        drivers.Pin,
        kernel.Timer,
        other
    ]

    try:
        objs.append(drivers.Exti)
    except:
        pass

    try:
        objs.append(drivers.Dac)
    except:
        pass

    try:
        objs.append(drivers.Spi)
    except:
        pass

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
        os.rmdir('bar')

    assert other.foo() == True

    with assert_raises(OSError):
        os.system("bad")

    print(os.system("kernel/thrd/list"))

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

    if os.uname().machine != "Linux with Linux":
        print('Free memory before gc:', gc.mem_free())
        gc.collect()
        print('Free memory after gc:', gc.mem_free())

        print('Free memory before allocations:', gc.mem_free())
        for i in range(899):
            buf = bytearray(599+i)
        print('Free memory after allocations:', gc.mem_free())
        del buf
        gc.collect()
        print('Free memory after gc:', gc.mem_free())

    if hasattr(kernel, 'thrd_yield'):
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


TESTCASES = [
    (test_smoke, "test_smoke")
]
