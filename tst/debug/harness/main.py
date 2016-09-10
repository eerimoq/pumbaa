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
from harness import TestCaseSkippedError, SuiteError


def test_passed():
    pass


def test_skipped():
    raise TestCaseSkippedError()


def test_failed():
    raise Exception()


def main():
    testcases = [
        (test_passed, "test_passed"),
        (test_skipped, "test_skipped"),
        (test_failed, "test_failed")
    ]

    try:
        harness.run(testcases)
        raise RuntimeError()
    except harness.SuiteError as e:
        print((e.total, e.passed, e.skipped, e.failed))
        pass


if __name__ == '__main__':
    main()
