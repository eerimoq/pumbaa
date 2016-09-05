/**
 * @file modos.c
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

static const qstr os_uname_info_fields[] = {
    MP_QSTR_sysname,
    MP_QSTR_nodename,
    MP_QSTR_release,
    MP_QSTR_version,
    MP_QSTR_machine
};

static const MP_DEFINE_STR_OBJ(os_uname_info_sysname_obj, MICROPY_PY_SYS_PLATFORM);
static const MP_DEFINE_STR_OBJ(os_uname_info_nodename_obj, "");
static const MP_DEFINE_STR_OBJ(os_uname_info_release_obj, STRINGIFY(VERSION));
static const MP_DEFINE_STR_OBJ(os_uname_info_version_obj, MICROPY_GIT_TAG);
static const MP_DEFINE_STR_OBJ(os_uname_info_machine_obj, "Arduino Due");

static mp_obj_tuple_t os_uname_info_obj = {
    .base = {&mp_type_attrtuple},
    .len = 5,
    .items = {
        (mp_obj_t)&os_uname_info_sysname_obj,
        (mp_obj_t)&os_uname_info_nodename_obj,
        (mp_obj_t)&os_uname_info_release_obj,
        (mp_obj_t)&os_uname_info_version_obj,
        (mp_obj_t)&os_uname_info_machine_obj,
        (void *)os_uname_info_fields,
    }
};

static mp_obj_t os_uname(void)
{
    return ((mp_obj_t)&os_uname_info_obj);
}

static MP_DEFINE_CONST_FUN_OBJ_0(os_uname_obj, os_uname);

static mp_obj_t os_listdir(mp_uint_t n_args, const mp_obj_t *args)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(os_listdir_obj, 0, 1, os_listdir);

static const mp_rom_map_elem_t module_os_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uos) },
    { MP_ROM_QSTR(MP_QSTR_uname), MP_ROM_PTR(&os_uname_obj) },
    { MP_ROM_QSTR(MP_QSTR_listdir), MP_ROM_PTR(&os_listdir_obj) },
};

static MP_DEFINE_CONST_DICT(module_os_globals, module_os_globals_table);

const mp_obj_module_t mp_module_os = {
    .base = { &mp_type_module },
    .name = MP_QSTR_uos,
    .globals = (mp_obj_dict_t*)&module_os_globals,
};
