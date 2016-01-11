TESTS = \
	tst/kernel/queue \
	tst/kernel/sem

test:
	for test in $(TESTS); do \
	    (cd $$test && python main.py); \
	done
