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

#ifndef _IOT_CAPS_HELPER_VIDEO_CAMERA_
#define _IOT_CAPS_HELPER_VIDEO_CAMERA_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MUTED,
    CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_UNMUTED,
    CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MAX
};

enum {
    CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_OFF,
    CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_ON,
    CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_RESTARTING,
    CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_UNAVAILABLE,
    CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_MAX
};

const static struct iot_caps_videoCamera {
    const char *id;
    const struct videoCamera_attr_mute {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MAX];
    } attr_mute;
    const struct videoCamera_attr_camera {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_MAX];
    } attr_camera;
    const struct videoCamera_attr_statusMessage {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_statusMessage;
    const struct videoCamera_attr_settings {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_settings;
    const struct videoCamera_cmd_unmute { const char* name; } cmd_unmute;
    const struct videoCamera_cmd_on { const char* name; } cmd_on;
    const struct videoCamera_cmd_off { const char* name; } cmd_off;
    const struct videoCamera_cmd_flip { const char* name; } cmd_flip;
    const struct videoCamera_cmd_mute { const char* name; } cmd_mute;
} caps_helper_videoCamera = {
    .id = "videoCamera",
    .attr_mute = {
        .name = "mute",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
        .values = {"muted", "unmuted"},
    },
    .attr_camera = {
        .name = "camera",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
        .values = {"off", "on", "restarting", "unavailable"},
    },
    .attr_statusMessage = {
        .name = "statusMessage",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_settings = {
        .name = "settings",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_OBJECT,
    },
    .cmd_unmute = { .name = "unmute" },
    .cmd_on = { .name = "on" },
    .cmd_off = { .name = "off" },
    .cmd_flip = { .name = "flip" },
    .cmd_mute = { .name = "mute" },
};

#endif /* _IOT_CAPS_HERLPER_VIDEO_CAMERA_ */
