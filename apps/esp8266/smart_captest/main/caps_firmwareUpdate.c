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
#include "caps_firmwareUpdate.h"

#include "freertos/FreeRTOS.h"

static int caps_firmwareUpdate_attr_lastUpdateStatus_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_FIRMWAREUPDATE_LASTUPDATESTATUS_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_firmwareUpdate.attr_lastUpdateStatus.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_firmwareUpdate_get_lastUpdateStatus_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->lastUpdateStatus_value;
}

static void caps_firmwareUpdate_set_lastUpdateStatus_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->lastUpdateStatus_value) {
        free(caps_data->lastUpdateStatus_value);
    }
    caps_data->lastUpdateStatus_value = strdup(value);
}

static void caps_firmwareUpdate_attr_lastUpdateStatus_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->lastUpdateStatus_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_lastUpdateStatus.name,
        caps_data->lastUpdateStatus_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send lastUpdateStatus value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_firmwareUpdate_attr_state_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_FIRMWAREUPDATE_STATE_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_firmwareUpdate.attr_state.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_firmwareUpdate_get_state_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->state_value;
}

static void caps_firmwareUpdate_set_state_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->state_value) {
        free(caps_data->state_value);
    }
    caps_data->state_value = strdup(value);
}

static void caps_firmwareUpdate_attr_state_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->state_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_state.name,
        caps_data->state_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send state value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_firmwareUpdate_get_currentVersion_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->currentVersion_value;
}

static void caps_firmwareUpdate_set_currentVersion_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->currentVersion_value) {
        free(caps_data->currentVersion_value);
    }
    caps_data->currentVersion_value = strdup(value);
}

static void caps_firmwareUpdate_attr_currentVersion_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->currentVersion_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_currentVersion.name,
        caps_data->currentVersion_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send currentVersion value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_firmwareUpdate_get_lastUpdateTime_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->lastUpdateTime_value;
}

static void caps_firmwareUpdate_set_lastUpdateTime_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->lastUpdateTime_value) {
        free(caps_data->lastUpdateTime_value);
    }
    caps_data->lastUpdateTime_value = strdup(value);
}

static void caps_firmwareUpdate_attr_lastUpdateTime_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->lastUpdateTime_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_lastUpdateTime.name,
        caps_data->lastUpdateTime_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send lastUpdateTime value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_firmwareUpdate_get_availableVersion_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->availableVersion_value;
}

static void caps_firmwareUpdate_set_availableVersion_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->availableVersion_value) {
        free(caps_data->availableVersion_value);
    }
    caps_data->availableVersion_value = strdup(value);
}

static void caps_firmwareUpdate_attr_availableVersion_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->availableVersion_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_availableVersion.name,
        caps_data->availableVersion_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send availableVersion value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_firmwareUpdate_get_lastUpdateStatusReason_value(caps_firmwareUpdate_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->lastUpdateStatusReason_value;
}

static void caps_firmwareUpdate_set_lastUpdateStatusReason_value(caps_firmwareUpdate_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->lastUpdateStatusReason_value) {
        free(caps_data->lastUpdateStatusReason_value);
    }
    caps_data->lastUpdateStatusReason_value = strdup(value);
}

static void caps_firmwareUpdate_attr_lastUpdateStatusReason_send(caps_firmwareUpdate_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->lastUpdateStatusReason_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_firmwareUpdate.attr_lastUpdateStatusReason.name,
        caps_data->lastUpdateStatusReason_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send lastUpdateStatusReason value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_firmwareUpdate_cmd_checkForFirmwareUpdate_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_firmwareUpdate_data_t *caps_data = (caps_firmwareUpdate_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_checkForFirmwareUpdate_usr_cb)
        caps_data->cmd_checkForFirmwareUpdate_usr_cb(caps_data);
}

static void caps_firmwareUpdate_cmd_updateFirmware_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_firmwareUpdate_data_t *caps_data = (caps_firmwareUpdate_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_updateFirmware_usr_cb)
        caps_data->cmd_updateFirmware_usr_cb(caps_data);
}

static void caps_firmwareUpdate_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_firmwareUpdate_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_firmwareUpdate_attr_lastUpdateStatus_send(caps_data);
    caps_firmwareUpdate_attr_state_send(caps_data);
    caps_firmwareUpdate_attr_currentVersion_send(caps_data);
    caps_firmwareUpdate_attr_lastUpdateTime_send(caps_data);
    caps_firmwareUpdate_attr_availableVersion_send(caps_data);
    caps_firmwareUpdate_attr_lastUpdateStatusReason_send(caps_data);
}

caps_firmwareUpdate_data_t *caps_firmwareUpdate_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_firmwareUpdate_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_firmwareUpdate_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_firmwareUpdate_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_firmwareUpdate_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_lastUpdateStatus_value = caps_firmwareUpdate_get_lastUpdateStatus_value;
    caps_data->set_lastUpdateStatus_value = caps_firmwareUpdate_set_lastUpdateStatus_value;
    caps_data->attr_lastUpdateStatus_str2idx = caps_firmwareUpdate_attr_lastUpdateStatus_str2idx;
    caps_data->attr_lastUpdateStatus_send = caps_firmwareUpdate_attr_lastUpdateStatus_send;
    caps_data->get_state_value = caps_firmwareUpdate_get_state_value;
    caps_data->set_state_value = caps_firmwareUpdate_set_state_value;
    caps_data->attr_state_str2idx = caps_firmwareUpdate_attr_state_str2idx;
    caps_data->attr_state_send = caps_firmwareUpdate_attr_state_send;
    caps_data->get_currentVersion_value = caps_firmwareUpdate_get_currentVersion_value;
    caps_data->set_currentVersion_value = caps_firmwareUpdate_set_currentVersion_value;
    caps_data->attr_currentVersion_send = caps_firmwareUpdate_attr_currentVersion_send;
    caps_data->get_lastUpdateTime_value = caps_firmwareUpdate_get_lastUpdateTime_value;
    caps_data->set_lastUpdateTime_value = caps_firmwareUpdate_set_lastUpdateTime_value;
    caps_data->attr_lastUpdateTime_send = caps_firmwareUpdate_attr_lastUpdateTime_send;
    caps_data->get_availableVersion_value = caps_firmwareUpdate_get_availableVersion_value;
    caps_data->set_availableVersion_value = caps_firmwareUpdate_set_availableVersion_value;
    caps_data->attr_availableVersion_send = caps_firmwareUpdate_attr_availableVersion_send;
    caps_data->get_lastUpdateStatusReason_value = caps_firmwareUpdate_get_lastUpdateStatusReason_value;
    caps_data->set_lastUpdateStatusReason_value = caps_firmwareUpdate_set_lastUpdateStatusReason_value;
    caps_data->attr_lastUpdateStatusReason_send = caps_firmwareUpdate_attr_lastUpdateStatusReason_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_firmwareUpdate.id, caps_firmwareUpdate_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_firmwareUpdate.cmd_checkForFirmwareUpdate.name, caps_firmwareUpdate_cmd_checkForFirmwareUpdate_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for checkForFirmwareUpdate of firmwareUpdate\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_firmwareUpdate.cmd_updateFirmware.name, caps_firmwareUpdate_cmd_updateFirmware_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for updateFirmware of firmwareUpdate\n");
    }
    } else {
        printf("fail to init firmwareUpdate handle\n");
    }

    return caps_data;
}
