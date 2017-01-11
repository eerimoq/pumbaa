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

#include "pumbaa.h"

static ssize_t base_chan_read(void *base_p, void *buf_p, size_t size)
{
    struct can_driver_t *self_p;

    self_p = base_p;

    return (queue_read(&self_p->chin, buf_p, size));
}

static size_t base_chan_size(void *base_p)
{
    struct can_driver_t *self_p;

    self_p = base_p;

    return (queue_size(&self_p->chin));
}

int can_module_init()
{
    return (0);
}

int can_init(struct can_driver_t *self_p,
             struct can_device_t *dev_p,
             uint32_t speed,
             void *rxbuf_p,
             size_t size)
{
    chan_init(&self_p->base,
              base_chan_read,
              chan_null(),
              base_chan_size);

    queue_init(&self_p->chin, rxbuf_p, size);
    
    return (0);
}

int can_start(struct can_driver_t *self_p)
{
    return (0);
}

int can_stop(struct can_driver_t *self_p)
{
    return (0);
}

ssize_t can_read(struct can_driver_t *self_p,
                 struct can_frame_t *frame_p,
                 size_t size)
{
    static int counter = 0;

    if (counter == 0) {
        memset(frame_p, 0, sizeof(*frame_p));
        frame_p->id = 0x58;
        frame_p->size = 0;
    } else {
        memset(frame_p, 0, sizeof(*frame_p));
        frame_p->id = 0x58;
        frame_p->extended_frame = 1;
        frame_p->size = 0;
    }

    counter++;

    return (size);
}

ssize_t can_write(struct can_driver_t *self_p,
                  const struct can_frame_t *frame_p,
                  size_t size)
{
    static int counter = 0;

    if (counter == 0) {
        if (frame_p->id != 0x57) {
            return (-1);
        }

        if (frame_p->size != 0) {
            return (-1);
        }

        if (frame_p->extended_frame != 0) {
            return (-1);
        }
    } else {
        if (frame_p->id != 0x57) {
            return (-1);
        }

        if (frame_p->size != 0) {
            return (-1);
        }

        if (frame_p->extended_frame != 1) {
            return (-1);
        }
    }

    counter++;

    return (size);
}

static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_can_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_can_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
};

static MP_DEFINE_CONST_DICT(module_can_stub_globals, module_can_stub_globals_table);

const mp_obj_module_t module_can_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_can_stub_globals,
};
