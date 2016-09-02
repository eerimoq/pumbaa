/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpconfig.h"

#if MICROPY_PY_UTIME == 1

#include "simba.h"

#include "py/runtime.h"
#include "py/smallint.h"
#include "py/mphal.h"

static mp_obj_t mod_time_time(void)
{
    return mp_obj_new_int((mp_int_t)0);
}

static mp_obj_t mod_time_ticks_us(void)
{
    return MP_OBJ_NEW_SMALL_INT(0);
}

static mp_obj_t mod_time_ticks_ms(void)
{
    return MP_OBJ_NEW_SMALL_INT(0);
}

static mp_obj_t mod_time_ticks_diff(mp_obj_t oldval, mp_obj_t newval)
{
    mp_uint_t old = MP_OBJ_SMALL_INT_VALUE(oldval);
    mp_uint_t new = MP_OBJ_SMALL_INT_VALUE(newval);

    return MP_OBJ_NEW_SMALL_INT((new - old) & MP_SMALL_INT_POSITIVE_MASK);
}

/* Note: this is deprecated since CPy3.3, but pystone still uses
   it. */
static mp_obj_t mod_time_clock(void)
{
    return mp_obj_new_int(0);
}

static mp_obj_t mod_time_sleep(mp_obj_t arg_p)
{
#if MICROPY_PY_BUILTINS_FLOAT
    thrd_sleep(mp_obj_get_float(arg_p));
#else
    thrd_sleep(mp_obj_get_int(arg_p));
#endif
    return mp_const_none;
}

static mp_obj_t mod_time_sleep_ms(mp_obj_t arg)
{
    thrd_sleep_ms(mp_obj_get_int(arg));
    
    return mp_const_none;
}

static mp_obj_t mod_time_sleep_us(mp_obj_t arg)
{
    thrd_sleep_us(mp_obj_get_int(arg));

    return mp_const_none;
}

static mp_obj_t mod_time_strftime(size_t n_args, const mp_obj_t *args)
{
    return mp_obj_new_str("1", 1, false);
}

static MP_DEFINE_CONST_FUN_OBJ_0(mod_time_time_obj, mod_time_time);
static MP_DEFINE_CONST_FUN_OBJ_0(mod_time_ticks_us_obj, mod_time_ticks_us);
static MP_DEFINE_CONST_FUN_OBJ_0(mod_time_ticks_ms_obj, mod_time_ticks_ms);
static MP_DEFINE_CONST_FUN_OBJ_2(mod_time_ticks_diff_obj, mod_time_ticks_diff);
static MP_DEFINE_CONST_FUN_OBJ_0(mod_time_clock_obj, mod_time_clock);
static MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_obj, mod_time_sleep);
static MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_ms_obj, mod_time_sleep_ms);
static MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_us_obj, mod_time_sleep_us);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_time_strftime_obj, 1, 2, mod_time_strftime);

static const mp_rom_map_elem_t mp_module_time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },
    { MP_ROM_QSTR(MP_QSTR_clock), MP_ROM_PTR(&mod_time_clock_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mod_time_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mod_time_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mod_time_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&mod_time_time_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mod_time_ticks_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mod_time_ticks_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mod_time_ticks_diff_obj) },
    { MP_ROM_QSTR(MP_QSTR_strftime), MP_ROM_PTR(&mod_time_strftime_obj) },
};

static MP_DEFINE_CONST_DICT(mp_module_time_globals,
                            mp_module_time_globals_table);

const mp_obj_module_t mp_module_time = {
    .base = { &mp_type_module },
    .name = MP_QSTR_utime,
    .globals = (mp_obj_dict_t*)&mp_module_time_globals,
};

#endif // MICROPY_PY_UTIME
