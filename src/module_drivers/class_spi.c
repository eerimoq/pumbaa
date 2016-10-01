/**
 * @file module_drivers/class_spi.c
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

/**
 * Print the spi object.
 */
static void class_spi_print(const mp_print_t *print_p,
                            mp_obj_t self_in,
                            mp_print_kind_t kind)
{
    struct class_spi_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Spi object associated with the id. If additional
 * arguments are given, they are used to initialise the spi. See
 * `init`.
 */
static mp_obj_t class_spi_make_new(const mp_obj_type_t *type_p,
                                   mp_uint_t n_args,
                                   mp_uint_t n_kw,
                                   const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_ss, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_mode, MP_ARG_INT, { .u_int = SPI_MODE_MASTER } },
        { MP_QSTR_speed, MP_ARG_INT, { .u_int = SPI_SPEED_1MBPS } },
        { MP_QSTR_cpol, MP_ARG_INT, { .u_int = 1 } },
        { MP_QSTR_cpha, MP_ARG_INT, { .u_int = 1 } }
    };
    struct class_spi_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int device;
    int ss;
    int mode;
    int speed;
    int cpol;
    int cpha;
    
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    /* Create a new spi object. */
    self_p = m_new_obj(struct class_spi_t);
    self_p->base.type = &module_drivers_class_spi;

    device = args[0].u_int;
    ss = args[1].u_int;
    mode = args[2].u_int;
    speed = args[3].u_int;
    cpol = args[4].u_int;
    cpha = args[5].u_int;

    if (spi_init(&self_p->drv,
                 &spi_device[device],
                 &pin_device[ss],
                 mode,
                 speed,
                 cpol,
                 cpha) != 0) {
        return (mp_const_none);
    }

    return (self_p);
}

/**
 * def start(self)
 */
static mp_obj_t class_spi_start(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_start(&self_p->drv);
    
    return (mp_const_none);
}

/**
 * def stop(self, samples)
 */
static mp_obj_t class_spi_stop(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_stop(&self_p->drv);

    return (mp_const_none);
}

/**
 * def take_bus(self)
 */
static mp_obj_t class_spi_take_bus(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_take_bus(&self_p->drv);

    return (mp_const_none);
}

/**
 * def give_bus(self)
 */
static mp_obj_t class_spi_give_bus(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_give_bus(&self_p->drv);

    return (mp_const_none);
}

/**
 * def select(self)
 */
static mp_obj_t class_spi_select(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_select(&self_p->drv);

    return (mp_const_none);
}

/**
 * def deselect(self)
 */
static mp_obj_t class_spi_deselect(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    spi_deselect(&self_p->drv);

    return (mp_const_none);
}

/**
 * def transfer(self, read_buffer, write_buffer)
 */
static mp_obj_t class_spi_transfer(mp_obj_t self_in,
                                   mp_obj_t read_buffer_in,
                                   mp_obj_t write_buffer_in)
{
    struct class_spi_t *self_p;
    mp_buffer_info_t read_buffer_info;
    mp_buffer_info_t write_buffer_info;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(read_buffer_in),
                        &read_buffer_info,
                        MP_BUFFER_WRITE);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(write_buffer_in),
                        &write_buffer_info,
                        MP_BUFFER_READ);

    /* Read and write buffers must have the same length. */
    if (read_buffer_info.len != write_buffer_info.len) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }
    
    return (MP_OBJ_NEW_SMALL_INT(spi_transfer(&self_p->drv,
                                              read_buffer_info.buf,
                                              write_buffer_info.buf,
                                              read_buffer_info.len)));
}

/**
 * def read(self, buffer)
 */
static mp_obj_t class_spi_read(mp_obj_t self_in, mp_obj_t buffer_in)
{
    struct class_spi_t *self_p;
    mp_buffer_info_t buffer_info;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(buffer_in),
                        &buffer_info,
                        MP_BUFFER_WRITE);
    
    return (MP_OBJ_NEW_SMALL_INT(spi_read(&self_p->drv,
                                          buffer_info.buf,
                                          buffer_info.len)));
}

/**
 * def write(self, buffer)
 */
static mp_obj_t class_spi_write(mp_obj_t self_in, mp_obj_t buffer_in)
{
    struct class_spi_t *self_p;
    mp_buffer_info_t buffer_info;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    mp_get_buffer_raise(MP_OBJ_TO_PTR(buffer_in),
                        &buffer_info,
                        MP_BUFFER_READ);
    
    return (MP_OBJ_NEW_SMALL_INT(spi_write(&self_p->drv,
                                           buffer_info.buf,
                                           buffer_info.len)));
}

/**
 * def get(self)
 */
static mp_obj_t class_spi_get(mp_obj_t self_in)
{
    struct class_spi_t *self_p;
    uint8_t data;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    
    spi_get(&self_p->drv, &data);

    return (MP_OBJ_NEW_SMALL_INT(data));
}

/**
 * def put(self, data)
 */
static mp_obj_t class_spi_put(mp_obj_t self_in, mp_obj_t data_in)
{
    struct class_spi_t *self_p;
    
    self_p = MP_OBJ_TO_PTR(self_in);
    
    return (MP_OBJ_NEW_SMALL_INT(spi_put(&self_p->drv,
                                         mp_obj_get_int(data_in))));
}

static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_start_obj, class_spi_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_stop_obj, class_spi_stop);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_take_bus_obj, class_spi_take_bus);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_give_bus_obj, class_spi_give_bus);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_select_obj, class_spi_select);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_deselect_obj, class_spi_deselect);
static MP_DEFINE_CONST_FUN_OBJ_3(class_spi_transfer_obj, class_spi_transfer);
static MP_DEFINE_CONST_FUN_OBJ_2(class_spi_read_obj, class_spi_read);
static MP_DEFINE_CONST_FUN_OBJ_2(class_spi_write_obj, class_spi_write);
static MP_DEFINE_CONST_FUN_OBJ_1(class_spi_get_obj, class_spi_get);
static MP_DEFINE_CONST_FUN_OBJ_2(class_spi_put_obj, class_spi_put);

static const mp_rom_map_elem_t class_spi_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&class_spi_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&class_spi_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_take_bus), MP_ROM_PTR(&class_spi_take_bus_obj) },
    { MP_ROM_QSTR(MP_QSTR_give_bus), MP_ROM_PTR(&class_spi_give_bus_obj) },
    { MP_ROM_QSTR(MP_QSTR_select), MP_ROM_PTR(&class_spi_select_obj) },
    { MP_ROM_QSTR(MP_QSTR_deselect), MP_ROM_PTR(&class_spi_deselect_obj) },
    { MP_ROM_QSTR(MP_QSTR_transfer), MP_ROM_PTR(&class_spi_transfer_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&class_spi_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&class_spi_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&class_spi_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_put), MP_ROM_PTR(&class_spi_put_obj) },

    /* Class constants. */
    { MP_ROM_QSTR(MP_QSTR_MODE_MASTER), MP_ROM_INT(SPI_MODE_MASTER) },
    { MP_ROM_QSTR(MP_QSTR_MODE_SLAVE), MP_ROM_INT(SPI_MODE_SLAVE) },
};

static MP_DEFINE_CONST_DICT(class_spi_locals_dict, class_spi_locals_dict_table);

/**
 * Spi class type.
 */
const mp_obj_type_t module_drivers_class_spi = {
    { &mp_type_type },
    .name = MP_QSTR_Spi,
    .print = class_spi_print,
    .make_new = class_spi_make_new,
    .locals_dict = (mp_obj_t)&class_spi_locals_dict,
};
