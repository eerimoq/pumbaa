/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016-2017, Erik Moqvist
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Pumbaa project.
 */

#include "pumbaa.h"

/**
 * Date named tuple fields.
 */
static const qstr date_fields[] = {
    MP_QSTR_tm_year,
    MP_QSTR_tm_mon,
    MP_QSTR_tm_mday,
    MP_QSTR_tm_hour,
    MP_QSTR_tm_min,
    MP_QSTR_tm_sec,
    MP_QSTR_tm_wday,
    MP_QSTR_tm_yday,
    MP_QSTR_tm_isdst
};

/**
 * def localtime([time])
 */
static mp_obj_t module_time_localtime(mp_uint_t n_args, const mp_obj_t *args_p)
{
    struct time_t time;
    struct date_t date;
    mp_obj_t tuple[9];

    if ((n_args == 0) || (args_p[0] == mp_const_none)) {
        time_get(&time);
    } else {
        time.seconds = mp_obj_get_int(args_p[0]);
    }

    /* Covert to date from time. */
    time_unix_time_to_date(&date, &time);

    /* Copy the date to output tuple. */
    tuple[0] = mp_obj_new_int(date.year);
    tuple[1] = mp_obj_new_int(date.month);
    tuple[2] = mp_obj_new_int(date.date);
    tuple[3] = mp_obj_new_int(date.hour);
    tuple[4] = mp_obj_new_int(date.minute);
    tuple[5] = mp_obj_new_int(date.second);
    tuple[6] = mp_obj_new_int(date.day - 1);
    tuple[7] = mp_obj_new_int(-1);
    tuple[8] = mp_obj_new_int(-1);

    return (mp_obj_new_attrtuple(&date_fields[0], 9, tuple));
}

static mp_obj_t module_time_time(void)
{
    struct time_t now;

    time_get(&now);

#if MICROPY_PY_BUILTINS_FLOAT
    return (mp_obj_new_float(now.seconds + now.nanoseconds / 1000000000.0));
#else
    return (mp_obj_new_int(now.seconds));
#endif
}

static mp_obj_t module_time_sleep(mp_obj_t arg_p)
{
#if MICROPY_PY_BUILTINS_FLOAT
    thrd_sleep(mp_obj_get_float(arg_p));
#else
    thrd_sleep(mp_obj_get_int(arg_p));
#endif
    return (mp_const_none);
}

static mp_obj_t module_time_sleep_ms(mp_obj_t arg)
{
    thrd_sleep_ms(mp_obj_get_int(arg));

    return (mp_const_none);
}

static mp_obj_t module_time_sleep_us(mp_obj_t arg)
{
    thrd_sleep_us(mp_obj_get_int(arg));

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(module_time_localtime_obj, 0, 1, module_time_localtime);
static MP_DEFINE_CONST_FUN_OBJ_0(module_time_time_obj, module_time_time);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_obj, module_time_sleep);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_ms_obj, module_time_sleep_ms);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_us_obj, module_time_sleep_us);

static const mp_rom_map_elem_t module_time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },
    { MP_ROM_QSTR(MP_QSTR_localtime), MP_ROM_PTR(&module_time_localtime_obj) },
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&module_time_time_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&module_time_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&module_time_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&module_time_sleep_us_obj) }
};

static MP_DEFINE_CONST_DICT(module_time_globals, module_time_globals_table);

const mp_obj_module_t mp_module_utime = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_time_globals,
};
