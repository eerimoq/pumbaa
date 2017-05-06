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

#if CONFIG_PUMBAA_CLASS_ADC == 1

/**
 * Print the adc object.
 */
static void class_adc_print(const mp_print_t *print_p,
                            mp_obj_t self_in,
                            mp_print_kind_t kind)
{
    struct class_adc_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);
    mp_printf(print_p, "<0x%p>", self_p);
}

/**
 * Create a new Adc object associated with the id. If additional
 * arguments are given, they are used to initialise the adc. See
 * `init`.
 */
static mp_obj_t class_adc_make_new(const mp_obj_type_t *type_p,
                                   mp_uint_t n_args,
                                   mp_uint_t n_kw,
                                   const mp_obj_t *args_p)
{
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pin_device, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_reference, MP_ARG_INT, { .u_int = ADC_REFERENCE_VCC } },
        { MP_QSTR_sampling_rate, MP_ARG_INT, { .u_int = 10 } }
    };
    struct class_adc_t *self_p;
    mp_map_t kwargs;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    int device;
    int pin_dev;
    int reference;
    int sampling_rate;

    mp_arg_check_num(n_args, n_kw, 2, MP_OBJ_FUN_ARGS_MAX, true);

    /* Parse args. */
    mp_map_init(&kwargs, 0);
    mp_arg_parse_all(n_args,
                     args_p,
                     &kwargs,
                     MP_ARRAY_SIZE(allowed_args),
                     allowed_args,
                     args);

    device = args[0].u_int;
    pin_dev = args[1].u_int;
    reference = args[2].u_int;
    sampling_rate = args[3].u_int;

    if ((device < 0) || (device >= ADC_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad device"));
    }

    if ((pin_dev < 0) || (pin_dev >= PIN_DEVICE_MAX)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad pin device"));
    }

    if (reference != ADC_REFERENCE_VCC) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad reference"));
    }

    if (sampling_rate <= 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError,
                                           "bad sampling rate"));
    }

    /* Create a new adc object. */
    self_p = m_new0(struct class_adc_t, 1);
    self_p->base.type = &module_drivers_class_adc;

    if (adc_init((struct adc_driver_t *)&self_p->drv,
                 &adc_device[device],
                 &pin_device[pin_dev],
                 reference,
                 sampling_rate) != 0) {
        return (mp_const_none);
    }

    return (self_p);
}

/**
 * def convert(self, samples)
 */
static mp_obj_t class_adc_convert(mp_obj_t self_in,
                                  mp_obj_t number_of_samples_in)
{
    struct class_adc_t *self_p;
    int number_of_samples;

    self_p = MP_OBJ_TO_PTR(self_in);
    number_of_samples = mp_obj_get_int(number_of_samples_in);

    vstr_init_len(&self_p->vstr, sizeof(uint16_t) * number_of_samples);

    if (adc_convert(&self_p->drv,
                    (uint16_t *)self_p->vstr.buf,
                    number_of_samples) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "adc_convert() failed"));
    }

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &self_p->vstr));
}

/**
 * def async_convert(self, samples)
 */
static mp_obj_t class_adc_async_convert(mp_obj_t self_in,
                                        mp_obj_t number_of_samples_in)
{
    struct class_adc_t *self_p;
    int number_of_samples;

    self_p = MP_OBJ_TO_PTR(self_in);
    number_of_samples = mp_obj_get_int(number_of_samples_in);

    /* Must be stored in the object. Later returned by the wait
       function. */
    vstr_init_len(&self_p->vstr, sizeof(uint16_t) * number_of_samples);

    if (adc_async_convert(&self_p->drv,
                          (uint16_t *)self_p->vstr.buf,
                          number_of_samples) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "adc_async_convert() failed"));
    }

    return (mp_const_none);
}

/**
 * def async_wait(self)
 */
static mp_obj_t class_adc_async_wait(mp_obj_t self_in)
{
    struct class_adc_t *self_p;

    self_p = MP_OBJ_TO_PTR(self_in);

    if (adc_async_wait(&self_p->drv) != 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError,
                                           "adc_async_wait() failed"));
    }

    return (mp_obj_new_str_from_vstr(&mp_type_bytes, &self_p->vstr));
}

static MP_DEFINE_CONST_FUN_OBJ_2(class_adc_convert_obj, class_adc_convert);
static MP_DEFINE_CONST_FUN_OBJ_2(class_adc_async_convert_obj, class_adc_async_convert);
static MP_DEFINE_CONST_FUN_OBJ_1(class_adc_async_wait_obj, class_adc_async_wait);

static const mp_rom_map_elem_t class_adc_locals_dict_table[] = {
    /* Instance methods. */
    { MP_ROM_QSTR(MP_QSTR_convert), MP_ROM_PTR(&class_adc_convert_obj) },
    { MP_ROM_QSTR(MP_QSTR_async_convert), MP_ROM_PTR(&class_adc_async_convert_obj) },
    { MP_ROM_QSTR(MP_QSTR_async_wait), MP_ROM_PTR(&class_adc_async_wait_obj) },

    /* Class constants. */
    { MP_ROM_QSTR(MP_QSTR_REFERENCE_VCC), MP_ROM_INT(ADC_REFERENCE_VCC) },
};

static MP_DEFINE_CONST_DICT(class_adc_locals_dict, class_adc_locals_dict_table);

/**
 * Adc class type.
 */
const mp_obj_type_t module_drivers_class_adc = {
    { &mp_type_type },
    .name = MP_QSTR_Adc,
    .print = class_adc_print,
    .make_new = class_adc_make_new,
    .locals_dict = (mp_obj_t)&class_adc_locals_dict,
};

#endif
