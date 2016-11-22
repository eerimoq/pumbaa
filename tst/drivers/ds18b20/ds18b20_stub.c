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

int ds18b20_module_init()
{
    return (0);
}

int ds18b20_init(struct ds18b20_driver_t *self_p,
                 struct owi_driver_t *owi_p)
{
    self_p->owi_p = owi_p;

    return (0);
}

int ds18b20_convert(struct ds18b20_driver_t *self_p)
{
    return (0);
}

int ds18b20_get_temperature(struct ds18b20_driver_t *self_p,
                            const uint8_t *id_p,
                            int *temp_p)
{
    *temp_p = 352;

    return (0);
}

char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  const uint8_t *id_p,
                                  char *temp_p)
{
    return (NULL);
}
