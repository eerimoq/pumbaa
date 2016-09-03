/**
 * @file libpumbaa.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Pumbaa project.
 */

// options to control how Micro Python is built

#undef PATH_MAX
#define PATH_MAX 128

#define MICROPY_ALLOC_PATH_MAX      (PATH_MAX)
#define MICROPY_ENABLE_GC           (1)
#define MICROPY_ENABLE_FINALISER    (0)
#define MICROPY_STACK_CHECK         (0)
#define MICROPY_COMP_CONST          (0)
#define MICROPY_MEM_STATS           (0)
#define MICROPY_DEBUG_PRINTERS      (0)
#define MICROPY_HELPER_REPL         (1)
#define MICROPY_HELPER_LEXER_UNIX   (1)
#define MICROPY_ENABLE_SOURCE_LINE  (0)
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_WARNINGS            (0)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF   (0)
#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_NONE)
#define MICROPY_STREAMS_NON_BLOCK   (0)
#define MICROPY_OPT_COMPUTED_GOTO   (0)
#define MICROPY_OPT_CACHE_MAP_LOOKUP_IN_BYTECODE (0)
#define MICROPY_CAN_OVERRIDE_BUILTINS (0)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (0)
#define MICROPY_CPYTHON_COMPAT      (0)
#define MICROPY_PY_BUILTINS_FLOAT (1)
#define MICROPY_PY_BUILTINS_COMPLEX (0)
#define MICROPY_PY_BUILTINS_BYTEARRAY (0)
#define MICROPY_PY_BUILTINS_MEMORYVIEW (0)
#define MICROPY_PY_BUILTINS_COMPILE (0)
#define MICROPY_PY_BUILTINS_ENUMERATE (0)
#define MICROPY_PY_BUILTINS_FILTER  (0)
#define MICROPY_PY_BUILTINS_FROZENSET (0)
#define MICROPY_PY_BUILTINS_REVERSED (0)
#define MICROPY_PY_BUILTINS_SET     (0)
#define MICROPY_PY_BUILTINS_SLICE   (0)
#define MICROPY_PY_BUILTINS_STR_UNICODE (0)
#define MICROPY_PY_BUILTINS_PROPERTY (0)
#define MICROPY_PY_BUILTINS_MIN_MAX (0)
#define MICROPY_PY___FILE__         (0)
#define MICROPY_PY_MICROPYTHON_MEM_INFO (0)
#define MICROPY_PY_GC               (0)
#define MICROPY_PY_GC_COLLECT_RETVAL (0)
#define MICROPY_PY_ARRAY            (0)
#define MICROPY_PY_COLLECTIONS      (0)
#define MICROPY_PY_MATH             (0)
#define MICROPY_PY_CMATH            (0)
#define MICROPY_PY_IO               (0)
#define MICROPY_PY_IO_FILEIO        (0)
#define MICROPY_PY_STRUCT           (0)
#define MICROPY_PY_SYS              (1)
#define MICROPY_PY_SYS_EXIT         (0)
#define MICROPY_PY_SYS_PLATFORM     "Pumbaa"
#define MICROPY_PY_SYS_MAXSIZE      (0)
#define MICROPY_PY_SYS_STDFILES     (0)
#define MICROPY_PY_CMATH            (0)
#define MICROPY_PY_UCTYPES          (0)
#define MICROPY_PY_UZLIB            (0)
#define MICROPY_PY_UJSON            (0)
#define MICROPY_PY_URE              (0)
#define MICROPY_PY_UHEAPQ           (0)
#define MICROPY_PY_UHASHLIB         (0)
#define MICROPY_PY_UBINASCII        (0)
#define MICROPY_PY_UTIME            (1)

extern const struct _mp_obj_module_t mp_module_time;
extern const struct _mp_obj_module_t module_board;
extern const struct _mp_obj_module_t module_pin;
extern const struct _mp_obj_module_t module_fs;

#define MICROPY_PORT_BUILTIN_MODULES            \
    { MP_ROM_QSTR(MP_QSTR_utime), MP_ROM_PTR(&mp_module_time) }, \
    { MP_ROM_QSTR(MP_QSTR_board), MP_ROM_PTR(&module_board) }, \
    { MP_ROM_QSTR(MP_QSTR_pin), MP_ROM_PTR(&module_pin) }, \
    { MP_ROM_QSTR(MP_QSTR_fs), MP_ROM_PTR(&module_fs) },

#define MICROPY_PORT_ROOT_POINTERS \
    mp_obj_t keyboard_interrupt_obj;

//////////////////////////////////////////
// Do not change anything beyond this line
//////////////////////////////////////////

// Define to 1 to use undertested inefficient GC helper implementation
// (if more efficient arch-specific one is not available).
#ifndef MICROPY_GCREGS_SETJMP
    #ifdef __mips__
        #define MICROPY_GCREGS_SETJMP (1)
    #else
        #define MICROPY_GCREGS_SETJMP (0)
    #endif
#endif

// type definitions for the specific machine

#ifdef __LP64__
typedef long mp_int_t; // must be pointer size
typedef unsigned long mp_uint_t; // must be pointer size
#else
// These are definitions for machines where sizeof(int) == sizeof(void*),
// regardless for actual size.
typedef int mp_int_t; // must be pointer size
typedef unsigned int mp_uint_t; // must be pointer size
#endif

#define BYTES_PER_WORD sizeof(mp_int_t)

// Cannot include <sys/types.h>, as it may lead to symbol name clashes
#if _FILE_OFFSET_BITS == 64 && !defined(__LP64__)
typedef long long mp_off_t;
#else
typedef long mp_off_t;
#endif

#define mp_hal_pin_obj_t mp_obj_t

// We need to provide a declaration/definition of alloca()
#include <alloca.h>
