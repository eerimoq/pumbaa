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

#define FLAGS_EXTENDED_FRAME                    0x1

/**
 * CAN frame fields.
 */
static const qstr can_frame_fields[] = {
    MP_QSTR_id,
    MP_QSTR_data,
    MP_QSTR_flags
};

/**
 * Print the can object.
 */
static void class_can_print(const mp_print_t *print_p,
                            mp_obj_t self_in,
                            mp_print_kind_t kind)
{
    struct class_can_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Can object associated with the id. If additional
 * arguments are given, they are used to initialise the can. See
 * `init`.
 */
static mp_obj_t class_can_make_new(const mp_obj_type_t *type_p,
                                   mp_uint_t n_args,
                                   mp_uint_t n_kw,
                                   const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_speed, MP_ARG_INT, { .u_int = CAN_SPEED_500KBPS } }
    };
    struct class_can_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int device;
    int speed;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Create a new can object. */
    self_p = m_new_obj(struct class_can_t);
    self_p->base.type = &module_drivers_class_can;

    /* Device argument. */
    device = args[0].u_int;

    if ((device < 0) || (device > CAN_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad device"));
    }

    /* Speed argument. */
    speed = args[1].u_int;

    if (can_init(&self_p->drv,
                 &can_device[device],
                 speed,
                 &self_p->rxbuf[0],
                 sizeof(self_p->rxbuf)) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "can_init() failed"));
    }

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_can_start(mp_obj_t self_in)
{
    struct class_can_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    can_start(&self_p->drv);

    return (mp_const_none);
}

/**
 * def stop(self)
 */
static mp_obj_t class_can_stop(mp_obj_t self_in)
{
    struct class_can_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    can_stop(&self_p->drv);

    return (mp_const_none);
}

/**
 * def read(self)
 */
static mp_obj_t class_can_read(mp_obj_t self_in)
{
    struct class_can_t *self_p;
    struct can_frame_t frame;
    mp_obj_t tuple[3];
    int flags;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (can_read(&self_p->drv, &frame, sizeof(frame)) != sizeof(frame)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "can_read() failed"));
    }

    flags = 0;

    if (frame.extended_id == 1) {
        flags |= FLAGS_EXTENDED_FRAME;
    }

    tuple[0] = MP_OBJ_NEW_SMALL_INT(frame.id);
    tuple[1] = mp_obj_new_bytes((const byte *)&frame.data, frame.size);
    tuple[2] = MP_OBJ_NEW_SMALL_INT(flags);

    return (mp_obj_new_attrtuple(&can_frame_fields[0], 3, tuple));
}

/**
 * def write(self, id, data[, flags])
 */
static mp_obj_t class_can_write(mp_uint_t n_args, const mp_obj_t *args_p)
{
    struct class_can_t *self_p;
    struct can_frame_t frame;
    mp_buffer_info_t buffer_info;
    int id;
    int flags;

    self_p = MP_OBJ_TO_PTR(args_p[0]);

    /* Id arguement. */
    id = mp_obj_get_int(args_p[1]);

    if ((id < 0) || (id > 0x7ff)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad frame id"));
    }

    /* Data arguement. */
    mp_get_buffer_raise(args_p[2], &buffer_info, MP_BUFFER_READ);

    if (buffer_info.len > 8) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad frame data length"));
    }

    /* Flags argument. */
    if (n_args != 4) {
        flags = 0;
    } else {
        flags = mp_obj_get_int(args_p[3]);
    }

    /* Initiate the frame. */
    memset(&frame, 0, sizeof(frame));
    frame.id = id;
    frame.extended_id = (flags & FLAGS_EXTENDED_FRAME);
    frame.size = buffer_info.len;
    memcpy(&frame.data, buffer_info.buf, buffer_info.len);

    if (can_write(&self_p->drv, &frame, sizeof(frame)) != sizeof(frame)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "can_write() failed"));
    }

    return (mp_const_none);
}

/**
 * def read(self)
 */
static mp_obj_t class_can_size(mp_obj_t self_in)
{
    struct class_can_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    return (MP_OBJ_NEW_SMALL_INT(chan_size(&self_p->drv)));
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_can_start_obj, class_can_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_can_stop_obj, class_can_stop);
static MP_DEFINE_CONST_FUN_OBJ_1(class_can_read_obj, class_can_read);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_can_write_obj, 3, 4, class_can_write);
static MP_DEFINE_CONST_FUN_OBJ_1(class_can_size_obj, class_can_size);

static const mp_rom_map_elem_t class_can_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_can_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_can_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_can_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_can_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_size), MP_ROM_PTR(&class_can_size_obj) },

    /* Class constants. */
    /* { MP_ROM_QSTR(MP_QSTR_SPEED_1MBPS), MP_ROM_INT(CAN_SPEED_1MBPS) }, */
    { MP_ROM_QSTR(MP_QSTR_SPEED_500KBPS), MP_ROM_INT(CAN_SPEED_500KBPS) },
    /* { MP_ROM_QSTR(MP_QSTR_SPEED_250KBPS), MP_ROM_INT(CAN_SPEED_250KBPS) } */

    { MP_ROM_QSTR(MP_QSTR_FLAGS_EXTENDED_FRAME), MP_ROM_INT(FLAGS_EXTENDED_FRAME) },
};

static MP_DEFINE_CONST_DICT(class_can_locals_dict, class_can_locals_dict_table);

/**
 * Can class type.
 */
const mp_obj_type_t module_drivers_class_can = {
    { &mp_type_type },
    .name = MP_QSTR_Can,
    .print = class_can_print,
    .make_new = class_can_make_new,
    .locals_dict = (mp_obj_t)&class_can_locals_dict,
};
