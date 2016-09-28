/**
 * @file module_simba.c
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

#if CONFIG_PUMBAA_FS_CALL == 1

/**
 * The vstr channel stores all written data in a vstr. No read not
 * size functions are available.
 */
struct vstr_chan_t {
    struct chan_t base;
    vstr_t string;
};

static ssize_t vstr_chan_write(struct vstr_chan_t *self_p,
                               const void *buf_p,
                               size_t size);

static int vstr_chan_init(struct vstr_chan_t *self_p)
{
    chan_init(&self_p->base,
              chan_read_null,
              (chan_write_fn_t)vstr_chan_write,
              chan_size_null);
    vstr_init(&self_p->string, 128);

    return (0);
}

static ssize_t vstr_chan_write(struct vstr_chan_t *self_p,
                               const void *buf_p,
                               size_t size)
{
    vstr_add_strn(&self_p->string, buf_p, size);

    return (size);
}

static vstr_t *vstr_chan_get_vstr(struct vstr_chan_t *self_p)
{
    return (&self_p->string);
}

/**
 * def fs_call(command)
 */
static mp_obj_t module_fs_call(mp_obj_t command_in)
{
    char command[128];
    struct vstr_chan_t chout;

    strncpy(command, mp_obj_str_get_str(command_in), membersof(command));
    command[membersof(command) - 1] = '\0';

    vstr_chan_init(&chout);

    if (fs_call(command, sys_get_stdin(), &chout, NULL) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "-1"));
    }

    return (mp_obj_new_str_from_vstr(&mp_type_str,
                                     vstr_chan_get_vstr(&chout)));
}

static MP_DEFINE_CONST_FUN_OBJ_1(module_fs_call_obj, module_fs_call);

#endif

#if CONFIG_PUMBAA_FS_FORMAT == 1

/**
 * def fs_format(path)
 */
static mp_obj_t module_fs_format(mp_obj_t path_in)
{
    if (fs_format(mp_obj_str_get_str(path_in)) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_1(module_fs_format_obj, module_fs_format);

#endif

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
#if CONFIG_PUMBAA_CLASS_TIMER == 1
    timer_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_PIN == 1
    pin_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_EXTI == 1
    exti_module_init();
#endif

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_rom_map_elem_t module_simba_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_simba) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Module classes. */
#if CONFIG_PUMBAA_CLASS_BOARD == 1
    { MP_ROM_QSTR(MP_QSTR_Board), MP_ROM_PTR(&module_simba_class_board) },
#endif
#if CONFIG_PUMBAA_CLASS_DAC == 1
    { MP_ROM_QSTR(MP_QSTR_Dac), MP_ROM_PTR(&module_simba_class_dac) },
#endif
#if CONFIG_PUMBAA_CLASS_EVENT == 1
    { MP_ROM_QSTR(MP_QSTR_Event), MP_ROM_PTR(&module_simba_class_event) },
#endif
#if CONFIG_PUMBAA_CLASS_EXTI == 1
    { MP_ROM_QSTR(MP_QSTR_Exti), MP_ROM_PTR(&module_simba_class_exti) },
#endif
#if CONFIG_PUMBAA_CLASS_PIN == 1
    { MP_ROM_QSTR(MP_QSTR_Pin), MP_ROM_PTR(&module_simba_class_pin) },
#endif
#if CONFIG_PUMBAA_CLASS_QUEUE == 1
    { MP_ROM_QSTR(MP_QSTR_Queue), MP_ROM_PTR(&module_simba_class_queue) },
#endif
#if CONFIG_PUMBAA_CLASS_TIMER == 1
    { MP_ROM_QSTR(MP_QSTR_Timer), MP_ROM_PTR(&module_simba_class_timer) },
#endif

    /* Module functions. */
#if CONFIG_PUMBAA_FS_CALL == 1
    { MP_ROM_QSTR(MP_QSTR_fs_call), MP_ROM_PTR(&module_fs_call_obj) },
#endif
#if CONFIG_PUMBAA_FS_FORMAT == 1
    { MP_ROM_QSTR(MP_QSTR_fs_format), MP_ROM_PTR(&module_fs_format_obj) },
#endif
};

static MP_DEFINE_CONST_DICT(module_simba_globals, module_simba_globals_table);

const mp_obj_module_t module_simba = {
    { &mp_type_module },
    .name = MP_QSTR_simba,
    .globals = (mp_obj_t)&module_simba_globals,
};
