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
#include "caps_zwMultichannel.h"

#include "freertos/FreeRTOS.h"

static const char *caps_zwMultichannel_get_epInfo_value(caps_zwMultichannel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->epInfo_value;
}

static void caps_zwMultichannel_set_epInfo_value(caps_zwMultichannel_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->epInfo_value) {
        free(caps_data->epInfo_value);
    }
    caps_data->epInfo_value = strdup(value);
}

static void caps_zwMultichannel_attr_epInfo_send(caps_zwMultichannel_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->epInfo_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_zwMultichannel.attr_epInfo.name,
        caps_data->epInfo_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send epInfo value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_zwMultichannel_get_epEvent_value(caps_zwMultichannel_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->epEvent_value;
}

static void caps_zwMultichannel_set_epEvent_value(caps_zwMultichannel_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->epEvent_value) {
        free(caps_data->epEvent_value);
    }
    caps_data->epEvent_value = strdup(value);
}

static void caps_zwMultichannel_attr_epEvent_send(caps_zwMultichannel_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->epEvent_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_zwMultichannel.attr_epEvent.name,
        caps_data->epEvent_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send epEvent value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_zwMultichannel_cmd_enableEpEvents_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_zwMultichannel_data_t *caps_data = (caps_zwMultichannel_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_enableEpEvents_usr_cb)
        caps_data->cmd_enableEpEvents_usr_cb(caps_data);
}

static void caps_zwMultichannel_cmd_epCmd_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_zwMultichannel_data_t *caps_data = (caps_zwMultichannel_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_epCmd_usr_cb)
        caps_data->cmd_epCmd_usr_cb(caps_data);
}

static void caps_zwMultichannel_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_zwMultichannel_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_zwMultichannel_attr_epInfo_send(caps_data);
    caps_zwMultichannel_attr_epEvent_send(caps_data);
}

caps_zwMultichannel_data_t *caps_zwMultichannel_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_zwMultichannel_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_zwMultichannel_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_zwMultichannel_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_zwMultichannel_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_epInfo_value = caps_zwMultichannel_get_epInfo_value;
    caps_data->set_epInfo_value = caps_zwMultichannel_set_epInfo_value;
    caps_data->attr_epInfo_send = caps_zwMultichannel_attr_epInfo_send;
    caps_data->get_epEvent_value = caps_zwMultichannel_get_epEvent_value;
    caps_data->set_epEvent_value = caps_zwMultichannel_set_epEvent_value;
    caps_data->attr_epEvent_send = caps_zwMultichannel_attr_epEvent_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_zwMultichannel.id, caps_zwMultichannel_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_zwMultichannel.cmd_enableEpEvents.name, caps_zwMultichannel_cmd_enableEpEvents_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for enableEpEvents of zwMultichannel\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_zwMultichannel.cmd_epCmd.name, caps_zwMultichannel_cmd_epCmd_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for epCmd of zwMultichannel\n");
    }
    } else {
        printf("fail to init zwMultichannel handle\n");
    }

    return caps_data;
}
