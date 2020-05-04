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

#include "caps/iot_caps_helper_tvChannel.h"

typedef struct caps_tvChannel_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    char *tvChannel_value;
    char *tvChannelName_value;

    const char *(*get_tvChannel_value)(struct caps_tvChannel_data *caps_data);
    void (*set_tvChannel_value)(struct caps_tvChannel_data *caps_data, const char *value);
    void (*attr_tvChannel_send)(struct caps_tvChannel_data *caps_data);
    const char *(*get_tvChannelName_value)(struct caps_tvChannel_data *caps_data);
    void (*set_tvChannelName_value)(struct caps_tvChannel_data *caps_data, const char *value);
    void (*attr_tvChannelName_send)(struct caps_tvChannel_data *caps_data);

    void (*init_usr_cb)(struct caps_tvChannel_data *caps_data);

    void (*cmd_channelUp_usr_cb)(struct caps_tvChannel_data *caps_data);
    void (*cmd_channelDown_usr_cb)(struct caps_tvChannel_data *caps_data);
    void (*cmd_setTvChannelName_usr_cb)(struct caps_tvChannel_data *caps_data);
    void (*cmd_setTvChannel_usr_cb)(struct caps_tvChannel_data *caps_data);
} caps_tvChannel_data_t;

caps_tvChannel_data_t *caps_tvChannel_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
