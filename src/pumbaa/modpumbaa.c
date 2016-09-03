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

#include "pumbaa.h"

extern const mp_obj_type_t board_class;

static const mp_map_elem_t module_pumbaa_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_pumbaa) },

    /* Board class. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Board), (mp_obj_t)&board_class },

    /* Pin class. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&pin_class },
};

static MP_DEFINE_CONST_DICT(module_pumbaa_globals, module_pumbaa_globals_table);

const mp_obj_module_t module_pumbaa = {
    .base = { &mp_type_module },
    .name = MP_QSTR_pumbaa,
    .globals = (mp_obj_dict_t*)&module_pumbaa_globals,
};
