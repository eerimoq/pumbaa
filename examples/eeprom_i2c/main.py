#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Pumbaa project.
#

from drivers import I2C, EepromI2C

# Change those to match your EEPROM.
EEPROM_I2C_ADDRESS = 0x57
EEPROM_SIZE = 32768

i2c = I2C(0)
i2c.start()
eeprom = EepromI2C(i2c, EEPROM_I2C_ADDRESS, EEPROM_SIZE)

address = 0
data = b'Hello World!'

print('Writing {} to EEPROM address {}:'.format(data, address))

eeprom.write(address, data)
read_data = eeprom.read(address, 12)

print('Read {} from EEPROM address {}.'.format(read_data, address))
