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

#ifndef __PUMBAA_H__
#define __PUMBAA_H__

#include "simba.h"

#include "pumbaa_config_default.h"

#include "genhdr/mpversion.h"

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objstr.h"
#include "py/smallint.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/stackctrl.h"
#include "py/mphal.h"
#include "py/frozenmod.h"
#include "py/stream.h"
#include "lib/utils/pyexec.h"
#include "lib/mp-readline/readline.h"

#include "module_kernel/class_timer.h"
#include "module_sync/class_event.h"
#include "module_sync/class_queue.h"
#include "module_drivers/class_pin.h"
#include "module_drivers/class_uart.h"
#include "module_inet/class_http_server.h"
#include "module_inet/class_http_server_websocket.h"

#if defined(FAMILY_SAM)
#    include "module_drivers/class_adc.h"
#    include "module_drivers/class_dac.h"
#    include "module_drivers/class_exti.h"
#    include "module_drivers/class_spi.h"
#    include "module_drivers/class_sd.h"
#    include "module_drivers/class_owi.h"
#    include "module_drivers/class_ds18b20.h"
#    include "module_drivers/class_can.h"
#endif

#if defined(FAMILY_LINUX)
#    include "module_drivers/class_adc.h"
#    include "module_drivers/class_dac.h"
#    include "module_drivers/class_exti.h"
#    include "module_drivers/class_spi.h"
#    include "module_drivers/class_sd.h"
#    include "module_drivers/class_i2c_soft.h"
#    include "module_drivers/class_owi.h"
#    include "module_drivers/class_ds18b20.h"
#    include "module_drivers/class_can.h"
#endif

#if defined(ARCH_ESP)
#    include "module_drivers/class_exti.h"
#    include "module_drivers/class_spi.h"
#    include "module_drivers/class_esp_wifi.h"
#endif

#if defined(ARCH_ESP32)
#    include "module_drivers/class_adc.h"
#    include "module_drivers/class_dac.h"
#    include "module_drivers/class_esp_wifi.h"
#    include "module_drivers/class_owi.h"
#    include "module_drivers/class_spi.h"
#    include "module_drivers/class_ds18b20.h"
#    include "module_drivers/class_can.h"
#endif

extern void *mp_thread_add_begin(void);
extern void mp_thread_add_end(void *thread_p, struct thrd_t *thrd_p);

struct class_socket_t {
    mp_obj_base_t base;
    struct socket_t socket;
};

#endif
