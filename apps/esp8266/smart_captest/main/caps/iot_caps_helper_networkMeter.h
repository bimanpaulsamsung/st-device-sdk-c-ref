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

#ifndef _IOT_CAPS_HELPER_NETWORK_METER_
#define _IOT_CAPS_HELPER_NETWORK_METER_

#include "iot_caps_helper.h"

const static struct iot_caps_networkMeter {
    const char *id;
    const struct networkMeter_attr_downlinkSpeed {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const int min;
    } attr_downlinkSpeed;
    const struct networkMeter_attr_uplinkSpeed {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const int min;
    } attr_uplinkSpeed;
} caps_helper_networkMeter = {
    .id = "networkMeter",
    .attr_downlinkSpeed = {
        .name = "downlinkSpeed",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_INTEGER,
        .min = 0,
    },
    .attr_uplinkSpeed = {
        .name = "uplinkSpeed",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_INTEGER,
        .min = 0,
    },
};

#endif /* _IOT_CAPS_HERLPER_NETWORK_METER_ */
