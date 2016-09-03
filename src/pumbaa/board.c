/**
 * @file board.c
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
 * class Board(object):
 *
 *     PIN_LED
 */
static const mp_map_elem_t board_locals_dict_table[] = {
    /* Class constants. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_PIN_LED), MP_OBJ_NEW_SMALL_INT(13) },
};

static MP_DEFINE_CONST_DICT(board_locals_dict, board_locals_dict_table);

const mp_obj_type_t board_class = {
    { &mp_type_type },
    .name = MP_QSTR_Board,
    .locals_dict = (mp_obj_t)&board_locals_dict,
};
