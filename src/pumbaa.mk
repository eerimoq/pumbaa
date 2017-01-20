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


INC += \
	. \
	$(PUMBAA_ROOT)/src \
	$(PUMBAA_ROOT)/src/port \
	$(PUMBAA_ROOT)/src/boards/$(BOARD)

PUMBAA_SRC += \
	boards/$(BOARD)/module_board.c \
	builtin_help.c \
	builtin_input.c \
	port/port.c \
	module_io.c \
	module_os.c \
	module_kernel.c \
	module_drivers.c \
	module_drivers/class_pin.c \
	module_drivers/class_uart.c \
	module_drivers/class_flash.c \
	module_kernel/class_timer.c \
	module_inet.c \
	module_inet/class_http_server.c \
	module_inet/class_http_server_websocket.c \
	module_select.c \
	module_socket.c \
	module_ssl.c \
	module_sync.c \
	module_sync/class_event.c \
	module_sync/class_queue.c \
	module_sys.c \
	module_text.c \
	module_time.c \
	module_thread.c \
	port/lexer_port.c

ifeq ($(BOARD),arduino_due)
PUMBAA_SRC += \
	module_drivers/class_adc.c \
	module_drivers/class_can.c \
	module_drivers/class_dac.c \
	module_drivers/class_ds18b20.c \
	module_drivers/class_exti.c \
	module_drivers/class_owi.c \
	module_drivers/class_spi.c \
	module_drivers/class_sd.c \
	boards/arduino_due/gccollect.c \
	boards/arduino_due/gchelper.S
endif

ifeq ($(BOARD),linux)
PUMBAA_SRC += \
	boards/linux/gccollect.c \
	module_drivers/class_adc.c \
	module_drivers/class_can.c \
	module_drivers/class_dac.c \
	module_drivers/class_ds18b20.c \
	module_drivers/class_exti.c \
	module_drivers/class_owi.c \
	module_drivers/class_spi.c \
	module_drivers/class_sd.c \
	module_drivers/class_i2c_soft.c
endif

ifeq ($(BOARD),$(filter $(BOARD), esp12e esp01 nodemcu))
PUMBAA_SRC += \
	mcus/esp8266/gccollect.c \
	mcus/esp8266/gchelper.S \
	module_drivers/class_exti.c \
	module_drivers/class_spi.c \
	module_drivers/class_esp_wifi.c
endif

ifeq ($(BOARD),$(filter $(BOARD), nano32))
PUMBAA_SRC += \
	mcus/esp32/gccollect.c \
	module_drivers/class_adc.c \
	module_drivers/class_can.c \
	module_drivers/class_dac.c \
	module_drivers/class_spi.c \
	module_drivers/class_ds18b20.c \
	module_drivers/class_esp_wifi.c \
	module_drivers/class_owi.c
endif

ifeq ($(BOARD),photon)
PUMBAA_SRC += \
	boards/photon/gccollect.c \
	boards/photon/gchelper.S
endif

SRC += $(PUMBAA_SRC:%=$(PUMBAA_ROOT)/src/%)

MICROPYTHON_ROOT = $(PUMBAA_ROOT)/micropython

INC += $(MICROPYTHON_ROOT)

MICROPYTHON_SRC += \
	extmod/modubinascii.c \
	extmod/moduhashlib.c \
	extmod/modujson.c \
	extmod/modurandom.c \
	extmod/moduzlib.c \
	lib/utils/pyhelp.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c \
	py/argcheck.c \
	py/asmarm.c \
	py/asmthumb.c \
	py/asmx64.c \
	py/asmx86.c \
	py/bc.c \
	py/binary.c \
	py/builtinevex.c \
	py/builtinimport.c \
	py/compile.c \
	py/emitbc.c \
	py/emitcommon.c \
	py/emitglue.c \
	py/emitinlinethumb.c \
	py/emitnative.c \
	py/formatfloat.c \
	py/frozenmod.c \
	py/gc.c \
	py/lexer.c \
	py/malloc.c \
	py/map.c \
	py/mkenv.mk \
	py/mkrules.mk \
	py/modarray.c \
	py/modbuiltins.c \
	py/modcmath.c \
	py/modcollections.c \
	py/modgc.c \
	py/modio.c \
	py/modmath.c \
	py/modmicropython.c \
	py/modstruct.c \
	py/modsys.c \
	py/modthread.c \
	py/moduerrno.c \
	py/mpprint.c \
	py/mpstate.c \
	py/mpz.c \
	py/nativeglue.c \
	py/nlrsetjmp.c \
	py/nlrthumb.c \
	py/nlrxtensa.S \
	py/objarray.c \
	py/objattrtuple.c \
	py/objbool.c \
	py/objboundmeth.c \
	py/obj.c \
	py/objcell.c \
	py/objclosure.c \
	py/objcomplex.c \
	py/objdict.c \
	py/objenumerate.c \
	py/objexcept.c \
	py/objfilter.c \
	py/objfloat.c \
	py/objfun.c \
	py/objgenerator.c \
	py/objgetitemiter.c \
	py/objint.c \
	py/objint_longlong.c \
	py/objint_mpz.c \
	py/objlist.c \
	py/objmap.c \
	py/objmodule.c \
	py/objnamedtuple.c \
	py/objnone.c \
	py/objobject.c \
	py/objpolyiter.c \
	py/objproperty.c \
	py/objrange.c \
	py/objreversed.c \
	py/objset.c \
	py/objsingleton.c \
	py/objslice.c \
	py/objstr.c \
	py/objstringio.c \
	py/objstrunicode.c \
	py/objtuple.c \
	py/objtype.c \
	py/objzip.c \
	py/opmethods.c \
	py/parse.c \
	py/parsenumbase.c \
	py/parsenum.c \
	py/qstr.c \
	py/reader.c \
	py/repl.c \
	py/runtime.c \
	py/runtime_utils.c \
	py/scope.c \
	py/sequence.c \
	py/showbc.c \
	py/smallint.c \
	py/stackctrl.c \
	py/stream.c \
	py/unicode.c \
	py/vm.c \
	py/vstr.c \
	py/warning.c

ifeq ($(BOARD),$(filter $(BOARD), esp12e esp01 nano32 nodemcu))
MICROPYTHON_SRC += \
	lib/libm/acoshf.c \
	lib/libm/asinfacosf.c \
	lib/libm/asinhf.c \
	lib/libm/atan2f.c \
	lib/libm/atanf.c \
	lib/libm/atanhf.c \
	lib/libm/ef_rem_pio2.c \
	lib/libm/ef_sqrt.c \
	lib/libm/erf_lgamma.c \
	lib/libm/fdlibm.h \
	lib/libm/fmodf.c \
	lib/libm/kf_cos.c \
	lib/libm/kf_rem_pio2.c \
	lib/libm/kf_sin.c \
	lib/libm/kf_tan.c \
	lib/libm/libm.h \
	lib/libm/log1pf.c \
	lib/libm/math.c \
	lib/libm/roundf.c \
	lib/libm/sf_cos.c \
	lib/libm/sf_erf.c \
	lib/libm/sf_frexp.c \
	lib/libm/sf_ldexp.c \
	lib/libm/sf_modf.c \
	lib/libm/sf_sin.c \
	lib/libm/sf_tan.c \
	lib/libm/wf_lgamma.c \
	lib/libm/wf_tgamma.c
endif

SRC += $(MICROPYTHON_SRC:%=$(MICROPYTHON_ROOT)/%)

-include $(PUMBAA_ROOT)/src/boards/$(BOARD)/board.mk
