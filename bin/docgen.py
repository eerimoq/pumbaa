#!/usr/bin/env python

"""Generate documentation.
"""

import os
import argparse
import json
import subprocess
import re

BOARD_FMT = """{desc}
{desc_underline}

Pinout and general information
------------------------------

Simba documentation: http://simba-os.readthedocs.io/en/latest/boards/{name}.html

Drivers
-------

Supported drivers for this board.

{drivers}

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


def driver_file_name_to_class(filename):
    class_map = {
        'i2c_soft': 'I2CSoft',
        'esp_wifi': 'esp_wifi'
    }
    name = filename[6:-2]
    try:
        return class_map[name]
    except KeyError:
        return name[0].upper() + name[1:]


def boards_generate(database):
    """Generate boards.

    """

    for board, data in database["boards"].items():
        # Board drivers.
        drivers = []
        for driver in sorted(data["src"]):
            mo = re.match(r'src/module_drivers/(.+)', driver)
            if mo:
                drivers.append('- :class:`{name}<drivers.drivers.{name}>`'.format(name=driver_file_name_to_class(mo.group(1))))

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
                               drivers='\n'.join(drivers),
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
        "select",
        "tcp_server"
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


def main():
    """Main.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("database", help="JSON database.")
    args = parser.parse_args()

    with open(args.database) as fin:
        database = json.load(fin)

    examples_generate()
    boards_generate(database)

if __name__ == "__main__":
    main()
