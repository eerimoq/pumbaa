class TestCaseSkippedError(Exception):
    pass


class SuiteError(Exception):

    def __init__(self, total, passed, skipped, failed):
        super().__init__(self)
        self.total = total
        self.passed = passed
        self.skipped = skipped
        self.failed = failed


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
            print(type(e), e)
            print("exit: {}: FAILED\n".format(name))

    ok = passed + skipped == total
            
    print("harness report: total({}), passed({}), failed({}), skipped({})\n".format(
        total, passed, failed, skipped))

    print("=============================== TEST END ({}) ==============================\n".format(
        "PASSED" if ok else "FAILED"))

    if not ok:
        raise SuiteError(total, passed, skipped, failed)
