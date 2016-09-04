#
# @file Makefile
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

.PHONY: tags docs all help travis clean

TESTS = \
	tst/smoke \
	tst/kernel/timer

all:
	$(MAKE) -C examples all
	$(MAKE) -C tst/smoke all
	$(MAKE) -C tst/drivers/pin all
	$(MAKE) -C tst/kernel/timer all

clean:
	$(MAKE) -C examples clean
	$(MAKE) -C tst/smoke clean
	$(MAKE) -C tst/drivers/pin clean
	$(MAKE) -C tst/kernel/timer clean

test:
	$(MAKE) -C tst/smoke run
	$(MAKE) -C tst/kernel/timer run

travis:
	$(MAKE) all
	$(MAKE) test

codecov-coverage: $(TESTS:%=%.ccc)

$(TESTS:%=%.ccc):
	$(MAKE) -C $(basename $@) codecov-coverage

docs:
	+bin/docgen.py
	$(MAKE) -C docs sphinx

tags:
	echo "Creating tags file .TAGS"
	etags -o .TAGS --declarations $$(git ls-files *.[hci] | xargs)

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo
