/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017, Erik Moqvist
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

extern const mp_obj_type_t module_ssl_class_ssl_context;

#if CONFIG_PUMBAA_CLASS_HTTP_SERVER == 1

static void response_write(struct class_http_server_connection_t *connection_obj_p,
                           mp_obj_t response_obj)
{
    mp_obj_tuple_t *tuple_p;
    struct http_server_response_t response;
    mp_buffer_info_t buffer_info;

    if (mp_obj_get_type(response_obj) != &mp_type_tuple) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "expected tuple"));
    }

    tuple_p = MP_OBJ_TO_PTR(response_obj);

    if (tuple_p->len == 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "expected tuple of length >0"));
    }

    if (mp_obj_get_type(tuple_p->items[0]) == &mp_type_int) {
        response.content.buf_p = NULL;
        response.content.size = mp_obj_get_int(tuple_p->items[0]);
    } else {
        mp_get_buffer_raise(tuple_p->items[0],
                            &buffer_info,
                            MP_BUFFER_READ);
        response.content.buf_p = buffer_info.buf;
        response.content.size = buffer_info.len;
    }

    if (tuple_p->len > 1) {
        response.code = mp_obj_get_int(tuple_p->items[1]);
    } else {
        response.code = http_server_response_code_200_ok_t;
    }

    if (tuple_p->len > 2) {
        response.content.type = mp_obj_get_int(tuple_p->items[2]);
    } else {
        response.content.type = http_server_content_type_text_html_t;
    }

    http_server_response_write(connection_obj_p->connection_p,
                               connection_obj_p->request_p,
                               &response);
}

/**
 * def response_write(self, response)
 */
static mp_obj_t class_http_server_connection_response_write(mp_obj_t self_in,
                                                            mp_obj_t response_in)
{
    struct class_http_server_connection_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    response_write(self_p, response_in);

    return (mp_const_none);
}

/**
 * def socket_write(self, buffer)
 */
static mp_obj_t class_http_server_connection_socket_write(mp_obj_t self_in,
                                                          mp_obj_t buffer_in)
{
    struct class_http_server_connection_t *self_p;
    mp_buffer_info_t buffer_info;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(buffer_in, &buffer_info, MP_BUFFER_READ);

    size = socket_write(&self_p->connection_p->socket,
                        buffer_info.buf,
                        buffer_info.len);

    return (mp_obj_new_int(size));
}

/**
 * def socket_read(self, buffer)
 */
static mp_obj_t class_http_server_connection_socket_read(mp_obj_t self_in,
                                                         mp_obj_t size_in)
{
    struct class_http_server_connection_t *self_p;
    vstr_t vstr;
    size_t size;
    ssize_t res;

    self_p = MP_OBJ_TO_PTR(self_in);
    size = mp_obj_get_int(size_in);

    vstr_init_len(&vstr, size);
    res = socket_read(&self_p->connection_p->socket, vstr.buf, size);

    if (res >= 0) {
        vstr.len = res;
    } else {
        vstr.len = 0;
    }

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
}

static MP_DEFINE_CONST_FUN_OBJ_2(class_http_server_connection_response_write_obj,
                                 class_http_server_connection_response_write);
static MP_DEFINE_CONST_FUN_OBJ_2(class_http_server_connection_socket_write_obj,
                                 class_http_server_connection_socket_write);
static MP_DEFINE_CONST_FUN_OBJ_2(class_http_server_connection_socket_read_obj,
                                 class_http_server_connection_socket_read);

static const mp_rom_map_elem_t class_http_server_connection_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_response_write),
      MP_ROM_PTR(&class_http_server_connection_response_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_socket_write),
      MP_ROM_PTR(&class_http_server_connection_socket_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_socket_read),
      MP_ROM_PTR(&class_http_server_connection_socket_read_obj) }
};

static MP_DEFINE_CONST_DICT(class_http_server_connection_locals_dict, class_http_server_connection_locals_dict_table);

/**
 * HttpServerConnection class type.
 */
const mp_obj_type_t module_inet_class_http_server_connection = {
    { &mp_type_type },
    .name = MP_QSTR_HttpServerConnection,
    .locals_dict = (mp_obj_t)&class_http_server_connection_locals_dict,
};

/**
 * Request named tuple fields.
 */
static const qstr request_fields[] = {
    MP_QSTR_action,
    MP_QSTR_path,
    MP_QSTR_sec_websocket_key,
    MP_QSTR_content_type,
    MP_QSTR_content_length,
    MP_QSTR_authorization,
    MP_QSTR_expect
};

/**
 * All routes are handled by this function.
 */
static int on_no_route(struct http_server_connection_t *connection_p,
                       struct http_server_request_t *request_p)
{
    struct class_http_server_t *self_p;
    struct class_http_server_connection_t *connection_obj_p;
    nlr_buf_t nlr;
    mp_obj_t route_callback;
    mp_obj_t request_obj;
    mp_obj_t response_obj;
    mp_obj_t *routes_p;
    mp_obj_tuple_t *tuple_p;
    mp_obj_t tuple[7];
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_uint_t i;
    vstr_t vstr;
    size_t size;

    self_p = container_of(connection_p->self_p,
                          struct class_http_server_t,
                          http_server);

    MP_THREAD_GIL_ENTER();

    /* Create the request object (a named tuple). */
    tuple[0] = mp_obj_new_int(request_p->action);

    size = strlen(&request_p->path[0]);
    vstr_init_fixed_buf(&vstr, size + 1, &request_p->path[0]);
    vstr.len = size;
    tuple[1] = mp_obj_new_str_from_vstr(&mp_type_str, &vstr);

    if (request_p->headers.sec_websocket_key.present == 1) {
        size = strlen(&request_p->headers.sec_websocket_key.value[0]);
        vstr_init_fixed_buf(&vstr, size + 1, &request_p->headers.sec_websocket_key.value[0]);
        vstr.len = size;
        tuple[2] = mp_obj_new_str_from_vstr(&mp_type_str, &vstr);
    } else {
        tuple[2] = mp_const_none;
    }

    if (request_p->headers.content_type.present == 1) {
        size = strlen(&request_p->headers.content_type.value[0]);
        vstr_init_fixed_buf(&vstr, size + 1, &request_p->headers.content_type.value[0]);
        vstr.len = size;
        tuple[3] = mp_obj_new_str_from_vstr(&mp_type_str, &vstr);
    } else {
        tuple[3] = mp_const_none;
    }

    if (request_p->headers.content_length.present == 1) {
        tuple[4] = mp_obj_new_int(request_p->headers.content_length.value);
    } else {
        tuple[4] = mp_const_none;
    }

    if (request_p->headers.authorization.present == 1) {
        size = strlen(&request_p->headers.authorization.value[0]);
        vstr_init_fixed_buf(&vstr, size + 1, &request_p->headers.authorization.value[0]);
        vstr.len = size;
        tuple[5] = mp_obj_new_str_from_vstr(&mp_type_str, &vstr);
    } else {
        tuple[5] = mp_const_none;
    }

    if (request_p->headers.expect.present == 1) {
        size = strlen(&request_p->headers.expect.value[0]);
        vstr_init_fixed_buf(&vstr, size + 1, &request_p->headers.expect.value[0]);
        vstr.len = size;
        tuple[6] = mp_obj_new_str_from_vstr(&mp_type_str, &vstr);
    } else {
        tuple[6] = mp_const_none;
    }

    request_obj = mp_obj_new_attrtuple(&request_fields[0],
                                       membersof(tuple),
                                       tuple);

    /* Create the connection object. */
    connection_obj_p = m_new_obj(struct class_http_server_connection_t);
    connection_obj_p->base.type = &module_inet_class_http_server_connection;
    connection_obj_p->connection_p = connection_p;
    connection_obj_p->request_p = request_p;

    /* Iterate over registered routes. */
    mp_obj_list_get(self_p->routes, &len, &routes_p);
    route_callback = mp_const_none;

    for (i = 0; i < len; i++) {
        tuple_p = MP_OBJ_TO_PTR(routes_p[i]);
        mp_get_buffer_raise(tuple_p->items[0],
                            &buffer_info,
                            MP_BUFFER_READ);

        if (strncmp(buffer_info.buf,
                    &request_p->path[0],
                    buffer_info.len) == 0) {
            route_callback = tuple_p->items[1];
            break;
        }
    }

    if (route_callback == mp_const_none) {
        route_callback = self_p->no_route;
    }

    if (nlr_push(&nlr) == 0) {
        response_obj = mp_call_function_2(route_callback,
                                          MP_OBJ_FROM_PTR(connection_obj_p),
                                          request_obj);

        if (response_obj != mp_const_none) {
            response_write(connection_obj_p, response_obj);
        }

        nlr_pop();
    } else {
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
    }

    MP_THREAD_GIL_EXIT();

    return (0);
}

/**
 * Print the http_server object.
 */
static void class_http_server_print(const mp_print_t *print_p,
                                    mp_obj_t self_in,
                                    mp_print_kind_t kind)
{
    struct class_http_server_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new HttpServer object associated with the id. If
 * additional arguments are given, they are used to initialise the
 * http_server. See `init`.
 */
static mp_obj_t class_http_server_make_new(const mp_obj_type_t *type_p,
                                           mp_uint_t n_args,
                                           mp_uint_t n_kw,
                                           const mp_obj_t *args_p)
{
    struct class_http_server_t *self_p;
    mp_map_t kwargs;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_ip_address, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_port, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_routes, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_no_route, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_clients_max, MP_ARG_INT, { .u_int = 1 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    const char *address_p;
    int port;
    void *stack_p;

    mp_arg_check_num(n_args, n_kw, 0, 5, true);

    /* Parse args. */
    mp_map_init_fixed_table(&kwargs, n_kw, args_p + n_args);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Parse the arguments. */
    address_p = mp_obj_str_get_str(args[0].u_obj);
    port = args[1].u_int;

    /* Initiate a new HttpServer object. */
    self_p = m_new_obj(struct class_http_server_t);
    self_p->base.type = &module_inet_class_http_server;

    stack_p = thrd_stack_alloc(1024);

    if (stack_p == NULL) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "out of thread memory"));
    }

    self_p->listener.address_p = address_p;
    self_p->listener.port = port;
    self_p->listener.thrd.name_p = "http_listener";
    self_p->listener.thrd.stack.buf_p = stack_p;
    self_p->listener.thrd.stack.size = 1024;

    stack_p = thrd_stack_alloc(4096);

    if (stack_p == NULL) {
        thrd_stack_free(self_p->listener.thrd.stack.buf_p);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "out of thread memory"));
    }

    self_p->connections[0].thrd.name_p = "http_conn_0";
    self_p->connections[0].thrd.stack.buf_p = stack_p;
    self_p->connections[0].thrd.stack.size = 4096;

    self_p->connections[1].thrd.name_p = NULL;

    self_p->empty_routes[0].path_p = NULL;
    self_p->routes = args[2].u_obj;
    self_p->no_route = args[3].u_obj;

    if (http_server_init(&self_p->http_server,
                         &self_p->listener,
                         &self_p->connections[0],
                         NULL,
                         &self_p->empty_routes[0],
                         on_no_route) != 0) {
        thrd_stack_free(stack_p);
        thrd_stack_free(self_p->listener.thrd.stack.buf_p);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "http_server_init() failed"));
    }

    return (self_p);
}

/**
 * def wrap_ssl(self, context)
 */
static mp_obj_t class_http_server_wrap_ssl(mp_obj_t self_in,
                                           mp_obj_t context_in)
{
#if CONFIG_PUMBAA_MODULE_SSL == 1
    struct class_http_server_t *self_p;
    struct class_ssl_context_t *context_p;

    if (!MP_OBJ_IS_TYPE(context_in, &module_ssl_class_ssl_context)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "SSL Context object required"));        
    }

    self_p = MP_OBJ_TO_PTR(self_in);
    context_p = MP_OBJ_TO_PTR(context_in);
    self_p->ssl_context_obj = context_in;

    if (http_server_wrap_ssl(&self_p->http_server, &context_p->context) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "http_server_wrap_ssl() failed"));        
    }

    return (mp_const_none);
#else
    mp_not_implemented("class_http_server_wrap_ssl()");
#endif
}

/**
 * def start(self)
 */
static mp_obj_t class_http_server_start(mp_obj_t self_in)
{
    struct class_http_server_t *self_p;
    void *thread_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    thread_p = mp_thread_add_begin();
    http_server_start(&self_p->http_server);
    mp_thread_add_end(thread_p, self_p->connections[0].thrd.id_p);

    return (mp_const_none);
}

/**
 * def stop(self)
 */
static mp_obj_t class_http_server_stop(mp_obj_t self_in)
{
    struct class_http_server_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    http_server_stop(&self_p->http_server);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_2(class_http_server_wrap_ssl_obj, class_http_server_wrap_ssl);
static MP_DEFINE_CONST_FUN_OBJ_1(class_http_server_start_obj, class_http_server_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_http_server_stop_obj, class_http_server_stop);

static const mp_rom_map_elem_t class_http_server_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_wrap_ssl), MP_ROM_PTR(&class_http_server_wrap_ssl_obj) },
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_http_server_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_http_server_stop_obj) },

    /* Module constants. */
    { MP_ROM_QSTR(MP_QSTR_GET),
      MP_ROM_INT(http_server_request_action_get_t) },
    { MP_ROM_QSTR(MP_QSTR_POST),
      MP_ROM_INT(http_server_request_action_post_t) },

    { MP_ROM_QSTR(MP_QSTR_RESPONSE_CODE_200_OK),
      MP_ROM_INT(http_server_response_code_200_ok_t) },
    { MP_ROM_QSTR(MP_QSTR_RESPONSE_CODE_404_NOT_FOUND),
      MP_ROM_INT(http_server_response_code_404_not_found_t) },

    { MP_ROM_QSTR(MP_QSTR_CONTENT_TYPE_TEXT_HTML),
      MP_ROM_INT(http_server_content_type_text_html_t) },
    { MP_ROM_QSTR(MP_QSTR_CONTENT_TYPE_TEXT_PLAIN),
      MP_ROM_INT(http_server_content_type_text_plain_t) }
};

static MP_DEFINE_CONST_DICT(class_http_server_locals_dict, class_http_server_locals_dict_table);

/**
 * Http_Server class type.
 */
const mp_obj_type_t module_inet_class_http_server = {
    { &mp_type_type },
    .name = MP_QSTR_HttpServer,
    .print = class_http_server_print,
    .make_new = class_http_server_make_new,
    .locals_dict = (mp_obj_t)&class_http_server_locals_dict,
};

#endif
