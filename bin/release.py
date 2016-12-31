#!/usr/bin/env python

"""Release handling.

"""

from __future__ import print_function

import sys
import os
import subprocess
import glob
import argparse
import hashlib
import json
import shutil
from collections import OrderedDict

PACKAGE_ESP32_RELEASE_ENTRY_FMT = """
{{
    "name": "Pumbaa",
    "architecture": "esp32",
    "version": "{version}",
    "category": "Pumbaa",
    "help": {{
        "online": "https://pumbaa.readthedocs.org"
    }},
    "url": "https://github.com/eerimoq/pumbaa-releases/raw/master/arduino/esp32/pumbaa-arduino-esp32-{version}.zip",
    "archiveFileName": "pumbaa-arduino-esp32-{version}.zip",
    "checksum": "SHA-256:{sha256sum}",
    "size": "{size}",
    "boards": [
        {{"name" : "Nano32"}}
    ],
    "toolsDependencies": [
        {{
            "packager": "Pumbaa",
            "version": "1.22.0-59",
            "name": "xtensa-esp32-elf-gcc"
        }}
    ]
}}"""

PACKAGE_SAM_RELEASE_ENTRY_FMT = """
{{
  "name": "Pumbaa",
  "architecture": "sam",
  "version": "{version}",
  "category": "Pumbaa",
  "help": {{
    "online": "https://pumbaa.readthedocs.org"
  }},
  "url": "https://github.com/eerimoq/pumbaa-releases/raw/master/arduino/sam/pumbaa-arduino-sam-{version}.zip",
  "archiveFileName": "pumbaa-arduino-sam-{version}.zip",
  "checksum": "SHA-256:{sha256sum}",
  "size": "{size}",
  "boards": [
      {{"name" : "Arduino Due"}}
  ],
  "toolsDependencies": [
    {{
      "packager": "arduino",
      "name": "arm-none-eabi-gcc",
      "version": "4.8.3-2014q1"
    }},
    {{
      "packager": "arduino",
      "name": "bossac",
      "version": "1.6.1-arduino"
    }}
  ]
}}"""

PACKAGE_RELEASE_ENTRY_FMTS = {
    'esp32': PACKAGE_ESP32_RELEASE_ENTRY_FMT,
    'sam': PACKAGE_SAM_RELEASE_ENTRY_FMT
}

def git_clean_dfx():
    """Clean.

    """

    command = [
        'git',
        'submodule',
        'foreach',
        '--recursive',
        'git',
        'clean',
        '-dfx']
    subprocess.check_call(command)

    command = [
        'git',
        'clean',
        '-dfx']
    subprocess.check_call(command)


def git_check_no_modified_files():
    """Make sure that there are no modified files.

    """

    command = [
        'git',
        'submodule',
        'foreach',
        '--recursive',
        'git',
        'diff',
        '--quiet',
        'HEAD']
    subprocess.check_call(command)

    command = [
        'git',
        'diff',
        '--quiet',
        'HEAD']
    subprocess.check_call(command)


def test():
    """Test.

    """

    all_boards = glob.glob("src/boards/*")
    boards = set(all_boards) - set("cygwin")

    for board in boards:
        git_clean_dfx()

        # Building one application is enough to ensure that all code
        # in src/ compiles.
        command = [
            "make",
            "TESTS=examples/default-configuration",
            "BOARD=" + os.path.basename(board),
            "all"
        ]

        print(" ".join(command))

        subprocess.check_call(command)

    # Run linux tests.
    git_clean_dfx()

    command = [
        "make",
        "test"
    ]

    print(" ".join(command))

    subprocess.check_call(command)


def generate_docs():
    """Generate the documentation.

    """

    command = [
        "make",
        "docs"
    ]

    subprocess.check_call(command)


def package(version):
    """Create the release artifacts for Arduino IDE and PlatformIO.

    """
    
    git_clean_dfx()
    try:
        git_check_no_modified_files()
    except:
        print('Modified files under version control are not allowed '
              'when creating a release.')
        sys.exit(1)
    
    # Create the PlatformIO package.
    package_name = version + '.zip'
    command = [
        'git-archive-all',
        '--prefix', 'pumbaa-' + version,
        package_name
    ]
    print('Creating', package_name)
    subprocess.check_call(command)

    # Calculate the SHA1 checksum of the archive.
    with open(package_name, 'rb') as f:
        sha1sum = hashlib.sha1(f.read()).hexdigest()
    print('sha1sum of {} is {}:'.format(package_name, sha1sum))
    
    # Add the release to the PlatformIO manifest.
    manifest_name = 'make/platformio/manifest.json'
    with open(manifest_name) as f:
        data = json.load(f, object_pairs_hook=OrderedDict)

    release_entry = OrderedDict([
        ('url', 'https://github.com/eerimoq/pumbaa-releases/raw/master/platformio/{}.zip'.format(version)),
        ('sha1', sha1sum),
        ('version', version)
    ])
    data['framework-pumbaa'].insert(0, release_entry)

    with open(manifest_name, 'w') as f:
        f.write(json.dumps(data, indent=4, separators=(',', ': ')))

    platformio_release_dir = '../pumbaa-releases/platformio'
    print('Copying {} to {}'.format(package_name, platformio_release_dir))
    shutil.copy(package_name, platformio_release_dir)
    os.remove(package_name)

    # Create the arduino IDE release
    subprocess.check_call([
        'make/arduino/arduino.py',
        '--remove-outdir',
        '--version', version
    ])

    for family in ['esp32', 'sam']:
        # Add the release to the Arduino IDE manifests.
        manifest_name = 'make/arduino/{family}/package_pumbaa_{family}_index.json'.format(
            family=family)

        # Calculate the SHA256 checksum of the archive.
        package_name = 'pumbaa-arduino/pumbaa-arduino-{}-{}.zip'.format(
            family, version)

        with open(package_name) as f:
            sha256sum = hashlib.sha256(f.read()).hexdigest()

        print('sha256sum of {} is {}.'.format(package_name, sha256sum))

        # Create the release entry.
        release_entry_formatted = PACKAGE_RELEASE_ENTRY_FMTS[family].format(
            version=version,
            sha256sum=sha256sum,
            size=os.stat(package_name).st_size)
        release_entry = json.loads(release_entry_formatted, object_pairs_hook=OrderedDict)
    
        # Insert the release entry into the manifest.
        with open(manifest_name) as f:
            data = json.load(f, object_pairs_hook=OrderedDict)

        data['packages'][0]['platforms'].append(release_entry)
    
        with open(manifest_name, 'w') as f:
            f.write(json.dumps(data, indent=4, separators=(',', ': ')))

        shutil.copy(package_name, '../pumbaa-releases/arduino/' + family)
        shutil.copy(manifest_name, '../pumbaa-releases/arduino/' + family)

    
def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--package', action='store_true')
    parser.add_argument('--version', required=True)
    args = parser.parse_args()

    # first of all, the repository must be clean
    raw_input("ATTENTION: All files not version controlled by git will be "
              "_removed_. Press any key to continue or ctrl-c to exit.")
    print()

    git_clean_dfx()

    if args.test:
        test()

        # Generate the documenation after the last "git clean -dfx"
        # execution.
        generate_docs()

    if args.package:
        package(args.version)


if __name__ == "__main__":
    main()
