#
# @file pumbaa.mk
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

INC += $(PUMBAA_ROOT)/src

PUMBAA_SRC += \
	boards/$(BOARD)/modboard.c \
	boards/$(BOARD)/gccollect.c \
	builtinhelp.c \
	builtininput.c \
	junk.c \
	modevent.c \
	modfs.c \
	modio.c \
	modos.c \
	modpin.c \
	modtime.c \
	modtimer.c

ifeq ($(BOARD),arduino_due)
PUMBAA_SRC += \
	boards/$(BOARD)/gchelper.S \
	lexer.c
endif

SRC += $(PUMBAA_SRC:%=$(PUMBAA_ROOT)/src/%)

include $(PUMBAA_ROOT)/src/micropython/micropython.mk
