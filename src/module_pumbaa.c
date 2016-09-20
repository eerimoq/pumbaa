/**
 * @file module_pumbaa.c
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
 * def fs_call(command)
 */
static mp_obj_t module_fs_call(mp_obj_t command_in)
{
    char command[128];

    strncpy(command, mp_obj_str_get_str(command_in), membersof(command));
    command[membersof(command) - 1] = '\0';
    fs_call(command, sys_get_stdin(), sys_get_stdout(), NULL);

    return (mp_const_none);
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

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_map_elem_t module_pumbaa_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_pumbaa) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&module_init_obj },

    /* Module classes. */
#if CONFIG_PUMBAA_CLASS_BOARD == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_Board), (mp_obj_t)&module_pumbaa_class_board },
#endif
#if CONFIG_PUMBAA_CLASS_DAC == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_Dac), (mp_obj_t)&module_pumbaa_class_dac },
#endif
#if CONFIG_PUMBAA_CLASS_EVENT == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_Event), (mp_obj_t)&module_pumbaa_class_event },
#endif
#if CONFIG_PUMBAA_CLASS_PIN == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&module_pumbaa_class_pin },
#endif
#if CONFIG_PUMBAA_CLASS_TIMER == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_Timer), (mp_obj_t)&module_pumbaa_class_timer },
#endif

    /* Module functions. */
#if CONFIG_PUMBAA_FS_CALL == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_fs_call), (mp_obj_t)&module_fs_call_obj },
#endif
#if CONFIG_PUMBAA_FS_FORMAT == 1
    { MP_OBJ_NEW_QSTR(MP_QSTR_fs_format), (mp_obj_t)&module_fs_format_obj },
#endif
};

static MP_DEFINE_CONST_DICT(module_pumbaa_globals, module_pumbaa_globals_table);

const mp_obj_module_t module_pumbaa = {
    { &mp_type_module },
    .name = MP_QSTR_pumbaa,
    .globals = (mp_obj_t)&module_pumbaa_globals,
};
