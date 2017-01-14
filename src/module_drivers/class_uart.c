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

#if CONFIG_PUMBAA_CLASS_UART == 1

/**
 * Print the uart object.
 */
static void class_uart_print(const mp_print_t *print_p,
                             mp_obj_t self_in,
                             mp_print_kind_t kind)
{
    struct class_uart_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Uart object associated with the id. If additional
 * arguments are given, they are used to initialise the uart. See
 * `init`.
 */
static mp_obj_t class_uart_make_new(const mp_obj_type_t *type_p,
                                    mp_uint_t n_args,
                                    mp_uint_t n_kw,
                                    const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_baudrate, MP_ARG_INT, { .u_int = 115200 } }
    };
    struct class_uart_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int device;
    int baudrate;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Create a new uart object. */
    self_p = m_new_obj(struct class_uart_t);
    self_p->base.type = &module_drivers_class_uart;

    /* Device argument. */
    device = args[0].u_int;

    if ((device < 0) || (device >= UART_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad device"));
    }

    /* Baudrate argument. */
    baudrate = args[1].u_int;

    if (uart_init(&self_p->drv,
                  &uart_device[device],
                  baudrate,
                  &self_p->rxbuf[0],
                  sizeof(self_p->rxbuf)) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "uart_init() failed"));
    }

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_uart_start(mp_obj_t self_in)
{
    struct class_uart_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    uart_start(&self_p->drv);

    return (mp_const_none);
}

/**
 * def stop(self)
 */
static mp_obj_t class_uart_stop(mp_obj_t self_in)
{
    struct class_uart_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    uart_stop(&self_p->drv);

    return (mp_const_none);
}

/**
 * def read(self, size)
 */
static mp_obj_t class_uart_read(mp_obj_t self_in,
                                mp_obj_t size_in)
{
    struct class_uart_t *self_p;
    vstr_t vstr;
    size_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    size = mp_obj_get_int(size_in);

    vstr_init_len(&vstr, size);

    if (uart_read(&self_p->drv, vstr.buf, size) != size) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "uart_read() failed"));
    }

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
}

/**
 * def read_into(self, buffer[, size])
 */
static mp_obj_t class_uart_read_into(mp_uint_t n_args,
                                     const mp_obj_t *args_p)
{
    struct class_uart_t *self_p;
    mp_buffer_info_t buffer_info;
    size_t size;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args_p[1]),
                        &buffer_info,
                        MP_BUFFER_WRITE);

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

    if (uart_read(&self_p->drv, buffer_info.buf, size) != size) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "uart_read_into() failed"));
    }

    return (MP_OBJ_NEW_SMALL_INT(size));
}

/**
 * def write(self, buffer[, size])
 */
static mp_obj_t class_uart_write(mp_uint_t n_args,
                                 const mp_obj_t *args_p)
{
    struct class_uart_t *self_p;
    mp_buffer_info_t buffer_info;
    size_t size;
    void *buf_p;

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

#if defined(FAMILY_SAM)
    /* The data must be in RAM for the SAM UART driver DMA
       implementation. */
    buf_p = m_malloc(size);
    memcpy(buf_p, buffer_info.buf, size);
#else
    buf_p = buffer_info.buf;
#endif

    if (uart_write(&self_p->drv, buf_p, size) != size) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "uart_write() failed"));
    }

    return (MP_OBJ_NEW_SMALL_INT(size));
}

/**
 * def read(self)
 */
static mp_obj_t class_uart_size(mp_obj_t self_in)
{
    struct class_uart_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    return (MP_OBJ_NEW_SMALL_INT(queue_size(&self_p->drv.chin)));
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_uart_start_obj, class_uart_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_uart_stop_obj, class_uart_stop);
static MP_DEFINE_CONST_FUN_OBJ_2(class_uart_read_obj, class_uart_read);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_uart_read_into_obj,
                                           2,
                                           3,
                                           class_uart_read_into);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_uart_write_obj,
                                           2,
                                           3,
                                           class_uart_write);
static MP_DEFINE_CONST_FUN_OBJ_1(class_uart_size_obj, class_uart_size);

static const mp_rom_map_elem_t class_uart_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_uart_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_uart_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_uart_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_into), MP_ROM_PTR(&class_uart_read_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_uart_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_size), MP_ROM_PTR(&class_uart_size_obj) },
};

static MP_DEFINE_CONST_DICT(class_uart_locals_dict, class_uart_locals_dict_table);

/**
 * Uart class type.
 */
const mp_obj_type_t module_drivers_class_uart = {
    { &mp_type_type },
    .name = MP_QSTR_Uart,
    .print = class_uart_print,
    .make_new = class_uart_make_new,
    .locals_dict = (mp_obj_t)&class_uart_locals_dict,
};

#endif
