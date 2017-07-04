/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *expected_path_p;
    int expected_flags;
    int res;

    counter++;

    if (counter == 1) {
        /* Micropython open main.py. */
        return (-1);
    }

    expected_path_p = "file.txt";
    expected_flags = FS_CREAT;
    res = 0;
    
    if (MP_STATE_VM(fs_stub_open_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_open_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. res (integer)
           2. (filename, mode, res) */
        if (MP_OBJ_IS_INT(item)) {
            res = mp_obj_get_int(item);
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len >= 2);
            
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            expected_path_p = buffer_info.buf;
            expected_flags = mp_obj_get_int(items_p[1]);
            
            if (len == 3) {
                res = mp_obj_get_int(items_p[2]);
            }
        }

        mp_obj_list_remove(MP_STATE_VM(fs_stub_open_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_open_obj) = mp_const_none;
        }
    }

    BTASSERT(strcmp(path_p, expected_path_p) == 0);
    BTASSERT(flags == expected_flags);
    
    return (res);
}

int fs_close(struct fs_file_t *self_p)
{
    return (-1);
}

ssize_t fs_read(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *response_dst_p;
    int expected_size;
    ssize_t res;
    
    response_dst_p = "read()";
    expected_size = 6;
    res = 6;
    
    if (MP_STATE_VM(fs_stub_read_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_read_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        /* Items may be:
           1. res (integer)
           2. dst (string)
           3. (dst, res) */
        if (MP_OBJ_IS_INT(item)) {
            response_dst_p = NULL;
            res = mp_obj_get_int(item);
        } else if (MP_OBJ_IS_STR(item)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_dst_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = buffer_info.len;
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            response_dst_p = buffer_info.buf;
            expected_size = buffer_info.len;
            res = mp_obj_get_int(items_p[1]);
        }

        mp_obj_list_remove(MP_STATE_VM(fs_stub_read_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_read_obj) = mp_const_none;
        }
    }

    if (response_dst_p != NULL) {
        BTASSERT(size >= expected_size);
        memcpy(dst_p, response_dst_p, expected_size);
    }
    
    return (res);
}

ssize_t fs_read_line(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    return (-1);
}

ssize_t fs_write(struct fs_file_t *self_p, const void *src_p, size_t size)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    const char *expected_src_p;
    int expected_size;
    ssize_t res;

    expected_src_p = "write()";
    expected_size = 7;
    res = 7;

    if (MP_STATE_VM(fs_stub_write_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_write_obj), &len, &items_p);
        BTASSERT(len > 0);
        item = items_p[0];

        /* Items may be:
           1. src (string)
           2. (src, res) */
        if (MP_OBJ_IS_STR(item)) {
            mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                                &buffer_info,
                                MP_BUFFER_READ);
            res = buffer_info.len;
        } else {
            mp_obj_tuple_get(item, &len, &items_p);
            BTASSERT(len == 2);
            
            mp_get_buffer_raise(MP_OBJ_TO_PTR(items_p[0]),
                                &buffer_info,
                                MP_BUFFER_READ);
            res = mp_obj_get_int(items_p[1]);
        }

        expected_src_p = buffer_info.buf;
        expected_size = buffer_info.len;

        mp_obj_list_remove(MP_STATE_VM(fs_stub_write_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_write_obj) = mp_const_none;
        }
    }

    BTASSERT(size == expected_size);
    BTASSERT(memcmp(src_p, expected_src_p, expected_size) == 0);
    
    return (res);
}

int fs_seek(struct fs_file_t *self_p, int offset, int whence)
{
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    int expected_offset;
    int expected_whence;
    ssize_t res;

    expected_offset = 0;
    expected_whence = 0;
    res = 0;
    
    if (MP_STATE_VM(fs_stub_seek_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_seek_obj), &len, &items_p);
        BTASSERT(len > 0);
        item = items_p[0];

        mp_obj_tuple_get(item, &len, &items_p);
        BTASSERT(len == 3);
        
        expected_offset = mp_obj_get_int(items_p[0]);
        expected_whence = mp_obj_get_int(items_p[1]);
        res = mp_obj_get_int(items_p[2]);

        mp_obj_list_remove(MP_STATE_VM(fs_stub_seek_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_seek_obj) = mp_const_none;
        }
    }

    BTASSERT(offset == expected_offset);
    BTASSERT(whence == expected_whence);

    return (res);
}

ssize_t fs_tell(struct fs_file_t *self_p)
{
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    ssize_t res;

    res = 0;

    if (MP_STATE_VM(fs_stub_tell_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_tell_obj), &len, &items_p);
        BTASSERT(len > 0);
        item = items_p[0];

        res = mp_obj_get_int(item);
        
        mp_obj_list_remove(MP_STATE_VM(fs_stub_tell_obj), item);
        
        if (len == 1) {
            MP_STATE_VM(fs_stub_tell_obj) = mp_const_none;
        }
    }

    return (res);
}

int fs_mkdir(const char *path_p)
{
    if (strcmp(path_p, "dir") != 0) {
        return (-1);
    }

    return (0);
}

int fs_dir_open(struct fs_dir_t *dir_p,
                const char *path_p,
                int oflag)
{
    return (0);
}

int fs_dir_close(struct fs_dir_t *dir_p)
{
    return (0);
}

int fs_dir_read(struct fs_dir_t *dir_p,
                struct fs_dir_entry_t *entry_p)
{
    int res;

    res = -1;
    
    return (res);
}

int fs_remove(const char *path_p)
{
    mp_buffer_info_t buffer_info;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    int res;

    res = -1;

    if (MP_STATE_VM(fs_stub_remove_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_remove_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];
        mp_get_buffer_raise(MP_OBJ_TO_PTR(item),
                            &buffer_info,
                            MP_BUFFER_READ);
        res = 0;
        
        mp_obj_list_remove(MP_STATE_VM(fs_stub_remove_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_remove_obj) = mp_const_none;
        }
    }
    
    return (res);
}

int fs_stat(const char *path_p, struct fs_stat_t *stat_p)
{
    static int counter = 0;
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    int res;

    counter++;

    if (counter < 7) {
        /* Micropython open main.py. */
        return (-1);
    }
    
    memset(stat_p, 0, sizeof(*stat_p));
    res = -1;

    if (MP_STATE_VM(fs_stub_stat_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_stat_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];

        mp_obj_tuple_get(item, &len, &items_p);
        BTASSERT(len == 2);
        
        stat_p->size = mp_obj_get_int(items_p[0]);
        res = mp_obj_get_int(items_p[1]);
        
        mp_obj_list_remove(MP_STATE_VM(fs_stub_stat_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_stat_obj) = mp_const_none;
        }
    }
    
    return (res);
}

int fs_format(const char *path_p)
{
    mp_uint_t len;
    mp_obj_t *items_p;
    mp_obj_t item;
    int res;

    res = 0;

    if (MP_STATE_VM(fs_stub_format_obj) != mp_const_none) {
        mp_obj_list_get(MP_STATE_VM(fs_stub_format_obj), &len, &items_p);
        BTASSERT(len > 0);

        item = items_p[0];
        res = mp_obj_get_int(item);
        mp_obj_list_remove(MP_STATE_VM(fs_stub_format_obj), item);

        if (len == 1) {
            MP_STATE_VM(fs_stub_format_obj) = mp_const_none;
        }
    }

    return (res);
}

int fs_ls(const char *path_p,
          const char *filter_p,
          void *chout_p)
{
    PRINT_FILE_LINE();
    return (-1);
}

int fs_list(const char *path_p,
            const char *filter_p,
            void *chout_p)
{
    PRINT_FILE_LINE();
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

static mp_obj_t module_set_open(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_open_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_read(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_read_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_write(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_write_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_seek(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_seek_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_tell(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_tell_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_format(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_format_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_remove(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_remove_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_set_stat(mp_obj_t data_in)
{
    if (!MP_OBJ_IS_TYPE(data_in, &mp_type_list)) {
        data_in = mp_obj_new_list(1, &data_in);
    }

    MP_STATE_VM(fs_stub_stat_obj) = data_in;

    return (mp_const_none);
}

static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_open_obj, module_set_open);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_read_obj, module_set_read);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_write_obj, module_set_write);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_seek_obj, module_set_seek);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_tell_obj, module_set_tell);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_format_obj, module_set_format);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_remove_obj, module_set_remove);
static MP_DEFINE_CONST_FUN_OBJ_1(module_set_stat_obj, module_set_stat);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_fs_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_fs_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },

    /* Functions. */
    { MP_ROM_QSTR(MP_QSTR_set_open), MP_ROM_PTR(&module_set_open_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_read), MP_ROM_PTR(&module_set_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_write), MP_ROM_PTR(&module_set_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_seek), MP_ROM_PTR(&module_set_seek_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_tell), MP_ROM_PTR(&module_set_tell_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_format), MP_ROM_PTR(&module_set_format_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_remove), MP_ROM_PTR(&module_set_remove_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_stat), MP_ROM_PTR(&module_set_stat_obj) }
};

static MP_DEFINE_CONST_DICT(module_fs_stub_globals, module_fs_stub_globals_table);

const mp_obj_module_t module_fs_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_fs_stub_globals,
};
