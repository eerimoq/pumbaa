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
#if CONFIG_PUMBAA_HTTP_SERVER == 1
    { MP_ROM_QSTR(MP_QSTR_HttpServer), MP_ROM_PTR(&module_inet_class_http_server) },
#endif
#if CONFIG_PUMBAA_HTTP_SERVER_WEBSOCKET == 1
    { MP_ROM_QSTR(MP_QSTR_HttpServerWebSocket), MP_ROM_PTR(&module_inet_class_http_server_websocket) },
#endif

    /* Module functions. */
#if CONFIG_PUMBAA_PING == 1
    { MP_ROM_QSTR(MP_QSTR_ping_host_by_ip_address), MP_ROM_PTR(&module_ping_host_by_ip_address_obj) },
#endif
};

static MP_DEFINE_CONST_DICT(module_inet_globals, module_inet_globals_table);

const mp_obj_module_t module_inet = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_inet_globals,
};
