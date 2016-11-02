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
#include "lib/utils/pyhelp.h"

/**
 * The Pumbaa help text.
 */
static const FAR char *help_text_p =
    "Welcome to Pumbaa!\r\n"
    "\r\n"
    "For online docs please visit http://pumbaa.readthedocs.io/en/latest/.\r\n"
    "\r\n"
    "Control commands:\r\n"
    "  CTRL-E        -- on a blank line, enter paste mode\r\n"
    "\r\n"
    "For further help on a specific object, type help(obj)\r\n"
    ;

/**
 * The builtin help() function.
 *
 * help([object])
 */
static mp_obj_t builtin_help(size_t n_args, const mp_obj_t *args_p)
{
    if (n_args == 0) {
        std_printf(help_text_p);
    } else {
        pyhelp_print_obj(args_p[0]);
    }

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_builtin_help_obj, 0, 1, builtin_help);
