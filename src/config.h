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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifndef CONFIG_ASSERT
#    define CONFIG_ASSERT                                   0
#endif

#ifndef CONFIG_START_NETWORK
#    define CONFIG_START_NETWORK                            0
#endif

#ifndef CONFIG_START_SHELL
#    define CONFIG_START_SHELL                              0
#endif

#ifndef CONFIG_THRD_IDLE_STACK_SIZE
#    if defined(ARCH_ARM)
#        define CONFIG_THRD_IDLE_STACK_SIZE              1024
#    endif
#endif

#ifndef CONFIG_START_FILESYSTEM_ADDRESS
#    if defined(BOARD_ARDUINO_DUE)
#        define CONFIG_START_FILESYSTEM_ADDRESS    0x000f8000
#    endif
#endif

#endif
