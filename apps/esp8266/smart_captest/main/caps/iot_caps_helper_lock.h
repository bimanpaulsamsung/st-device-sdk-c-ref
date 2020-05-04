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

#ifndef _IOT_CAPS_HELPER_LOCK_
#define _IOT_CAPS_HELPER_LOCK_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_LOCK_LOCK_VALUE_LOCKED,
    CAP_ENUM_LOCK_LOCK_VALUE_UNKNOWN,
    CAP_ENUM_LOCK_LOCK_VALUE_UNLOCKED,
    CAP_ENUM_LOCK_LOCK_VALUE_UNLOCKED_WITH_TIMEOUT,
    CAP_ENUM_LOCK_LOCK_VALUE_MAX
};

const static struct iot_caps_lock {
    const char *id;
    const struct lock_attr_lock {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_LOCK_LOCK_VALUE_MAX];
    } attr_lock;
    const struct lock_cmd_lock { const char* name; } cmd_lock;
    const struct lock_cmd_unlock { const char* name; } cmd_unlock;
} caps_helper_lock = {
    .id = "lock",
    .attr_lock = {
        .name = "lock",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"locked", "unknown", "unlocked", "unlocked_with_timeout"},
    },
    .cmd_lock = { .name = "lock" },
    .cmd_unlock = { .name = "unlock" },
};

#endif /* _IOT_CAPS_HERLPER_LOCK_ */
