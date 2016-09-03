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
	argcheck.c \
	asmarm.c \
	asmthumb.c \
	asmx64.c \
	asmx86.c \
	bc.c \
	binary.c \
	builtinevex.c \
	builtinimport.c \
	compile.c \
	emitbc.c \
	emitcommon.c \
	emitglue.c \
	emitinlinethumb.c \
	emitnative.c \
	formatfloat.c \
	frozenmod.c \
	gc.c \
	lexer.c \
	lexerstr.c \
	lexerunix.c \
	malloc.c \
	map.c \
	mkenv.mk \
	mkrules.mk \
	modarray.c \
	modbuiltins.c \
	modcmath.c \
	modcollections.c \
	modgc.c \
	modio.c \
	modmath.c \
	modmicropython.c \
	modstruct.c \
	modsys.c \
	modthread.c \
	moduerrno.c \
	mpprint.c \
	mpstate.c \
	mpz.c \
	nativeglue.c \
	nlrsetjmp.c \
	nlrthumb.c \
	nlrx64.S \
	nlrx86.S \
	nlrxtensa.S \
	objarray.c \
	objattrtuple.c \
	objbool.c \
	objboundmeth.c \
	obj.c \
	objcell.c \
	objclosure.c \
	objcomplex.c \
	objdict.c \
	objenumerate.c \
	objexcept.c \
	objfilter.c \
	objfloat.c \
	objfun.c \
	objgenerator.c \
	objgetitemiter.c \
	objint.c \
	objint_longlong.c \
	objint_mpz.c \
	objlist.c \
	objmap.c \
	objmodule.c \
	objnamedtuple.c \
	objnone.c \
	objobject.c \
	objpolyiter.c \
	objproperty.c \
	objrange.c \
	objreversed.c \
	objset.c \
	objsingleton.c \
	objslice.c \
	objstr.c \
	objstringio.c \
	objstrunicode.c \
	objtuple.c \
	objtype.c \
	objzip.c \
	opmethods.c \
	parse.c \
	parsenumbase.c \
	parsenum.c \
	qstr.c \
	repl.c \
	runtime.c \
	runtime_utils.c \
	scope.c \
	sequence.c \
	showbc.c \
	smallint.c \
	stackctrl.c \
	stream.c \
	unicode.c \
	vm.c \
	vstr.c \
	warning.c

SRC += $(MICROPYTHON_SRC:%=$(MICROPYTHON_ROOT)/py/%)
