/**
 * @file module_drivers/class_esp_wifi.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Pumbaa project.
 */

#include "pumbaa.h"

static const mp_obj_type_t class_esp_wifi;

/**
 * Singleton instance of the Esp WiFi class.
 */
const struct class_esp_wifi_t module_drivers_esp_wifi_obj = {
    { &class_esp_wifi }
};

/**
 * IP info fields.
 */
static const qstr ip_info_fields[] = {
    MP_QSTR_address,
    MP_QSTR_netmask,
    MP_QSTR_gateway
};

/**
 * Station info fields.
 */
static const qstr station_info_fields[] = {
    MP_QSTR_bssid,
    MP_QSTR_address
};

/**
 * def set_op_mode(self, mode)
 */
static mp_obj_t class_esp_wifi_set_op_mode(mp_obj_t self_in,
                                                 mp_obj_t mode_in)
{
    int mode;

    mode = mp_obj_get_int(mode_in);

    if ((mode < esp_wifi_op_mode_null_t)
        || (mode >= esp_wifi_op_mode_max_t)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                                                "bad mode %d",
                                                mode));
    }

    if (esp_wifi_set_op_mode(mode) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def get_op_mode(self)
 */
static mp_obj_t class_esp_wifi_get_op_mode(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_get_op_mode()));
}

/**
 * def set_phy_mode(self, mode)
 */
static mp_obj_t class_esp_wifi_set_phy_mode(mp_obj_t self_in,
                                                  mp_obj_t mode_in)
{
    int mode;

    mode = mp_obj_get_int(mode_in);

    if ((mode < esp_wifi_phy_mode_11b_t)
        || (mode > esp_wifi_phy_mode_11n_t)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                                                "bad mode %d",
                                                mode));
    }

    if (esp_wifi_set_phy_mode(mode) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def get_phy_mode(self)
 */
static mp_obj_t class_esp_wifi_get_phy_mode(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_get_phy_mode()));
}

/**
 * def softap_init(self)
 */
static mp_obj_t class_esp_wifi_softap_init(mp_obj_t self_in,
                                                 mp_obj_t ssid_in,
                                                 mp_obj_t password_in)
{
    const char *ssid_p;
    const char *password_p;

    ssid_p = mp_obj_str_get_str(ssid_in);
    password_p = mp_obj_str_get_str(password_in);

    if (esp_wifi_softap_init(ssid_p, password_p) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def softap_set_ip_info(self)
 */
static mp_obj_t class_esp_wifi_softap_set_ip_info(mp_obj_t self_in,
                                                        mp_obj_t info_in)
{
    struct inet_if_ip_info_t info;
    mp_uint_t len;
    mp_obj_t *items_p;

    mp_obj_tuple_get(info_in, &len, &items_p);

    if (len != 3) {
        mp_raise_TypeError("expected tuple of length 3");
    }

    inet_aton(mp_obj_str_get_str(items_p[0]), &info.address);
    inet_aton(mp_obj_str_get_str(items_p[1]), &info.netmask);
    inet_aton(mp_obj_str_get_str(items_p[2]), &info.gateway);

    if (esp_wifi_softap_set_ip_info(&info) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def softap_get_ip_info(self)
 */
static mp_obj_t class_esp_wifi_softap_get_ip_info(mp_obj_t self_in)
{
    struct inet_if_ip_info_t info;
    mp_obj_t tuple[3];
    char buf[16];

    if (esp_wifi_softap_get_ip_info(&info) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    tuple[0] = mp_obj_new_str(inet_ntoa(&info.address, buf),
                              strlen(buf),
                              false);
    tuple[1] = mp_obj_new_str(inet_ntoa(&info.netmask, buf),
                              strlen(buf),
                              false);
    tuple[2] = mp_obj_new_str(inet_ntoa(&info.gateway, buf),
                              strlen(buf),
                              false);

    return (mp_obj_new_attrtuple(&ip_info_fields[0], 3, tuple));
}

/**
 * def softap_get_number_of_connected_stations(self)
 */
static mp_obj_t class_esp_wifi_softap_get_number_of_connected_stations(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_softap_get_number_of_connected_stations()));
}

/**
 * def softap_get_station_info(self)
 */
static mp_obj_t class_esp_wifi_softap_get_station_info(mp_obj_t self_in)
{
    struct esp_wifi_softap_station_info_t info[4];
    int i;
    int number_of_infos;
    mp_obj_t tuple[2];
    mp_obj_t list;
    char buf[16];

    number_of_infos = esp_wifi_softap_get_station_info(&info[0],
                                                             membersof(info));
    if (number_of_infos < 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    list = mp_obj_new_list(0, NULL);

    for (i = 0; i < number_of_infos; i++) {
        tuple[0] = mp_obj_new_bytes(&info[i].bssid[0], 6);
        tuple[1] = mp_obj_new_str(inet_ntoa(&info[i].ip_address, buf),
                                  strlen(buf),
                                  false);
        mp_obj_list_append(list,
                           mp_obj_new_attrtuple(&station_info_fields[0],
                                                2,
                                                tuple));
    }

    return (list);
}

/**
 * def softap_dhcp_server_start(self)
 */
static mp_obj_t class_esp_wifi_softap_dhcp_server_start(mp_obj_t self_in)
{
    if (esp_wifi_softap_dhcp_server_start() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def softap_dhcp_server_stop(self)
 */
static mp_obj_t class_esp_wifi_softap_dhcp_server_stop(mp_obj_t self_in)
{
    if (esp_wifi_softap_dhcp_server_stop() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def esp_wifi_softap_dhcp_server_status(self)
 */
static mp_obj_t class_esp_wifi_softap_dhcp_server_status(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_softap_dhcp_server_status()));
}

/**
 * def station_init(self)
 */
static mp_obj_t class_esp_wifi_station_init(mp_uint_t n_args,
                                                  const mp_obj_t *args_p)
{
    const char *ssid_p;
    const char *password_p;
    struct inet_if_ip_info_t info;
    struct inet_if_ip_info_t *info_p;
    mp_uint_t len;
    mp_obj_t *items_p;

    ssid_p = mp_obj_str_get_str(args_p[1]);
    password_p = mp_obj_str_get_str(args_p[2]);

    if (n_args == 4) {
        mp_obj_tuple_get(args_p[3], &len, &items_p);

        if (len != 3) {
            mp_raise_TypeError("expected tuple of length 3");
        }

        inet_aton(mp_obj_str_get_str(items_p[0]), &info.address);
        inet_aton(mp_obj_str_get_str(items_p[1]), &info.netmask);
        inet_aton(mp_obj_str_get_str(items_p[2]), &info.gateway);
        info_p = &info;
    } else {
        info_p = NULL;
    }

    if (esp_wifi_station_init(ssid_p, password_p, info_p) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_connect(self)
 */
static mp_obj_t class_esp_wifi_station_connect(mp_obj_t self_in)
{
    if (esp_wifi_station_connect() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_disconnect(self)
 */
static mp_obj_t class_esp_wifi_station_disconnect(mp_obj_t self_in)
{
    if (esp_wifi_station_disconnect() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_set_ip_info(self)
 */
static mp_obj_t class_esp_wifi_station_set_ip_info(mp_obj_t self_in,
                                                         mp_obj_t info_in)
{
    struct inet_if_ip_info_t info;
    mp_uint_t len;
    mp_obj_t *items_p;

    mp_obj_tuple_get(info_in, &len, &items_p);

    if (len != 3) {
        mp_raise_TypeError("expected tuple of length 3");
    }

    inet_aton(mp_obj_str_get_str(items_p[0]), &info.address);
    inet_aton(mp_obj_str_get_str(items_p[1]), &info.netmask);
    inet_aton(mp_obj_str_get_str(items_p[2]), &info.gateway);

    if (esp_wifi_station_set_ip_info(&info) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_get_ip_info(self)
 */
static mp_obj_t class_esp_wifi_station_get_ip_info(mp_obj_t self_in)
{
    struct inet_if_ip_info_t info;
    mp_obj_t tuple[3];
    char buf[16];

    if (esp_wifi_station_get_ip_info(&info) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    tuple[0] = mp_obj_new_str(inet_ntoa(&info.address, buf),
                              strlen(buf),
                              false);
    tuple[1] = mp_obj_new_str(inet_ntoa(&info.netmask, buf),
                              strlen(buf),
                              false);
    tuple[2] = mp_obj_new_str(inet_ntoa(&info.gateway, buf),
                              strlen(buf),
                              false);

    return (mp_obj_new_attrtuple(&ip_info_fields[0], 3, tuple));
}

/**
 * def station_set_reconnect_policy(self)
 */
static mp_obj_t class_esp_wifi_station_set_reconnect_policy(mp_obj_t self_in,
                                                                  mp_obj_t policy_in)
{
    int policy;

    policy = mp_obj_get_int(policy_in);

    if ((policy != 0) && (policy != 1)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                                                "bad policy %d",
                                                policy));
    }

    if (esp_wifi_station_set_reconnect_policy(policy) != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_get_reconnect_policy(self)
 */
static mp_obj_t class_esp_wifi_station_get_reconnect_policy(mp_obj_t self_in)
{
    return (mp_obj_new_bool(esp_wifi_station_get_reconnect_policy()));
}

/**
 * def station_get_connect_status(self)
 */
static mp_obj_t class_esp_wifi_station_get_connect_status(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_station_get_connect_status()));
}

/**
 * def station_dhcp_client_start(self)
 */
static mp_obj_t class_esp_wifi_station_dhcp_client_start(mp_obj_t self_in)
{
    if (esp_wifi_station_dhcp_client_start() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_dhcp_client_stop(self)
 */
static mp_obj_t class_esp_wifi_station_dhcp_client_stop(mp_obj_t self_in)
{
    if (esp_wifi_station_dhcp_client_stop() != 0) {
        nlr_raise(mp_obj_new_exception(&mp_type_OSError));
    }

    return (mp_const_none);
}

/**
 * def station_dhcp_client_status(self)
 */
static mp_obj_t class_esp_wifi_station_dhcp_client_status(mp_obj_t self_in)
{
    return (MP_OBJ_NEW_SMALL_INT(esp_wifi_station_dhcp_client_status()));
}

static MP_DEFINE_CONST_FUN_OBJ_2(class_esp_wifi_set_op_mode_obj,
                                 class_esp_wifi_set_op_mode);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_get_op_mode_obj,
                                 class_esp_wifi_get_op_mode);
static MP_DEFINE_CONST_FUN_OBJ_2(class_esp_wifi_set_phy_mode_obj,
                                 class_esp_wifi_set_phy_mode);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_get_phy_mode_obj,
                                 class_esp_wifi_get_phy_mode);
static MP_DEFINE_CONST_FUN_OBJ_3(class_esp_wifi_softap_init_obj,
                                 class_esp_wifi_softap_init);
static MP_DEFINE_CONST_FUN_OBJ_2(class_esp_wifi_softap_set_ip_info_obj,
                                 class_esp_wifi_softap_set_ip_info);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_get_ip_info_obj,
                                 class_esp_wifi_softap_get_ip_info);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_get_number_of_connected_stations_obj,
                                 class_esp_wifi_softap_get_number_of_connected_stations);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_get_station_info_obj,
                                 class_esp_wifi_softap_get_station_info);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_dhcp_server_start_obj,
                                 class_esp_wifi_softap_dhcp_server_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_dhcp_server_stop_obj,
                                 class_esp_wifi_softap_dhcp_server_stop);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_softap_dhcp_server_status_obj,
                                 class_esp_wifi_softap_dhcp_server_status);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(class_esp_wifi_station_init_obj,
                                           3,
                                           4,
                                           class_esp_wifi_station_init);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_connect_obj,
                                 class_esp_wifi_station_connect);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_disconnect_obj,
                                 class_esp_wifi_station_disconnect);
static MP_DEFINE_CONST_FUN_OBJ_2(class_esp_wifi_station_set_ip_info_obj,
                                 class_esp_wifi_station_set_ip_info);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_get_ip_info_obj,
                                 class_esp_wifi_station_get_ip_info);
static MP_DEFINE_CONST_FUN_OBJ_2(class_esp_wifi_station_set_reconnect_policy_obj,
                                 class_esp_wifi_station_set_reconnect_policy);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_get_reconnect_policy_obj,
                                 class_esp_wifi_station_get_reconnect_policy);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_get_connect_status_obj,
                                 class_esp_wifi_station_get_connect_status);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_dhcp_client_start_obj,
                                 class_esp_wifi_station_dhcp_client_start);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_dhcp_client_stop_obj,
                                 class_esp_wifi_station_dhcp_client_stop);
static MP_DEFINE_CONST_FUN_OBJ_1(class_esp_wifi_station_dhcp_client_status_obj,
                                 class_esp_wifi_station_dhcp_client_status);

static const mp_rom_map_elem_t class_esp_wifi_locals_dict_table[] = {
    /* Class methods. */
    { MP_ROM_QSTR(MP_QSTR_set_op_mode),
      MP_ROM_PTR(&class_esp_wifi_set_op_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_op_mode),
      MP_ROM_PTR(&class_esp_wifi_get_op_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_phy_mode),
      MP_ROM_PTR(&class_esp_wifi_set_phy_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_phy_mode),
      MP_ROM_PTR(&class_esp_wifi_get_phy_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_init),
      MP_ROM_PTR(&class_esp_wifi_softap_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_set_ip_info),
      MP_ROM_PTR(&class_esp_wifi_softap_set_ip_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_get_ip_info),
      MP_ROM_PTR(&class_esp_wifi_softap_get_ip_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_get_number_of_connected_stations),
      MP_ROM_PTR(&class_esp_wifi_softap_get_number_of_connected_stations_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_get_station_info),
      MP_ROM_PTR(&class_esp_wifi_softap_get_station_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_dhcp_server_start),
      MP_ROM_PTR(&class_esp_wifi_softap_dhcp_server_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_dhcp_server_stop),
      MP_ROM_PTR(&class_esp_wifi_softap_dhcp_server_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_softap_dhcp_server_status),
      MP_ROM_PTR(&class_esp_wifi_softap_dhcp_server_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_init),
      MP_ROM_PTR(&class_esp_wifi_station_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_connect),
      MP_ROM_PTR(&class_esp_wifi_station_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_disconnect),
      MP_ROM_PTR(&class_esp_wifi_station_disconnect_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_set_ip_info),
      MP_ROM_PTR(&class_esp_wifi_station_set_ip_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_get_ip_info),
      MP_ROM_PTR(&class_esp_wifi_station_get_ip_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_set_reconnect_policy),
      MP_ROM_PTR(&class_esp_wifi_station_set_reconnect_policy_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_get_reconnect_policy),
      MP_ROM_PTR(&class_esp_wifi_station_get_reconnect_policy_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_get_connect_status),
      MP_ROM_PTR(&class_esp_wifi_station_get_connect_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_dhcp_client_start),
      MP_ROM_PTR(&class_esp_wifi_station_dhcp_client_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_dhcp_client_stop),
      MP_ROM_PTR(&class_esp_wifi_station_dhcp_client_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_station_dhcp_client_status),
      MP_ROM_PTR(&class_esp_wifi_station_dhcp_client_status_obj) },

    /* Constants. */
    { MP_ROM_QSTR(MP_QSTR_OP_MODE_NULL),
      MP_ROM_INT(esp_wifi_op_mode_null_t) },
    { MP_ROM_QSTR(MP_QSTR_OP_MODE_STATION),
      MP_ROM_INT(esp_wifi_op_mode_null_t) },
    { MP_ROM_QSTR(MP_QSTR_OP_MODE_SOFTAP),
      MP_ROM_INT(esp_wifi_op_mode_null_t) },
    { MP_ROM_QSTR(MP_QSTR_OP_MODE_STATION_SOFTAP),
      MP_ROM_INT(esp_wifi_op_mode_null_t) },

    { MP_ROM_QSTR(MP_QSTR_PHY_MODE_11B),
      MP_ROM_INT(esp_wifi_phy_mode_11b_t) },
    { MP_ROM_QSTR(MP_QSTR_PHY_MODE_11G),
      MP_ROM_INT(esp_wifi_phy_mode_11g_t) },
    { MP_ROM_QSTR(MP_QSTR_PHY_MODE_11N),
      MP_ROM_INT(esp_wifi_phy_mode_11n_t) },
};

static MP_DEFINE_CONST_DICT(class_esp_wifi_locals_dict,
                            class_esp_wifi_locals_dict_table);

/**
 * Esp Wifi class type.
 */
static const mp_obj_type_t class_esp_wifi = {
    { &mp_type_type },
    .name = MP_QSTR_esp_wifi,
    .locals_dict = (mp_obj_t)&class_esp_wifi_locals_dict,
};
