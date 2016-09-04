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
static char heap[16384];

int main()
{
    int stack_dummy;

    sys_start();

    std_printf(sys_get_info());
    std_printf(FSTR("\r\n"));

    stack_top_p = (char*)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));
    mp_init();
    
    /* 1. Execute the file main.py. */
    pyexec_file("main.py");
    
    /* 2. Execute the frozen main.py module. */
    pyexec_frozen_module("main.py");

#if CONFIG_MAIN_FRIENDLY_REPL == 1
    /* 3. Execute the interactive interpreter. */
    pyexec_friendly_repl();
#endif
    
    return (0);
}
