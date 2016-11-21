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
 * Print the ds18b20 object.
 */
static void class_ds18b20_print(const mp_print_t *print_p,
                                mp_obj_t self_in,
                                mp_print_kind_t kind)
{
    struct class_ds18b20_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Ds18b20 object associated with the id. If additional
 * arguments are given, they are used to initialise the ds18b20. See
 * `init`.
 */
static mp_obj_t class_ds18b20_make_new(const mp_obj_type_t *type_p,
                                       mp_uint_t n_args,
                                       mp_uint_t n_kw,
                                       const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_owi, MP_ARG_REQUIRED | MP_ARG_OBJ }
    };
    struct class_ds18b20_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_obj_t owi;
    struct class_owi_t *owi_p;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    owi = args[0].u_obj;

    if (!MP_OBJ_IS_TYPE(owi, &module_drivers_class_owi)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "Owi object expected"));
    }

    owi_p = MP_OBJ_TO_PTR(owi);

    /* Create a new ds18b20 object. */
    self_p = m_new0(struct class_ds18b20_t, 1);
    self_p->base.type = &module_drivers_class_ds18b20;
    self_p->owi = owi;

    if (ds18b20_init((struct ds18b20_driver_t *)&self_p->drv,
                     &owi_p->drv) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (self_p);
}

/**
 * def convert(self)
 */
static mp_obj_t class_ds18b20_convert(mp_obj_t self_in)
{
    struct class_ds18b20_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (ds18b20_convert(&self_p->drv) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "ds18b20_convert() failed"));
    }

    return (mp_const_none);
}

/**
 * def get_devices(self)
 */
static mp_obj_t class_ds18b20_get_devices(mp_obj_t self_in)
{
    struct class_ds18b20_t *self_p;
    struct owi_driver_t *owi_p;
    mp_obj_t devices_list;
    int i;

    self_p = MP_OBJ_TO_PTR(self_in);
    owi_p = self_p->drv.owi_p;

    /* Create en empty list for device ids. */
    devices_list = mp_obj_new_list(0, NULL);

    for (i = 0; i < owi_p->len; i++) {
        if (owi_p->devices_p->id[0] != DS18B20_FAMILY_CODE) {
            continue;
        }

        /* Create a new bytes object of the device id. */
        mp_obj_list_append(devices_list,
                           mp_obj_new_bytes(owi_p->devices_p->id,
                                            sizeof(owi_p->devices_p->id)));
    }

    return (devices_list);
}

/**
 * def get_temperature(self, device_id)
 */
static mp_obj_t class_ds18b20_get_temperature(mp_obj_t self_in,
                                              mp_obj_t device_id_in)
{
    struct class_ds18b20_t *self_p;
    mp_buffer_info_t device_id_info;
    int temperature;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(device_id_in),
                        &device_id_info,
                        MP_BUFFER_READ);

    if (ds18b20_get_temperature(&self_p->drv,
                                device_id_info.buf,
                                &temperature) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "ds18b20_async_wait() failed"));
    }

    return (mp_obj_new_float(temperature * 0.0625f));
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_ds18b20_convert_obj, class_ds18b20_convert);
static MP_DEFINE_CONST_FUN_OBJ_1(class_ds18b20_get_devices_obj, class_ds18b20_get_devices);
static MP_DEFINE_CONST_FUN_OBJ_2(class_ds18b20_get_temperature_obj, class_ds18b20_get_temperature);

static const mp_rom_map_elem_t class_ds18b20_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_convert), MP_ROM_PTR(&class_ds18b20_convert_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_devices), MP_ROM_PTR(&class_ds18b20_get_devices_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_temperature), MP_ROM_PTR(&class_ds18b20_get_temperature_obj) },
};

static MP_DEFINE_CONST_DICT(class_ds18b20_locals_dict, class_ds18b20_locals_dict_table);

/**
 * Ds18b20 class type.
 */
const mp_obj_type_t module_drivers_class_ds18b20 = {
    { &mp_type_type },
    .name = MP_QSTR_Ds18b20,
    .print = class_ds18b20_print,
    .make_new = class_ds18b20_make_new,
    .locals_dict = (mp_obj_t)&class_ds18b20_locals_dict,
};
