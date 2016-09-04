/**
 * @file modfs.c
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

struct fs_file_obj_t {
    mp_obj_base_t base;
    struct fs_file_t file;
};

/* static const mp_stream_p_t textio_stream_p = { */
/*     .read = NULL, */
/*     .write = NULL, */
/*     .ioctl = NULL, */
/*     .is_text = true, */
/* }; */

/* static const mp_obj_type_t mp_type_textio = { */
/*     { &mp_type_type }, */
/*     .name = MP_QSTR_TextIOWrapper, */
/*     .print = file_obj_print, */
/*     .make_new = file_obj_make_new, */
/*     .getiter = mp_identity, */
/*     .iternext = mp_stream_unbuffered_iter, */
/*     .protocol = &textio_stream_p, */
/*     .locals_dict = (mp_obj_dict_t*)&rawfile_locals_dict, */
/* }; */

/**
 * def call()
 */
static mp_obj_t module_call(mp_obj_t command_in)
{
    char command[128];

    strncpy(command, mp_obj_str_get_str(command_in), membersof(command));
    command[membersof(command) - 1] = '\0';
    fs_call(command, sys_get_stdin(), sys_get_stdout(), NULL);

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(module_call_obj, module_call);

static const mp_map_elem_t module_fs_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_fs) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_call), (mp_obj_t)&module_call_obj },
};

static MP_DEFINE_CONST_DICT(module_fs_globals, module_fs_globals_table);

/* static mp_obj_t file_open(const mp_obj_type_t *type_p, */
/*                           mp_arg_val_t *args_p) */
/* { */
/*     int mode; */
/*     const char *mode_p; */
/*     struct fs_file_obj_t *obj_p; */
/*     const char *fname_p; */
/*     int res; */
    
/*     mode = 0; */
/*     mode_p = mp_obj_str_get_str(args_p[1].u_obj); */

/*     /\* TODO make sure only one of r, w, x, a, and b, t are */
/*        specified. *\/ */
/*     while (*mode_p) { */
/*         switch (*mode_p++) { */
/*             case 'r': */
/*                 mode |= FS_READ; */
/*                 break; */
/*             case 'w': */
/*                 mode |= FS_WRITE | FS_CREAT; */
/*                 break; */
/*             case 'x': */
/*                 mode |= FS_WRITE | FS_CREAT; */
/*                 break; */
/*             case 'a': */
/*                 mode |= FS_WRITE | FS_APPEND; */
/*                 break; */
/*             case '+': */
/*                 mode |= FS_READ | FS_WRITE; */
/*                 break; */
/*             case 't': */
/*                 type_p = &mp_type_textio; */
/*                 break; */
/*         } */
/*     } */

/*     obj_p = m_new_obj_with_finaliser(struct fs_file_obj_t); */
/*     obj_p->base.type = type_p; */

/*     fname_p = mp_obj_str_get_str(args_p[0].u_obj); */
/*     res = fs_open(&obj_p->file, fname_p, mode); */
    
/*     if (res != 0) { */
/*         m_del_obj(pyb_file_obj_t, obj_p); */
/*         nlr_raise(mp_obj_new_exception_arg1(&mp_type_OSError, */
/*                                             MP_OBJ_NEW_SMALL_INT(res))); */
/*     } */

/*     return MP_OBJ_FROM_PTR(obj_p); */
/* } */

/* static mp_obj_t builtin_open(mp_uint_t n_args, */
/*                              const mp_obj_t *args_p, */
/*                              mp_map_t *kwargs_p) */
/* { */
/*     /\* TODO: analyze buffering args and instantiate appropriate */
/*        type. *\/ */
/*     static const mp_arg_t file_open_args[] = { */
/*         { */
/*             MP_QSTR_file, */
/*             MP_ARG_OBJ | MP_ARG_REQUIRED, */
/*             { .u_rom_obj = MP_ROM_PTR(&mp_const_none_obj) } */
/*         }, */
/*         { */
/*             MP_QSTR_mode, */
/*             MP_ARG_OBJ, */
/*             { .u_obj = MP_OBJ_NEW_QSTR(MP_QSTR_r) } */
/*         }, */
/*         { */
/*             MP_QSTR_encoding, */
/*             MP_ARG_OBJ | MP_ARG_KW_ONLY, */
/*             { .u_rom_obj = MP_ROM_PTR(&mp_const_none_obj) } */
/*         }, */
/*     }; */
/*     mp_arg_val_t arg_vals[MP_ARRAY_SIZE(file_open_args)]; */

/*     mp_arg_parse_all(n_args, */
/*                      args_p, */
/*                      kwargs_p, */
/*                      MP_ARRAY_SIZE(file_open_args), */
/*                      file_open_args, */
/*                      arg_vals); */

/*     return (file_open(&mp_type_textio, arg_vals)); */
/* } */

/* MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, builtin_open); */

const mp_obj_module_t module_fs = {
    .base = { &mp_type_module },
    .name = MP_QSTR_fs,
    .globals = (mp_obj_dict_t*)&module_fs_globals,
};
