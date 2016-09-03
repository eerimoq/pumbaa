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

/**
 * The Python script.
 */
static const char script[] =
    "import sys\n"
    "import utime as time\n"
    "\n"
    "import pumbaa\n"
    "\n"
    "def main():\n"
    "    '''Main function of the Python blink application.\n"
    "\n"
    "    '''\n"
    "\n"
    "    print('Python version:', sys.version)\n"
    "\n"
    "    # Initialize the LED pin and set it high.\n"
    "    led = pumbaa.Pin(pumbaa.Board.PIN_LED, pumbaa.Pin.OUTPUT)\n"
    "    led.write(1)\n"
    "\n"
    "    # Toggle the LED state periodically.\n"
    "    while True:\n"
    "        time.sleep(0.5)\n"
    "        print('toggle')\n"
    "        led.toggle()\n" 
    "\n"
    "\n"
    "if __name__ == '__main__':\n"
    "    main()\n"
    "";

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

    sys_start();
    
    stack_top_p = (char*)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    /* Script. */
    lex_p = mp_lexer_new_from_str_len(0, script, strlen(script), false);

    if (execute_from_lexer(lex_p) != 0) {
        std_printf(FSTR("Error\r\n"));
    } else {
        std_printf(FSTR("Ok\r\n"));
    }

    return (0);
}
