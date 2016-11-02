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


SIMBA_ROOT ?= $(PUMBAA_ROOT)/simba

MAIN_PY ?= main.py
PYSRC += $(MAIN_PY)

MAIN_C ?= $(PUMBAA_ROOT)/src/main.c
FROZEN_C = $(BUILDDIR)/frozen.c

SRC += $(FROZEN_C)

include $(PUMBAA_ROOT)/src/pumbaa.mk
include $(SIMBA_ROOT)/make/app.mk

SED ?= sed
PYTHON ?= python

# Micropython string generation.
GENHDR_DIR = $(BUILDDIR)/genhdr
QSTR_DIR = $(GENHDR_DIR)/qstr
QSTR_I_LAST = $(GENHDR_DIR)/qstr.i.last
QSTR_SPLIT = $(GENHDR_DIR)/qstr.split
QSTR_DEFS_GENERATED_H = $(GENHDR_DIR)/qstrdefs.generated.h
QSTR_DEFS_COLLECTED_H = $(GENHDR_DIR)/qstrdefs.collected.h
QSTR_DEFS_PREPROCESSED_H = $(GENHDR_DIR)/qstrdefs.preprocessed.h
QSTR_DEFS_PY_CORE_H = $(MICROPYTHON_ROOT)/py/qstrdefs.h

MAKEQSTRDATA_PY = $(MICROPYTHON_ROOT)/py/makeqstrdata.py
MAKEQSTRDEFS_PY = $(MICROPYTHON_ROOT)/py/makeqstrdefs.py
MAKE_FROZEN_PY = $(PUMBAA_ROOT)/bin/make_frozen.py

ifneq ($(ARCH),esp)
LIB += m
CDEFS += \
	MICROPY_EMIT_X86=0 \
	MICROPY_NLR_SETJMP
endif

CDEFS_QSTR = $(CDEFS) NO_QSTR

$(QSTR_I_LAST): $(CSRC)
	echo "Generating $@"
	mkdir -p $(GENHDR_DIR)
	$(CC) $(INC:%=-I%) $(CDEFS_QSTR:%=-D%) $(CFLAGS) -E $? > $@

$(QSTR_DEFS_COLLECTED_H): $(QSTR_I_LAST)
	echo "Generating $@"
	$(PYTHON) $(MAKEQSTRDEFS_PY) split $^ $(QSTR_DIR) $@
	$(PYTHON) $(MAKEQSTRDEFS_PY) cat $(QSTR_I_LAST) $(QSTR_DIR) $@

$(QSTR_DEFS_GENERATED_H): $(QSTR_DEFS_PY_CORE_H) $(QSTR_DEFS_COLLECTED_H)
	echo "Generating $@"
	cat $^ | $(SED) 's/^Q(.*)/"&"/' | $(CC) $(INC:%=-I%) $(CDEFS:%=-D%) $(CFLAGS) -E - | sed 's/^"\(Q(.*)\)"/\1/' > $(QSTR_DEFS_PREPROCESSED_H)
	$(PYTHON) $(MAKEQSTRDATA_PY) $(QSTR_DEFS_PREPROCESSED_H) > $@

$(FROZEN_C): $(PYSRC)
	echo "Generating $@"
	mkdir -p $(BUILDDIR)
	$(PYTHON) $(MAKE_FROZEN_PY) $^ > $@

generate: $(QSTR_DEFS_GENERATED_H) $(FROZEN_C)

pumbaa-default-configuration:
	gcc -E -dM $(CDEFS:%=-D%) $(PUMBAA_ROOT)/src/pumbaa_config_default.h \
		| grep "#define CONFIG"_ \
		| grep -v "#define __CONFIG"
