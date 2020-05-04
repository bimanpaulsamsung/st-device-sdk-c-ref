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

#include "caps/iot_caps_helper_videoCapture.h"
#include "JSON.h"

typedef struct caps_videoCapture_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    JSON_H *clip_value;
    JSON_H *stream_value;

    const JSON_H *(*get_clip_value)(struct caps_videoCapture_data *caps_data);
    void (*set_clip_value)(struct caps_videoCapture_data *caps_data, const JSON_H *value);
    void (*attr_clip_send)(struct caps_videoCapture_data *caps_data);
    const JSON_H *(*get_stream_value)(struct caps_videoCapture_data *caps_data);
    void (*set_stream_value)(struct caps_videoCapture_data *caps_data, const JSON_H *value);
    void (*attr_stream_send)(struct caps_videoCapture_data *caps_data);

    void (*init_usr_cb)(struct caps_videoCapture_data *caps_data);

    void (*cmd_capture_usr_cb)(struct caps_videoCapture_data *caps_data);
} caps_videoCapture_data_t;

caps_videoCapture_data_t *caps_videoCapture_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
