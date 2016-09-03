/**
 * @file junk.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

char *stack_top_p;

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

    std_printf(FSTR("collect\r\n"));

    gc_collect_start();
    gc_collect_root(&dummy_p,
                    ((mp_uint_t)stack_top_p - (mp_uint_t)&dummy_p)
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
