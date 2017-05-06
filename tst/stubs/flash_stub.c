/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2017, Erik Moqvist
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
 * This file is part of the Simba project.
 */

#include "simba.h"

int flash_module_init(void)
{
    return (0);
}

int flash_init(struct flash_driver_t *self_p,
               struct flash_device_t *dev_p)
{
    return (0);
}

ssize_t flash_read(struct flash_driver_t *self_p,
                   void *dst_p,
                   size_t src,
                   size_t size)
{
    static int counter = 0;

    BTASSERT(src == 0);
    BTASSERT(size == 1);
    
    counter++;
    
    if (counter == 1) {
        memcpy(dst_p, "\xff", 1);
    } else {
        memcpy(dst_p, "\x12", 1);
    }
    
    return (1);
}

ssize_t flash_write(struct flash_driver_t *self_p,
                    size_t dst,
                    const void *src_p,
                    size_t size)
{
    BTASSERT(dst == 0);
    BTASSERT(memcmp(src_p, "\x12", 1) == 0);
    BTASSERT(size == 1);
    
    return (1);
}

int flash_erase(struct flash_driver_t *self_p,
                size_t addr,
                size_t size)
{
    return (0);
}
