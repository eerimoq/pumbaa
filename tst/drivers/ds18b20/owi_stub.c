/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

static struct owi_device_t devices[4] = {
    { .id = { DS18B20_FAMILY_CODE, '2', '3', '4', '5', '6', '7', '8' } },
    { .id = { DS18B20_FAMILY_CODE, '2', '3', '4', '5', '6', '7', '9' } },
    { .id = { '1', '2', '3', '4', '5', '6', '7', '8' } },
    { .id = { '1', '2', '3', '4', '5', '6', '7', '8' } }
};

int owi_init(struct owi_driver_t *self_p,
             struct pin_device_t *dev_p,
             struct owi_device_t *devices_p,
             size_t nmemb)
{
    self_p->devices_p = &devices[0];
    self_p->len = membersof(devices);

    return (0);
}

int owi_reset(struct owi_driver_t *self_p)
{
    return (0);
}

int owi_search(struct owi_driver_t *self_p)
{
    return (0);
}

ssize_t owi_read(struct owi_driver_t *self_p,
                 void *buf_p,
                 size_t size)
{
    return (0);
}

ssize_t owi_write(struct owi_driver_t *self_p,
                  const void *buf_p,
                  size_t size)
{
    return (0);
}
