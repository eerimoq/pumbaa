/**
 * @file modevent.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

extern const mp_obj_type_t module_event_class_event;

/**
 * Print the event object.
 */
static void class_event_print(const mp_print_t *print_p,
                              mp_obj_t self_in,
                              mp_print_kind_t kind)
{
    struct class_event_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Event object associated with the id. If additional
 * arguments are given, they are used to initialise the event. See
 * `init`.
 */
static mp_obj_t class_event_make_new(const mp_obj_type_t *type_p,
                                     mp_uint_t n_args,
                                     mp_uint_t n_kw,
                                     const mp_obj_t *args_p)
{
    struct class_event_t *self_p;

    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    /* Create a new Event object. */
    self_p = m_new0(struct class_event_t, 1);
    self_p->base.type = &module_event_class_event;

    /* Initialize the event if event and mode are given. */
    event_init(&self_p->event);

    return (self_p);
}

/**
 * Object initialization function.
 *
 * def __init__(self, event, mode)
 */
static mp_obj_t class_event_init(mp_uint_t n_args,
                                 const mp_obj_t *args_p,
                                 mp_map_t *kwargs_p)
{
    struct class_event_t *self_p;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    event_init(&self_p->event);

    return (mp_const_none);
}

/**
 * def read(self)
 */
static mp_obj_t class_event_read(size_t n_args, const mp_obj_t *args_p)
{
    struct class_event_t *self_p;
    uint32_t mask;

    mask = 0x1;
    self_p = MP_OBJ_TO_PTR(args_p[0]);

    if (event_read(&self_p->event, &mask, sizeof(mask)) != sizeof(mask)) {
        mp_not_implemented("failed to read an event");
    }
    
    return (MP_OBJ_NEW_SMALL_INT(mask));
}

/**
 * def write(self, value)
 */
static mp_obj_t class_event_write(mp_obj_t self_in, mp_obj_t value_in)
{
    struct class_event_t *self_p;
    uint32_t mask;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    mask = mp_obj_get_int(value_in);
    event_write(&self_p->event, &mask, sizeof(mask));

    return (mp_const_none);
}

/**
 * def size(self)
 */
static mp_obj_t class_event_size(mp_obj_t self_in)
{
    struct class_event_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    return (MP_OBJ_NEW_SMALL_INT(event_size(&self_p->event)));
}

static MP_DEFINE_CONST_FUN_OBJ_KW(class_event_init_obj, 1, class_event_init);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_event_read_obj, 1, 2, class_event_read);
static MP_DEFINE_CONST_FUN_OBJ_2(class_event_write_obj, class_event_write);
static MP_DEFINE_CONST_FUN_OBJ_1(class_event_size_obj, class_event_size);

static const mp_map_elem_t class_event_locals_dict_table[] = {
    /* Instance methods. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_init), (mp_obj_t)&class_event_init_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read), (mp_obj_t)&class_event_read_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write), (mp_obj_t)&class_event_write_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_size), (mp_obj_t)&class_event_size_obj },
};

static MP_DEFINE_CONST_DICT(class_event_locals_dict, class_event_locals_dict_table);

/**
 * Event class type.
 */
const mp_obj_type_t module_event_class_event = {
    { &mp_type_type },
    .name = MP_QSTR_Event,
    .print = class_event_print,
    .make_new = class_event_make_new,
    .locals_dict = (mp_obj_t)&class_event_locals_dict,
};

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_map_elem_t module_event_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_event) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&module_init_obj },

    /* Event class. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Event), (mp_obj_t)&module_event_class_event },
};

static MP_DEFINE_CONST_DICT(module_event_globals, module_event_globals_table);

const mp_obj_module_t module_event = {
    { &mp_type_module },
    .name = MP_QSTR_event,
    .globals = (mp_obj_t)&module_event_globals,
};
