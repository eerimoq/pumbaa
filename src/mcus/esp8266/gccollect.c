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

char *stack_top_p;

extern mp_uint_t gc_helper_get_regs_and_sp(mp_uint_t *regs);

void gc_collect(void)
{
    mp_uint_t regs[8];

    gc_collect_start();
    gc_helper_get_regs_and_sp(regs);

#if MICROPY_PY_THREAD == 1
    mp_thread_gc_others();
#else
    uintptr_t stack_top;
    uintptr_t stack_bottom;
    size_t stack_size;

    stack_top = (uintptr_t)thrd_get_top_of_stack(thrd_self());
    stack_bottom = (uintptr_t)thrd_get_bottom_of_stack(thrd_self());
    stack_bottom &= 0xfffffffc;
    stack_size = (stack_top - stack_bottom);
    gc_collect_root((void**)stack_bottom, stack_size / sizeof(mp_uint_t));
#endif

    gc_collect_end();
}


int *__errno() {
    return NULL;
}

void __assert_func(const char *file,
                   int line,
                   const char *func,
                   const char *expr)
{
    std_printf(FSTR("assert:%s:%d:%s: %s\n"), file, line, func, expr);
    nlr_raise(mp_obj_new_exception_msg(&mp_type_AssertionError,
                                       "C-level assert"));
}
