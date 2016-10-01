/**
 * @file module_kernel.c
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

#if CONFIG_PUMBAA_SYS_LOCK == 1

/**
 * def sys_lock()
 */
static mp_obj_t module_sys_lock()
{
    sys_lock();

    return (mp_const_none);
}

/**
 * def sys_unlock()
 */
static mp_obj_t module_sys_unlock()
{
    sys_unlock();

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_sys_lock_obj, module_sys_lock);
static MP_DEFINE_CONST_FUN_OBJ_0(module_sys_unlock_obj, module_sys_unlock);

#endif

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
#if CONFIG_PUMBAA_CLASS_TIMER == 1
    timer_module_init();
#endif

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_rom_map_elem_t module_kernel_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_kernel) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Module classes. */
#if CONFIG_PUMBAA_CLASS_TIMER == 1
    { MP_ROM_QSTR(MP_QSTR_Timer), MP_ROM_PTR(&module_kernel_class_timer) },
#endif

    /* Module functions. */
#if CONFIG_PUMBAA_SYS_LOCK == 1
    { MP_ROM_QSTR(MP_QSTR_sys_lock), MP_ROM_PTR(&module_sys_lock_obj) },
    { MP_ROM_QSTR(MP_QSTR_sys_unlock), MP_ROM_PTR(&module_sys_unlock_obj) },
#endif
};

static MP_DEFINE_CONST_DICT(module_kernel_globals, module_kernel_globals_table);

const mp_obj_module_t module_kernel = {
    { &mp_type_module },
    .name = MP_QSTR_kernel,
    .globals = (mp_obj_t)&module_kernel_globals,
};
