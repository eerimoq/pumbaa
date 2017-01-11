/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
 * This file is part of the Simba project.
 */

#include "pumbaa.h"

static struct owi_device_t devices[4] = {
    { .id = { DS18B20_FAMILY_CODE, '2', '3', '4', '5', '6', '7', '8' } },
    { .id = { DS18B20_FAMILY_CODE, '2', '3', '4', '5', '6', '7', '9' } },
    { .id = { '1', '2', '3', '4', '5', '6', '7', '8' } },
    { .id = { '1', '2', '3', '4', '5', '6', '7', '8' } }
};

int owi_init(struct owi_driver_t *self_p,
             struct pin_device_t *dev_p,
             struct owi_device_t *devices_p,
             size_t nmemb)
{
    self_p->devices_p = &devices[0];
    self_p->len = membersof(devices);

    return (0);
}

int owi_reset(struct owi_driver_t *self_p)
{
    return (0);
}

int owi_search(struct owi_driver_t *self_p)
{
    return (4);
}

ssize_t owi_read(struct owi_driver_t *self_p,
                 void *buf_p,
                 size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *response_buf_p;
    size_t response_size;
    ssize_t res;

    BTASSERT(buf_p != NULL);

    response_buf_p = "read()";
    response_size = 6;
    res = 6;
    
    if (MP_STATE_VM(owi_stub_read_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(owi_stub_read_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            response_size = buffer_info.len;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            response_buf_p = NULL;
            response_size = 0;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_buf_p = buffer_info.buf;
            response_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(owi_stub_read_obj), item);

        if (len == 1) {
            MP_STATE_VM(owi_stub_read_obj) = mp_const_none;
        }
    }

    if (response_buf_p != NULL) {
        BTASSERT(size == response_size);
        memcpy(buf_p, response_buf_p, size);
    }

    return (res);
}

ssize_t owi_write(struct owi_driver_t *self_p,
                  const void *buf_p,
                  size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;
    const char *expected_buf_p;
    size_t expected_size;

    BTASSERT(buf_p != NULL);

    expected_buf_p = "send()";
    expected_size = 6;
    res = 6;

    if (MP_STATE_VM(owi_stub_write_obj) != mp_const_none) {
        /* Compare with the first element in the list. */
        mp_obj_list_get(MP_STATE_VM(owi_stub_write_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. buf (bytes)
           2. res (integer)
           2. (buf, res) */
        if (MP_OBJ_IS_TYPE(item, &mp_type_bytes)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = buffer_info.len;
        } else if (MP_OBJ_IS_INT(item)) {
            expected_buf_p = NULL;
            expected_size = 0;
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_buf_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(owi_stub_write_obj), item);

        if (len == 1) {
            MP_STATE_VM(owi_stub_write_obj) = mp_const_none;
        }
    }

    if (expected_buf_p != NULL) {
        BTASSERT(size == expected_size);
        BTASSERT(memcmp(expected_buf_p, buf_p, size) == 0);
    }

    return (res);
}

static mp_obj_t module_set_read(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(owi_stub_read_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_write(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(owi_stub_write_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_init(void)
{
    MP_STATE_VM(owi_stub_read_obj) = mp_const_none;
    MP_STATE_VM(owi_stub_write_obj) = mp_const_none;

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_read_obj, module_set_read);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_write_obj, module_set_write);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_owi_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_owi_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Functions. */
    { MP_ROM_QSTR(MP_QSTR_set_read), MP_ROM_PTR(&module_set_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_write), MP_ROM_PTR(&module_set_write_obj) },
};

static MP_DEFINE_CONST_DICT(module_owi_stub_globals, module_owi_stub_globals_table);

const mp_obj_module_t module_owi_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_owi_stub_globals,
};
