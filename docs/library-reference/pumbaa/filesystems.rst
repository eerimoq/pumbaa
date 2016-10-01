:mod:`filesystems` --- File systems
===================================

.. module:: filesystems
   :synopsis: File systems.

File systems and file system like frameworks.

Simba documentation: `filesystems`_

----------------------------------------------

.. function:: filesystems.fs_call(command)

   Returns the output of given file system command. Raises OSError if
   the command is missing or fails to execute.

   
.. function:: filesystems.fs_format(path)

   Format file system at given path. All data in the file system will
   be lost.


.. _filesystems: http://simba-os.readthedocs.io/en/latest/library-reference/filesystems.html
