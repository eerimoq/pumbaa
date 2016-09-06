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

BOARD ?= linux

TESTS = \
	tst/smoke \
	tst/kernel/timer \
	tst/sync/event

all: $(TESTS:%=%.all)
	$(MAKE) -C examples all

clean: $(TESTS:%=%.clean)
	$(MAKE) -C examples clean

rerun:
	for test in $(TESTS) ; do \
	    $(MAKE) -C $$test run || exit 1 ; \
	done

# Depend on 'all' to build all applications (optinally with -j) before
# uploading and running them one at a time.
run: all
	for test in $(TESTS) ; do \
	    if [ ! -e $$test/.$(BOARD).passed ] ; then \
	        $(MAKE) -C $$test run || exit 1 ; \
	        touch $$test/.$(BOARD).passed ; \
	    else \
	        echo ; \
	        echo "$$test already passed." ; \
	        echo ; \
	    fi \
	done

report:
	for test in $(TESTS) ; do \
	    $(MAKE) -C $(basename $$test) report ; \
	    echo ; \
	done

test: run
	$(MAKE) report

travis:
	$(MAKE) test

codecov-coverage: $(TESTS:%=%.ccc)

$(TESTS:%=%.all):
	$(MAKE) -C $(basename $@) all

$(TESTS:%=%.clean):
	$(MAKE) -C $(basename $@) clean

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
