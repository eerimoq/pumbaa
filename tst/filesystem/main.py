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

import harness


def test_create():
    """Create, write and read. Recreate, write and read.

    """

    # Create, write and read.
    with open("create.txt", "w") as fout:
        fout.write("test")

    with open("create.txt", "r") as fin:
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


def main():
    testcases = [
        (test_create, "test_create"),
        (test_append, "test_append"),
        (test_read_write, "test_read_write"),
        (test_seek_tell, "test_seek_tell")
    ]
    harness.run(testcases)


if __name__ == '__main__':
    main()
