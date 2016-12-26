#!/usr/bin/env python
#
# Wrapper for mpy-cross.py.
#

from __future__ import print_function

import sys
import os
import subprocess


def main():
    command = sys.argv[3:]
    env = dict(os.environ, PYTHONPATH=sys.argv[2])
    with open(sys.argv[1], 'w') as f:
        f.write(subprocess.check_output(command, env=env))
    

if __name__ == '__main__':
    main()
