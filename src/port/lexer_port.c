/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016, Erik Moqvist
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

/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "pumbaa.h"

struct lexer_file_t {
    struct fs_file_t file;
    byte buf[20];
    mp_uint_t len;
    mp_uint_t pos;
};

static mp_uint_t lexer_file_readbyte(void *lexer_file_p)
{
    struct lexer_file_t *lf_p;
    int n;

    lf_p = (struct lexer_file_t *)lexer_file_p;

    if (lf_p->pos >= lf_p->len) {
        if (lf_p->len == 0) {
            return (MP_READER_EOF);
        } else {
            n = fs_read(&lf_p->file, lf_p->buf, sizeof(lf_p->buf));

            if (n <= 0) {
                lf_p->len = 0;
                return (MP_READER_EOF);
            }

            lf_p->len = n;
            lf_p->pos = 0;
        }
    }

    return (lf_p->buf[lf_p->pos++]);
}

static void lexer_file_close(void *lexer_file_p)
{
    struct lexer_file_t *lf_p;

    lf_p = (struct lexer_file_t *)lexer_file_p;
    fs_close(&lf_p->file);
    m_del_obj(struct lexer_file_t, lf_p);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename_p)
{
    mp_reader_t reader;
    struct lexer_file_t *lf_p;
    int n;

    lf_p = m_new_obj_maybe(struct lexer_file_t);

    if (lf_p == NULL) {
        return (NULL);
    }

    if (fs_open(&lf_p->file, filename_p, FS_READ) != 0) {
        m_del_obj(struct lexer_file_t, lf_p);
        return (NULL);
    }

    n = fs_read(&lf_p->file, lf_p->buf, sizeof(lf_p->buf));
    lf_p->len = n;
    lf_p->pos = 0;
    reader.data = lf_p;
    reader.readbyte = lexer_file_readbyte;
    reader.close = lexer_file_close;

    return (mp_lexer_new(qstr_from_str(filename_p), reader));
}
