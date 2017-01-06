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
 * This file is part of the Simba project.
 */

#include "simba.h"

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
    BTASSERT(strlen(cert_p) == 400);
    BTASSERT(strlen(key_p) == 400);

    while (*cert_p != '\0') {
        if (*cert_p++ != 2) {
            return (-1);
        }
    }
    while (*key_p != '\0') {
        if (*key_p++ != 3) {
            return (-1);
        }
    }

    return (0);
}

int ssl_context_load_verify_location(struct ssl_context_t *self_p,
                                     const char *ca_certs_p)
{
    BTASSERT(strlen(ca_certs_p) == 400);

    while (*ca_certs_p != '\0') {
        if (*ca_certs_p++ != 1) {
            return (-1);
        }
    }

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
                    int server_side)
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
