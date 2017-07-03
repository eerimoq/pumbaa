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

/**
 * Function called when this module is imported.
 */
static mp_obj_t module_init(void)
{
#if CONFIG_PUMBAA_CLASS_PIN == 1
    pin_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_EXTI == 1
    exti_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_ADC == 1
    adc_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_ESP_WIFI == 1
    esp_wifi_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_DS18B20 == 1
    ds18b20_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_FLASH == 1
    flash_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_I2C == 1
    i2c_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_I2C_SOFT == 1
    i2c_soft_module_init();
#endif
#if CONFIG_PUMBAA_CLASS_EEPROM_I2C == 1
    eeprom_i2c_module_init();
#endif

    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * A table of all the modules' global objects.
 */
static const mp_rom_map_elem_t module_drivers_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_drivers) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Module classes. */
#if CONFIG_PUMBAA_CLASS_ADC == 1
    { MP_ROM_QSTR(MP_QSTR_Adc), MP_ROM_PTR(&module_drivers_class_adc) },
#endif
#if CONFIG_PUMBAA_CLASS_CAN == 1
    { MP_ROM_QSTR(MP_QSTR_Can), MP_ROM_PTR(&module_drivers_class_can) },
#endif
#if CONFIG_PUMBAA_CLASS_DAC == 1
    { MP_ROM_QSTR(MP_QSTR_Dac), MP_ROM_PTR(&module_drivers_class_dac) },
#endif
#if CONFIG_PUMBAA_CLASS_EXTI == 1
    { MP_ROM_QSTR(MP_QSTR_Exti), MP_ROM_PTR(&module_drivers_class_exti) },
#endif
#if CONFIG_PUMBAA_CLASS_PIN == 1
    { MP_ROM_QSTR(MP_QSTR_Pin), MP_ROM_PTR(&module_drivers_class_pin) },
#endif
#if CONFIG_PUMBAA_CLASS_SD == 1
    { MP_ROM_QSTR(MP_QSTR_Sd), MP_ROM_PTR(&module_drivers_class_sd) },
#endif
#if CONFIG_PUMBAA_CLASS_SPI == 1
    { MP_ROM_QSTR(MP_QSTR_Spi), MP_ROM_PTR(&module_drivers_class_spi) },
#endif
#if CONFIG_PUMBAA_CLASS_I2C == 1
    { MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&module_drivers_class_i2c) },
#endif
#if CONFIG_PUMBAA_CLASS_I2C_SOFT == 1
    { MP_ROM_QSTR(MP_QSTR_I2CSoft), MP_ROM_PTR(&module_drivers_class_i2c_soft) },
#endif
#if CONFIG_PUMBAA_CLASS_EEPROM_I2C == 1
    { MP_ROM_QSTR(MP_QSTR_EepromI2C), MP_ROM_PTR(&module_drivers_class_eeprom_i2c) },
#endif
#if CONFIG_PUMBAA_CLASS_ESP_WIFI == 1
    { MP_ROM_QSTR(MP_QSTR_esp_wifi), MP_ROM_PTR(&module_drivers_esp_wifi_obj) },
#endif
#if CONFIG_PUMBAA_CLASS_OWI == 1
    { MP_ROM_QSTR(MP_QSTR_Owi), MP_ROM_PTR(&module_drivers_class_owi) },
#endif
#if CONFIG_PUMBAA_CLASS_DS18B20 == 1
    { MP_ROM_QSTR(MP_QSTR_Ds18b20), MP_ROM_PTR(&module_drivers_class_ds18b20) },
#endif
#if CONFIG_PUMBAA_CLASS_UART == 1
    { MP_ROM_QSTR(MP_QSTR_Uart), MP_ROM_PTR(&module_drivers_class_uart) },
#endif
#if CONFIG_PUMBAA_CLASS_FLASH == 1
    { MP_ROM_QSTR(MP_QSTR_Flash), MP_ROM_PTR(&module_drivers_class_flash) },
#endif
#if CONFIG_PUMBAA_CLASS_WS2812 == 1
    { MP_ROM_QSTR(MP_QSTR_Ws2812), MP_ROM_PTR(&module_drivers_class_ws2812) },
#endif
};

static MP_DEFINE_CONST_DICT(module_drivers_globals, module_drivers_globals_table);

const mp_obj_module_t module_drivers = {
    { &mp_type_module },
    .globals = (mp_obj_t)&module_drivers_globals,
};
