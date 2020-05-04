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

#ifndef _IOT_CAPS_HELPER_AIR_CONDITIONER_MODE_
#define _IOT_CAPS_HELPER_AIR_CONDITIONER_MODE_

#include "iot_caps_helper.h"

const static struct iot_caps_airConditionerMode {
    const char *id;
    const struct airConditionerMode_attr_airConditionerMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_airConditionerMode;
    const struct airConditionerMode_attr_supportedAcModes {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_supportedAcModes;
    const struct airConditionerMode_cmd_setAirConditionerMode { const char* name; } cmd_setAirConditionerMode;
} caps_helper_airConditionerMode = {
    .id = "airConditionerMode",
    .attr_airConditionerMode = {
        .name = "airConditionerMode",
        .property = ATTR_SET_VALUE_REQUIRED | ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_supportedAcModes = {
        .name = "supportedAcModes",
        .property = ATTR_SET_VALUE_ARRAY,
        .value_type = VALUE_TYPE_STRING,
    },
    .cmd_setAirConditionerMode = { .name = "setAirConditionerMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_AIR_CONDITIONER_MODE_ */
