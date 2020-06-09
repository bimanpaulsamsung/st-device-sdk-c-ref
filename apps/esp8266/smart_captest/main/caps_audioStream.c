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
#include "caps_audioStream.h"

#include "freertos/FreeRTOS.h"

static const char *caps_audioStream_get_uri_value(caps_audioStream_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->uri_value;
}

static void caps_audioStream_set_uri_value(caps_audioStream_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->uri_value) {
        free(caps_data->uri_value);
    }
    caps_data->uri_value = strdup(value);
}

static void caps_audioStream_attr_uri_send(caps_audioStream_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->uri_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_audioStream.attr_uri.name,
        caps_data->uri_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send uri value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_audioStream_cmd_startAudio_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_audioStream_data_t *caps_data = (caps_audioStream_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_startAudio_usr_cb)
        caps_data->cmd_startAudio_usr_cb(caps_data);
}

static void caps_audioStream_cmd_stopAudio_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_audioStream_data_t *caps_data = (caps_audioStream_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_stopAudio_usr_cb)
        caps_data->cmd_stopAudio_usr_cb(caps_data);
}

static void caps_audioStream_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_audioStream_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_audioStream_attr_uri_send(caps_data);
}

caps_audioStream_data_t *caps_audioStream_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_audioStream_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_audioStream_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_audioStream_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_audioStream_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_uri_value = caps_audioStream_get_uri_value;
    caps_data->set_uri_value = caps_audioStream_set_uri_value;
    caps_data->attr_uri_send = caps_audioStream_attr_uri_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_audioStream.id, caps_audioStream_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_audioStream.cmd_startAudio.name, caps_audioStream_cmd_startAudio_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for startAudio of audioStream\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_audioStream.cmd_stopAudio.name, caps_audioStream_cmd_stopAudio_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for stopAudio of audioStream\n");
    }
    } else {
        printf("fail to init audioStream handle\n");
    }

    return caps_data;
}
