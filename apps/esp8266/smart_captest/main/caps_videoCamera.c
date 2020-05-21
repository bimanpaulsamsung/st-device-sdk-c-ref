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
#include "caps_videoCamera.h"

#include "freertos/FreeRTOS.h"

static int caps_videoCamera_attr_mute_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_videoCamera.attr_mute.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_videoCamera_get_mute_value(caps_videoCamera_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->mute_value;
}

static void caps_videoCamera_set_mute_value(caps_videoCamera_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->mute_value = (char *)value;
}

static void caps_videoCamera_attr_mute_send(caps_videoCamera_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->mute_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_videoCamera.attr_mute.name,
        caps_data->mute_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send mute value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_videoCamera_attr_camera_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_videoCamera.attr_camera.values[index])) {
            return index;
        }
    }   
    return -1; 
}

static const char *caps_videoCamera_get_camera_value(caps_videoCamera_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->camera_value;
}

static void caps_videoCamera_set_camera_value(caps_videoCamera_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->camera_value = (char *)value;
}

static void caps_videoCamera_attr_camera_send(caps_videoCamera_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->camera_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_videoCamera.attr_camera.name,
        caps_data->camera_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send camera value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_videoCamera_get_statusMessage_value(caps_videoCamera_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->statusMessage_value;
}

static void caps_videoCamera_set_statusMessage_value(caps_videoCamera_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->statusMessage_value = (char *)value;
}

static void caps_videoCamera_attr_statusMessage_send(caps_videoCamera_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
	if (!caps_data->statusMessage_value) {
		printf("value is NULL\n");
		return;
	}

    cap_evt = st_cap_attr_create_string((char *)caps_helper_videoCamera.attr_statusMessage.name,
        caps_data->statusMessage_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send statusMessage value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const JSON_H *caps_videoCamera_get_settings_value(caps_videoCamera_data_t *caps_data)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return NULL;
	}
	return (const JSON_H *)caps_data->settings_value;
}

static void caps_videoCamera_set_settings_value(caps_videoCamera_data_t *caps_data, const JSON_H *value)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return;
	}
	if (caps_data->settings_value) {
		JSON_DELETE(caps_data->settings_value);
	}
	caps_data->settings_value = JSON_DUPLICATE(value, true);
}

static void caps_videoCamera_attr_settings_send(caps_videoCamera_data_t *caps_data)
{
	IOT_EVENT *cap_evt;
	uint8_t evt_num = 1;
	int sequence_no;
	iot_cap_val_t value;

	if (!caps_data || !caps_data->handle) {
		printf("fail to get handle\n");
		return;
	}
	if (!caps_data->settings_value) {
		printf("value is NULL\n");
		return;
	}

	value.type = IOT_CAP_VAL_TYPE_JSON_OBJECT;
	value.json_object = JSON_PRINT(caps_data->settings_value);

	cap_evt = st_cap_attr_create((char *)caps_helper_videoCamera.attr_settings.name,
		&value, NULL, NULL);
	if (!cap_evt) {
		printf("fail to create cap_evt\n");
		return;
	}

	sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
	if (sequence_no < 0)
		printf("fail to send settings value\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(cap_evt);
}


static void caps_videoCamera_cmd_unmute_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = (caps_videoCamera_data_t *)usr_data;
    const char* value = caps_helper_videoCamera.attr_mute.values[CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_UNMUTED];

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    caps_videoCamera_set_mute_value(caps_data, value);
    if (caps_data && caps_data->cmd_unmute_usr_cb)
        caps_data->cmd_unmute_usr_cb(caps_data);
    caps_videoCamera_attr_mute_send(caps_data);
}

static void caps_videoCamera_cmd_on_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = (caps_videoCamera_data_t *)usr_data;
    const char* value = caps_helper_videoCamera.attr_camera.values[CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_ON];

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    caps_videoCamera_set_camera_value(caps_data, value);
    if (caps_data && caps_data->cmd_on_usr_cb)
        caps_data->cmd_on_usr_cb(caps_data);
    caps_videoCamera_attr_camera_send(caps_data);
}

static void caps_videoCamera_cmd_off_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = (caps_videoCamera_data_t *)usr_data;
    const char* value = caps_helper_videoCamera.attr_camera.values[CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_OFF];

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    caps_videoCamera_set_camera_value(caps_data, value);
    if (caps_data && caps_data->cmd_off_usr_cb)
        caps_data->cmd_off_usr_cb(caps_data);
    caps_videoCamera_attr_camera_send(caps_data);
}

static void caps_videoCamera_cmd_flip_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = (caps_videoCamera_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_flip_usr_cb)
        caps_data->cmd_flip_usr_cb(caps_data);
}

static void caps_videoCamera_cmd_mute_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = (caps_videoCamera_data_t *)usr_data;
    const char* value = caps_helper_videoCamera.attr_mute.values[CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MUTED];

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    caps_videoCamera_set_mute_value(caps_data, value);
    if (caps_data && caps_data->cmd_mute_usr_cb)
        caps_data->cmd_mute_usr_cb(caps_data);
    caps_videoCamera_attr_mute_send(caps_data);
}

static void caps_videoCamera_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_videoCamera_attr_mute_send(caps_data);
    caps_videoCamera_attr_camera_send(caps_data);
    caps_videoCamera_attr_statusMessage_send(caps_data);
    caps_videoCamera_attr_settings_send(caps_data);
}

caps_videoCamera_data_t *caps_videoCamera_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_videoCamera_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_videoCamera_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_videoCamera_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_videoCamera_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_mute_value = caps_videoCamera_get_mute_value;
    caps_data->set_mute_value = caps_videoCamera_set_mute_value;
    caps_data->attr_mute_str2idx = caps_videoCamera_attr_mute_str2idx;
    caps_data->attr_mute_send = caps_videoCamera_attr_mute_send;
    caps_data->get_camera_value = caps_videoCamera_get_camera_value;
    caps_data->set_camera_value = caps_videoCamera_set_camera_value;
    caps_data->attr_camera_str2idx = caps_videoCamera_attr_camera_str2idx;
    caps_data->attr_camera_send = caps_videoCamera_attr_camera_send;
    caps_data->get_statusMessage_value = caps_videoCamera_get_statusMessage_value;
    caps_data->set_statusMessage_value = caps_videoCamera_set_statusMessage_value;
    caps_data->attr_statusMessage_send = caps_videoCamera_attr_statusMessage_send;
    caps_data->get_settings_value = caps_videoCamera_get_settings_value;
    caps_data->set_settings_value = caps_videoCamera_set_settings_value;
    caps_data->attr_settings_send = caps_videoCamera_attr_settings_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_videoCamera.id, caps_videoCamera_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_videoCamera.cmd_unmute.name, caps_videoCamera_cmd_unmute_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for unmute of videoCamera\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_videoCamera.cmd_on.name, caps_videoCamera_cmd_on_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for on of videoCamera\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_videoCamera.cmd_off.name, caps_videoCamera_cmd_off_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for off of videoCamera\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_videoCamera.cmd_flip.name, caps_videoCamera_cmd_flip_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for flip of videoCamera\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_videoCamera.cmd_mute.name, caps_videoCamera_cmd_mute_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for mute of videoCamera\n");
    }
    } else {
        printf("fail to init videoCamera handle\n");
    }

    return caps_data;
}
