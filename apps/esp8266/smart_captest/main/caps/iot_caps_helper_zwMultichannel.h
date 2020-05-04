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

#ifndef _IOT_CAPS_HELPER_ZW_MULTICHANNEL_
#define _IOT_CAPS_HELPER_ZW_MULTICHANNEL_

#include "iot_caps_helper.h"

const static struct iot_caps_zwMultichannel {
    const char *id;
    const struct zwMultichannel_attr_epInfo {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_epInfo;
    const struct zwMultichannel_attr_epEvent {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const unsigned int max_length;
    } attr_epEvent;
    const struct zwMultichannel_cmd_enableEpEvents { const char* name; } cmd_enableEpEvents;
    const struct zwMultichannel_cmd_epCmd { const char* name; } cmd_epCmd;
} caps_helper_zwMultichannel = {
    .id = "zwMultichannel",
    .attr_epInfo = {
        .name = "epInfo",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .attr_epEvent = {
        .name = "epEvent",
        .property = ATTR_SET_MAX_LENGTH,
        .value_type = VALUE_TYPE_STRING,
        .max_length = 255,
    },
    .cmd_enableEpEvents = { .name = "enableEpEvents" }, // arguments: enabledEndpoints(string) 
    .cmd_epCmd = { .name = "epCmd" }, // arguments: endpoint(number) commands(string) 
};

#endif /* _IOT_CAPS_HERLPER_ZW_MULTICHANNEL_ */
