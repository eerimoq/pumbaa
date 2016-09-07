/**
 * @file pumbaa_config_default.h
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

#ifndef __PUMBAA_CONFIG_DEFAULT_H__
#define __PUMBAA_CONFIG_DEFAULT_H__

/**
 * This is the default configuration of Pumbaa.
 */

#ifndef PATH_MAX
#    define PATH_MAX 128
#endif

#ifndef MICROPY_ALLOC_PATH_MAX
#    define MICROPY_ALLOC_PATH_MAX      (PATH_MAX)
#endif

#ifndef MICROPY_ENABLE_GC
#    define MICROPY_ENABLE_GC           (1)
#endif

#ifndef MICROPY_ENABLE_FINALISER
#    define MICROPY_ENABLE_FINALISER    (0)
#endif

#ifndef MICROPY_STACK_CHECK
#    define MICROPY_STACK_CHECK         (0)
#endif

#ifndef MICROPY_COMP_CONST
#    define MICROPY_COMP_CONST          (0)
#endif

#ifndef MICROPY_MEM_STATS
#    define MICROPY_MEM_STATS           (1)
#endif

#ifndef MICROPY_DEBUG_PRINTERS
#    define MICROPY_DEBUG_PRINTERS      (1)
#endif

#ifndef MICROPY_HELPER_REPL
#    define MICROPY_HELPER_REPL         (1)
#endif

#ifndef MICROPY_HELPER_LEXER_UNIX
#    if defined(ARCH_LINUX)
#        define MICROPY_HELPER_LEXER_UNIX   (1)
#    else
#        define MICROPY_HELPER_LEXER_UNIX   (0)
#    endif
#endif

#ifndef MICROPY_ENABLE_SOURCE_LINE
#    define MICROPY_ENABLE_SOURCE_LINE  (1)
#endif

#ifndef MICROPY_ERROR_REPORTING
#    define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_NORMAL)
#endif

#ifndef MICROPY_WARNINGS
#    define MICROPY_WARNINGS            (0)
#endif

#ifndef MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF
#    define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF   (0)
#endif

#ifndef MICROPY_FLOAT_IMPL
#    define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)
#endif

#ifndef MICROPY_LONGINT_IMPL
#    define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_NONE)
#endif

#ifndef MICROPY_STREAMS_NON_BLOCK
#    define MICROPY_STREAMS_NON_BLOCK   (0)
#endif

#ifndef MICROPY_OPT_COMPUTED_GOTO
#    define MICROPY_OPT_COMPUTED_GOTO   (0)
#endif

#ifndef MICROPY_OPT_CACHE_MAP_LOOKUP_IN_BYTECODE
#    define MICROPY_OPT_CACHE_MAP_LOOKUP_IN_BYTECODE (0)
#endif

#ifndef MICROPY_CAN_OVERRIDE_BUILTINS
#    define MICROPY_CAN_OVERRIDE_BUILTINS (0)
#endif

#ifndef MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG
#    define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (0)
#endif

#ifndef MICROPY_CPYTHON_COMPAT
#    define MICROPY_CPYTHON_COMPAT      (0)
#endif

#ifndef MICROPY_PY_BUILTINS_FLOAT
#    define MICROPY_PY_BUILTINS_FLOAT (1)
#endif

#ifndef MICROPY_PY_BUILTINS_COMPLEX
#    define MICROPY_PY_BUILTINS_COMPLEX (0)
#endif

#ifndef MICROPY_PY_BUILTINS_BYTEARRAY
#    define MICROPY_PY_BUILTINS_BYTEARRAY (1)
#endif

#ifndef MICROPY_PY_BUILTINS_MEMORYVIEW
#    define MICROPY_PY_BUILTINS_MEMORYVIEW (0)
#endif

#ifndef MICROPY_PY_BUILTINS_COMPILE
#    define MICROPY_PY_BUILTINS_COMPILE (0)
#endif

#ifndef MICROPY_PY_BUILTINS_ENUMERATE
#    define MICROPY_PY_BUILTINS_ENUMERATE (1)
#endif

#ifndef MICROPY_PY_BUILTINS_FILTER
#    define MICROPY_PY_BUILTINS_FILTER  (0)
#endif

#ifndef MICROPY_PY_BUILTINS_FROZENSET
#    define MICROPY_PY_BUILTINS_FROZENSET (1)
#endif

#ifndef MICROPY_PY_BUILTINS_REVERSED
#    define MICROPY_PY_BUILTINS_REVERSED (0)
#endif

#ifndef MICROPY_PY_BUILTINS_SET
#    define MICROPY_PY_BUILTINS_SET     (1)
#endif

#ifndef MICROPY_PY_BUILTINS_SLICE
#    define MICROPY_PY_BUILTINS_SLICE   (1)
#endif

#ifndef MICROPY_PY_BUILTINS_STR_UNICODE
#    define MICROPY_PY_BUILTINS_STR_UNICODE (0)
#endif

#ifndef MICROPY_PY_BUILTINS_PROPERTY
#    define MICROPY_PY_BUILTINS_PROPERTY (0)
#endif

#ifndef MICROPY_PY_BUILTINS_MIN_MAX
#    define MICROPY_PY_BUILTINS_MIN_MAX (1)
#endif

#ifndef MICROPY_PY___FILE__
#    define MICROPY_PY___FILE__         (0)
#endif

#ifndef MICROPY_PY_MICROPYTHON_MEM_INFO
#    define MICROPY_PY_MICROPYTHON_MEM_INFO (1)
#endif

#ifndef MICROPY_MALLOC_USES_ALLOCATED_SIZE
#    define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
#endif

#ifndef MICROPY_PY_GC
#    define MICROPY_PY_GC               (1)
#endif

#ifndef MICROPY_PY_GC_COLLECT_RETVAL
#    define MICROPY_PY_GC_COLLECT_RETVAL (0)
#endif

#ifndef MICROPY_PY_ARRAY
#    define MICROPY_PY_ARRAY            (1)
#endif

#ifndef MICROPY_PY_COLLECTIONS
#    define MICROPY_PY_COLLECTIONS      (1)
#endif

#ifndef MICROPY_PY_MATH
#    define MICROPY_PY_MATH             (1)
#endif

#ifndef MICROPY_PY_CMATH
#    define MICROPY_PY_CMATH            (0)
#endif

#ifndef MICROPY_PY_IO
#    define MICROPY_PY_IO               (0)
#endif

#ifndef MICROPY_PY_IO_FILEIO
#    define MICROPY_PY_IO_FILEIO        (1)
#endif

#ifndef MICROPY_PY_STRUCT
#    define MICROPY_PY_STRUCT           (1)
#endif

#ifndef MICROPY_PY_SYS
#    define MICROPY_PY_SYS              (1)
#endif

#ifndef MICROPY_PY_SYS_EXIT
#    define MICROPY_PY_SYS_EXIT         (0)
#endif

#ifndef MICROPY_PY_SYS_PLATFORM
#    define MICROPY_PY_SYS_PLATFORM     "Pumbaa"
#endif

#ifndef MICROPY_PY_SYS_MAXSIZE
#    define MICROPY_PY_SYS_MAXSIZE      (0)
#endif

#ifndef MICROPY_PY_SYS_STDFILES
#    define MICROPY_PY_SYS_STDFILES     (0)
#endif

#ifndef MICROPY_PY_CMATH
#    define MICROPY_PY_CMATH            (0)
#endif

#ifndef MICROPY_PY_UCTYPES
#    define MICROPY_PY_UCTYPES          (0)
#endif

#ifndef MICROPY_PY_UZLIB
#    define MICROPY_PY_UZLIB            (0)
#endif

#ifndef MICROPY_PY_UJSON
#    define MICROPY_PY_UJSON            (1)
#endif

#ifndef MICROPY_PY_URE
#    define MICROPY_PY_URE              (1)
#endif

#ifndef MICROPY_PY_UHEAPQ
#    define MICROPY_PY_UHEAPQ           (0)
#endif

#ifndef MICROPY_PY_UHASHLIB
#    define MICROPY_PY_UHASHLIB         (0)
#endif

#ifndef MICROPY_PY_UBINASCII
#    define MICROPY_PY_UBINASCII        (1)
#endif

#ifndef MICROPY_PY_UTIME
#    define MICROPY_PY_UTIME            (1)
#endif

#ifndef MICROPY_MODULE_FROZEN_STR
#    define MICROPY_MODULE_FROZEN_STR   (1)
#endif

#ifndef MICROPY_MODULE_BUILTIN_INIT
#    define MICROPY_MODULE_BUILTIN_INIT (1)
#endif

#ifndef CONFIG_MAIN_FRIENDLY_REPL
#    define CONFIG_MAIN_FRIENDLY_REPL    (1)
#endif

extern const struct _mp_obj_module_t mp_module_os;
extern const struct _mp_obj_module_t mp_module_time;
extern const struct _mp_obj_module_t module_pumbaa;

#define MICROPY_PORT_BUILTIN_MODULES                            \
    { MP_ROM_QSTR(MP_QSTR_os), MP_ROM_PTR(&mp_module_os) },     \
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&mp_module_time) }, \
    { MP_ROM_QSTR(MP_QSTR_pumbaa), MP_ROM_PTR(&module_pumbaa) },

/* Extra built in names to add to the global namespace. */
#define MICROPY_PORT_BUILTINS                                           \
    { MP_ROM_QSTR(MP_QSTR_help), MP_ROM_PTR(&mp_builtin_help_obj) },    \
    { MP_ROM_QSTR(MP_QSTR_input), MP_ROM_PTR(&mp_builtin_input_obj) },  \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },

#define MICROPY_PORT_ROOT_POINTERS \
    mp_obj_t keyboard_interrupt_obj; \
    const char *readline_hist[8];

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

#define MP_PLAT_PRINT_STRN(str, len) mp_hal_stdout_tx_strn_cooked(str, len)

#define MP_STATE_PORT MP_STATE_VM

#endif
