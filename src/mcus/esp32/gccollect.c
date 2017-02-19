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

/**
 * Copies all registers to the function save areas on the stack.
 */
extern void esp_xthal_window_spill(void);

/**
 * Trace the stacks, including the registers (since they live on the
 * stack in this function).
 */
void gc_collect(void)
{
    gc_collect_start();
    esp_xthal_window_spill();

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
