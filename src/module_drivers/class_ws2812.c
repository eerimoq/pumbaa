/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017, Erik Moqvist
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

#if CONFIG_PUMBAA_CLASS_WS2812 == 1

/**
 * Print the ws2812 object.
 */
static void class_ws2812_print(const mp_print_t *print_p,
                            mp_obj_t self_in,
                            mp_print_kind_t kind)
{
    struct class_ws2812_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Ws2812 object associated with the id. If additional
 * arguments are given, they are used to initialise the ws2812. See
 * `init`.
 */
static mp_obj_t class_ws2812_make_new(const mp_obj_type_t *type_p,
                                   mp_uint_t n_args,
                                   mp_uint_t n_kw,
                                   const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pin_devices, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };
    struct class_ws2812_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int pin_dev;
    mp_obj_t *pin_devices_p;
    mp_uint_t len;
    mp_obj_t *items_p;
    int i;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Create a new Ws2812 object. */
    self_p = m_new0(struct class_ws2812_t, 1);
    self_p->base.type = &module_drivers_class_ws2812;

    pin_devices_p = MP_OBJ_TO_PTR(args[0].u_obj);

    if (MP_OBJ_IS_TYPE(pin_devices_p, &mp_type_list)) {
        mp_obj_list_get(pin_devices_p, &len, &items_p);

        if (len > WS2812_PIN_DEVICES_MAX) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                               "too many pin devices"));
        }

        for (i = 0; i < len; i++) {
            pin_dev = mp_obj_get_int(items_p[i]);

            if ((pin_dev < 0) || (pin_dev >= PIN_DEVICE_MAX)) {
                nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                                   "bad pin device"));
            }

            self_p->pin_devices[i] = &pin_device[pin_dev];
        }
    } else if (MP_OBJ_IS_INT(pin_devices_p)) {
        len = 1;
        self_p->pin_devices[0] = &pin_device[mp_obj_get_int(pin_devices_p)];
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "bad pin devices"));
    }

    if (ws2812_init((struct ws2812_driver_t *)&self_p->drv,
                    &self_p->pin_devices[0],
                    len) != 0) {
        return (mp_const_none);
    }

    return (self_p);
}

/**
 * def write(self, buffer[, size])
 */
static mp_obj_t class_ws2812_write(mp_uint_t n_args,
                                   const mp_obj_t *args_p)
{
    struct class_ws2812_t *self_p;
    mp_buffer_info_t buffer_info;
    int number_of_pixles;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args_p[1]),
                        &buffer_info,
                        MP_BUFFER_READ);

    /* Get the number of pixels. */
    if (n_args == 3) {
        number_of_pixles = mp_obj_get_int(args_p[2]);

        if (buffer_info.len < number_of_pixles * 3) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                               "bad buffer length"));
        }
    } else {
        if ((buffer_info.len % 3) != 0) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                               "bad buffer length"));
        }

        number_of_pixles = (buffer_info.len / 3);
    }

    if (ws2812_write(&self_p->drv,
                     (uint8_t *)buffer_info.buf,
                     number_of_pixles) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "ws2812_write() failed"));
    }

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_ws2812_write_obj, 2, 3, class_ws2812_write);

static const mp_rom_map_elem_t class_ws2812_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_ws2812_write_obj) }
};

static MP_DEFINE_CONST_DICT(class_ws2812_locals_dict, class_ws2812_locals_dict_table);

/**
 * Ws2812 class type.
 */
const mp_obj_type_t module_drivers_class_ws2812 = {
    { &mp_type_type },
    .name = MP_QSTR_Ws2812,
    .print = class_ws2812_print,
    .make_new = class_ws2812_make_new,
    .locals_dict = (mp_obj_t)&class_ws2812_locals_dict,
};

#endif
