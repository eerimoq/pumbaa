#
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2016-2017, Erik Moqvist
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

NAME = all_suite
BOARD ?= linux

INC += \
	$(PUMBAA_ROOT)/tst/stubs

ifeq ($(BOARD), linux)
SRC_SD = $(PUMBAA_ROOT)/tst/stubs/sd_stub.c
SRC_IGNORE_SD = $(SIMBA_ROOT)/src/drivers/sd.c
SRC_FLASH = $(PUMBAA_ROOT)/tst/stubs/flash_stub.c
SRC_IGNORE_FLASH = $(SIMBA_ROOT)/src/drivers/flash.c
endif

SRC += \
	$(PUMBAA_ROOT)/tst/stubs/can_stub.c \
	$(PUMBAA_ROOT)/tst/stubs/ds18b20_stub.c \
	$(SRC_FLASH) \
	$(SRC_SD) \
	$(PUMBAA_ROOT)/tst/stubs/owi_stub.c \
	$(PUMBAA_ROOT)/tst/stubs/socket_stub.c \
	$(PUMBAA_ROOT)/tst/stubs/ssl_stub.c

SRC_IGNORE += \
	$(SIMBA_ROOT)/src/drivers/can.c \
	$(SIMBA_ROOT)/src/drivers/ds18b20.c \
	$(SRC_IGNORE_FLASH) \
	$(SRC_IGNORE_SD) \
	$(SIMBA_ROOT)/src/drivers/owi.c \
	$(SIMBA_ROOT)/src/inet/socket.c \
	$(SIMBA_ROOT)/src/inet/ssl.c

PYSRC += \
	$(PUMBAA_ROOT)/src/py/harness.py \
	$(PUMBAA_ROOT)/tst/smoke/smoke_suite.py \
	$(PUMBAA_ROOT)/tst/smoke/other.py \
	$(PUMBAA_ROOT)/tst/kernel/timer/timer_suite.py \
	$(PUMBAA_ROOT)/tst/os/os_suite.py \
	$(PUMBAA_ROOT)/tst/sync/event/event_suite.py \
	$(PUMBAA_ROOT)/tst/sync/queue/queue_suite.py \
	$(PUMBAA_ROOT)/tst/select/select_suite.py \
	$(PUMBAA_ROOT)/tst/thread/thread_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/pin/pin_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/adc/adc_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/sd/sd_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/owi/owi_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/exti/exti_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/flash/flash_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/dac/dac_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/can/can_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/ds18b20/ds18b20_suite.py \
	$(PUMBAA_ROOT)/tst/drivers/spi/spi_suite.py \
	$(PUMBAA_ROOT)/tst/inet/ssl/ssl_suite.py \
	$(PUMBAA_ROOT)/tst/inet/http_server/http_server_suite.py \
	$(PUMBAA_ROOT)/tst/socket/socket_suite.py

RUN_END_PATTERN ?= "=============================== ALL END \(\w+\) ==============================\r\n\r\n"
RUN_END_PATTERN_SUCCESS ?= "=============================== ALL END \(PASSED\) ==============================\r\n\r\n"

PUMBAA_ROOT ?= ../..
include $(PUMBAA_ROOT)/make/app.mk
