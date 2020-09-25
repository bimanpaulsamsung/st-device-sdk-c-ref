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
#include <stdio.h>
#include <stdlib.h>

#include "st_dev.h"
#include "caps_logTrigger.h"

static const JSON_H *caps_logTrigger_get_logInfo_value(caps_logTrigger_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return (const JSON_H *)caps_data->logInfo_value;
}

static void caps_logTrigger_set_logInfo_value(caps_logTrigger_data_t *caps_data, const JSON_H *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->logInfo_value) {
        JSON_DELETE(caps_data->logInfo_value);
    }
    caps_data->logInfo_value = JSON_DUPLICATE(value, true);
}

static void caps_logTrigger_attr_logInfo_send(caps_logTrigger_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no = -1;
    iot_cap_val_t value;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->logInfo_value) {
        printf("value is NULL\n");
        return;
    }

    value.type = IOT_CAP_VAL_TYPE_JSON_OBJECT;
    value.json_object = JSON_PRINT(caps_data->logInfo_value);

    cap_evt = st_cap_create_attr(caps_data->handle,
        (char *)caps_helper_logTrigger.attr_logInfo.name,
        &value,
        NULL,
        NULL);

    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_send_attr(&cap_evt, evt_num);
    if (sequence_no < 0)
        printf("fail to send logInfo value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_free_attr(cap_evt);
}


static int caps_logTrigger_attr_logState_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_LOGTRIGGER_LOGSTATE_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_logTrigger.attr_logState.values[index])) {
            return index;
        }
    }
    return -1;
}

static const char *caps_logTrigger_get_logState_value(caps_logTrigger_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->logState_value;
}

static void caps_logTrigger_set_logState_value(caps_logTrigger_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->logState_value) {
        free(caps_data->logState_value);
    }
    caps_data->logState_value = strdup(value);
}

static void caps_logTrigger_attr_logState_send(caps_logTrigger_data_t *caps_data)
{
    int sequence_no = -1;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->logState_value) {
        printf("value is NULL\n");
        return;
    }

    ST_CAP_SEND_ATTR_STRING(caps_data->handle,
            (char *)caps_helper_logTrigger.attr_logState.name,
            caps_data->logState_value,
            NULL,
            NULL,
            sequence_no);

    if (sequence_no < 0)
        printf("fail to send logState value\n");
    else
        printf("Sequence number return : %d\n", sequence_no);

}


static int caps_logTrigger_attr_logRequestState_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_LOGTRIGGER_LOGREQUESTSTATE_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_logTrigger.attr_logRequestState.values[index])) {
            return index;
        }
    }
    return -1;
}

static const char *caps_logTrigger_get_logRequestState_value(caps_logTrigger_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->logRequestState_value;
}

static void caps_logTrigger_set_logRequestState_value(caps_logTrigger_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->logRequestState_value) {
        free(caps_data->logRequestState_value);
    }
    caps_data->logRequestState_value = strdup(value);
}

static void caps_logTrigger_attr_logRequestState_send(caps_logTrigger_data_t *caps_data)
{
    int sequence_no = -1;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->logRequestState_value) {
        printf("value is NULL\n");
        return;
    }

    ST_CAP_SEND_ATTR_STRING(caps_data->handle,
            (char *)caps_helper_logTrigger.attr_logRequestState.name,
            caps_data->logRequestState_value,
            NULL,
            NULL,
            sequence_no);

    if (sequence_no < 0)
        printf("fail to send logRequestState value\n");
    else
        printf("Sequence number return : %d\n", sequence_no);

}


static void caps_logTrigger_cmd_triggerLog_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_logTrigger_data_t *caps_data = (caps_logTrigger_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_triggerLog_usr_cb)
        caps_data->cmd_triggerLog_usr_cb(caps_data);
}

static void caps_logTrigger_cmd_triggerLogWithLogInfo_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_logTrigger_data_t *caps_data = (caps_logTrigger_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);
    caps_data->cmd_data = cmd_data;

    if (caps_data && caps_data->cmd_triggerLogWithLogInfo_usr_cb)
        caps_data->cmd_triggerLogWithLogInfo_usr_cb(caps_data);
}

static void caps_logTrigger_cmd_triggerLogWithUrl_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_logTrigger_data_t *caps_data = (caps_logTrigger_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);
    caps_data->cmd_data = cmd_data;

    if (caps_data && caps_data->cmd_triggerLogWithUrl_usr_cb)
        caps_data->cmd_triggerLogWithUrl_usr_cb(caps_data);
}

static void caps_logTrigger_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_logTrigger_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_logTrigger_attr_logInfo_send(caps_data);
    caps_logTrigger_attr_logState_send(caps_data);
    caps_logTrigger_attr_logRequestState_send(caps_data);
}

caps_logTrigger_data_t *caps_logTrigger_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_logTrigger_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_logTrigger_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_logTrigger_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_logTrigger_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_logInfo_value = caps_logTrigger_get_logInfo_value;
    caps_data->set_logInfo_value = caps_logTrigger_set_logInfo_value;
    caps_data->attr_logInfo_send = caps_logTrigger_attr_logInfo_send;
    caps_data->get_logState_value = caps_logTrigger_get_logState_value;
    caps_data->set_logState_value = caps_logTrigger_set_logState_value;
    caps_data->attr_logState_str2idx = caps_logTrigger_attr_logState_str2idx;
    caps_data->attr_logState_send = caps_logTrigger_attr_logState_send;
    caps_data->get_logRequestState_value = caps_logTrigger_get_logRequestState_value;
    caps_data->set_logRequestState_value = caps_logTrigger_set_logRequestState_value;
    caps_data->attr_logRequestState_str2idx = caps_logTrigger_attr_logRequestState_str2idx;
    caps_data->attr_logRequestState_send = caps_logTrigger_attr_logRequestState_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_logTrigger.id, caps_logTrigger_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_logTrigger.cmd_triggerLog.name, caps_logTrigger_cmd_triggerLog_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for triggerLog of logTrigger\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_logTrigger.cmd_triggerLogWithLogInfo.name, caps_logTrigger_cmd_triggerLogWithLogInfo_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for triggerLogWithLogInfo of logTrigger\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_logTrigger.cmd_triggerLogWithUrl.name, caps_logTrigger_cmd_triggerLogWithUrl_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for triggerLogWithUrl of logTrigger\n");
    }
    } else {
        printf("fail to init logTrigger handle\n");
    }

    return caps_data;
}
