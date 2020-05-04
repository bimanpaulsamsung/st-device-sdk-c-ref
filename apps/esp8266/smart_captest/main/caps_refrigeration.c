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
#include "caps_refrigeration.h"

#include "freertos/FreeRTOS.h"

static int caps_refrigeration_attr_defrost_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_REFRIGERATION_DEFROST_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_refrigeration.attr_defrost.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_refrigeration_get_defrost_value(caps_refrigeration_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->defrost_value;
}

static void caps_refrigeration_set_defrost_value(caps_refrigeration_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->defrost_value = (char *)value;
}

static void caps_refrigeration_attr_defrost_send(caps_refrigeration_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->defrost_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_refrigeration.attr_defrost.name,
        caps_data->defrost_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send defrost value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_refrigeration_attr_rapidFreezing_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_refrigeration.attr_rapidFreezing.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_refrigeration_get_rapidFreezing_value(caps_refrigeration_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->rapidFreezing_value;
}

static void caps_refrigeration_set_rapidFreezing_value(caps_refrigeration_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->rapidFreezing_value = (char *)value;
}

static void caps_refrigeration_attr_rapidFreezing_send(caps_refrigeration_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->rapidFreezing_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_refrigeration.attr_rapidFreezing.name,
        caps_data->rapidFreezing_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send rapidFreezing value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_refrigeration_attr_rapidCooling_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_refrigeration.attr_rapidCooling.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_refrigeration_get_rapidCooling_value(caps_refrigeration_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->rapidCooling_value;
}

static void caps_refrigeration_set_rapidCooling_value(caps_refrigeration_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->rapidCooling_value = (char *)value;
}

static void caps_refrigeration_attr_rapidCooling_send(caps_refrigeration_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->rapidCooling_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_refrigeration.attr_rapidCooling.name,
        caps_data->rapidCooling_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send rapidCooling value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_refrigeration_cmd_setRapidCooling_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_refrigeration_data_t *caps_data = (caps_refrigeration_data_t *)usr_data;
    char *value;
    int index;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    index = caps_refrigeration_attr_rapidCooling_str2idx(cmd_data->cmd_data[0].string);
    if (index < 0) {
        printf("%s is not supported value for setRapidCooling\n", cmd_data->cmd_data[0].string);
        return;
    }
    value = (char *)caps_helper_refrigeration.attr_rapidCooling.values[index];

    caps_refrigeration_set_rapidCooling_value(caps_data, value);
    if (caps_data && caps_data->cmd_setRapidCooling_usr_cb)
        caps_data->cmd_setRapidCooling_usr_cb(caps_data);
    caps_refrigeration_attr_rapidCooling_send(caps_data);
}

static void caps_refrigeration_cmd_setRapidFreezing_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_refrigeration_data_t *caps_data = (caps_refrigeration_data_t *)usr_data;
    char *value;
    int index;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    index = caps_refrigeration_attr_rapidFreezing_str2idx(cmd_data->cmd_data[0].string);
    if (index < 0) {
        printf("%s is not supported value for setRapidFreezing\n", cmd_data->cmd_data[0].string);
        return;
    }
    value = (char *)caps_helper_refrigeration.attr_rapidFreezing.values[index];

    caps_refrigeration_set_rapidFreezing_value(caps_data, value);
    if (caps_data && caps_data->cmd_setRapidFreezing_usr_cb)
        caps_data->cmd_setRapidFreezing_usr_cb(caps_data);
    caps_refrigeration_attr_rapidFreezing_send(caps_data);
}

static void caps_refrigeration_cmd_setDefrost_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_refrigeration_data_t *caps_data = (caps_refrigeration_data_t *)usr_data;
    char *value;
    int index;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    index = caps_refrigeration_attr_defrost_str2idx(cmd_data->cmd_data[0].string);
    if (index < 0) {
        printf("%s is not supported value for setDefrost\n", cmd_data->cmd_data[0].string);
        return;
    }
    value = (char *)caps_helper_refrigeration.attr_defrost.values[index];

    caps_refrigeration_set_defrost_value(caps_data, value);
    if (caps_data && caps_data->cmd_setDefrost_usr_cb)
        caps_data->cmd_setDefrost_usr_cb(caps_data);
    caps_refrigeration_attr_defrost_send(caps_data);
}

static void caps_refrigeration_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_refrigeration_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_refrigeration_attr_defrost_send(caps_data);
    caps_refrigeration_attr_rapidFreezing_send(caps_data);
    caps_refrigeration_attr_rapidCooling_send(caps_data);
}

caps_refrigeration_data_t *caps_refrigeration_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_refrigeration_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_refrigeration_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_refrigeration_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_refrigeration_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_defrost_value = caps_refrigeration_get_defrost_value;
    caps_data->set_defrost_value = caps_refrigeration_set_defrost_value;
    caps_data->attr_defrost_str2idx = caps_refrigeration_attr_defrost_str2idx;
    caps_data->attr_defrost_send = caps_refrigeration_attr_defrost_send;
    caps_data->get_rapidFreezing_value = caps_refrigeration_get_rapidFreezing_value;
    caps_data->set_rapidFreezing_value = caps_refrigeration_set_rapidFreezing_value;
    caps_data->attr_rapidFreezing_str2idx = caps_refrigeration_attr_rapidFreezing_str2idx;
    caps_data->attr_rapidFreezing_send = caps_refrigeration_attr_rapidFreezing_send;
    caps_data->get_rapidCooling_value = caps_refrigeration_get_rapidCooling_value;
    caps_data->set_rapidCooling_value = caps_refrigeration_set_rapidCooling_value;
    caps_data->attr_rapidCooling_str2idx = caps_refrigeration_attr_rapidCooling_str2idx;
    caps_data->attr_rapidCooling_send = caps_refrigeration_attr_rapidCooling_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_refrigeration.id, caps_refrigeration_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_refrigeration.cmd_setRapidCooling.name, caps_refrigeration_cmd_setRapidCooling_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setRapidCooling of refrigeration\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_refrigeration.cmd_setRapidFreezing.name, caps_refrigeration_cmd_setRapidFreezing_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setRapidFreezing of refrigeration\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_refrigeration.cmd_setDefrost.name, caps_refrigeration_cmd_setDefrost_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for setDefrost of refrigeration\n");
    }
    } else {
        printf("fail to init refrigeration handle\n");
    }

    return caps_data;
}
