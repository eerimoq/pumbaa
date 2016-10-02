/**
 * @file module_socket.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Pumbaa project.
 */

#include "pumbaa.h"

struct class_socket_t {
    mp_obj_base_t base;
};

static mp_obj_t class_socket_accept(mp_obj_t self_in)
{
    mp_not_implemented("socket.accept");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_bind(mp_obj_t self_in)
{
    mp_not_implemented("socket.bind");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_close(mp_obj_t self_in)
{
    mp_not_implemented("socket.close");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_connect(mp_obj_t self_in)
{
    mp_not_implemented("socket.connect");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_listen(mp_obj_t self_in)
{
    mp_not_implemented("socket.listen");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_recv(mp_obj_t self_in)
{
    mp_not_implemented("socket.recv");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_recvfrom(mp_obj_t self_in)
{
    mp_not_implemented("socket.recvfrom");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_recv_into(mp_obj_t self_in)
{
    mp_not_implemented("socket.recv_into");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_recvfrom_into(mp_obj_t self_in)
{
    mp_not_implemented("socket.recvfrom_into");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_send(mp_obj_t self_in)
{
    mp_not_implemented("socket.send");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_sendall(mp_obj_t self_in)
{
    mp_not_implemented("socket.sendall");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_sendto(mp_obj_t self_in)
{
    mp_not_implemented("socket.sendto");
        
    return (mp_const_none);
}

static mp_obj_t class_socket_shutdown(mp_obj_t self_in)
{
    mp_not_implemented("socket.shutdown");
        
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(socket_accept_obj, class_socket_accept);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_bind_obj, class_socket_bind);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_close_obj, class_socket_close);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_connect_obj, class_socket_connect);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_listen_obj, class_socket_listen);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_recv_obj, class_socket_recv);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_recvfrom_obj, class_socket_recvfrom);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_recv_into_obj, class_socket_recv_into);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_recvfrom_into_obj, class_socket_recvfrom_into);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_send_obj, class_socket_send);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_sendall_obj, class_socket_sendall);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_sendto_obj, class_socket_sendto);
static MP_DEFINE_CONST_FUN_OBJ_1(socket_shutdown_obj, class_socket_shutdown);

static const mp_rom_map_elem_t class_socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_accept), MP_ROM_PTR(&socket_accept_obj) },
    { MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&socket_bind_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&socket_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&socket_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_listen), MP_ROM_PTR(&socket_listen_obj) },
    { MP_ROM_QSTR(MP_QSTR_recv), MP_ROM_PTR(&socket_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_recvfrom), MP_ROM_PTR(&socket_recvfrom_obj) },
    { MP_ROM_QSTR(MP_QSTR_recv_into), MP_ROM_PTR(&socket_recv_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_recvfrom_into), MP_ROM_PTR(&socket_recvfrom_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&socket_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_sendall), MP_ROM_PTR(&socket_sendall_obj) },
    { MP_ROM_QSTR(MP_QSTR_sendto), MP_ROM_PTR(&socket_sendto_obj) },
    { MP_ROM_QSTR(MP_QSTR_shutdown), MP_ROM_PTR(&socket_shutdown_obj) }
};

static MP_DEFINE_CONST_DICT(class_socket_locals_dict, class_socket_locals_dict_table);

/**
 * The socket class.
 */
static const mp_obj_type_t class_socket = {
    { &mp_type_type },
    .name = MP_QSTR_SocketType,
    .locals_dict = (void*)&class_socket_locals_dict,
};

/**
 * Create a socket.
 */
static mp_obj_t module_socket_socket(void)
{
    struct class_socket_t *socket_p;

    socket_p = m_new_obj(struct class_socket_t);
    socket_p->base.type = &class_socket;

    return (socket_p);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_socket_socket_obj, module_socket_socket);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t mp_module_socket_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_usocket) },

    { MP_ROM_QSTR(MP_QSTR_socket), MP_ROM_PTR(&module_socket_socket_obj) },
};

static MP_DEFINE_CONST_DICT(mp_module_socket_globals, mp_module_socket_globals_table);

const mp_obj_module_t mp_module_usocket = {
    .base = { &mp_type_module },
    .name = MP_QSTR_usocket,
    .globals = (mp_obj_dict_t*)&mp_module_socket_globals,
};
