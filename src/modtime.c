/**
 * @file modtime.c
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

#include "pumbaa.h"

#if MICROPY_PY_UTIME == 1

static mp_obj_t module_time_time(void)
{
    return (mp_obj_new_int((mp_int_t)0));
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

static MP_DEFINE_CONST_FUN_OBJ_0(module_time_time_obj, module_time_time);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_obj, module_time_sleep);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_ms_obj, module_time_sleep_ms);
static MP_DEFINE_CONST_FUN_OBJ_1(module_time_sleep_us_obj, module_time_sleep_us);

static const mp_rom_map_elem_t module_time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&module_time_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&module_time_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&module_time_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&module_time_time_obj) },
};

static MP_DEFINE_CONST_DICT(module_time_globals, module_time_globals_table);

const mp_obj_module_t mp_module_time = {
    .base = { &mp_type_module },
    .name = MP_QSTR_time,
    .globals = (mp_obj_dict_t*)&module_time_globals,
};

#endif // MICROPY_PY_UTIME
