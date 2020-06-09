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
#include "caps_tvChannel.h"

#include "freertos/FreeRTOS.h"

static const char *caps_tvChannel_get_tvChannel_value(caps_tvChannel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->tvChannel_value;
}

static void caps_tvChannel_set_tvChannel_value(caps_tvChannel_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->tvChannel_value) {
        free(caps_data->tvChannel_value);
    }
    caps_data->tvChannel_value = strdup(value);
}

static void caps_tvChannel_attr_tvChannel_send(caps_tvChannel_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->tvChannel_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tvChannel.attr_tvChannel.name,
        caps_data->tvChannel_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send tvChannel value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_tvChannel_get_tvChannelName_value(caps_tvChannel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->tvChannelName_value;
}

static void caps_tvChannel_set_tvChannelName_value(caps_tvChannel_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->tvChannelName_value) {
        free(caps_data->tvChannelName_value);
    }
    caps_data->tvChannelName_value = strdup(value);
}

static void caps_tvChannel_attr_tvChannelName_send(caps_tvChannel_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->tvChannelName_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tvChannel.attr_tvChannelName.name,
        caps_data->tvChannelName_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send tvChannelName value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_tvChannel_cmd_channelUp_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = (caps_tvChannel_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_channelUp_usr_cb)
        caps_data->cmd_channelUp_usr_cb(caps_data);
}

static void caps_tvChannel_cmd_channelDown_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = (caps_tvChannel_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_channelDown_usr_cb)
        caps_data->cmd_channelDown_usr_cb(caps_data);
}

static void caps_tvChannel_cmd_setTvChannelName_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = (caps_tvChannel_data_t *)usr_data;
    char *value;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    value = cmd_data->cmd_data[0].string;

    caps_tvChannel_set_tvChannelName_value(caps_data, value);
    if (caps_data && caps_data->cmd_setTvChannelName_usr_cb)
        caps_data->cmd_setTvChannelName_usr_cb(caps_data);
    caps_tvChannel_attr_tvChannelName_send(caps_data);
}

static void caps_tvChannel_cmd_setTvChannel_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = (caps_tvChannel_data_t *)usr_data;
    char *value;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    value = cmd_data->cmd_data[0].string;

    caps_tvChannel_set_tvChannel_value(caps_data, value);
    if (caps_data && caps_data->cmd_setTvChannel_usr_cb)
        caps_data->cmd_setTvChannel_usr_cb(caps_data);
    caps_tvChannel_attr_tvChannel_send(caps_data);
}

static void caps_tvChannel_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_tvChannel_attr_tvChannel_send(caps_data);
    caps_tvChannel_attr_tvChannelName_send(caps_data);
}

caps_tvChannel_data_t *caps_tvChannel_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_tvChannel_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_tvChannel_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_tvChannel_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_tvChannel_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_tvChannel_value = caps_tvChannel_get_tvChannel_value;
    caps_data->set_tvChannel_value = caps_tvChannel_set_tvChannel_value;
    caps_data->attr_tvChannel_send = caps_tvChannel_attr_tvChannel_send;
    caps_data->get_tvChannelName_value = caps_tvChannel_get_tvChannelName_value;
    caps_data->set_tvChannelName_value = caps_tvChannel_set_tvChannelName_value;
    caps_data->attr_tvChannelName_send = caps_tvChannel_attr_tvChannelName_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_tvChannel.id, caps_tvChannel_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tvChannel.cmd_channelUp.name, caps_tvChannel_cmd_channelUp_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for channelUp of tvChannel\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tvChannel.cmd_channelDown.name, caps_tvChannel_cmd_channelDown_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for channelDown of tvChannel\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tvChannel.cmd_setTvChannelName.name, caps_tvChannel_cmd_setTvChannelName_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setTvChannelName of tvChannel\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tvChannel.cmd_setTvChannel.name, caps_tvChannel_cmd_setTvChannel_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setTvChannel of tvChannel\n");
    }
    } else {
        printf("fail to init tvChannel handle\n");
    }

    return caps_data;
}
