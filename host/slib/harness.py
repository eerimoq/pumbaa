class Harness():
    
    def __init__(self, testcases):
        self.testcases = testcases

    def run(self):
        for testcase in self.testcases:
            print testcase.__name__
            testcase(self)
            print testcase.__name__, "PASSED"
