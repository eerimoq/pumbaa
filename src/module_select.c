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

#define CHAN_POLLIN   1
#define CHAN_POLLHUP  2

extern const mp_obj_type_t module_socket_class_socket;
extern const mp_obj_type_t module_ssl_class_ssl_socket;

struct class_chan_t {
    mp_obj_base_t base;
    struct chan_t chan;
};

struct class_poll_t {
    mp_obj_base_t base;
    struct chan_list_t list;
    int workspace[32];
};

/**
 * Returns true(1) if the object is a Simba channel, otherwise
 * false(0).
 */
static int is_channel(mp_obj_t obj)
{
    return (MP_OBJ_IS_TYPE(obj, &module_sync_class_event)
            || MP_OBJ_IS_TYPE(obj, &module_sync_class_queue)
#if CONFIG_PUMBAA_MODULE_SOCKET == 1
            || MP_OBJ_IS_TYPE(obj, &module_socket_class_socket)
#endif
#if CONFIG_PUMBAA_CLASS_CAN == 1
            || MP_OBJ_IS_TYPE(obj, &module_drivers_class_can)
#endif
#if CONFIG_PUMBAA_CLASS_UART == 1
            || MP_OBJ_IS_TYPE(obj, &module_drivers_class_uart)
#endif
            );
}

/**
 * def register(obj[, eventmask])
 */
static mp_obj_t poll_register(size_t n_args, const mp_obj_t *args_p)
{
    struct class_poll_t *self_p;
    mp_obj_t chan;
    struct class_chan_t *chan_p;

    chan = args_p[1];

    /* The object must be a channel. */
    if (is_channel(chan) == 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "channel object required"));
    }

    /* Add the channel to the poll list. */
    self_p = MP_OBJ_TO_PTR(args_p[0]);
    chan_p = MP_OBJ_TO_PTR(chan);

    if (chan_list_add(&self_p->list, &chan_p->chan) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "cannot register channel"));
    }

    return (mp_const_none);
}

/**
 * def unregister(obj)
 */
static mp_obj_t poll_unregister(mp_obj_t self_in, mp_obj_t obj_in)
{
    struct class_poll_t *self_p;
    struct class_chan_t *chan_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    chan_p = MP_OBJ_TO_PTR(obj_in);

    if (chan_list_remove(&self_p->list, &chan_p->chan) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "cannot unregister channel"));
    }

    return (mp_const_none);
}

/**
 * def modify(obj, eventmask)
 */
static mp_obj_t poll_modify(mp_obj_t self_in, mp_obj_t obj_in, mp_obj_t eventmask_in)
{
    return (mp_const_none);
}

/**
 * def poll([timeout])
 */
static mp_obj_t poll_poll(size_t n_args, const mp_obj_t *args)
{
    struct class_poll_t *self_p;
    mp_obj_t tuple_items[2];
    mp_obj_t list_items[1];
    float f_timeout;
    struct time_t timeout;
    struct time_t *timeout_p;
    void *chan_p;

    self_p = MP_OBJ_TO_PTR(args[0]);
    timeout_p = NULL;

    /* Parse the timeout argument. */
    if (n_args >= 2) {
        if (args[1] != mp_const_none) {
            f_timeout = mp_obj_get_float(args[1]);
            timeout.seconds = (long)f_timeout;
            timeout.nanoseconds = (f_timeout - timeout.seconds) * 1000000000L;
            timeout_p = &timeout;
        }
    }

    /* Poll the list of channel(s) waiting for an event or timeout (if
       given). */
    chan_p = chan_list_poll(&self_p->list, timeout_p);

    /* A timeout occured if NULL is returned. Return the empty
       list. */
    if (chan_p == NULL) {
        return (mp_obj_new_list(0, NULL));
    }

    /* No timeout. Return the a list containing the one channel with
       an event. */
    tuple_items[0] = container_of(chan_p, struct class_chan_t, chan);
    tuple_items[1] = MP_OBJ_NEW_SMALL_INT(CHAN_POLLIN);
    list_items[0] = mp_obj_new_tuple(2, &tuple_items[0]);

    return (mp_obj_new_list(1, &list_items[0]));
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(poll_register_obj, 2, 3, poll_register);
MP_DEFINE_CONST_FUN_OBJ_2(poll_unregister_obj, poll_unregister);
MP_DEFINE_CONST_FUN_OBJ_3(poll_modify_obj, poll_modify);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(poll_poll_obj, 1, 3, poll_poll);

static const mp_rom_map_elem_t class_poll_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_register), MP_ROM_PTR(&poll_register_obj) },
    { MP_ROM_QSTR(MP_QSTR_unregister), MP_ROM_PTR(&poll_unregister_obj) },
    { MP_ROM_QSTR(MP_QSTR_modify), MP_ROM_PTR(&poll_modify_obj) },
    { MP_ROM_QSTR(MP_QSTR_poll), MP_ROM_PTR(&poll_poll_obj) },
};

static MP_DEFINE_CONST_DICT(class_poll_locals_dict, class_poll_locals_dict_table);

static const mp_obj_type_t class_poll = {
    { &mp_type_type },
    .name = MP_QSTR_poll,
    .locals_dict = (void*)&class_poll_locals_dict,
};

static mp_obj_t select_poll(void)
{
    struct class_poll_t *poll_p;
    poll_p = m_new_obj(struct class_poll_t);
    poll_p->base.type = &class_poll;

    if (chan_list_init(&poll_p->list,
                       &poll_p->workspace[0],
                       sizeof(poll_p->workspace)) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (poll_p);
}

MP_DEFINE_CONST_FUN_OBJ_0(mp_select_poll_obj, select_poll);

static const mp_rom_map_elem_t mp_module_select_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uselect) },
    { MP_ROM_QSTR(MP_QSTR_poll), MP_ROM_PTR(&mp_select_poll_obj) },
    { MP_ROM_QSTR(MP_QSTR_POLLIN), MP_ROM_INT(CHAN_POLLIN) },
    { MP_ROM_QSTR(MP_QSTR_POLLHUP), MP_ROM_INT(CHAN_POLLHUP) },
};

static MP_DEFINE_CONST_DICT(mp_module_select_globals, mp_module_select_globals_table);

const mp_obj_module_t mp_module_uselect = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_select_globals,
};
