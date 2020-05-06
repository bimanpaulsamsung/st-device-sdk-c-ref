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

#ifndef _IOT_CAPS_HELPER_DRYER_MODE_
#define _IOT_CAPS_HELPER_DRYER_MODE_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_REGULAR,
    CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_LOWHEAT,
    CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_HIGHHEAT,
    CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_MAX
};

const static struct iot_caps_dryerMode {
    const char *id;
    const struct dryerMode_attr_dryerMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_MAX];
    } attr_dryerMode;
    const struct dryerMode_cmd_setDryerMode { const char* name; } cmd_setDryerMode;
} caps_helper_dryerMode = {
    .id = "dryerMode",
    .attr_dryerMode = {
        .name = "dryerMode",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"regular", "lowHeat", "highHeat"},
    },
    .cmd_setDryerMode = { .name = "setDryerMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_DRYER_MODE_ */
