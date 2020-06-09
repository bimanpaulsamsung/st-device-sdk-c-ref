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
#include "caps_statelessTemperatureButton.h"

#include "freertos/FreeRTOS.h"

static const char **caps_statelessTemperatureButton_get_availableTemperatureButtons_value(caps_statelessTemperatureButton_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return (const char **)caps_data->availableTemperatureButtons_value;
}

static void caps_statelessTemperatureButton_set_availableTemperatureButtons_value(caps_statelessTemperatureButton_data_t *caps_data, const char **value, int arraySize)
{
    int i;
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->availableTemperatureButtons_value) {
        for (i = 0; i < caps_data->availableTemperatureButtons_arraySize; i++) {
            free(caps_data->availableTemperatureButtons_value[i]);
        }
        free(caps_data->availableTemperatureButtons_value);
    }
    caps_data->availableTemperatureButtons_value = malloc(sizeof(char *) * arraySize);
    for (i = 0; i < arraySize; i++) {
        caps_data->availableTemperatureButtons_value[i] = strdup(value[i]);
    }

    caps_data->availableTemperatureButtons_arraySize = arraySize;
}

static void caps_statelessTemperatureButton_attr_availableTemperatureButtons_send(caps_statelessTemperatureButton_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->availableTemperatureButtons_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string_array((char *)caps_helper_statelessTemperatureButton.attr_availableTemperatureButtons.name,
        caps_data->availableTemperatureButtons_arraySize, caps_data->availableTemperatureButtons_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send availableTemperatureButtons value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_statelessTemperatureButton_cmd_setButton_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_statelessTemperatureButton_data_t *caps_data = (caps_statelessTemperatureButton_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_setButton_usr_cb)
        caps_data->cmd_setButton_usr_cb(caps_data);
}

static void caps_statelessTemperatureButton_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_statelessTemperatureButton_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_statelessTemperatureButton_attr_availableTemperatureButtons_send(caps_data);
}

caps_statelessTemperatureButton_data_t *caps_statelessTemperatureButton_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_statelessTemperatureButton_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_statelessTemperatureButton_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_statelessTemperatureButton_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_statelessTemperatureButton_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_availableTemperatureButtons_value = caps_statelessTemperatureButton_get_availableTemperatureButtons_value;
    caps_data->set_availableTemperatureButtons_value = caps_statelessTemperatureButton_set_availableTemperatureButtons_value;
    caps_data->attr_availableTemperatureButtons_send = caps_statelessTemperatureButton_attr_availableTemperatureButtons_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_statelessTemperatureButton.id, caps_statelessTemperatureButton_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_statelessTemperatureButton.cmd_setButton.name, caps_statelessTemperatureButton_cmd_setButton_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setButton of statelessTemperatureButton\n");
    }
    } else {
        printf("fail to init statelessTemperatureButton handle\n");
    }

    return caps_data;
}
