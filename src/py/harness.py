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


import os
import io
import sys

class TestCaseSkippedError(Exception):
    pass


class SuiteError(Exception):

    def __init__(self, total, passed, skipped, failed):
        super().__init__(self)
        self.total = total
        self.passed = passed
        self.skipped = skipped
        self.failed = failed


class ExceptionNotRaisedError(Exception):
    pass


class AssertRaises(object):

    def __init__(self, expected_type, expected_message):
        self.expected_type = expected_type
        self.expected_message = expected_message

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, tb):
        if exc_type is None:
            raise ExceptionNotRaisedError("{} not raised".format(
                self.expected_type))
        elif exc_type == self.expected_type:
            if self.expected_message in [None, str(exc_value)]:
                return True


def assert_raises(expected_type, expected_message=None):
    return AssertRaises(expected_type, expected_message)


def run(testcases):
    """Run all test cases in the list.

    """

    # Print a header.
    print()
    print("================================== TEST BEGIN ==================================\n")
    print()

    passed = 0
    skipped = 0
    failed = 0
    total = len(testcases)

    for callback, name in testcases:
        print("enter:", name)

        try:
            callback()
            passed += 1
            print("exit: {}: PASSED\n".format(name))
        except TestCaseSkippedError:
            skipped += 1
            print("exit: {}: SKIPPED\n".format(name))
        except Exception as e:
            failed += 1
            sys.print_exception(e)
            print("exit: {}: FAILED\n".format(name))

    ok = passed + skipped == total

    print("harness report: total({}), passed({}), failed({}), skipped({})\n".format(
        total, passed, failed, skipped))

    print("=============================== TEST END ({}) ==============================\n".format(
        "PASSED" if ok else "FAILED"))

    print(os.system("kernel/thrd/list"))
    
    if not ok:
        raise SuiteError(total, passed, skipped, failed)
