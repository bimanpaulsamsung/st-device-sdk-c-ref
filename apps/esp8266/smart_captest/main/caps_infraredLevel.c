/* ***************************************************************************
 *
 * Copyright 2019-2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <string.h>

#include "st_dev.h"
#include "caps_infraredLevel.h"

#include "freertos/FreeRTOS.h"

static double caps_infraredLevel_get_infraredLevel_value(caps_infraredLevel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_infraredLevel.attr_infraredLevel.min - 1;
    }
    return caps_data->infraredLevel_value;
}

static void caps_infraredLevel_set_infraredLevel_value(caps_infraredLevel_data_t *caps_data, double value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->infraredLevel_value = value;
}

static const char *caps_infraredLevel_get_infraredLevel_unit(caps_infraredLevel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->infraredLevel_unit;
}

static void caps_infraredLevel_set_infraredLevel_unit(caps_infraredLevel_data_t *caps_data, const char *unit)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->infraredLevel_unit = (char *)unit;
}

static void caps_infraredLevel_attr_infraredLevel_send(caps_infraredLevel_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_number((char *) caps_helper_infraredLevel.attr_infraredLevel.name, caps_data->infraredLevel_value, caps_data->infraredLevel_unit);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send infraredLevel value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_infraredLevel_cmd_setInfraredLevel_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_infraredLevel_data_t *caps_data = (caps_infraredLevel_data_t *)usr_data;
    double value;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    value = cmd_data->cmd_data[0].number;

    caps_infraredLevel_set_infraredLevel_value(caps_data, value);
    if (caps_data && caps_data->cmd_setInfraredLevel_usr_cb)
        caps_data->cmd_setInfraredLevel_usr_cb(caps_data);
    caps_infraredLevel_attr_infraredLevel_send(caps_data);
}

static void caps_infraredLevel_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_infraredLevel_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_infraredLevel_attr_infraredLevel_send(caps_data);
}

caps_infraredLevel_data_t *caps_infraredLevel_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_infraredLevel_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_infraredLevel_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_infraredLevel_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_infraredLevel_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_infraredLevel_value = caps_infraredLevel_get_infraredLevel_value;
    caps_data->set_infraredLevel_value = caps_infraredLevel_set_infraredLevel_value;
    caps_data->get_infraredLevel_unit = caps_infraredLevel_get_infraredLevel_unit;
    caps_data->set_infraredLevel_unit = caps_infraredLevel_set_infraredLevel_unit;
    caps_data->attr_infraredLevel_send = caps_infraredLevel_attr_infraredLevel_send;
    caps_data->infraredLevel_value = 0;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_infraredLevel.id, caps_infraredLevel_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_infraredLevel.cmd_setInfraredLevel.name, caps_infraredLevel_cmd_setInfraredLevel_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setInfraredLevel of infraredLevel\n");
    }
    } else {
        printf("fail to init infraredLevel handle\n");
    }

    return caps_data;
}
