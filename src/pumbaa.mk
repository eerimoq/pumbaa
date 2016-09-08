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

INC += \
	$(PUMBAA_ROOT)/src \
	$(PUMBAA_ROOT)/src/boards/$(BOARD)

PUMBAA_SRC += \
	boards/$(BOARD)/class_board.c \
	boards/$(BOARD)/gccollect.c \
	builtin_help.c \
	builtin_input.c \
	junk.c \
	module_io.c \
	module_os.c \
	module_pumbaa.c \
	module_pumbaa/class_event.c \
	module_pumbaa/class_pin.c \
	module_pumbaa/class_timer.c \
	module_time.c

ifeq ($(BOARD),arduino_due)
PUMBAA_SRC += \
	boards/$(BOARD)/gchelper.S \
	lexer.c
endif

SRC += $(PUMBAA_SRC:%=$(PUMBAA_ROOT)/src/%)

include $(PUMBAA_ROOT)/src/micropython/micropython.mk
