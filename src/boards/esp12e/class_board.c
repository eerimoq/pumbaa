/**
 * @file class_board.c
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

static const mp_rom_map_elem_t class_board_locals_table[] = {
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO0), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO2), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO4), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO5), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO12), MP_ROM_INT(12) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO13), MP_ROM_INT(13) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO14), MP_ROM_INT(14) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO15), MP_ROM_INT(15) },

    { MP_ROM_QSTR(MP_QSTR_PIN_LED), MP_ROM_INT(2) }
};

static MP_DEFINE_CONST_DICT(class_board_locals_dict, class_board_locals_table);

/**
 * Board class type.
 */
const mp_obj_type_t module_simba_class_board = {
    { &mp_type_type },
    .name = MP_QSTR_Board,
    .locals_dict = (mp_obj_t)&class_board_locals_dict,
};
