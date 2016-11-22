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
 * Print the owi object.
 */
static void class_owi_print(const mp_print_t *print_p,
                            mp_obj_t self_in,
                            mp_print_kind_t kind)
{
    struct class_owi_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Owi object associated with the id. If additional
 * arguments are given, they are used to initialise the owi. See
 * `init`.
 */
static mp_obj_t class_owi_make_new(const mp_obj_type_t *type_p,
                                   mp_uint_t n_args,
                                   mp_uint_t n_kw,
                                   const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pin_device, MP_ARG_REQUIRED | MP_ARG_INT }
    };
    struct class_owi_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int pin_dev;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    pin_dev = args[0].u_int;

    if ((pin_dev < 0) || (pin_dev >= PIN_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad pin device"));
    }

    /* Create a new owi object. */
    self_p = m_new0(struct class_owi_t, 1);
    self_p->base.type = &module_drivers_class_owi;

    if (owi_init((struct owi_driver_t *)&self_p->drv,
                 &pin_device[pin_dev],
                 self_p->devices,
                 membersof(self_p->devices)) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (self_p);
}

/**
 * def reset(self)
 */
static mp_obj_t class_owi_reset(mp_obj_t self_in)
{
    struct class_owi_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (owi_reset(&self_p->drv) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "owi_reset() failed"));
    }

    return (mp_const_none);
}

/**
 * def search(self)
 */
static mp_obj_t class_owi_search(mp_obj_t self_in)
{
    struct class_owi_t *self_p;
    int number_of_devices;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    number_of_devices = owi_search(&self_p->drv);
    
    if (number_of_devices < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "owi_search() failed"));
    }
    
    return (mp_obj_new_int(number_of_devices));
}

/**
 * def get_devices(self)
 */
static mp_obj_t class_owi_get_devices(mp_obj_t self_in)
{
    struct class_owi_t *self_p;
    mp_obj_t devices_list;
    int i;

    self_p = MP_OBJ_TO_PTR(self_in);

    /* Create en empty list for device ids. */
    devices_list = mp_obj_new_list(0, NULL);

    for (i = 0; i < self_p->drv.len; i++) {
        /* Create a new bytes object of the device id. */
        mp_obj_list_append(devices_list,
                           mp_obj_new_bytes(self_p->drv.devices_p[i].id,
                                            sizeof(self_p->drv.devices_p[i].id)));
    }

    return (devices_list);
}

/**
 * def read(self, size)
 */
static mp_obj_t class_owi_read(mp_obj_t self_in, mp_obj_t size_in)
{
    struct class_owi_t *self_p;
    vstr_t vstr;
    size_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    size = mp_obj_get_int(size_in);

    vstr_init_len(&vstr, size);

    if (owi_read(&self_p->drv, vstr.buf, size) != size) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "owi_read() failed"));
    }

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
}

/**
 * def write(self, buffer[, size])
 */
static mp_obj_t class_owi_write(mp_uint_t n_args, const mp_obj_t *args_p)
{
    struct class_owi_t *self_p;
    mp_buffer_info_t buffer_info;
    size_t size;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args_p[1]),
                        &buffer_info,
                        MP_BUFFER_READ);

    /* Get the size. */
    if (n_args == 3) {
        size = mp_obj_get_int(args_p[2]);

        if (buffer_info.len < size) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                               "bad buffer length"));
        }
    } else {
        size = buffer_info.len;
    }

    if (owi_write(&self_p->drv, buffer_info.buf, size) != size) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "owi_write() failed"));
    }

    return (MP_OBJ_NEW_SMALL_INT(size));
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_owi_reset_obj, class_owi_reset);
static MP_DEFINE_CONST_FUN_OBJ_1(class_owi_search_obj, class_owi_search);
static MP_DEFINE_CONST_FUN_OBJ_1(class_owi_get_devices_obj, class_owi_get_devices);
static MP_DEFINE_CONST_FUN_OBJ_2(class_owi_read_obj, class_owi_read);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_owi_write_obj, 2, 3, class_owi_write);

static const mp_rom_map_elem_t class_owi_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&class_owi_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_search), MP_ROM_PTR(&class_owi_search_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_devices), MP_ROM_PTR(&class_owi_get_devices_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_owi_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_owi_write_obj) },
};

static MP_DEFINE_CONST_DICT(class_owi_locals_dict, class_owi_locals_dict_table);

/**
 * Owi class type.
 */
const mp_obj_type_t module_drivers_class_owi = {
    { &mp_type_type },
    .name = MP_QSTR_Owi,
    .print = class_owi_print,
    .make_new = class_owi_make_new,
    .locals_dict = (mp_obj_t)&class_owi_locals_dict,
};
