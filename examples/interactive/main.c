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

static int execute_from_lexer(mp_lexer_t *lex_p,
                              mp_parse_input_kind_t input_kind,
                              bool is_repl)
{
    nlr_buf_t nlr;
    qstr source_name;
    mp_parse_tree_t parse_tree;
    mp_obj_t module_fun;
    mp_obj_t obj;
    
    PRINT_FILE_LINE();
    
    if (lex_p == NULL) {
        std_printf(FSTR("MemoryError: lexer could not allocate memory\n"));
        return (-1);
    }

    PRINT_FILE_LINE();
    
    if (nlr_push(&nlr) == 0) {
        source_name = lex_p->source_name;

        /* Parse, compile and execute. */
        parse_tree = mp_parse(lex_p, input_kind);
        module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, is_repl);
        mp_call_function_0(module_fun);

        PRINT_FILE_LINE();

        /* Check for pending exception. */
        if (MP_STATE_VM(mp_pending_exception) != MP_OBJ_NULL) {
            obj = MP_STATE_VM(mp_pending_exception);
            MP_STATE_VM(mp_pending_exception) = MP_OBJ_NULL;
            nlr_raise(obj);
        }
        
        nlr_pop();
        
        return (0);
    } else {
        /* Uncaught exception. */
        return (-1);
    }
}

static void interactive(void)
{
    char c;
    char line[128];
    int length;
    mp_lexer_t *lex_p;
    
    std_printf(FSTR("MicroPython " MICROPY_GIT_TAG " on " MICROPY_BUILD_DATE "; "
                    MICROPY_PY_SYS_PLATFORM " version\n"
                    "Use Ctrl-D to exit\n"));

    for (;;) {
        std_printf(FSTR(">>> "));

        /* Read a line. */
        c = '\0';
        length = 0;
        
        while (c != '\n') {
            chan_read(sys_get_stdin(), &c, 1);
            chan_write(sys_get_stdout(), &c, 1);
            line[length] = c;
            length++;
        }

        line[length] = '\0';

        /* Parse and execute the line. */
        lex_p = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_,
                                          line,
                                          length,
                                          false);
        execute_from_lexer(lex_p, MP_PARSE_SINGLE_INPUT, true);
    }
}

int main()
{
    int stack_dummy;

    sys_start();
    
    stack_top_p = (char*)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    interactive();
    
    return (0);
}
