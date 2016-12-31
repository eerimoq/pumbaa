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

extern intptr_t stack_top;
static char heap[CONFIG_PUMBAA_HEAP_SIZE];

/**
 * Print a message after a script has been executed.
 */
static void print_exit_message(int res, const char *prefix_p)
{
    if (res == 1) {
        std_printf(FSTR("%s exited normally.\r\n\r\n"), prefix_p);
    } else if (res & PYEXEC_FORCED_EXIT) {
        std_printf(FSTR("%s forced exit.\r\n\r\n"), prefix_p);
    } else {
        std_printf(FSTR("%s exited abnormally.\r\n\r\n"), prefix_p);
    }
}

/**
 * The entry function for a Pumbaa application.
 */
int main()
{
    int stack_dummy;
    int res;

    /* Start the system. */
    sys_start();

    std_printf(sys_get_info());
    std_printf(FSTR("\r\n"));

    stack_top = (intptr_t)&stack_dummy;
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));

    /* Initialize the thread module. */
#if MICROPY_PY_THREAD == 1
    module_thread_init();
#endif

    mp_init();

    /* Initialize the keyboard interrupt object. */
    MP_STATE_VM(keyboard_interrupt_obj) =
        mp_obj_new_exception(&mp_type_KeyboardInterrupt);

    /* Initialize sys.path and sys.argv. */
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_path), 0);
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_argv), 0);

    /* 1. Execute the file main.py. */
    std_printf(FSTR("Executing file 'main.py'.\r\n"));
    res = pyexec_file("main.py");
    print_exit_message(res, "File 'main.py'");

    /* 2. Execute the frozen module main.py. */
    std_printf(FSTR("Executing frozen module 'main.py'.\r\n"));
    res = pyexec_frozen_module("main.py");
    print_exit_message(res, "Frozen module 'main.py'");

#if CONFIG_PUMBAA_MAIN_FRIENDLY_REPL == 1
    /* 3. Execute the interactive shell. */
    res = pyexec_friendly_repl();
    print_exit_message(res, "Interactive shell");
#endif

#if CONFIG_PUMBAA_MAIN_REBOOT_AT_EXIT == 1
    sys_reboot();
#endif

    return (res != 1);
}
