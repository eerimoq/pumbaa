/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

static struct event_t accept_events;
static int failed = 0;

static ssize_t read(void *self_p,
                    void *buf_p,
                    size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *response_buf_p;
    size_t response_size;
    ssize_t res;

    BTASSERT(buf_p != NULL);

    response_buf_p = "recv()";
    response_size = 6;
    res = 6;
    
    if (MP_STATE_VM(socket_stub_recv_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(socket_stub_recv_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            response_size = buffer_info.len;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            response_buf_p = NULL;
            response_size = 0;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            response_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_recv_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_recv_obj) = mp_const_none;
        }
    }

    if (response_buf_p != NULL) {
        if (size != response_size) {
            failed = 1;
            res = -1;
        }

        memcpy(buf_p, response_buf_p, size);
    }

    return (res);
}

static ssize_t write(void *self_p,
                     const void *buf_p,
                     size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;
    const char *expected_buf_p;
    size_t expected_size;

    BTASSERT(buf_p != NULL);

    expected_buf_p = "send()";
    expected_size = 6;
    res = 6;

    if (MP_STATE_VM(socket_stub_send_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(socket_stub_send_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            expected_buf_p = NULL;
            expected_size = 0;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_send_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_send_obj) = mp_const_none;
        }
    }

    if (expected_buf_p != NULL) {
        if (size != expected_size) {
            failed = 1;
            res = -1;
        }

        if (memcmp(expected_buf_p, buf_p, size) != 0) {
            failed = 1;
            res = -1;
        }
    }

    return (res);
}

static size_t size(void *self_p)
{
    return (0);
}

int socket_module_init()
{
    return (0);
}

int socket_open_tcp(struct socket_t *self_p)
{
    return (chan_init(&self_p->base, read, write, size));
}

int socket_open_udp(struct socket_t *self_p)
{
    return (0);
}

int socket_open_raw(struct socket_t *self_p)
{
    return (0);
}

int socket_close(struct socket_t *self_p)
{
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    res = 0;

    if (MP_STATE_VM(socket_stub_close_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(socket_stub_close_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];
        res = mp_obj_get_int(item);
        mp_obj_list_remove(MP_STATE_VM(socket_stub_close_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_close_obj) = mp_const_none;
        }
    }

    return (res);
}

int socket_bind(struct socket_t *self_p,
                const struct inet_addr_t *local_addr_p)
{
    char address[16];
    int port;
    const char *expected_address_p;
    int expected_port;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    BTASSERT(inet_ntoa(&local_addr_p->ip, &address[0]) != NULL);
    port = local_addr_p->port;

    expected_address_p = "192.168.0.1";
    expected_port = 8080;
    res = 0;

    if (MP_STATE_VM(socket_stub_bind_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(socket_stub_bind_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        if (MP_OBJ_IS_INT(item)) {
            res = mp_obj_get_int(item);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_bind_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_bind_obj) = mp_const_none;
        }
    }

    BTASSERT(strcmp(&address[0], expected_address_p) == 0);
    BTASSERT(port == expected_port);

    return (res);
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    res = 0;

    if (MP_STATE_VM(socket_stub_listen_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(socket_stub_listen_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];
        res = mp_obj_get_int(item);
        mp_obj_list_remove(MP_STATE_VM(socket_stub_listen_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_listen_obj) = mp_const_none;
        }
    }

    return (res);
}

int socket_connect(struct socket_t *self_p,
                   const struct inet_addr_t *addr_p)
{
    char address[16];
    int port;
    const char *expected_address_p;
    int expected_port;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    BTASSERT(inet_ntoa(&addr_p->ip, &address[0]) != NULL);
    port = addr_p->port;

    expected_address_p = "192.168.0.1";
    expected_port = 8080;
    res = 0;

    if (MP_STATE_VM(socket_stub_connect_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(socket_stub_connect_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        if (MP_OBJ_IS_INT(item)) {
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);

            expected_address_p = mp_obj_str_get_str(items_p[0]);
            expected_port = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_connect_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_connect_obj) = mp_const_none;
        }
    }

    BTASSERT(strcmp(&address[0], expected_address_p) == 0);
    BTASSERT(port == expected_port);

    return (res);
}

int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct inet_addr_t *addr_p)
{
    uint32_t mask;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    res = 0;

    mask = 1;
    event_read(&accept_events, &mask, sizeof(mask));

    if (MP_STATE_VM(socket_stub_accept_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(socket_stub_accept_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];
        res = mp_obj_get_int(item);
        mp_obj_list_remove(MP_STATE_VM(socket_stub_accept_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_accept_obj) = mp_const_none;
        } else {
            event_write(&accept_events, &mask, sizeof(mask));
        }
    }

    chan_init(&accepted_p->base, read, write, size);

    return (res);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;
    const char *expected_buf_p;
    size_t expected_size;

    BTASSERT(buf_p != NULL);

    expected_buf_p = "sendto()";
    expected_size = 8;
    res = 8;

    if (MP_STATE_VM(socket_stub_sendto_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(socket_stub_sendto_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            expected_buf_p = NULL;
            expected_size = 0;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_sendto_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_sendto_obj) = mp_const_none;
        }
    }

    if (expected_buf_p != NULL) {
        BTASSERT(size == expected_size);
        BTASSERT(memcmp(expected_buf_p, buf_p, size) == 0);
    }

    return (res);
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr_p)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *response_buf_p;
    size_t expected_size;
    const char *remote_address_p;
    int remote_port;
    ssize_t res;

    BTASSERT(buf_p != NULL);

    remote_address_p = "192.168.0.1";
    remote_port = 8080;
    response_buf_p = "recvfrom()";
    expected_size = 1024;
    res = 10;

    if (MP_STATE_VM(socket_stub_recvfrom_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(socket_stub_recvfrom_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            response_buf_p = NULL;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(socket_stub_recvfrom_obj), item);

        if (len == 1) {
            MP_STATE_VM(socket_stub_recvfrom_obj) = mp_const_none;
        }
    }

    if (response_buf_p != NULL) {
        BTASSERT(size == expected_size);
        memcpy(buf_p, response_buf_p, size);
    }

    inet_aton(remote_address_p, &remote_addr_p->ip);
    remote_addr_p->port = remote_port;

    return (res);
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    return (write(self_p, buf_p, size));
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    return (read(self_p, buf_p, size));
}

static mp_obj_t module_init(void)
{
    event_init(&accept_events);

    MP_STATE_VM(socket_stub_close_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_bind_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_listen_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_accept_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_connect_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_recv_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_send_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_recvfrom_obj) = mp_const_none;
    MP_STATE_VM(socket_stub_sendto_obj) = mp_const_none;

    return (mp_const_none);
}

static mp_obj_t module_set_close(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_close_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_bind(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_bind_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_listen(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_listen_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_accept(mp_obj_t data_in)
{
    uint32_t mask;

    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_accept_obj) = data_in;

    mask = 1;
    event_write(&accept_events, &mask, sizeof(mask));

    return (mp_const_none);
}

static mp_obj_t module_set_connect(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_connect_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_recv(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_recv_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_send(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_send_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_recvfrom(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_recvfrom_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_sendto(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(socket_stub_sendto_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_wait_closed(void)
{
    while (MP_STATE_VM(socket_stub_close_obj) != mp_const_none) {
        thrd_sleep_ms(10);
    }

    return (mp_const_none);
}

static mp_obj_t module_reset_failed(void)
{
    int res;

    res = failed;
    failed = 0;
    
    return (mp_obj_new_int(res));
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_close_obj, module_set_close);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_bind_obj, module_set_bind);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_accept_obj, module_set_accept);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_listen_obj, module_set_listen);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_connect_obj, module_set_connect);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_recv_obj, module_set_recv);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_send_obj, module_set_send);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_recvfrom_obj, module_set_recvfrom);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_sendto_obj, module_set_sendto);
static MP_DEFINE_CONST_FUN_OBJ_0(module_wait_closed_obj, module_wait_closed);
static MP_DEFINE_CONST_FUN_OBJ_0(module_reset_failed_obj, module_reset_failed);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_socket_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_socket_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Functions. */
    { MP_ROM_QSTR(MP_QSTR_set_close), MP_ROM_PTR(&module_set_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_bind), MP_ROM_PTR(&module_set_bind_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_listen), MP_ROM_PTR(&module_set_listen_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_accept), MP_ROM_PTR(&module_set_accept_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_connect), MP_ROM_PTR(&module_set_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_recv), MP_ROM_PTR(&module_set_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_send), MP_ROM_PTR(&module_set_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_recvfrom), MP_ROM_PTR(&module_set_recvfrom_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_sendto), MP_ROM_PTR(&module_set_sendto_obj) },
    { MP_ROM_QSTR(MP_QSTR_wait_closed), MP_ROM_PTR(&module_wait_closed_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset_failed), MP_ROM_PTR(&module_reset_failed_obj) },
};

static MP_DEFINE_CONST_DICT(module_socket_stub_globals, module_socket_stub_globals_table);

const mp_obj_module_t module_socket_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_socket_stub_globals,
};
