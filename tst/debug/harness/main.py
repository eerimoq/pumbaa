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


import harness
from harness import TestCaseSkippedError, SuiteError
from harness import assert_raises, ExceptionNotRaisedError


def test_assert_raises():
    with assert_raises(RuntimeError, "expected error"):
        raise RuntimeError("expected error")

    try:
        with assert_raises(TypeError):
            raise ValueError("unexpected error")
    except ValueError as e:
        assert str(e) == "unexpected error"

    try:
        with assert_raises(TypeError):
            pass
    except ExceptionNotRaisedError as e:
        assert str(e) == "<class 'TypeError'> not raised"


def test_passed():
    pass


def test_skipped():
    raise TestCaseSkippedError()


def test_failed():
    raise Exception()


def main():
    testcases = [
        (test_assert_raises, "test_assert_raises"),
        (test_passed, "test_passed"),
        (test_skipped, "test_skipped"),
        (test_failed, "test_failed")
    ]

    try:
        harness.run(testcases)
        raise RuntimeError()
    except harness.SuiteError as e:
        if e.failed != 1 or e.skipped != 1:
            raise RuntimeError()


if __name__ == '__main__':
    main()
