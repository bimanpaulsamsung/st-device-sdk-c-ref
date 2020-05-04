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

#ifndef _IOT_CAPS_HELPER_INFRARED_LEVEL_
#define _IOT_CAPS_HELPER_INFRARED_LEVEL_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_INFRAREDLEVEL_INFRAREDLEVEL_UNIT_PERCENT,
    CAP_ENUM_INFRAREDLEVEL_INFRAREDLEVEL_UNIT_MAX
};

const static struct iot_caps_infraredLevel {
    const char *id;
    const struct infraredLevel_attr_infraredLevel {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *units[CAP_ENUM_INFRAREDLEVEL_INFRAREDLEVEL_UNIT_MAX];
        const double min;
        const double max;
    } attr_infraredLevel;
    const struct infraredLevel_cmd_setInfraredLevel { const char* name; } cmd_setInfraredLevel;
} caps_helper_infraredLevel = {
    .id = "infraredLevel",
    .attr_infraredLevel = {
        .name = "infraredLevel",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_MAX | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_NUMBER,
        .units = {"%"},
        .min = 0,
        .max = 100,
    },
    .cmd_setInfraredLevel = { .name = "setInfraredLevel" }, // arguments: level(number) 
};

#endif /* _IOT_CAPS_HERLPER_INFRARED_LEVEL_ */
