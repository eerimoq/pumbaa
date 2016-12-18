#!/usr/bin/env python

from __future__ import print_function

import subprocess
import argparse
import json
import os
import shutil


SIMBA_GEN_C_FMT = """
#include "simba.h"

const FAR char sysinfo[] = "app:    myapp built - by -.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""

PLATFORMIO_SCONSSCRIPT_FMT = """#
# @file make/platformio.sconscript
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2014-2016, Erik Moqvist
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

import os
from os.path import join
import subprocess
import shutil

from SCons.Script import DefaultEnvironment
from platformio.builder.tools.platformio import SRC_DEFAULT_FILTER

# Map the PlatformIO board name to the Simba board name
BOARD_MAP = {{
    "due": "arduino_due"
}}

# Map the PlatformIO board name to the Simba board name
SUPPORTED_BOARDS = [
    "arduino_due",
    "nano32"
]


BOARDS = {boards}


def add_include_paths(env, paths, board):
    \"\"\"Add given list of include paths.

    \"\"\"

    env.Append(CPPPATH=["$PROJECTSRC_DIR"])

    paths += [join("make", "platformio", board)]

    for path in paths:
        env.VariantDirWrap(
            join("$BUILD_DIR", "PumbaaFramework", path),
            join("$PLATFORMFW_DIR", path)
        )
        env.Append(CPPPATH=[join("$BUILD_DIR", "PumbaaFramework", path)])


def set_default_values(env):
    \"\"\"Set default values in the environment.

    \"\"\"

    if "VERSION" not in env:
        with open(env.subst(join("$PLATFORMFW_DIR", "VERSION.txt"))) as f:
            env.Append(VERSION=f.read().strip())

    if "NAME" not in env:
        env.Append(NAME="app_name")


def create_src_filter(srcs):
    \"\"\"Create the source filter string.
    \"\"\"

    src_filter = [
        "-<3pp/>",
        "-<micropython/>",
        "-<simba/>",
        "-<src/>",
        "-<tst/>"
    ]

    for src in srcs:
        src_filter.append("+<{{}}>".format(src))

    return SRC_DEFAULT_FILTER + " " + " ".join(src_filter)


def setup_board_nano32(env):
    \"\"\"Setup the Nano32 environment.

    \"\"\"

    ccflags = []

    for flag in env["CCFLAGS"]:
        if "-Wpointer-arith" in flag:
            continue
        ccflags.append(flag)
    env.Replace(CCFLAGS=ccflags)


env = DefaultEnvironment()

set_default_values(env)

# Rename the name of boards that have different name in PlatformIO and
# Simba.
if env["BOARD"] in BOARD_MAP:
    board = BOARD_MAP[env["BOARD"]]
else:
    board = env.subst(env["BOARD"])

if board not in SUPPORTED_BOARDS:
   raise ValueError("BOARD {{}} is not supported by Simba.".format(board))

# Add the default configuration for the board.
add_include_paths(env, BOARDS[board]["inc"], board)
env.Append(CPPDEFINES=BOARDS[board]["cdefs"])
src_filter = create_src_filter(BOARDS[board]["src"])
env.Replace(CFLAGS=BOARDS[board]["cflags"])
env.Replace(CXXFLAGS=BOARDS[board]["cxxflags"])
env.Replace(LINKFLAGS=BOARDS[board]["ldflags"])
env.Replace(LIBPATH=[os.path.join("$PLATFORMFW_DIR", path)
                     for path in BOARDS[board]["libpath"]])
env.Replace(MCU_DESC=BOARDS[board]["mcu_desc"])
env.Replace(BOARD_DESC=BOARDS[board]["board_desc"])
env.Append(LIBS=BOARDS[board]["lib"])

# Always replace the map file path.
linkflags = []

for flag in env["LINKFLAGS"]:
    if flag.startswith("-Wl,-Map="):
        flag = "-Wl,-Map=$BUILD_DIR/firmware.map"
    linkflags.append(flag)
env.Replace(LINKFLAGS=linkflags)

# For some boards the configuration has to be modified.
if board == "arduino_due":
    linkflags = []

    for flag in env["LINKFLAGS"]:
        if flag.startswith("-T"):
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)
    env.Replace(LDSCRIPT_PATH="script.ld")
elif board == "nano32":
    setup_board_nano32(env)

# generated files
SIMBA_GEN_C = "$BUILD_DIR/SimbaFramework/simba_gen.c"
FROZEN_C = "$BUILD_DIR/SimbaFramework/frozen.c"

# create a list of all sources
variant_dir = join("$BUILD_DIR", "PumbaaFramework")
src_dir = join("$PLATFORMFW_DIR")

source_files = []

for src in env.LookupSources(variant_dir, src_dir, True, src_filter):
    source_files.append(env.Object(src))

# Command to generate simba_gen.c
env.Command(SIMBA_GEN_C,
            source_files,
            ('"$PYTHONEXE" "$PLATFORMFW_DIR/simba/src/kernel/tools/gen.py" "$NAME" "$VERSION" '
             '"$BOARD_DESC" "$MCU_DESC" "$TARGET"'))
source_files.append(SIMBA_GEN_C)

# Command to generate frozen.c
env.Command(FROZEN_C,
            [],
            '"$PYTHONEXE" "$PLATFORMFW_DIR/bin/make_frozen.py" --output-file "$TARGET"')
source_files.append(FROZEN_C)

lib = env.Library(target=join("$BUILD_DIR", "PumbaaFramework"), source=source_files)

env.Append(LIBS=[lib])
"""


def create_database():
    """Generate the simba database with baord and mcu information.

    """

    return json.loads(subprocess.check_output(["bin/dbgen.py"]))


def generate_platformio_sconsscript(database, version):
    """Generate the platformio scons script.

    """

    pumbaa_root = os.environ["PUMBAA_ROOT"]
    boards = {}

    # Only add selceted parts the database to the scons script for
    # less unnecessary information.
    for board, data in database["boards"].items():
        if board not in ['arduino_due', 'nano32']:
            continue
        # Add everything we need, and a little more.
        selected_data = {
            'inc': data['inc'],
            'cdefs': [cdef for cdef in data['cdefs'] if not cdef.startswith("F_CPU")],
            'src': data['src'],
            'cflags': data['cflags'],
            'cxxflags': data['cxxflags'],
            'libpath': data['libpath'],
            'ldflags': data['ldflags'],
            'linker_script': data['linker_script'],
            'lib': data['lib'],
            'board_desc': data['board_desc'],
            'mcu_desc': database['mcus'][data['mcu']]['mcu_desc']
        }
        boards[board] = selected_data

        # Generate the QSTR file for the default configuration.
        default_configuration_dir = os.path.join(pumbaa_root,
                                                 "examples",
                                                 "default-configuration")
        subprocess.check_call(["make",
                               "-s",
                               "BOARD=" + board],
                              cwd=default_configuration_dir)
        qstr_file = os.path.join(default_configuration_dir,
                                 "build",
                                 board,
                                 "genhdr",
                                 "qstrdefs.generated.h")
        genhdr_dir = os.path.join(pumbaa_root, "make", "platformio", board, "genhdr")
        shutil.copy(qstr_file, genhdr_dir)

    outfile = os.path.join(pumbaa_root, "make", "platformio.sconscript")
    with open(outfile, "w") as fout:
        fout.write(PLATFORMIO_SCONSSCRIPT_FMT.format(
            boards=json.dumps(boards, indent=4, sort_keys=True)))


def main():
    """Create the platformio build script.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--version", required=True)
    args = parser.parse_args()

    print("Creating software database.")
    database = create_database()

    generate_platformio_sconsscript(database, args.version)


if __name__ == "__main__":
    main()
