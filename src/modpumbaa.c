/**
 * @file modpumbaa.c
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

/**
 * def fs_call(command)
 */
static mp_obj_t module_fs_call(mp_obj_t command_in)
{
    char command[128];

    strncpy(command, mp_obj_str_get_str(command_in), membersof(command));
    command[membersof(command) - 1] = '\0';
    fs_call(command, sys_get_stdin(), sys_get_stdout(), NULL);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(module_fs_call_obj, module_fs_call);

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    timer_module_init();
    pin_module_init();

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_map_elem_t module_pumbaa_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_pumbaa) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&module_init_obj },

    /* Module classes. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Board), (mp_obj_t)&module_pumbaa_class_board },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Event), (mp_obj_t)&module_pumbaa_class_event },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&module_pumbaa_class_pin },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Timer), (mp_obj_t)&module_pumbaa_class_timer },

    /* Module functions. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_fs_call), (mp_obj_t)&module_fs_call_obj },
};

static MP_DEFINE_CONST_DICT(module_pumbaa_globals, module_pumbaa_globals_table);

const mp_obj_module_t module_pumbaa = {
    { &mp_type_module },
    .name = MP_QSTR_pumbaa,
    .globals = (mp_obj_t)&module_pumbaa_globals,
};
