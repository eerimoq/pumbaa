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
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

static const mp_rom_map_elem_t module_board_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_board) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
    
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO00), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO01), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO02), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO03), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO04), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO05), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO06), MP_ROM_INT(6) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO07), MP_ROM_INT(7) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO08), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO09), MP_ROM_INT(9) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO10), MP_ROM_INT(10) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO11), MP_ROM_INT(11) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO12), MP_ROM_INT(12) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO13), MP_ROM_INT(13) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO14), MP_ROM_INT(14) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO15), MP_ROM_INT(15) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO16), MP_ROM_INT(16) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO17), MP_ROM_INT(17) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO18), MP_ROM_INT(18) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO19), MP_ROM_INT(19) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO21), MP_ROM_INT(21) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO22), MP_ROM_INT(22) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO23), MP_ROM_INT(23) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO25), MP_ROM_INT(25) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO26), MP_ROM_INT(26) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO27), MP_ROM_INT(27) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO32), MP_ROM_INT(28) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO33), MP_ROM_INT(29) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO34), MP_ROM_INT(30) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO35), MP_ROM_INT(31) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO36), MP_ROM_INT(32) },
    { MP_ROM_QSTR(MP_QSTR_PIN_GPIO39), MP_ROM_INT(35) },

    { MP_ROM_QSTR(MP_QSTR_PIN_LED), MP_ROM_INT(16) },

    { MP_ROM_QSTR(MP_QSTR_PIN_A0), MP_ROM_INT(32) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A3), MP_ROM_INT(35) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A4), MP_ROM_INT(28) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A5), MP_ROM_INT(29) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A6), MP_ROM_INT(30) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A7), MP_ROM_INT(31) },

    { MP_ROM_QSTR(MP_QSTR_SPI_0), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_SPI_1), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_SPI_2), MP_ROM_INT(2) },

    { MP_ROM_QSTR(MP_QSTR_CAN_0), MP_ROM_INT(0) },

    { MP_ROM_QSTR(MP_QSTR_ADC_0), MP_ROM_INT(0) },
};

static MP_DEFINE_CONST_DICT(module_board_globals, module_board_globals_table);

/**
 * Board module type.
 */
const mp_obj_module_t module_board = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_board_globals,
};
