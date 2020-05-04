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

#include "caps/iot_caps_helper_tV.h"

typedef struct caps_tV_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    char *sound_value;
    char *picture_value;
    char *movieMode_value;
    char *power_value;
    int volume_value;
    int channel_value;

    const char *(*get_sound_value)(struct caps_tV_data *caps_data);
    void (*set_sound_value)(struct caps_tV_data *caps_data, const char *value);
    void (*attr_sound_send)(struct caps_tV_data *caps_data);
    const char *(*get_picture_value)(struct caps_tV_data *caps_data);
    void (*set_picture_value)(struct caps_tV_data *caps_data, const char *value);
    void (*attr_picture_send)(struct caps_tV_data *caps_data);
    const char *(*get_movieMode_value)(struct caps_tV_data *caps_data);
    void (*set_movieMode_value)(struct caps_tV_data *caps_data, const char *value);
    void (*attr_movieMode_send)(struct caps_tV_data *caps_data);
    const char *(*get_power_value)(struct caps_tV_data *caps_data);
    void (*set_power_value)(struct caps_tV_data *caps_data, const char *value);
    void (*attr_power_send)(struct caps_tV_data *caps_data);
    int (*get_volume_value)(struct caps_tV_data *caps_data);
    void (*set_volume_value)(struct caps_tV_data *caps_data, int value);
    void (*attr_volume_send)(struct caps_tV_data *caps_data);
    int (*get_channel_value)(struct caps_tV_data *caps_data);
    void (*set_channel_value)(struct caps_tV_data *caps_data, int value);
    void (*attr_channel_send)(struct caps_tV_data *caps_data);

    void (*init_usr_cb)(struct caps_tV_data *caps_data);

    void (*cmd_channelUp_usr_cb)(struct caps_tV_data *caps_data);
    void (*cmd_volumeDown_usr_cb)(struct caps_tV_data *caps_data);
    void (*cmd_volumeUp_usr_cb)(struct caps_tV_data *caps_data);
    void (*cmd_channelDown_usr_cb)(struct caps_tV_data *caps_data);
} caps_tV_data_t;

caps_tV_data_t *caps_tV_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
