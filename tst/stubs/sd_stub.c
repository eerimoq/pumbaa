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

#include "pumbaa.h"

static uint8_t block[SD_BLOCK_SIZE];

int sd_init(struct sd_driver_t *self_p,
            struct spi_driver_t *spi_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);

    return (0);
}

int sd_start(struct sd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

int sd_stop(struct sd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

ssize_t sd_read_cid(struct sd_driver_t *self_p,
                    struct sd_cid_t *cid_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(cid_p != NULL, EINVAL);

    cid_p->mid = 22;
    strcpy(&cid_p->oid[0], "Si");
    strcpy(&cid_p->pnm[0], "mba  ");
    cid_p->prv = 1;
    cid_p->psn = 1;
    cid_p->mdt = 1;
    cid_p->crc = 57;

    return (sizeof(*cid_p));
}

ssize_t sd_read_csd(struct sd_driver_t *self_p,
                    union sd_csd_t *csd_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(csd_p != NULL, EINVAL);

    static int count = 0;

    if (count == 0) {
        csd_p->v1.csd_structure = 0;
        csd_p->v1.taac = 3;
        csd_p->v1.nsac = 3;
        csd_p->v1.tran_speed = 3;
        csd_p->v1.ccc_high = 3;
        csd_p->v1.read_bl_len = 3;
        csd_p->v1.ccc_low = 3;
        csd_p->v1.c_size_high = 3;
        csd_p->v1.dsr_imp = 1;
        csd_p->v1.read_blk_misalign = 1;
        csd_p->v1.write_blk_misalign = 1;
        csd_p->v1.read_bl_partial = 1;
        csd_p->v1.c_size_mid = 3;
        csd_p->v1.vdd_r_curr_max = 3;
        csd_p->v1.vdd_r_curr_min = 3;
        csd_p->v1.c_size_low  = 3;
        csd_p->v1.c_size_mult_high = 3;
        csd_p->v1.vdd_w_curr_max = 3;
        csd_p->v1.vdd_w_curr_min = 3;
        csd_p->v1.sector_size_high = 3;
        csd_p->v1.erase_blk_en = 1;
        csd_p->v1.c_size_mult_low = 1;
        csd_p->v1.wp_grp_size = 3;
        csd_p->v1.sector_size_low = 1;
        csd_p->v1.write_bl_len_high = 3;
        csd_p->v1.r2w_factor = 3;
        csd_p->v1.wp_grp_enable = 1;
        csd_p->v1.write_bl_partial = 1;
        csd_p->v1.write_bl_len_low = 3;
        csd_p->v1.file_format = 3;
        csd_p->v1.tmp_write_protect = 1;
        csd_p->v1.perm_write_protect = 1;
        csd_p->v1.copy = 1;
        csd_p->v1.file_format_grp = 1;
        csd_p->v1.crc = 3;
    } else if (count == 1) {
        csd_p->v2.csd_structure = 1;
        csd_p->v2.taac = 3;
        csd_p->v2.nsac = 3;
        csd_p->v2.tran_speed = 3;
        csd_p->v2.ccc_high = 3;
        csd_p->v2.read_bl_len = 3;
        csd_p->v2.ccc_low = 3;
        csd_p->v2.dsr_imp = 1;
        csd_p->v2.read_blk_misalign = 1;
        csd_p->v2.write_blk_misalign = 1;
        csd_p->v2.read_bl_partial = 1;
        csd_p->v2.c_size_high = 3;
        csd_p->v2.reserved3 = 3;
        csd_p->v2.c_size_mid = 3;
        csd_p->v2.c_size_low = 3;
        csd_p->v2.sector_size_high = 3;
        csd_p->v2.erase_blk_en = 1;
        csd_p->v2.wp_grp_size = 3;
        csd_p->v2.sector_size_low = 1;
        csd_p->v2.write_bl_len_high = 3;
        csd_p->v2.r2w_factor = 3;
        csd_p->v2.wp_grp_enable = 1;
        csd_p->v2.write_bl_partial = 1;
        csd_p->v2.write_bl_len_low = 3;
        csd_p->v2.file_format = 3;
        csd_p->v2.tmp_write_protect = 1;
        csd_p->v2.perm_write_protect = 1;
        csd_p->v2.copy = 1;
        csd_p->v2.file_format_grp = 1;
        csd_p->v2.crc = 3;
    } else {
        return (-1);
    }

    count++;

    return (sizeof(*csd_p));
}

ssize_t sd_read_block(struct sd_driver_t *self_p,
                      void *dst_p,
                      uint32_t src_block)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(dst_p != NULL, -EINVAL);

    static int count = 0;
    ssize_t res = -1;

    if (count < 2) {
        memcpy(dst_p, &block[0], sizeof(block));
        res = sizeof(block);
    }

    count++;

    return (res);
}

ssize_t sd_write_block(struct sd_driver_t *self_p,
                       uint32_t dst_block,
                       const void *src_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    static int count = 0;
    ssize_t res = -1;

    if (count == 0) {
        memcpy(&block[0], src_p, sizeof(block));
        res = sizeof(block);
    }

    count++;

    return (res);
}

static mp_obj_t module_init(void)
{
    return (mp_const_none);
}

static MP_DEFINE_CONST_FUN_OBJ_0(module_init_obj, module_init);

/**
 * The module globals table.
 */
static const mp_rom_map_elem_t module_sd_stub_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sd_stub) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&module_init_obj) },
};

static MP_DEFINE_CONST_DICT(module_sd_stub_globals, module_sd_stub_globals_table);

const mp_obj_module_t module_sd_stub = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&module_sd_stub_globals,
};
