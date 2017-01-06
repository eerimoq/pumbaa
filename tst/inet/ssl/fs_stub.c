/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
 * This file is part of the Simba project.
 */

#include "simba.h"

int fs_module_init()
{
    return (0);
}

int fs_call(char *command_p,
            void *chin_p,
            void *chout_p,
            void *arg_p)
{
    return (0);
}

int fs_open(struct fs_file_t *self_p, const char *path_p, int flags)
{
    static int counter = 0;

    counter++;

    if (counter == 1) {
        /* Micropython open main.py. */
        return (-1);
    }

    return (0);
}

int fs_close(struct fs_file_t *self_p)
{
    return (-1);
}

ssize_t fs_read(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    static int counter = 0;

    BTASSERT(size == 400);

    counter++;

    memset(dst_p, counter, size);

    return (size);
}

ssize_t fs_read_line(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    return (-1);
}

ssize_t fs_write(struct fs_file_t *self_p, const void *src_p, size_t size)
{
    return (-1);
}

int fs_seek(struct fs_file_t *self_p, int offset, int whence)
{
    return (-1);
}

ssize_t fs_tell(struct fs_file_t *self_p)
{
    return (-1);
}

int fs_mkdir(const char *path_p)
{
    return (-1);
}

int fs_dir_open(struct fs_dir_t *dir_p,
                const char *path_p,
                int oflag)
{
    return (-1);
}

int fs_dir_close(struct fs_dir_t *dir_p)
{
    return (-1);
}

int fs_dir_read(struct fs_dir_t *dir_p,
                struct fs_dir_entry_t *entry_p)
{
    return (-1);
}

int fs_remove(const char *path_p)
{
    return (-1);
}

int fs_stat(const char *path_p, struct fs_stat_t *stat_p)
{
    stat_p->size = 400;

    return (0);
}

int fs_format(const char *path_p)
{
    return (-1);
}

int fs_ls(const char *path_p,
          const char *filter_p,
          void *chout_p)
{
    return (-1);
}

int fs_list(const char *path_p,
            const char *filter_p,
            void *chout_p)
{
    return (-1);
}

int fs_auto_complete(char *path_p)
{
    return (-1);
}

void fs_split(char *buf_p, char **path_pp, char **cmd_pp)
{
}

void fs_merge(char *path_p, char *cmd_p)
{
}

int fs_filesystem_init_fat16(struct fs_filesystem_t *self_p,
                             const char *name_p,
                             struct fat16_t *fat16_p)
{
    return (-1);
}

int fs_filesystem_init_spiffs(struct fs_filesystem_t *self_p,
                              const char *name_p,
                              struct spiffs_t *spiffs_p,
                              struct fs_filesystem_spiffs_config_t *config_p)
{
    return (-1);
}

int fs_filesystem_register(struct fs_filesystem_t *filesystem_p)
{
    return (-1);
}

int fs_filesystem_deregister(struct fs_filesystem_t *self_p)
{
    return (-1);
}

int fs_command_init(struct fs_command_t *self_p,
                    const FAR char *path_p,
                    fs_callback_t callback,
                    void *arg_p)
{
    return (0);
}

int fs_command_register(struct fs_command_t *command_p)
{
    return (0);
}

int fs_command_deregister(struct fs_command_t *command_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int fs_counter_init(struct fs_counter_t *self_p,
                    const FAR char *path_p,
                    uint64_t value)
{
    return (0);
}

int fs_counter_increment(struct fs_counter_t *self_p,
                         uint64_t value)
{
    return (0);
}

int fs_counter_register(struct fs_counter_t *counter_p)
{
    return (0);
}

int fs_counter_deregister(struct fs_counter_t *counter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_parameter_init(struct fs_parameter_t *self_p,
                      const FAR char *path_p,
                      fs_parameter_set_callback_t set_cb,
                      fs_parameter_print_callback_t print_cb,
                      void *value_p)
{
    return (0);
}

int fs_parameter_register(struct fs_parameter_t *parameter_p)
{
    return (0);
}

int fs_parameter_deregister(struct fs_parameter_t *parameter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_parameter_int_set(void *value_p, const char *src_p)
{
    return (-1);
}

int fs_parameter_int_print(void *chout_p, void *value_p)
{
    return (-1);
}
