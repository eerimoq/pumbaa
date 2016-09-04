#!/usr/bin/env python

"""Generate documentation.
"""

import os


SOURCE_CODE_FMT = """.. code-block:: python

{source}
"""


def examples_generate():
    """Generate examples.

    """

    examples = [
        "blink",
        "hello_world",
    ]

    for example in examples:
        c_path = os.path.join("examples", example, "main.py")

        source = []
        with open(c_path) as fin:
            for line in fin.readlines():
                source.append("   " + line)

        rst = SOURCE_CODE_FMT.format(source=''.join(source))

        rst_path = os.path.join("docs", "examples", example, "source-code.rst")

        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def main():
    """Main.

    """

    examples_generate()


if __name__ == "__main__":
    main()
