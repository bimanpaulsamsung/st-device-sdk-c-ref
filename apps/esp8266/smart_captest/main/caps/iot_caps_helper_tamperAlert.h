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

#ifndef _IOT_CAPS_HELPER_TAMPER_ALERT_
#define _IOT_CAPS_HELPER_TAMPER_ALERT_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_TAMPERALERT_TAMPER_VALUE_CLEAR,
    CAP_ENUM_TAMPERALERT_TAMPER_VALUE_DETECTED,
    CAP_ENUM_TAMPERALERT_TAMPER_VALUE_MAX
};

const static struct iot_caps_tamperAlert {
    const char *id;
    const struct tamperAlert_attr_tamper {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_TAMPERALERT_TAMPER_VALUE_MAX];
    } attr_tamper;
} caps_helper_tamperAlert = {
    .id = "tamperAlert",
    .attr_tamper = {
        .name = "tamper",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"clear", "detected"},
    },
};

#endif /* _IOT_CAPS_HERLPER_TAMPER_ALERT_ */
