/**
 * @file pumbaa.h
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

#ifndef __PUMBAA_H__
#define __PUMBAA_H__

#include "simba.h"

#include <pumbaa_config.h>
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

#include "class_board.h"
#include "module_simba/class_event.h"
#include "module_simba/class_pin.h"
#include "module_simba/class_timer.h"

#if defined(FAMILY_SAM)
#    include "module_simba/class_dac.h"
#endif

#if defined(FAMILY_LINUX)
#    include "module_simba/class_dac.h"
#endif

#endif
