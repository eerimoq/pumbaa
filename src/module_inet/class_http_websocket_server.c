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
 * Print the http_websocket_server object.
 */
static void class_http_websocket_server_print(const mp_print_t *print_p,
                                              mp_obj_t self_in,
                                              mp_print_kind_t kind)
{
    struct class_http_websocket_server_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new HttpWebsocketServer object associated with the id. If
 * additional arguments are given, they are used to initialise the
 * http_websocket_server. See `init`.
 */
static mp_obj_t class_http_websocket_server_make_new(const mp_obj_type_t *type_p,
                                                     mp_uint_t n_args,
                                                     mp_uint_t n_kw,
                                                     const mp_obj_t *args_p)
{
    struct class_http_websocket_server_t *self_p;
    mp_map_t kwargs;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_socket, MP_ARG_REQUIRED | MP_ARG_OBJ }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_check_num(n_args, n_kw, 0, 1, true);

    /* Parse args. */
    mp_map_init_fixed_table(&kwargs, n_kw, args_p + n_args);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Parse the arguments. */

    /* Create a new Http_Websocket_Server object. */
    self_p = m_new_obj(struct class_http_websocket_server_t);
    self_p->base.type = &module_inet_class_http_websocket_server;

    if (http_websocket_server_init(&self_p->http_websocket_server,
                                   NULL) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "http_websocket_server_init() failed"));
    }

    return (self_p);
}

/**
 * def handshake(self, request)
 */
static mp_obj_t class_http_websocket_server_handshake(mp_obj_t self_in)
{
    return (mp_const_none);
}

/**
 * def read(self, size)
 */
static mp_obj_t class_http_websocket_server_read(mp_obj_t self_in)
{
    return (mp_const_none);
}

/**
 * def write(self, buffer[, size])
 */
static mp_obj_t class_http_websocket_server_write(mp_obj_t self_in)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_http_websocket_server_handshake_obj, class_http_websocket_server_handshake);
static MP_DEFINE_CONST_FUN_OBJ_1(class_http_websocket_server_read_obj, class_http_websocket_server_read);
static MP_DEFINE_CONST_FUN_OBJ_1(class_http_websocket_server_write_obj, class_http_websocket_server_write);

static const mp_rom_map_elem_t class_http_websocket_server_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_handshake), MP_ROM_PTR(&class_http_websocket_server_handshake_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_http_websocket_server_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_http_websocket_server_write_obj) },
};

static MP_DEFINE_CONST_DICT(class_http_websocket_server_locals_dict, class_http_websocket_server_locals_dict_table);

/**
 * Http_Websocket_Server class type.
 */
const mp_obj_type_t module_inet_class_http_websocket_server = {
    { &mp_type_type },
    .name = MP_QSTR_Http_Websocket_Server,
    .print = class_http_websocket_server_print,
    .make_new = class_http_websocket_server_make_new,
    .locals_dict = (mp_obj_t)&class_http_websocket_server_locals_dict,
};
