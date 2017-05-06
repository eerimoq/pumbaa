/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
 * This file is part of the Simba project.
 */

#include "pumbaa.h"

int ssl_module_init()
{
    return (0);
}

int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol)
{
    return (0);
}

int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p)
{
    BTASSERT(strlen(cert_p) == 3);
    BTASSERT(strlen(key_p) == 3);
    BTASSERT(memcmp(cert_p, "bar", 3) == 0);
    BTASSERT(memcmp(key_p, "fie", 3) == 0);

    return (0);
}

int ssl_context_load_verify_location(struct ssl_context_t *self_p,
                                     const char *ca_certs_p)
{
    BTASSERT(strlen(ca_certs_p) == 3);
    BTASSERT(memcmp(ca_certs_p, "foo", 3) == 0);

    return (0);
}

int ssl_context_set_verify_mode(struct ssl_context_t *self_p,
                                enum ssl_verify_mode_t mode)
{
    BTASSERT(mode == ssl_verify_mode_cert_required_t);

    return (0);
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p)
{
    BTASSERT(self_p != NULL);
    BTASSERT(context_p != NULL);
    BTASSERT(socket_p != NULL);

    return (chan_init(&self_p->base,
                      (chan_read_fn_t)ssl_socket_read,
                      (chan_write_fn_t)ssl_socket_write,
                      (chan_size_fn_t)ssl_socket_size));
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    BTASSERT(self_p != NULL);

    if (size == 5) {
        if (memcmp(buf_p, "hello", 5) != 0) {
            return (-1);
        }
    }

    if (size == 7) {
        if (memcmp(buf_p, "goodbye", 7) != 0) {
            return (-1);
        }
    }

    return (size);
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    BTASSERT(self_p != NULL);

    if (size == 7) {
        memcpy(buf_p, "goodbye", 7);

        return (7);
    }

    if (size == 5) {
        memcpy(buf_p, "hello", 5);

        return (5);
    }

    return (-1);
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    BTASSERT(self_p != NULL);
    PRINT_FILE_LINE();

    return (0);
}

const char *ssl_socket_get_server_hostname(struct ssl_socket_t *self_p)
{
    static int counter = 0;

    counter++;

    if (counter == 1) {
        return ("test_server");
    }

    return (NULL);
}

int ssl_socket_get_cipher(struct ssl_socket_t *self_p,
                          const char **cipher_pp,
                          const char **protocol_pp,
                          int *number_of_secret_bits_p)
{
    *cipher_pp = "TLS-RSA-WITH-AES-256-GCM-SHA384";
    *protocol_pp = "TLSv1.1";
    *number_of_secret_bits_p = -1;
    
    return (0);
}

static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_ssl_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ssl_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
};

static MP_DEFINE_CONST_DICT(module_ssl_stub_globals, module_ssl_stub_globals_table);

const mp_obj_module_t module_ssl_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_ssl_stub_globals,
};
