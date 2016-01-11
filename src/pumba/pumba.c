/**
 * @file pumba.c
 * @version 0.0.0
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
 * This file is part of the Pumba project.
 */

#include "simba.h"

int pumba_init(struct pumba_t *self_p,
               void *heap_p,
               size_t size)
{
    heap_init(&self_p->heap, heap_p, size);

    self_p->regs.pc = 0;

    self_p->root_p = pumba_list(self_p);
    self_p->builtins = pumba_dict(self_p);
    self_p->modules = pumba_dict(self_p);
    
    return (0);
}

int pumba_main()
{
    /* The instruction execution loop. */
    while (1) {
        switch () {
            
        case PUMBA_INSTRUCTION_ADD:
            break;
            
        default:
            return (-1);
        }
    }

    return (0);
}
