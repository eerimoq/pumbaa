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
import harness

if 'Linux' in os.uname().machine:
    SUITES = [
        "os_suite",
        "select_suite",
        "socket_suite",
        "ssl_suite",
        "thread_suite",
        "adc_suite",
        "can_suite",
        "dac_suite",
        "ds18b20_suite",
        "exti_suite",
        "pin_suite",
        "owi_suite",
        "spi_suite",
        "sd_suite",
        "timer_suite",
        "event_suite",
        "queue_suite",
        "http_server_suite"
    ]

elif 'ESP32' in os.uname().machine:
    SUITES = [
        "thread_suite",
        "pin_suite",
        "timer_suite",
        "ssl_suite",
        "event_suite",
        "queue_suite",
        "os_suite"
    ]

else:
    raise Exception("Unsupported board.")


def main():
    passed = 0
    skipped = 0
    failed = 0
    total = 0

    print()
    print("================================== ALL BEGIN ==================================\n")

    for suite in SUITES:
        print(suite + ":")
        module = __import__(suite, None, None)
        try:
            result = harness.run(module.TESTCASES)
            total += result[0]
            passed += result[1]
            skipped += result[2]
            failed += result[3]
        except harness.SuiteError as e:
            total += e.total
            passed += e.passed
            skipped += e.skipped
            failed += e.failed
        del module

    ok = passed + skipped == total

    print("report: total({}), passed({}), failed({}), skipped({})\n".format(
        total, passed, failed, skipped))

    print("=============================== ALL END ({}) ==============================\n".format(
        "PASSED" if ok else "FAILED"))

    if not ok:
        raise Exception()


if __name__ == '__main__':
    main()
