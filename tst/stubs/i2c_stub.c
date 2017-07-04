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
 * This file is part of the Pumbaa project.
 */

#include "pumbaa.h"

int i2c_module_init()
{
    return (0);
}

int i2c_init(struct i2c_driver_t *self_p,
             struct i2c_device_t *dev_p,
             int baudrate,
             int address)
{
    return (0);
}

int i2c_start(struct i2c_driver_t *self_p)
{
    return (0);
}

int i2c_stop(struct i2c_driver_t *self_p)
{
    return (0);
}

ssize_t i2c_read(struct i2c_driver_t *self_p,
                 int address,
                 void *buf_p,
                 size_t size)
{
    static int counter = 0;

    if (address != 0x57) {
        return (-1);
    }

    if (counter == 0) {
        if (size != 4) {
            return (-1);
        }

        memcpy(buf_p, "2345", 4);
    } else if (counter == 1) {
        if (size != 3) {
            return (-1);
        }

        memcpy(buf_p, "678", 4);
    } else if (counter == 2) {
        if (size != 1) {
            return (-1);
        }

        memcpy(buf_p, "9", 1);
    } else {
        return (-1);
    }

    counter++;

    return (size);
}

ssize_t i2c_write(struct i2c_driver_t *self_p,
                  int address,
                  const void *buf_p,
                  size_t size)
{
    static int counter = 0;

    if (address != 0x57) {
        return (-1);
    }

    if (counter == 0) {
        if (memcmp("1234", buf_p, 4) != 0) {
            return (-1);
        }
    } else {
        return (-1);
    }

    counter++;

    return (size);
}

int i2c_scan(struct i2c_driver_t *self_p, int address)
{
    if (address == 0x57) {
        return (1);
    }

    return (0);
}

static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_i2c_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_i2c_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
};

static MP_DEFINE_CONST_DICT(module_i2c_stub_globals, module_i2c_stub_globals_table);

const mp_obj_module_t module_i2c_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_i2c_stub_globals,
};
