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

#if CONFIG_PUMBAA_CLASS_EXTI == 1

/**
 * Enternal interrupt callback. Called from an interrupt.
 */
static void exti_cb_isr(void *self_in)
{
    struct class_exti_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (self_p->callback != mp_const_none) {
        mp_call_function_0(self_p->callback);
    }

    if (self_p->chan_type == class_exti_chan_type_event_t) {
        event_write_isr(&self_p->chan.event.obj_p->event,
                        &self_p->chan.event.mask,
                        sizeof(self_p->chan.event.mask));
    } else if (self_p->chan_type == class_exti_chan_type_queue_t) {
        queue_write_isr(&self_p->chan.queue.obj_p->queue,
                        self_p->chan.queue.buf_p,
                        self_p->chan.queue.size);
    }
}

/**
 * Print the exti object.
 */
static void class_exti_print(const mp_print_t *print_p,
                              mp_obj_t self_in,
                              mp_print_kind_t kind)
{
    struct class_exti_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Exti object. If additional arguments are given, they
 * are used to initialise the exti.
 */
static mp_obj_t class_exti_make_new(const mp_obj_type_t *type_p,
                                     mp_uint_t n_args,
                                     mp_uint_t n_kw,
                                     const mp_obj_t *args_p)
{
    struct class_exti_t *self_p;
    mp_map_t kwargs;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_trigger, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_channel, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_data, MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_callback, MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int device;
    int trigger;
    mp_buffer_info_t buffer_info;

    mp_arg_check_num(n_args, n_kw, 0, 5, true);

    /* Parse args. */
    mp_map_init_fixed_table(&kwargs, n_kw, args_p + n_args);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Validate the device. */
    device = args[0].u_int;

    if ((device < 0) || (device >= EXTI_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                                                "bad exti device %d",
                                                device));
    }

    /* Validate the trigger. */
    trigger = args[1].u_int;

    if (!((trigger == EXTI_TRIGGER_RISING_EDGE)
          || (trigger == EXTI_TRIGGER_FALLING_EDGE)
          || (trigger == EXTI_TRIGGER_BOTH_EDGES))) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                                                "bad trigger %d",
                                                trigger));
    }

    /* Create a new Exti object. */
    self_p = m_new_obj(struct class_exti_t);
    self_p->base.type = &module_drivers_class_exti;

    /* Third argument must be an event object, queue object or
       None. */
    if (mp_obj_get_type(args[2].u_obj) == mp_const_none) {
        self_p->chan_type = class_exti_chan_type_none_t;
    } else if (mp_obj_get_type(args[2].u_obj) == &module_sync_class_event) {
        self_p->chan_type = class_exti_chan_type_event_t;
        self_p->chan.event.obj_p = args[2].u_obj;
        self_p->chan.event.mask = mp_obj_get_int(args[3].u_obj);
    } else if (mp_obj_get_type(args[2].u_obj) == &module_sync_class_queue) {
        self_p->chan_type = class_exti_chan_type_queue_t;
        self_p->chan.queue.obj_p = args[2].u_obj;
        self_p->chan.queue.str_obj = args[3].u_obj;

        mp_get_buffer_raise(MP_OBJ_TO_PTR(args[3].u_obj),
                            &buffer_info,
                            MP_BUFFER_READ);

        self_p->chan.queue.buf_p = buffer_info.buf;
        self_p->chan.queue.size = buffer_info.len;
    } else {
        mp_raise_TypeError("expected <class 'Event'> or <class 'Queue'>");
    }

    /* Fifth argument must be a callback or None. */
    if (args[4].u_obj != mp_const_none) {
        if (!mp_obj_is_callable(args[4].u_obj)) {
            mp_raise_TypeError("bad callback");
        }
    }

    self_p->callback = args[4].u_obj;

    if (exti_init(&self_p->exti,
                  &exti_device[device],
                  trigger,
                  exti_cb_isr,
                  self_p) != 0) {
        return (mp_const_none);
    }

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_exti_start(mp_obj_t self_in)
{
    struct class_exti_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    exti_start(&self_p->exti);

    return (mp_const_none);
}

/**
 * def stop(self)
 */
static mp_obj_t class_exti_stop(mp_obj_t self_in)
{
    struct class_exti_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    exti_stop(&self_p->exti);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_exti_start_obj, class_exti_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_exti_stop_obj, class_exti_stop);

static const mp_rom_map_elem_t class_exti_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_exti_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_exti_stop_obj) },

    /* Module constants. */
    { MP_ROM_QSTR(MP_QSTR_RISING), MP_ROM_INT(EXTI_TRIGGER_RISING_EDGE) },
    { MP_ROM_QSTR(MP_QSTR_FALLING), MP_ROM_INT(EXTI_TRIGGER_FALLING_EDGE) },
    { MP_ROM_QSTR(MP_QSTR_BOTH), MP_ROM_INT(EXTI_TRIGGER_BOTH_EDGES) }
};

static MP_DEFINE_CONST_DICT(class_exti_locals_dict, class_exti_locals_dict_table);

/**
 * Exti class type.
 */
const mp_obj_type_t module_drivers_class_exti = {
    { &mp_type_type },
    .name = MP_QSTR_Exti,
    .print = class_exti_print,
    .make_new = class_exti_make_new,
    .locals_dict = (mp_obj_t)&class_exti_locals_dict,
};

#endif
