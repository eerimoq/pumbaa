/**
 * @file pin.c
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

extern const mp_obj_type_t pin_class;

/**
 * class Pin(object):
 *
 *     INPUT
 *     OUTPUT
 *
 *     def __init__(self, pin, mode)
 *
 *     def read(self)
 *
 *     def write(self, value)
 *
 *     def toggle(self)
 *
 *     def set_mode(self, mode)
 */
struct pin_obj_t {
    mp_obj_base_t base;
    struct pin_driver_t drv;
};

/**
 * Print the pin object.
 */
static void pin_print(const mp_print_t *print_p,
                      mp_obj_t self_in,
                      mp_print_kind_t kind)
{
}

/**
 * Init helper function.
 */
static mp_obj_t pin_obj_init_helper(const struct pin_obj_t *self_p,
                                    mp_uint_t n_args,
                                    const mp_obj_t *pos_args_p,
                                    mp_map_t *kwargs_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    uint mode;
    uint pin;

    /* Parse args. */
    mp_arg_parse_all(n_args,
                     pos_args_p,
                     kwargs_p,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    pin = args[0].u_int;
    mode = args[1].u_int;

    pin_init((struct pin_driver_t *)&self_p->drv,
             &pin_device[pin],
             mode);

    return (mp_const_none);
}

/**
 * Object initialization function.
 *
 * def __init__(self, pin, mode)
 */
static mp_obj_t pin_obj_init(mp_uint_t n_args,
                             const mp_obj_t *args_p,
                             mp_map_t *kwargs_p)
{
    return (pin_obj_init_helper(args_p[0], n_args - 1, args_p + 1, kwargs_p));
}

/**
 * Create a new Pin object associated with the id. If additional
 * arguments are given, they are used to initialise the pin. See
 * `init`.
 */
static mp_obj_t pin_make_new(const mp_obj_type_t *type_p,
                             mp_uint_t n_args,
                             mp_uint_t n_kw,
                             const mp_obj_t *args_p)
{
    struct pin_obj_t *self_p;
    mp_map_t kwargs;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Create a new Pin object. */
    self_p = m_new0(struct pin_obj_t, 1);
    self_p->base.type = &pin_class;

    /* Initialize the pin if pin and mode are given. */
    if (n_args == 2) {
        mp_map_init(&kwargs, 0);
        pin_obj_init_helper(self_p, n_args, args_p, &kwargs);
    }

    return (self_p);
}

/**
 * def set_mode(self, mode)
 */
static mp_obj_t pin_obj_set_mode(mp_obj_t self_in, mp_obj_t mode_in)
{
    return (mp_const_none);
}

/**
 * def read(self)
 */
static mp_obj_t pin_obj_read(mp_obj_t self_in)
{
    struct pin_obj_t *self_p;

    self_p = self_in;

    return (MP_OBJ_NEW_SMALL_INT(pin_read(&self_p->drv)));
}

/**
 * def write(self, value)
 */
static mp_obj_t pin_obj_write(mp_obj_t self_in, mp_obj_t value_in)
{
    struct pin_obj_t *self_p;

    self_p = self_in;
    pin_write(&self_p->drv, mp_obj_get_int(value_in));

    return (mp_const_none);
}

/**
 * def toggle(self)
 */
static mp_obj_t pin_obj_toggle(mp_obj_t self_in)
{
    struct pin_obj_t *self_p;

    self_p = self_in;
    pin_toggle(&self_p->drv);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_KW(pin_obj_init_fun_obj, 1, pin_obj_init);
static MP_DEFINE_CONST_FUN_OBJ_2(pin_obj_set_mode_fun_obj, pin_obj_set_mode);
static MP_DEFINE_CONST_FUN_OBJ_1(pin_obj_read_fun_obj, pin_obj_read);
static MP_DEFINE_CONST_FUN_OBJ_2(pin_obj_write_fun_obj, pin_obj_write);
static MP_DEFINE_CONST_FUN_OBJ_1(pin_obj_toggle_fun_obj, pin_obj_toggle);

static const mp_map_elem_t pin_locals_dict_table[] = {
    /* Instance methods. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_init), (mp_obj_t)&pin_obj_init_fun_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read), (mp_obj_t)&pin_obj_read_fun_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write), (mp_obj_t)&pin_obj_write_fun_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_toggle), (mp_obj_t)&pin_obj_toggle_fun_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_set_mode), (mp_obj_t)&pin_obj_set_mode_fun_obj },
};

static MP_DEFINE_CONST_DICT(pin_locals_dict, pin_locals_dict_table);

const mp_obj_type_t pin_class = {
    { &mp_type_type },
    .name = MP_QSTR_Pin,
    .print = pin_print,
    .make_new = pin_make_new,
    .locals_dict = (mp_obj_t)&pin_locals_dict,
};

/**
 * Function called when the module is imported.
 */
static mp_obj_t mod_init(void)
{
    pin_module_init();

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(mod_init_obj, mod_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_map_elem_t module_pin_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_pin) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&mod_init_obj },

    /* Pin class. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&pin_class },

    /* Module constants. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_INPUT), MP_OBJ_NEW_SMALL_INT(PIN_INPUT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_OUTPUT), MP_OBJ_NEW_SMALL_INT(PIN_OUTPUT) },
};

static MP_DEFINE_CONST_DICT(module_pin_globals, module_pin_globals_table);

const mp_obj_module_t module_pin = {
    { &mp_type_module },
    .name = MP_QSTR_pin,
    .globals = (mp_obj_t)&module_pin_globals,
};
