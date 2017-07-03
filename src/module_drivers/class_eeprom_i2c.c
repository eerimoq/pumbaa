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

#include "pumbaa.h"

#if CONFIG_PUMBAA_CLASS_EEPROM_I2C == 1

/**
 * Print the eeprom i2c object.
 */
static void class_eeprom_i2c_print(const mp_print_t *print_p,
                                   mp_obj_t self_in,
                                   mp_print_kind_t kind)
{
    struct class_eeprom_i2c_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new I2CSoft object associated with the id.
 */
static mp_obj_t class_eeprom_i2c_make_new(const mp_obj_type_t *type_p,
                                          mp_uint_t n_args,
                                          mp_uint_t n_kw,
                                          const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_i2c_address, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_size, MP_ARG_REQUIRED | MP_ARG_INT },
    };
    struct class_eeprom_i2c_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_obj_t i2c;
    struct class_i2c_t *i2c_p;
    int i2c_address;
    uint32_t size;

    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* I2C argument. */
    i2c = args[0].u_obj;

    if (!MP_OBJ_IS_TYPE(i2c, &module_drivers_class_i2c)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError,
                                           "I2C object expected"));
    }

    i2c_p = MP_OBJ_TO_PTR(i2c);

    /* I2C address argument. */
    i2c_address = args[1].u_int;

    /* EEPROM size. */
    size = args[2].u_int;

    /* Create a new EepromI2C object. */
    self_p = m_new0(struct class_eeprom_i2c_t, 1);
    self_p->base.type = &module_drivers_class_eeprom_i2c;
    self_p->i2c = i2c;

    if (eeprom_i2c_init(&self_p->drv,
                        &i2c_p->drv,
                        i2c_address,
                        size) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "eeprom_i2c_init() failed"));
    }

    return (self_p);
}

/**
 * def read(self, address, size)
 */
static mp_obj_t class_eeprom_i2c_read(mp_obj_t self_in,
                                      mp_obj_t address_in,
                                      mp_obj_t size_in)
{
    struct class_eeprom_i2c_t *self_p;
    vstr_t vstr;
    uint32_t address;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(self_in);
    address = mp_obj_get_int(address_in);
    size = mp_obj_get_int(size_in);

    vstr_init_len(&vstr, size);
    size = eeprom_i2c_read(&self_p->drv, vstr.buf, address, size);

    if (size < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "eeprom_i2c_read() failed"));
    }

    vstr.len = size;

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr));
}

/**
 * def read_into(self, address, buffer[, size])
 */
static mp_obj_t class_eeprom_i2c_read_into(mp_uint_t n_args,
                                           const mp_obj_t *args_p)
{
    struct class_eeprom_i2c_t *self_p;
    int address;
    mp_buffer_info_t buffer_info;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    address = mp_obj_get_int(args_p[1]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args_p[2]),
                        &buffer_info,
                        MP_BUFFER_WRITE);

    /* Get the size. */
    if (n_args == 4) {
        size = mp_obj_get_int(args_p[3]);

        if (buffer_info.len < size) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                               "bad buffer length"));
        }
    } else {
        size = buffer_info.len;
    }

    size = eeprom_i2c_read(&self_p->drv, buffer_info.buf, address, size);

    if (size < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "eeprom_i2c_read() failed"));
    }

    return (MP_OBJ_NEW_SMALL_INT(size));
}

/**
 * def write(self, address, buffer[, size])
 */
static mp_obj_t class_eeprom_i2c_write(mp_uint_t n_args, const mp_obj_t *args_p)
{
    struct class_eeprom_i2c_t *self_p;
    mp_buffer_info_t buffer_info;
    int address;
    ssize_t size;

    self_p = MP_OBJ_TO_PTR(args_p[0]);
    address = mp_obj_get_int(args_p[1]);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(args_p[2]),
                        &buffer_info,
                        MP_BUFFER_READ);

    /* Get the size. */
    if (n_args == 4) {
        size = mp_obj_get_int(args_p[3]);

        if (buffer_info.len < size) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                               "bad buffer length"));
        }
    } else {
        size = buffer_info.len;
    }

    size = eeprom_i2c_write(&self_p->drv, address, buffer_info.buf, size);

    if (size < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "eeprom_i2c_write() failed"));
    }

    return (MP_OBJ_NEW_SMALL_INT(size));
}

static MP_DEFINE_CONST_FUN_OBJ_3(class_eeprom_i2c_read_obj, class_eeprom_i2c_read);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_eeprom_i2c_read_into_obj, 3, 4, class_eeprom_i2c_read_into);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_eeprom_i2c_write_obj, 3, 4, class_eeprom_i2c_write);

static const mp_rom_map_elem_t class_eeprom_i2c_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_eeprom_i2c_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_into), MP_ROM_PTR(&class_eeprom_i2c_read_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_eeprom_i2c_write_obj) }
};

static MP_DEFINE_CONST_DICT(class_eeprom_i2c_locals_dict, class_eeprom_i2c_locals_dict_table);

/**
 * Eeprom_I2c class type.
 */
const mp_obj_type_t module_drivers_class_eeprom_i2c = {
    { &mp_type_type },
    .name = MP_QSTR_Eeprom_I2c,
    .print = class_eeprom_i2c_print,
    .make_new = class_eeprom_i2c_make_new,
    .locals_dict = (mp_obj_t)&class_eeprom_i2c_locals_dict,
};

#endif
