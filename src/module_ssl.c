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

#if CONFIG_PUMBAA_MODULE_SSL == 1

struct class_ssl_context_t {
    mp_obj_base_t base;
    struct ssl_context_t context;
};

struct class_ssl_socket_t {
    mp_obj_base_t base;
    struct ssl_socket_t socket;
    mp_obj_t sock_obj;
};

extern const mp_obj_type_t module_socket_class_socket;
extern const mp_obj_type_t module_ssl_class_ssl_context;
extern const mp_obj_type_t module_ssl_class_ssl_socket;

/**
 * Allocate a buffer and read given file into it.
 */
const char *read_file(const char *path_p)
{
    struct fs_file_t file;
    struct fs_stat_t stat;
    char *buf_p;

    if (fs_stat(path_p, &stat) != 0) {
        return (NULL);
    }

    buf_p = m_malloc(stat.size + 1);

    if (fs_open(&file, path_p, FS_READ) != 0) {
        return (NULL);
    }

    if (fs_read(&file, buf_p, stat.size) != stat.size) {
        buf_p = NULL;
    }

    fs_close(&file);

    buf_p[stat.size] = '\0';

    return (buf_p);
}

static mp_obj_t class_ssl_context_make_new(const mp_obj_type_t *type_p,
                                           size_t n_args,
                                           size_t n_kw,
                                           const mp_obj_t *args_p)
{
    struct class_ssl_context_t *self_p;

    mp_arg_check_num(n_args, n_kw, 0, 1, true);

    self_p = m_new_obj(struct class_ssl_context_t);
    self_p->base.type = &module_ssl_class_ssl_context;

    if (ssl_context_init(&self_p->context, ssl_protocol_tls_v1_0) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to create ssl context"));
    }

    return (self_p);
}

/**
 * def load_cert_chain(certfile, keyfile=None, password=None)
 */
static mp_obj_t class_ssl_context_load_cert_chain(size_t n_args,
                                                  const mp_obj_t *pos_args_p,
                                                  mp_map_t *kw_args_p)
{
    struct class_ssl_context_t *self_p;
    mp_buffer_info_t buffer_info;
    const char *cert_p;
    const char *key_p;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_keyfile, MP_ARG_OBJ, { .u_obj = mp_const_none } }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_parse_all(n_args - 2,
                     pos_args_p + 2,
                     kw_args_p,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    self_p = MP_OBJ_TO_PTR(pos_args_p[0]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(pos_args_p[1]),
                        &buffer_info,
                        MP_BUFFER_READ);
    cert_p = read_file(buffer_info.buf);

    if (cert_p == NULL) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to read cert"));
    }

    if (args[0].u_obj != mp_const_none) {
        mp_get_buffer_raise(MP_OBJ_TO_PTR(args[0].u_obj),
                            &buffer_info,
                            MP_BUFFER_READ);
        key_p = read_file(buffer_info.buf);
    } else {
        key_p = NULL;
    }

    if (ssl_context_load_cert_chain(&self_p->context,
                                    cert_p,
                                    key_p) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to load certs"));
    }

    return (mp_const_none);
}

static mp_obj_t class_ssl_context_load_verify_locations(size_t n_args,
                                                        const mp_obj_t *pos_args_p,
                                                        mp_map_t *kw_args_p)
{
    struct class_ssl_context_t *self_p;
    mp_buffer_info_t buffer_info;
    const char *cafile_p;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_cafile, MP_ARG_OBJ, { .u_obj = mp_const_none } }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_parse_all(n_args - 1,
                     pos_args_p + 1,
                     kw_args_p,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    self_p = MP_OBJ_TO_PTR(pos_args_p[0]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args[0].u_obj),
                        &buffer_info,
                        MP_BUFFER_READ);
    cafile_p = read_file(buffer_info.buf);

    if (cafile_p == NULL) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to read cafile"));
    }

    if (ssl_context_load_verify_location(&self_p->context, cafile_p) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to load cafile"));
    }

    return (mp_const_none);
}

static mp_obj_t class_ssl_context_set_verify_mode(mp_obj_t self_in,
                                                  mp_obj_t mode_in)
{
    struct class_ssl_context_t *self_p;
    enum ssl_verify_mode_t mode;

    self_p = MP_OBJ_TO_PTR(self_in);
    mode = mp_obj_get_int(mode_in);

    if ((mode != ssl_verify_mode_cert_none_t)
        && (mode != ssl_verify_mode_cert_required_t)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad mode"));
    }

    if (ssl_context_set_verify_mode(&self_p->context, mode) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to set verify mode"));
    }

    return (mp_const_none);
}

/**
 * def wrap_socket(sock, server_side=False)
 */
static mp_obj_t class_ssl_context_wrap_socket(size_t n_args,
                                              const mp_obj_t *pos_args_p,
                                              mp_map_t *kw_args_p)
{
    struct class_ssl_context_t *self_p;
    struct class_ssl_socket_t *ssl_sock_p;
    struct class_socket_t *sock_p;
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_server_side, MP_ARG_BOOL, { .u_bool = false } }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int server_side;
    mp_obj_t sock_in;

    self_p = MP_OBJ_TO_PTR(pos_args_p[0]);
    sock_in = pos_args_p[1];

    if (!MP_OBJ_IS_TYPE(sock_in, &module_socket_class_socket)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "SocketType object required"));
    }

    sock_p = MP_OBJ_TO_PTR(sock_in);

    mp_arg_parse_all(n_args - 2,
                     pos_args_p + 2,
                     kw_args_p,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    server_side = args[0].u_bool;

    ssl_sock_p = m_new_obj(struct class_ssl_socket_t);
    ssl_sock_p->base.type = &module_ssl_class_ssl_socket;
    ssl_sock_p->sock_obj = pos_args_p[1];

    if (ssl_socket_open(&ssl_sock_p->socket,
                        &self_p->context,
                        &sock_p->socket,
                        server_side) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "failed to open SSL socket"));
    }


    return (MP_OBJ_FROM_PTR(ssl_sock_p));
}

static MP_DEFINE_CONST_FUN_OBJ_KW(class_ssl_context_load_cert_chain_obj,
                                  2,
                                  class_ssl_context_load_cert_chain);
static MP_DEFINE_CONST_FUN_OBJ_KW(class_ssl_context_load_verify_locations_obj,
                                  1,
                                  class_ssl_context_load_verify_locations);
static MP_DEFINE_CONST_FUN_OBJ_2(class_ssl_context_set_verify_mode_obj,
                                 class_ssl_context_set_verify_mode);
static MP_DEFINE_CONST_FUN_OBJ_KW(class_ssl_context_wrap_socket_obj,
                                  1,
                                  class_ssl_context_wrap_socket);

static const mp_rom_map_elem_t class_ssl_context_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_load_cert_chain),
      MP_ROM_PTR(&class_ssl_context_load_cert_chain_obj) },
    { MP_ROM_QSTR(MP_QSTR_load_verify_locations),
      MP_ROM_PTR(&class_ssl_context_load_verify_locations_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_verify_mode),
      MP_ROM_PTR(&class_ssl_context_set_verify_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_wrap_socket),
      MP_ROM_PTR(&class_ssl_context_wrap_socket_obj) }
};

static MP_DEFINE_CONST_DICT(class_ssl_context_locals_dict,
                            class_ssl_context_locals_dict_table);

/**
 * The SSLContext class.
 */
const mp_obj_type_t module_ssl_class_ssl_context = {
    { &mp_type_type },
    .name = MP_QSTR_SSLContext,
    .make_new = class_ssl_context_make_new,
    .locals_dict = (void*)&class_ssl_context_locals_dict,
};

static mp_obj_t class_ssl_socket_recv(mp_obj_t self_in,
                                      mp_obj_t bufsize_in)
{
    struct class_ssl_socket_t *self_p;
    vstr_t vstr;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    size = mp_obj_get_int(bufsize_in);

    vstr_init(&vstr, size);
    size = ssl_socket_read(&self_p->socket, vstr.buf, size);

    if (size < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "SSL socket recv failed"));
    }

    vstr.len = size;

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
}

static mp_obj_t class_ssl_socket_send(mp_obj_t self_in, mp_obj_t string_in)
{
    struct class_ssl_socket_t *self_p;
    mp_buffer_info_t buffer_info;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(string_in),
                        &buffer_info,
                        MP_BUFFER_READ);

    size = ssl_socket_write(&self_p->socket,
                            buffer_info.buf,
                            buffer_info.len);

    if (size < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "socket send failed"));
    }

    return (mp_obj_new_int(size));
}

static mp_obj_t class_ssl_socket_close(mp_obj_t self_in)
{
    struct class_ssl_socket_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (ssl_socket_close(&self_p->socket) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "socket close failed"));
    }

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_2(class_ssl_socket_recv_obj, class_ssl_socket_recv);
static MP_DEFINE_CONST_FUN_OBJ_2(class_ssl_socket_send_obj, class_ssl_socket_send);
static MP_DEFINE_CONST_FUN_OBJ_1(class_ssl_socket_close_obj, class_ssl_socket_close);

static const mp_rom_map_elem_t class_ssl_socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_recv), MP_ROM_PTR(&class_ssl_socket_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&class_ssl_socket_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&class_ssl_socket_close_obj) }
};

static MP_DEFINE_CONST_DICT(class_ssl_socket_locals_dict,
                            class_ssl_socket_locals_dict_table);

/**
 * The SSLSocket class.
 */
const mp_obj_type_t module_ssl_class_ssl_socket = {
    { &mp_type_type },
    .name = MP_QSTR_SSLSocket,
    .locals_dict = (void*)&class_ssl_socket_locals_dict,
};

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
    ssl_module_init();

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t mp_module_ssl_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ussl) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Types. */
    { MP_ROM_QSTR(MP_QSTR_SSLContext), MP_ROM_PTR(&module_ssl_class_ssl_context) },
    { MP_ROM_QSTR(MP_QSTR_SSLSocket), MP_ROM_PTR(&module_ssl_class_ssl_socket) },

    /* Constants. */
    { MP_ROM_QSTR(MP_QSTR_PROTOCOL_TLS), MP_ROM_INT(0) },

    { MP_ROM_QSTR(MP_QSTR_CERT_NONE), MP_ROM_INT(ssl_verify_mode_cert_none_t) },
    { MP_ROM_QSTR(MP_QSTR_CERT_REQUIRED), MP_ROM_INT(ssl_verify_mode_cert_required_t) },
};

static MP_DEFINE_CONST_DICT(mp_module_ssl_globals, mp_module_ssl_globals_table);

const mp_obj_module_t mp_module_ussl = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ssl_globals,
};

#endif
