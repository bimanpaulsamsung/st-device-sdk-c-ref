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

#ifndef _IOT_CAPS_HELPER_TV_
#define _IOT_CAPS_HELPER_TV_

#include "iot_caps_helper.h"

const static struct iot_caps_tV {
    const char *id;
    const struct tV_attr_sound {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_sound;
    const struct tV_attr_picture {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_picture;
    const struct tV_attr_movieMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_movieMode;
    const struct tV_attr_power {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_power;
    const struct tV_attr_volume {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const int min;
    } attr_volume;
    const struct tV_attr_channel {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const int min;
    } attr_channel;
    const struct tV_cmd_channelUp { const char* name; } cmd_channelUp;
    const struct tV_cmd_volumeDown { const char* name; } cmd_volumeDown;
    const struct tV_cmd_volumeUp { const char* name; } cmd_volumeUp;
    const struct tV_cmd_channelDown { const char* name; } cmd_channelDown;
} caps_helper_tV = {
    .id = "tV",
    .attr_sound = {
        .name = "sound",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_picture = {
        .name = "picture",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_movieMode = {
        .name = "movieMode",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_power = {
        .name = "power",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_volume = {
        .name = "volume",
        .property = ATTR_SET_VALUE_MIN,
        .value_type = VALUE_TYPE_INTEGER,
        .min = 0,
    },
    .attr_channel = {
        .name = "channel",
        .property = ATTR_SET_VALUE_MIN,
        .value_type = VALUE_TYPE_INTEGER,
        .min = 0,
    },
    .cmd_channelUp = { .name = "channelUp" },
    .cmd_volumeDown = { .name = "volumeDown" },
    .cmd_volumeUp = { .name = "volumeUp" },
    .cmd_channelDown = { .name = "channelDown" },
};

#endif /* _IOT_CAPS_HERLPER_TV_ */
