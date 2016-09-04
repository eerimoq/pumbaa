#
# @file micropython.mk
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

MICROPYTHON_ROOT = $(PUMBAA_ROOT)/src/micropython

INC += $(MICROPYTHON_ROOT)

MICROPYTHON_SRC += \
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
	py/lexerstr.c \
	py/lexerunix.c \
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

SRC += $(MICROPYTHON_SRC:%=$(MICROPYTHON_ROOT)/%)
