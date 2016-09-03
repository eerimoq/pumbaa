/**
 * @file modfs.c
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

#include "pumbaa.h"

/**
 * def call()
 */
static mp_obj_t fun_call(mp_obj_t command_in)
{
    char command[128];
    
    strncpy(command, mp_obj_str_get_str(command_in), membersof(command));
    command[membersof(command) - 1] = '\0';
    fs_call(command, sys_get_stdin(), sys_get_stdout(), NULL);
    
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(fun_call_obj, fun_call);

static const mp_map_elem_t module_fs_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_fs) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_call), (mp_obj_t)&fun_call_obj },
};

static MP_DEFINE_CONST_DICT(module_fs_globals, module_fs_globals_table);

const mp_obj_module_t module_fs = {
    .base = { &mp_type_module },
    .name = MP_QSTR_fs,
    .globals = (mp_obj_dict_t*)&module_fs_globals,
};
