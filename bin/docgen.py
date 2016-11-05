#!/usr/bin/env python

"""Generate documentation.
"""

import os
import argparse
import json
import subprocess


BOARD_FMT = """{desc}
{desc_underline}

Pinout and general information
------------------------------

Simba documentation: http://simba-os.readthedocs.io/en/latest/boards/{name}.html

Memory usage
------------

Below is the memory usage of two applications:

- The
  :github-tree:`minimal-configuration<examples/minimal-configuration>`
  application is configured to only include the bare minimum of
  functionality.

- The
  :github-tree:`default-configuration<examples/default-configuration>`
  application is built with the default configuration.

+--------------------------+-----------+-----------+
| Application              | Flash     | RAM       |
+==========================+===========+===========+
{memory_usage}
+--------------------------+-----------+-----------+

Default configuration
---------------------

Default Standard Library configuration.

+--------------------------------------------------------+-----------------------------------------------------+
|  Name                                                  |  Value                                              |
+========================================================+=====================================================+
{default_configuration}
"""


CONFIG_FMT = """|  {:53} |  {:50} |
+--------------------------------------------------------+-----------------------------------------------------+
"""

SOURCE_CODE_FMT = """.. code-block:: python

{source}
"""


def boards_generate(database):
    """Generate boards.

    """

    for board, data in database["boards"].items():
        # Default configuration.
        default_configuration = ""
        for config in data["default-configuration"]:
            default_configuration += CONFIG_FMT.format(config[0], config[1])

        # Memory usage.
        applications = [
            "minimal-configuration",
            "default-configuration"
        ]
        memory_usage = []
        for application in applications:
            subprocess.check_call(['make',
                                   '-s',
                                   '-C', os.path.join('examples', application),
                                   'BOARD=' + board,
                                   'all'])
            sizes_json = subprocess.check_output(['make',
                                                  '-s',
                                                  '-C', os.path.join('examples', application),
                                                  'BOARD=' + board,
                                                  'size-json'])
            sizes = json.loads(sizes_json)
            memory_usage.append('| {application:24} | {program:9} | {data:9} |'.format(
                application=application,
                program=sizes['program'],
                data=sizes['data']))

        rst = BOARD_FMT.format(name=board,
                               desc=data["board_desc"],
                               desc_underline="=" * len(data["board_desc"]),
                               default_configuration=default_configuration,
                               memory_usage='\n+-{}-+-----------+-----------+\n'.format(
                                   24 * '-').join(memory_usage))

        rst_path = os.path.join("docs", "boards", board + ".rst")
        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def examples_generate():
    """Generate examples.

    """

    examples = [
        "blink",
        "hello_world",
        "select"
    ]

    for example in examples:
        c_path = os.path.join("examples", example, "main.py")

        source = []
        with open(c_path) as fin:
            for line in fin.readlines():
                source.append("   " + line)

        rst = SOURCE_CODE_FMT.format(source=''.join(source))

        rst_path = os.path.join("docs", "examples", example, "source-code.rst")

        print "Writing to", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def format_comma_separated_list(lst):
    if len(lst) == 0:
        return ''
    elif len(lst) == 1:
        return lst[0] + '.'
    else:
        return ', '.join(lst[:-1]) + ' and ' + lst[-1] + '.'


def drivers_generate(database):
    pin_availability = []
    exti_availability = []
    dac_availability = []
    spi_availability = []
    i2c_soft_availability = []
    sd_availability = []
    esp_wifi_availability = []

    pumbaa_path = os.path.join("docs",
                               "library-reference",
                               "pumbaa")
    drivers_template_rst_path = os.path.join(pumbaa_path,
                                             "drivers_template.rst")
    drivers_rst_path = os.path.join(pumbaa_path,
                                    "drivers.rst")

    boards = database['boards']
    for board, data in boards.items():
        board_rst = ':doc:`../../boards/{}`'.format(board)
        if 'pin' in data['drivers']:
            pin_availability.append(board_rst)
        if 'exti' in data['drivers']:
            exti_availability.append(board_rst)
        if 'dac' in data['drivers']:
            dac_availability.append(board_rst)
        if 'spi' in data['drivers']:
            spi_availability.append(board_rst)
        if 'i2c_soft' in data['drivers']:
            i2c_soft_availability.append(board_rst)
        if 'sd' in data['drivers']:
            sd_availability.append(board_rst)
        if 'esp_wifi' in data['drivers']:
            esp_wifi_availability.append(board_rst)

    with open(drivers_template_rst_path) as fin:
        template = fin.read()

    formatted = template.format(
        pin_availability=format_comma_separated_list(pin_availability),
        exti_availability=format_comma_separated_list(exti_availability),
        dac_availability=format_comma_separated_list(dac_availability),
        spi_availability=format_comma_separated_list(spi_availability),
        i2c_soft_availability=format_comma_separated_list(i2c_soft_availability),
        sd_availability=format_comma_separated_list(sd_availability),
        esp_wifi_availability=format_comma_separated_list(esp_wifi_availability))

    print "Writing to", drivers_rst_path

    with open(drivers_rst_path, "w") as fout:
        fout.write(formatted)


def main():
    """Main.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("database", help="JSON database.")
    args = parser.parse_args()

    with open(args.database) as fin:
        database = json.load(fin)

    examples_generate()
    drivers_generate(database)
    boards_generate(database)

if __name__ == "__main__":
    main()
