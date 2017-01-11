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

#ifndef __STUBS_H__
#define __STUBS_H__

#define CAN_STUB_ROOT_POINTERS
#define DS18B20_STUB_ROOT_POINTERS

#define FS_STUB_ROOT_POINTERS                   \
    mp_obj_t fs_stub_open_obj;                  \
    mp_obj_t fs_stub_read_obj;                  \
    mp_obj_t fs_stub_write_obj;                 \
    mp_obj_t fs_stub_seek_obj;                  \
    mp_obj_t fs_stub_tell_obj;                  \
    mp_obj_t fs_stub_format_obj;                \
    mp_obj_t fs_stub_remove_obj;                \
    mp_obj_t fs_stub_stat_obj;

#define OWI_STUB_ROOT_POINTERS                  \
    mp_obj_t owi_stub_devices_obj;              \
    mp_obj_t owi_stub_read_obj;                 \
    mp_obj_t owi_stub_write_obj;

#define SD_STUB_ROOT_POINTERS

#define SOCKET_STUB_ROOT_POINTERS               \
    mp_obj_t socket_stub_close_obj;             \
    mp_obj_t socket_stub_bind_obj;              \
    mp_obj_t socket_stub_listen_obj;            \
    mp_obj_t socket_stub_accept_obj;            \
    mp_obj_t socket_stub_connect_obj;           \
    mp_obj_t socket_stub_recv_obj;              \
    mp_obj_t socket_stub_send_obj;              \
    mp_obj_t socket_stub_recvfrom_obj;          \
    mp_obj_t socket_stub_sendto_obj;

#define SSL_STUB_ROOT_POINTERS

#define CAN_STUB_BUILTIN_MODULE                                         \
    { MP_ROM_QSTR(MP_QSTR_can_stub), MP_ROM_PTR(&module_can_stub) },
#define DS18B20_STUB_BUILTIN_MODULE                                     \
    { MP_ROM_QSTR(MP_QSTR_ds18b20_stub), MP_ROM_PTR(&module_ds18b20_stub) },
#define FS_STUB_BUILTIN_MODULE                                          \
    { MP_ROM_QSTR(MP_QSTR_fs_stub), MP_ROM_PTR(&module_fs_stub) },      
#define OWI_STUB_BUILTIN_MODULE                                         \
    { MP_ROM_QSTR(MP_QSTR_owi_stub), MP_ROM_PTR(&module_owi_stub) },    
#define SD_STUB_BUILTIN_MODULE                                          \
    { MP_ROM_QSTR(MP_QSTR_sd_stub), MP_ROM_PTR(&module_sd_stub) },      
#define SOCKET_STUB_BUILTIN_MODULE                                      \
    { MP_ROM_QSTR(MP_QSTR_socket_stub), MP_ROM_PTR(&module_socket_stub) }, 
#define SSL_STUB_BUILTIN_MODULE                                         \
    { MP_ROM_QSTR(MP_QSTR_ssl_stub), MP_ROM_PTR(&module_ssl_stub) },

extern const struct _mp_obj_module_t module_can_stub;
extern const struct _mp_obj_module_t module_ds18b20_stub;
extern const struct _mp_obj_module_t module_fs_stub;
extern const struct _mp_obj_module_t module_owi_stub;
extern const struct _mp_obj_module_t module_sd_stub;
extern const struct _mp_obj_module_t module_socket_stub;
extern const struct _mp_obj_module_t module_ssl_stub;

#endif
