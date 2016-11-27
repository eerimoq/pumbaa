/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016, Erik Moqvist
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

/**
 * PIN_LED
 */
static const mp_rom_map_elem_t module_board_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_board) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
    
    { MP_ROM_QSTR(MP_QSTR_PIN_LED), MP_ROM_INT(0) },

    { MP_ROM_QSTR(MP_QSTR_PIN_D0), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_PIN_D1), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_PIN_D2), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_PIN_D3), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_PIN_D4), MP_ROM_INT(4) },

    { MP_ROM_QSTR(MP_QSTR_PIN_A0), MP_ROM_INT(5) },
    { MP_ROM_QSTR(MP_QSTR_PIN_A1), MP_ROM_INT(6) },

    { MP_ROM_QSTR(MP_QSTR_ADC_0), MP_ROM_INT(0) },

    { MP_ROM_QSTR(MP_QSTR_PIN_DAC0), MP_ROM_INT(10) },
    { MP_ROM_QSTR(MP_QSTR_PIN_DAC1), MP_ROM_INT(11) },

    /* External interrputs. */
    { MP_ROM_QSTR(MP_QSTR_EXTI_D0), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_EXTI_D1), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_EXTI_D2), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_EXTI_D3), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_EXTI_D4), MP_ROM_INT(4) },

    /* SPI. */
    { MP_ROM_QSTR(MP_QSTR_SPI_0), MP_ROM_INT(0) },

    /* CAN. */
    { MP_ROM_QSTR(MP_QSTR_CAN_0), MP_ROM_INT(0) },
};

static MP_DEFINE_CONST_DICT(module_board_globals, module_board_globals_table);

/**
 * Board module type.
 */
const mp_obj_module_t module_board = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_board_globals,
};
