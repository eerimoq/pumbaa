/**
 * @file builtininput.c
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
 * The builtin input() function.
 */
static mp_obj_t builtin_input(size_t n_args, const mp_obj_t *args)
{
    /* vstr_t line; */
    /* int res; */
    
    /* Print the prompt. */
    if (n_args == 1) {
        mp_obj_print(args[0], PRINT_STR);
    }

    /* TODO: Read a line. */
    
    /* if (ine.len == 0 && ret == CHAR_CTRL_D) { */
    nlr_raise(mp_obj_new_exception(&mp_type_EOFError));
    /* } */
    
    /* return (mp_obj_new_str_from_vstr(&mp_type_str, &line)); */
    
    return (mp_const_none);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_builtin_input_obj, 0, 1, builtin_input);
