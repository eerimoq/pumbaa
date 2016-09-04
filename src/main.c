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

static int parse_compile_execute(void *source_p,
                                 mp_parse_input_kind_t input_kind,
                                 int exec_flags)
{
    int ret = 0;
    mp_obj_t module_fun;
    nlr_buf_t nlr;

    if (nlr_push(&nlr) == 0) {
        /* Source is a lexer, parse and compile the script. */
        mp_lexer_t *lex = source_p;
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, 0);

        /* Execute code. */
        mp_call_function_0(module_fun);

        nlr_pop();
    } else {
            mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }

    return ret;
}

int main()
{
    int frozen_type;
    int stack_dummy;
    void *frozen_data_p;

    sys_start();

    std_printf(sys_get_info());
    std_printf(FSTR("\r\n"));

    stack_top_p = (char*)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    /* 1. Open the file main.py and execute it. */

    /* 2. Execute the frozen main module. */
    frozen_type = mp_find_frozen_module("main.py", 7, &frozen_data_p);

    if (frozen_type == MP_FROZEN_STR) {
        return (parse_compile_execute(frozen_data_p, MP_PARSE_FILE_INPUT, 0));
    }

    /* 3. Start the interactive interpreter. */
    while (1) {
        pyexec_friendly_repl();
    }

    return (0);
}
