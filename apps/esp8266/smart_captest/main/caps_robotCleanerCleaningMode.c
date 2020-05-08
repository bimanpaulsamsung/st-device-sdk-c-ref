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
#include "caps_robotCleanerCleaningMode.h"

#include "freertos/FreeRTOS.h"

static int caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_robotCleanerCleaningMode_get_robotCleanerCleaningMode_value(caps_robotCleanerCleaningMode_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->robotCleanerCleaningMode_value;
}

static void caps_robotCleanerCleaningMode_set_robotCleanerCleaningMode_value(caps_robotCleanerCleaningMode_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->robotCleanerCleaningMode_value = (char *)value;
}

static void caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_send(caps_robotCleanerCleaningMode_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->robotCleanerCleaningMode_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.name,
        caps_data->robotCleanerCleaningMode_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send robotCleanerCleaningMode value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_robotCleanerCleaningMode_cmd_setRobotCleanerCleaningMode_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_robotCleanerCleaningMode_data_t *caps_data = (caps_robotCleanerCleaningMode_data_t *)usr_data;
    char *value;
    int index;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    index = caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_str2idx(cmd_data->cmd_data[0].string);
    if (index < 0) {
        printf("%s is not supported value for setRobotCleanerCleaningMode\n", cmd_data->cmd_data[0].string);
        return;
    }
    value = (char *)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[index];

    caps_robotCleanerCleaningMode_set_robotCleanerCleaningMode_value(caps_data, value);
    if (caps_data && caps_data->cmd_setRobotCleanerCleaningMode_usr_cb)
        caps_data->cmd_setRobotCleanerCleaningMode_usr_cb(caps_data);
    caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_send(caps_data);
}

static void caps_robotCleanerCleaningMode_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_robotCleanerCleaningMode_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_send(caps_data);
}

caps_robotCleanerCleaningMode_data_t *caps_robotCleanerCleaningMode_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_robotCleanerCleaningMode_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_robotCleanerCleaningMode_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_robotCleanerCleaningMode_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_robotCleanerCleaningMode_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_robotCleanerCleaningMode_value = caps_robotCleanerCleaningMode_get_robotCleanerCleaningMode_value;
    caps_data->set_robotCleanerCleaningMode_value = caps_robotCleanerCleaningMode_set_robotCleanerCleaningMode_value;
    caps_data->attr_robotCleanerCleaningMode_str2idx = caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_str2idx;
    caps_data->attr_robotCleanerCleaningMode_send = caps_robotCleanerCleaningMode_attr_robotCleanerCleaningMode_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_robotCleanerCleaningMode.id, caps_robotCleanerCleaningMode_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_robotCleanerCleaningMode.cmd_setRobotCleanerCleaningMode.name, caps_robotCleanerCleaningMode_cmd_setRobotCleanerCleaningMode_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setRobotCleanerCleaningMode of robotCleanerCleaningMode\n");
    }
    } else {
        printf("fail to init robotCleanerCleaningMode handle\n");
    }

    return caps_data;
}