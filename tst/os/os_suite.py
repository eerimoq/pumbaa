#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2016-2017, Erik Moqvist
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


import os
import harness
from harness import assert_raises

if 'Linux' in os.uname().machine:
    fs = 'fat16'
else:
    fs = 'spiffs'

def test_format():
    """Format the file system.

    """

    with assert_raises(OSError):
        os.format("apa")

    try:
        os.format("/fs")
    except:
        print("Failed to format /fs.")


def test_directory():
    assert os.getcwd() == '/fs'

    with assert_raises(NotImplementedError):
        os.chdir('dir')

    if fs == 'fat16':
        os.mkdir('dir')

    with assert_raises(NotImplementedError):
        os.rename('dir', 'dir2')

    with assert_raises(NotImplementedError):
        os.rmdir('dir2')


def test_missing_file():
    """Try to open a non-existing file.

    """

    with assert_raises(OSError):
        open("missing.txt", "r")


def test_create():
    """Create, write and read. Recreate, write and read.

    """

    # Create, write and read.
    with open("create.txt", "w") as fout:
        fout.write("test")

    with open("create.txt", "rt") as fin:
        assert fin.read() == "test"

    # Recreate the file, write and read.
    with open("create.txt", "w") as fout:
        fout.write("foo")

    with open("create.txt", "r") as fin:
        assert fin.read() == "foo"


def test_append():
    """Test the append functionality.

    """

    with open("append.txt", "w") as fout:
        fout.write("foo")

    with open("append.txt", "a") as fout:
        fout.write("bar")

    with open("append.txt", "r") as fin:
        assert fin.read() == "foobar"


def test_read_write():
    """Test the read + write mode.

    """

    with open("rw.txt", "w+") as f:
        f.write("foo")
        f.seek(0)
        assert f.read() == "foo"

    with open("rw.txt", "r") as fin:
        assert fin.read() == "foo"

    with open("rw.txt", "r") as fin:
        with assert_raises(OSError):
            fin.write('foo')

    with open("rw.txt", "w") as fout:
        with assert_raises(OSError):
            fout.read()


def test_seek_tell():
    """Seek in a file.

    """

    with open("seek.txt", "w") as fout:
        fout.write("12345678")
        assert fout.tell() == 8

    with open("seek.txt", "r") as fin:
        # Offset relative to the beginning of the file.
        assert fin.seek(1) == 1
        assert fin.tell() == 1
        assert fin.read(1) == "2"
        assert fin.seek(1, 0) == 1
        assert fin.read(1) == "2"
        # Offset relative to the current cursor position.
        assert fin.seek(1, 1) == 3
        assert fin.read(1) == "4"
        # Offset relative to the end of the file.
        assert fin.seek(-1, 2) == 7
        assert fin.read(1) == "8"
        assert fin.tell() == 8
        # Seek outside of the file.
        with assert_raises(OSError):
            fin.seek(-1)
        with assert_raises(OSError):
            fin.seek(1, 2)


def test_stat():
    """Test to stat files.

    """

    if fs == 'fat16':
        print(os.stat("."))

    with open("stat.txt", "w") as fout:
        fout.write("12345678")

    assert os.stat("stat.txt") == (0, 0, 0, 0, 0, 0, 8, 0, 0, 0)

    try:
        os.stat("stat2.txt")
    except OSError as e:
        print(e)
    else:
        assert False


def test_remove():
    if fs == 'spiffs':
        os.remove('create.txt')


def test_listdir():
    if fs == 'fat16':
        assert os.listdir() == ['DIR',
                                'CREATE.TXT',
                                'APPEND.TXT',
                                'RW.TXT',
                                'SEEK.TXT',
                                'STAT.TXT']

        assert os.listdir('.') == ['DIR',
                                   'CREATE.TXT',
                                   'APPEND.TXT',
                                   'RW.TXT',
                                   'SEEK.TXT',
                                   'STAT.TXT']

        try:
            os.listdir('non-existing')
        except OSError as e:
            assert str(e) == "No such file or directory: 'non-existing'"
        else:
            assert False
    else:
        assert os.listdir() == ['append.txt',
                                'rw.txt',
                                'seek.txt',
                                'stat.txt']

        assert os.listdir('.') == ['append.txt',
                                   'rw.txt',
                                   'seek.txt',
                                   'stat.txt']


def test_flush():
    """Flush a file.

    """

    with assert_raises(NotImplementedError, "file_obj_flush"):
        with open("flush.txt", "w") as fout:
            fout.write('')
            fout.flush()


def test_print():
    print(os)
    print(os.uname())

    with open("print.txt", "w") as fout:
        print(fout)
        fout.write("")
        print(fout)


def test_system():
    os.system('kernel/thrd/list')

    with assert_raises(OSError, "Command not found: '1/2/3'"):
        os.system('1/2/3')

    with assert_raises(OSError, "Command failed with -1"):
        os.system('')


TESTCASES = [
    (test_format, "test_format"),
    (test_directory, "test_directory"),
    (test_missing_file, "test_missing_file"),
    (test_create, "test_create"),
    (test_append, "test_append"),
    (test_read_write, "test_read_write"),
    (test_seek_tell, "test_seek_tell"),
    (test_stat, "test_stat"),
    (test_remove, "test_remove"),
    (test_listdir, "test_listdir"),
    (test_flush, "test_flush"),
    (test_print, "test_print"),
    (test_system, "test_system")
]
