/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017, Erik Moqvist
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stubs.h"

#define CONFIG_PUMBAA_MAIN_REBOOT_AT_EXIT                   0
#define CONFIG_PUMBAA_MAIN_FRIENDLY_REPL                    0

#if defined(ARCH_LINUX)
#    define BUILTIN_MODULE_SD \
    { MP_ROM_QSTR(MP_QSTR_sd_stub), MP_ROM_PTR(&module_sd_stub) },
#else
#    define BUILTIN_MODULE_SD
#endif

#define MICROPY_PORT_BUILTIN_MODULES_EXTRA                              \
    { MP_ROM_QSTR(MP_QSTR_can_stub), MP_ROM_PTR(&module_can_stub) },    \
    { MP_ROM_QSTR(MP_QSTR_ds18b20_stub), MP_ROM_PTR(&module_ds18b20_stub) }, \
    { MP_ROM_QSTR(MP_QSTR_owi_stub), MP_ROM_PTR(&module_owi_stub) },    \
    BUILTIN_MODULE_SD \
    { MP_ROM_QSTR(MP_QSTR_socket_stub), MP_ROM_PTR(&module_socket_stub) }, \
    { MP_ROM_QSTR(MP_QSTR_ssl_stub), MP_ROM_PTR(&module_ssl_stub) },

#define MICROPY_PORT_ROOT_POINTERS_EXTRA        \
    CAN_STUB_ROOT_POINTERS                      \
    DS18B20_STUB_ROOT_POINTERS                  \
    OWI_STUB_ROOT_POINTERS                      \
    SD_STUB_ROOT_POINTERS                       \
    SOCKET_STUB_ROOT_POINTERS                   \
    SSL_STUB_ROOT_POINTERS

/* Changes of the default Simba configuration. */
#include "simba_config.h"

#endif
