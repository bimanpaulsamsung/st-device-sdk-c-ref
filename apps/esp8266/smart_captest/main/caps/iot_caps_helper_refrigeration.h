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

#ifndef _IOT_CAPS_HELPER_REFRIGERATION_
#define _IOT_CAPS_HELPER_REFRIGERATION_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_REFRIGERATION_DEFROST_VALUE_OFF,
    CAP_ENUM_REFRIGERATION_DEFROST_VALUE_ON,
    CAP_ENUM_REFRIGERATION_DEFROST_VALUE_MAX
};

enum {
    CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_OFF,
    CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_ON,
    CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_MAX
};

enum {
    CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_OFF,
    CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_ON,
    CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_MAX
};

const static struct iot_caps_refrigeration {
    const char *id;
    const struct refrigeration_attr_defrost {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_REFRIGERATION_DEFROST_VALUE_MAX];
    } attr_defrost;
    const struct refrigeration_attr_rapidFreezing {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_MAX];
    } attr_rapidFreezing;
    const struct refrigeration_attr_rapidCooling {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_MAX];
    } attr_rapidCooling;
    const struct refrigeration_cmd_setRapidCooling { const char* name; } cmd_setRapidCooling;
    const struct refrigeration_cmd_setRapidFreezing { const char* name; } cmd_setRapidFreezing;
    const struct refrigeration_cmd_setDefrost { const char* name; } cmd_setDefrost;
} caps_helper_refrigeration = {
    .id = "refrigeration",
    .attr_defrost = {
        .name = "defrost",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
        .values = {"off", "on"},
    },
    .attr_rapidFreezing = {
        .name = "rapidFreezing",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
        .values = {"off", "on"},
    },
    .attr_rapidCooling = {
        .name = "rapidCooling",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
        .values = {"off", "on"},
    },
    .cmd_setRapidCooling = { .name = "setRapidCooling" }, // arguments: rapidCooling(string) 
    .cmd_setRapidFreezing = { .name = "setRapidFreezing" }, // arguments: rapidCooling(string) 
    .cmd_setDefrost = { .name = "setDefrost" }, // arguments: rapidCooling(string) 
};

#endif /* _IOT_CAPS_HERLPER_REFRIGERATION_ */