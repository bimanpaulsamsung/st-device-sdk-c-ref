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
#include "caps_audioTrackData.h"

#include "freertos/FreeRTOS.h"

static int caps_audioTrackData_get_totalTime_value(caps_audioTrackData_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_audioTrackData.attr_totalTime.min - 1;
    }
    return caps_data->totalTime_value;
}

static void caps_audioTrackData_set_totalTime_value(caps_audioTrackData_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->totalTime_value = value;
}

static void caps_audioTrackData_attr_totalTime_send(caps_audioTrackData_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_int((char *) caps_helper_audioTrackData.attr_totalTime.name, caps_data->totalTime_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send totalTime value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_audioTrackData_get_elapsedTime_value(caps_audioTrackData_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_audioTrackData.attr_elapsedTime.min - 1;
    }
    return caps_data->elapsedTime_value;
}

static void caps_audioTrackData_set_elapsedTime_value(caps_audioTrackData_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->elapsedTime_value = value;
}

static void caps_audioTrackData_attr_elapsedTime_send(caps_audioTrackData_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_int((char *) caps_helper_audioTrackData.attr_elapsedTime.name, caps_data->elapsedTime_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send elapsedTime value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const JSON_H *caps_audioTrackData_get_audioTrackData_value(caps_audioTrackData_data_t *caps_data)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return NULL;
	}
	return (const JSON_H *)caps_data->audioTrackData_value;
}

static void caps_audioTrackData_set_audioTrackData_value(caps_audioTrackData_data_t *caps_data, const JSON_H *value)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return;
	}
	caps_data->audioTrackData_value = (JSON_H *)value;
}

static void caps_audioTrackData_attr_audioTrackData_send(caps_audioTrackData_data_t *caps_data)
{
	IOT_EVENT *cap_evt;
	uint8_t evt_num = 1;
	int sequence_no;
	iot_cap_val_t value;

	if (!caps_data || !caps_data->handle) {
		printf("fail to get handle\n");
		return;
	}
	if (!caps_data->audioTrackData_value) {
		printf("value is NULL\n");
		return;
	}

	value.type = IOT_CAP_VAL_TYPE_JSON_OBJECT;
	value.json_object = JSON_PRINT(caps_data->audioTrackData_value);

	cap_evt = st_cap_attr_create((char *)caps_helper_audioTrackData.attr_audioTrackData.name,
		&value, NULL, NULL);
	if (!cap_evt) {
		printf("fail to create cap_evt\n");
		return;
	}

	sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
	if (sequence_no < 0)
		printf("fail to send audioTrackData value\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(cap_evt);
}


static void caps_audioTrackData_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_audioTrackData_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_audioTrackData_attr_totalTime_send(caps_data);
    caps_audioTrackData_attr_elapsedTime_send(caps_data);
    caps_audioTrackData_attr_audioTrackData_send(caps_data);
}

caps_audioTrackData_data_t *caps_audioTrackData_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_audioTrackData_data_t *caps_data = NULL;

    caps_data = malloc(sizeof(caps_audioTrackData_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_audioTrackData_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_audioTrackData_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_totalTime_value = caps_audioTrackData_get_totalTime_value;
    caps_data->set_totalTime_value = caps_audioTrackData_set_totalTime_value;
    caps_data->attr_totalTime_send = caps_audioTrackData_attr_totalTime_send;
    caps_data->get_elapsedTime_value = caps_audioTrackData_get_elapsedTime_value;
    caps_data->set_elapsedTime_value = caps_audioTrackData_set_elapsedTime_value;
    caps_data->attr_elapsedTime_send = caps_audioTrackData_attr_elapsedTime_send;
    caps_data->get_audioTrackData_value = caps_audioTrackData_get_audioTrackData_value;
    caps_data->set_audioTrackData_value = caps_audioTrackData_set_audioTrackData_value;
    caps_data->attr_audioTrackData_send = caps_audioTrackData_attr_audioTrackData_send;
    caps_data->totalTime_value = 0;
    caps_data->elapsedTime_value = 0;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_audioTrackData.id, caps_audioTrackData_init_cb, caps_data);
    }
    if (!caps_data->handle) {
        printf("fail to init audioTrackData handle\n");
    }

    return caps_data;
}
