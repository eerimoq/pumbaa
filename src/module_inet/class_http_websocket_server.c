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
    struct class_http_server_connection_t *connection_p;
    mp_map_t kwargs;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_connection, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_request, MP_ARG_REQUIRED | MP_ARG_OBJ }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_check_num(n_args, n_kw, 0, 2, true);

    /* Parse args. */
    mp_map_init_fixed_table(&kwargs, n_kw, args_p + n_args);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Parse the arguments. */
    connection_p = args[0].u_obj;

    /* Create a new HttpWebsocketServer object. */
    self_p = m_new_obj(struct class_http_websocket_server_t);
    self_p->base.type = &module_inet_class_http_websocket_server;

    if (http_websocket_server_init(&self_p->http_websocket_server,
                                   &connection_p->connection_p->socket) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "http_websocket_server_init() failed"));
    }

    /* Perform the handshake. */
    if (http_websocket_server_handshake(&self_p->http_websocket_server,
                                        connection_p->request_p) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "handshake failed"));
    }

    return (self_p);
}

/**
 * def read(self[, size])
 */
static mp_obj_t class_http_websocket_server_read(mp_uint_t n_args,
                                                 const mp_obj_t *args_p)
{
    struct class_http_websocket_server_t *self_p;
    vstr_t vstr;
    size_t size;
    ssize_t res;
    int type;

    self_p = MP_OBJ_TO_PTR(args_p[0]);

    /* Size arguement. */
    if (n_args < 2) {
        size = 512;
    } else {
        size = mp_obj_get_int(args_p[1]);
    }

    vstr_init_len(&vstr, size);
    res = http_websocket_server_read(&self_p->http_websocket_server,
                                     &type,
                                     vstr.buf,
                                     size);

    if (res > 0) {
        vstr.len = res;

        return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
    } else {
        return (mp_const_none);
    }
}

/**
 * def read_into(self, buffer[, size])
 */
static mp_obj_t class_http_websocket_server_read_into(mp_uint_t n_args,
                                                      const mp_obj_t *args_p)
{
    struct class_http_websocket_server_t *self_p;
    mp_buffer_info_t buffer_info;
    size_t size;
    ssize_t res;
    int type;

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

    res = http_websocket_server_read(&self_p->http_websocket_server,
                                     &type,
                                     buffer_info.buf,
                                     size);

    return (MP_OBJ_NEW_SMALL_INT(res));
}

/**
 * def write(self, buffer[, size])
 */
static mp_obj_t class_http_websocket_server_write(mp_uint_t n_args,
                                                  const mp_obj_t *args_p)
{
    struct class_http_websocket_server_t *self_p;
    mp_buffer_info_t buffer_info;
    size_t size;
    ssize_t res;
    int type;

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

    type = 0;
    res = http_websocket_server_write(&self_p->http_websocket_server,
                                      type,
                                      buffer_info.buf,
                                      size);

    return (MP_OBJ_NEW_SMALL_INT(res));
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_http_websocket_server_read_obj, 1, 2, class_http_websocket_server_read);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_http_websocket_server_read_into_obj, 2, 3, class_http_websocket_server_read_into);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_http_websocket_server_write_obj, 2, 3, class_http_websocket_server_write);

static const mp_rom_map_elem_t class_http_websocket_server_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_http_websocket_server_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_into), MP_ROM_PTR(&class_http_websocket_server_read_into_obj) },
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
