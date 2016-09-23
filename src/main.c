/**
 * @file main.c
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

extern char *stack_top_p;
static char heap[CONFIG_PUMBAA_HEAP_SIZE];

int main()
{
    int stack_dummy;
    int res;
    
    sys_start();

    std_printf(sys_get_info());
    std_printf(FSTR("\r\n"));

    stack_top_p = (char*)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    /* Initialize sys.path and sys.argv. */
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_path), 0);
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_argv), 0);

    /* 1. Execute the file main.py. */
    res = (pyexec_file("main.py") != 1);

    /* 2. Execute the frozen main.py module. */
    res = (pyexec_frozen_module("main.py") != 1);
    
#if CONFIG_PUMBAA_MAIN_FRIENDLY_REPL == 1
    /* 3. Execute the interactive interpreter. */
    res = (pyexec_friendly_repl() != 1);
#endif

    return (res);
}
