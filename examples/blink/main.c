/**
 * @file main.c
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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
 * This file is part of the Simba project.
 */

#include "simba.h"

#include "py/compile.h"
#include "py/runtime.h"
#include "py/gc.h"
#include "py/stackctrl.h"

static const char script[] =
    "import sys\n"
    "import utime as time\n"
    /* "import pumbaa\n" */
    "\n"
    /* "import pumbaa.pin as pin\n" */
    /* "import pumbaa.board as board\n" */
    "\n"
    "def main():\n"
    "    '''Main function of the Python blink application.\n"
    "\n"
    "    '''\n"
    "\n"
    "    print('Python version:', sys.version)\n"
    "\n"
    "    # Initialize the LED pin and set it high.\n"
    /* "    led = pin.Pin(board.PIN_LED, pin.OUTPUT)\n" */
    /* "    led.write(1)\n" */
    "\n"
    "    # Toggle the LED state periodically.\n"
    "    while True:\n"
    "        time.sleep(0.5)\n"
    "        print('toggle')\n"
    /* "        led.toggle()\n" */
    "\n"
    "\n"
    "if __name__ == '__main__':\n"
    "    main()\n"
    "";

static char *stack_top;
static char heap[16384];

static mp_obj_t execute_from_lexer(mp_lexer_t *lex)
{
    nlr_buf_t nlr;
    mp_parse_tree_t pt;
    mp_obj_t module_fun;
    
    if (nlr_push(&nlr) == 0) {
        pt = mp_parse(lex, MP_PARSE_FILE_INPUT);
        module_fun = mp_compile(&pt, lex->source_name, MP_EMIT_OPT_NONE, false);
        mp_call_function_0(module_fun);
        nlr_pop();
        return 0;
    } else {
        /* uncaught exception */
        return (mp_obj_t)nlr.ret_val;
    }
}

int main()
{
    mp_lexer_t *lex_p;
    int stack_dummy;
    
    stack_top = (char*)&stack_dummy;

    sys_start();
    
    /* Initialized stack limit. */
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    
    /* Initialize heap. */
    gc_init(heap, heap + sizeof(heap));
    
    /* Initialize interpreter. */
    mp_init();

    /* Hello world. */
    lex_p = mp_lexer_new_from_str_len(0, script, strlen(script), false);

    if (execute_from_lexer(lex_p) != 0) {
        std_printf(FSTR("Error\r\n"));
    }

    std_printf(FSTR("Done\r\n"));

    return (0);
}

mp_import_stat_t mp_import_stat(const char *path_p)
{
    return (MP_IMPORT_STAT_NO_EXIST);
}

void nlr_jump_fail(void *val_p)
{
    std_printf(FSTR("FATAL: uncaught NLR 0x%x\r\n"), (long)val_p);
    sys_stop(1);
}

void gc_collect(void)
{
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy_p;
    
    gc_collect_start();
    gc_collect_root(&dummy_p,
                    ((mp_uint_t)stack_top - (mp_uint_t)&dummy_p)
                    / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

void mp_hal_stdout_tx_strn_cooked(const char *str_p, size_t len)
{
    char c;

    while (len--) {
        if (*str_p == '\n') {
            c = '\r';
            chan_write(sys_get_stdout(), &c, 1);
        }

        c = *str_p++;
        chan_write(sys_get_stdout(), &c, 1);
    }
}
