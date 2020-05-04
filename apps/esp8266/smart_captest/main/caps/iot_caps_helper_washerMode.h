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

#ifndef _IOT_CAPS_HELPER_WASHER_MODE_
#define _IOT_CAPS_HELPER_WASHER_MODE_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_REGULAR,
    CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_HEAVY,
    CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_RINSE,
    CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_SPINDRY,
    CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_MAX
};

const static struct iot_caps_washerMode {
    const char *id;
    const struct washerMode_attr_washerMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_MAX];
    } attr_washerMode;
    const struct washerMode_cmd_setWasherMode { const char* name; } cmd_setWasherMode;
} caps_helper_washerMode = {
    .id = "washerMode",
    .attr_washerMode = {
        .name = "washerMode",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"regular", "heavy", "rinse", "spinDry"},
    },
    .cmd_setWasherMode = { .name = "setWasherMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_WASHER_MODE_ */
