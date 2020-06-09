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
#include "caps_tV.h"

#include "freertos/FreeRTOS.h"

static const char *caps_tV_get_sound_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->sound_value;
}

static void caps_tV_set_sound_value(caps_tV_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->sound_value) {
        free(caps_data->sound_value);
    }
    caps_data->sound_value = strdup(value);
}

static void caps_tV_attr_sound_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->sound_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tV.attr_sound.name,
        caps_data->sound_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send sound value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_tV_get_picture_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->picture_value;
}

static void caps_tV_set_picture_value(caps_tV_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->picture_value) {
        free(caps_data->picture_value);
    }
    caps_data->picture_value = strdup(value);
}

static void caps_tV_attr_picture_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->picture_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tV.attr_picture.name,
        caps_data->picture_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send picture value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_tV_get_movieMode_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->movieMode_value;
}

static void caps_tV_set_movieMode_value(caps_tV_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->movieMode_value) {
        free(caps_data->movieMode_value);
    }
    caps_data->movieMode_value = strdup(value);
}

static void caps_tV_attr_movieMode_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->movieMode_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tV.attr_movieMode.name,
        caps_data->movieMode_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send movieMode value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static const char *caps_tV_get_power_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->power_value;
}

static void caps_tV_set_power_value(caps_tV_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->power_value) {
        free(caps_data->power_value);
    }
    caps_data->power_value = strdup(value);
}

static void caps_tV_attr_power_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->power_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string((char *)caps_helper_tV.attr_power.name,
        caps_data->power_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send power value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_tV_get_volume_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_tV.attr_volume.min - 1;
    }
    return caps_data->volume_value;
}

static void caps_tV_set_volume_value(caps_tV_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->volume_value = value;
}

static void caps_tV_attr_volume_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_int((char *) caps_helper_tV.attr_volume.name, caps_data->volume_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send volume value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static int caps_tV_get_channel_value(caps_tV_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_tV.attr_channel.min - 1;
    }
    return caps_data->channel_value;
}

static void caps_tV_set_channel_value(caps_tV_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->channel_value = value;
}

static void caps_tV_attr_channel_send(caps_tV_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_int((char *) caps_helper_tV.attr_channel.name, caps_data->channel_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send channel value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_tV_cmd_channelUp_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tV_data_t *caps_data = (caps_tV_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_channelUp_usr_cb)
        caps_data->cmd_channelUp_usr_cb(caps_data);
}

static void caps_tV_cmd_volumeDown_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tV_data_t *caps_data = (caps_tV_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_volumeDown_usr_cb)
        caps_data->cmd_volumeDown_usr_cb(caps_data);
}

static void caps_tV_cmd_volumeUp_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tV_data_t *caps_data = (caps_tV_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_volumeUp_usr_cb)
        caps_data->cmd_volumeUp_usr_cb(caps_data);
}

static void caps_tV_cmd_channelDown_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_tV_data_t *caps_data = (caps_tV_data_t *)usr_data;

    printf("called [%s] func with num_args:%u\n", __func__, cmd_data->num_args);

    if (caps_data && caps_data->cmd_channelDown_usr_cb)
        caps_data->cmd_channelDown_usr_cb(caps_data);
}

static void caps_tV_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_tV_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_tV_attr_sound_send(caps_data);
    caps_tV_attr_picture_send(caps_data);
    caps_tV_attr_movieMode_send(caps_data);
    caps_tV_attr_power_send(caps_data);
    caps_tV_attr_volume_send(caps_data);
    caps_tV_attr_channel_send(caps_data);
}

caps_tV_data_t *caps_tV_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_tV_data_t *caps_data = NULL;
    int err;

    caps_data = malloc(sizeof(caps_tV_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_tV_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_tV_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_sound_value = caps_tV_get_sound_value;
    caps_data->set_sound_value = caps_tV_set_sound_value;
    caps_data->attr_sound_send = caps_tV_attr_sound_send;
    caps_data->get_picture_value = caps_tV_get_picture_value;
    caps_data->set_picture_value = caps_tV_set_picture_value;
    caps_data->attr_picture_send = caps_tV_attr_picture_send;
    caps_data->get_movieMode_value = caps_tV_get_movieMode_value;
    caps_data->set_movieMode_value = caps_tV_set_movieMode_value;
    caps_data->attr_movieMode_send = caps_tV_attr_movieMode_send;
    caps_data->get_power_value = caps_tV_get_power_value;
    caps_data->set_power_value = caps_tV_set_power_value;
    caps_data->attr_power_send = caps_tV_attr_power_send;
    caps_data->get_volume_value = caps_tV_get_volume_value;
    caps_data->set_volume_value = caps_tV_set_volume_value;
    caps_data->attr_volume_send = caps_tV_attr_volume_send;
    caps_data->get_channel_value = caps_tV_get_channel_value;
    caps_data->set_channel_value = caps_tV_set_channel_value;
    caps_data->attr_channel_send = caps_tV_attr_channel_send;
    caps_data->volume_value = 0;
    caps_data->channel_value = 0;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_tV.id, caps_tV_init_cb, caps_data);
    }
    if (caps_data->handle) {
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tV.cmd_channelUp.name, caps_tV_cmd_channelUp_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for channelUp of tV\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tV.cmd_volumeDown.name, caps_tV_cmd_volumeDown_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for volumeDown of tV\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tV.cmd_volumeUp.name, caps_tV_cmd_volumeUp_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for volumeUp of tV\n");
    }
        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_tV.cmd_channelDown.name, caps_tV_cmd_channelDown_cb, caps_data);
        if (err) {
            printf("fail to set cmd_cb for channelDown of tV\n");
    }
    } else {
        printf("fail to init tV handle\n");
    }

    return caps_data;
}
