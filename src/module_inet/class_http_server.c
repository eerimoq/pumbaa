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
 * All routes are handled by this function.
 */
static int on_no_route(struct http_server_connection_t *connection_p,
                       struct http_server_request_t *request_p)
{
    struct class_http_server_t *self_p;

    self_p = container_of(connection_p->self_p,
                          struct class_http_server_t,
                          http_server);
    (void)self_p;
    /* /\* Iterate over registered routes. *\/ */
    /* route_p = self_p->routes_p; */

    /* while (route_p->path_p != NULL) { */
    /*     if (strncmp(route_p->path_p, request_p->path_p, request_p->path_length) == 0) { */
    /*         mp_obj_call_fun_2(); */
    /*         return (0); */
    /*     } */

    /*     route_p++; */
    /* } */

    /* /\* No route found. *\/ */
    /* mp_obj_call_fun_2(self_p->no_route); */

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

    /* Create a new Http_Server object. */
    self_p = m_new_obj(struct class_http_server_t);
    self_p->base.type = &module_inet_class_http_server;
    self_p->listener.address_p = address_p;
    self_p->listener.port = port;
    self_p->connections[0].thrd.name_p = NULL;
    self_p->connections[1].thrd.name_p = NULL;
    self_p->routes = args[2].u_obj;
    self_p->no_route = args[3].u_obj;

    if (http_server_init(&self_p->http_server,
                         &self_p->listener,
                         &self_p->connections[0],
                         NULL,
                         NULL,
                         on_no_route) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "http_server_init() failed"));
    }

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_http_server_start(mp_obj_t self_in)
{
    struct class_http_server_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    http_server_start(&self_p->http_server);

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

static MP_DEFINE_CONST_FUN_OBJ_1(class_http_server_start_obj, class_http_server_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_http_server_stop_obj, class_http_server_stop);

static const mp_rom_map_elem_t class_http_server_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_http_server_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_http_server_stop_obj) },

    /* Module constants. */
    { MP_ROM_QSTR(MP_QSTR_RESPONSE_CODE_200_OK), MP_ROM_INT(http_server_response_code_200_ok_t) },
    { MP_ROM_QSTR(MP_QSTR_RESPONSE_CODE_404_NOT_FOUND), MP_ROM_INT(http_server_response_code_404_not_found_t) }
};

static MP_DEFINE_CONST_DICT(class_http_server_locals_dict, class_http_server_locals_dict_table);

/**
 * Http_Server class type.
 */
const mp_obj_type_t module_inet_class_http_server = {
    { &mp_type_type },
    .name = MP_QSTR_Http_Server,
    .print = class_http_server_print,
    .make_new = class_http_server_make_new,
    .locals_dict = (mp_obj_t)&class_http_server_locals_dict,
};
