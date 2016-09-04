/**
 * @file modtimer.c
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

/**
 * class Timer(object):
 *
 *     def __init__(self, timeout, event, mask=-1, flags=0)
 *
 *     def start(self)
 *
 *     def stop(self)
 */
struct class_timer_t {
    mp_obj_base_t base;
    struct timer_t timer;
    struct class_event_t *event_obj_p;
    uint32_t mask;
};

extern const mp_obj_type_t module_timer_class_timer;

static void timer_cb_isr(void *self_in)
{
    struct class_timer_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);    
    event_write_isr(&self_p->event_obj_p->event,
                    &self_p->mask,
                    sizeof(self_p->mask));
}

/**
 * Print the timer object.
 */
static void class_timer_print(const mp_print_t *print_p,
                              mp_obj_t self_in,
                              mp_print_kind_t kind)
{
    struct class_timer_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Init helper function.
 */
static mp_obj_t class_timer_init_helper(struct class_timer_t *self_p,
                                        mp_uint_t n_args,
                                        const mp_obj_t *pos_args_p,
                                        mp_map_t *kwargs_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_timeout, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_event, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_mask, MP_ARG_INT, { .u_int = 0xffffffff } },
        { MP_QSTR_flags, MP_ARG_INT, { .u_int = 0x0 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    struct time_t timeout;
    int flags;
    
    /* Parse args. */
    mp_arg_parse_all(n_args,
                     pos_args_p,
                     kwargs_p,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Second argument must be an event object. */
    if (mp_obj_get_type(args[1].u_obj) != &module_event_class_event) {
        mp_raise_TypeError("expected <class 'Event'>");
    }
    
    timeout.seconds = args[0].u_int;
    timeout.nanoseconds = 0;
    self_p->event_obj_p = args[1].u_obj;
    self_p->mask = args[2].u_int;
    flags = args[3].u_int;
    
    timer_init(&self_p->timer, &timeout, timer_cb_isr, self_p, flags);

    return (mp_const_none);
}

/**
 * Create a new Timer object associated with the id. If additional
 * arguments are given, they are used to initialise the timer. See
 * `init`.
 */
static mp_obj_t class_timer_make_new(const mp_obj_type_t *type_p,
                                     mp_uint_t n_args,
                                     mp_uint_t n_kw,
                                     const mp_obj_t *args_p)
{
    struct class_timer_t *self_p;
    mp_map_t kwargs;
    
    mp_arg_check_num(n_args, n_kw, 0, 4, true);

    /* Create a new Timer object. */
    self_p = m_new0(struct class_timer_t, 1);
    self_p->base.type = &module_timer_class_timer;

    /* Initialize the object. */
    mp_map_init_fixed_table(&kwargs, n_kw, args_p + n_args);
    class_timer_init_helper(self_p, n_args, args_p, &kwargs);

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_timer_start(mp_obj_t self_in)
{
    struct class_timer_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    timer_start(&self_p->timer);

    return (mp_const_none);
}

/**
 * def stop(self)
 */
static mp_obj_t class_timer_stop(mp_obj_t self_in)
{
    struct class_timer_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    timer_stop(&self_p->timer);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_timer_start_obj, class_timer_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_timer_stop_obj, class_timer_stop);

static const mp_map_elem_t class_timer_locals_dict_table[] = {
    /* Instance methods. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_start), (mp_obj_t)&class_timer_start_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_stop), (mp_obj_t)&class_timer_stop_obj },
};

static MP_DEFINE_CONST_DICT(class_timer_locals_dict, class_timer_locals_dict_table);

/**
 * Timer class type.
 */
const mp_obj_type_t module_timer_class_timer = {
    { &mp_type_type },
    .name = MP_QSTR_Timer,
    .print = class_timer_print,
    .make_new = class_timer_make_new,
    .locals_dict = (mp_obj_t)&class_timer_locals_dict,
};

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    timer_module_init();

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_map_elem_t module_timer_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_timer) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&module_init_obj },

    /* Module constants. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_PERIODIC), MP_OBJ_NEW_SMALL_INT(TIMER_PERIODIC) },

    /* Timer class. */
    { MP_OBJ_NEW_QSTR(MP_QSTR_Timer), (mp_obj_t)&module_timer_class_timer },
};

static MP_DEFINE_CONST_DICT(module_timer_globals, module_timer_globals_table);

const mp_obj_module_t module_timer = {
    { &mp_type_module },
    .name = MP_QSTR_timer,
    .globals = (mp_obj_t)&module_timer_globals,
};
