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

#if CONFIG_PUMBAA_EMACS == 1

static mp_obj_t module_emacs(mp_uint_t n_args, const mp_obj_t *args_p)
{
    int res;
    const char *path_p;

    if (n_args == 0) {
        path_p = NULL;
    } else {
        path_p = mp_obj_str_get_str(args_p[0]);
    }
    
    mp_hal_set_interrupt_char(-1);
    res = emacs(path_p);
    mp_hal_set_interrupt_char(CHAR_CTRL_C);

    if (res != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "emacs failed"));
    }
        
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(module_emacs_obj, 0, 1, module_emacs);

#endif

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_rom_map_elem_t module_text_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_text) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Module classes. */

    /* Module functions. */
#if CONFIG_PUMBAA_EMACS == 1
    { MP_ROM_QSTR(MP_QSTR_emacs), MP_ROM_PTR(&module_emacs_obj) },
#endif
};

static MP_DEFINE_CONST_DICT(module_text_globals, module_text_globals_table);

const mp_obj_module_t module_text = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_text_globals,
};
