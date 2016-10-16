/**
 * @file module_inet.c
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

#if CONFIG_PUMBAA_PING == 1

static mp_obj_t module_ping_host_by_ip_address(mp_obj_t address_in,
                                               mp_obj_t timeout_in)
{
    struct inet_ip_addr_t address;
    struct time_t timeout;
    float f_timeout;
    struct time_t round_trip_time;
    unsigned long round_trip_time_ms;

    /* Argument convertion. */
    inet_aton(mp_obj_str_get_str(address_in), &address);
    f_timeout = mp_obj_get_float(timeout_in);
    timeout.seconds = (long)f_timeout;
    timeout.nanoseconds = (f_timeout - timeout.seconds) * 1000000000L;

    /* Ping the node. */
    if (ping_host_by_ip_address(&address,
                                &timeout,
                                &round_trip_time) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "ping_host_by_ip_address() failed"));
    }

    round_trip_time_ms = (round_trip_time.seconds * 1000);
    round_trip_time_ms += (round_trip_time.nanoseconds / 1000000L);

    return (MP_OBJ_NEW_SMALL_INT(round_trip_time_ms));
}

static MP_DEFINE_CONST_FUN_OBJ_2(module_ping_host_by_ip_address_obj, module_ping_host_by_ip_address);

#endif

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
#if CONFIG_PUMBAA_PING == 1
    ping_module_init();
#endif

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_rom_map_elem_t module_inet_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_inet) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Module classes. */
#if CONFIG_PUMBAA_PING == 1
    { MP_ROM_QSTR(MP_QSTR_ping_host_by_ip_address), MP_ROM_PTR(&module_ping_host_by_ip_address_obj) },
#endif
};

static MP_DEFINE_CONST_DICT(module_inet_globals, module_inet_globals_table);

const mp_obj_module_t module_inet = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_inet_globals,
};
