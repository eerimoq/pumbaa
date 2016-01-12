TESTS = \
	tst/kernel/queue \
	tst/kernel/sem \
	tst/drivers/can

test:
	for test in $(TESTS); do \
	    (cd $$test && python main.py); \
	done
